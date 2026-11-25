#include "TextAnalysis.h"
#include <cctype>
#include <cstddef>

int countWords(const std::string &text) {
  int wortanzahl = 0;
  bool imwort = false;
  for (std::size_t i = 0; i < text.length(); ++i) {
    if (isalpha(text[i])) {
      if (imwort == false) {
        imwort = true;
        ++wortanzahl;
      }
    } else if (text[i] == '-' && i > 0 && i + 1 < text.length() &&
               isalpha(text[i - 1]) && isalpha(text[i + 1])) {
      imwort = true;
    } else {
      imwort = false;
    }
  }
  return wortanzahl;
}

std::map<std::string, int>
countWordFrequency(const std::string &text,
                   const std::set<std::string> &stopWords) {
  std::map<std::string, int> wordFrequency;
  std::string wort = "";
  bool imwort = false;
  std::set<std::string> smallStopWords;

  // Kleinschreibung in allen Stop-Wörtern
  for (const auto &stopWort : stopWords) {
    std::string klein;
    for (char buchstabe : stopWort) {
      klein += std::tolower(buchstabe);
    }
    smallStopWords.insert(klein);
  }

  for (std::size_t i = 0; i < text.length(); ++i) {
    if (isalpha(text[i])) {
      imwort = true;
      wort += std::tolower(text[i]);
    } else if (text[i] == '-' && i > 0 && i + 1 < text.length() &&
               isalpha(text[i - 1]) && isalpha(text[i + 1])) {
      imwort = true;
      wort += text[i];
    } else {
      imwort = false;
    }
    if (imwort == false && !wort.empty()) {
      if (!smallStopWords.contains(wort)) {
        ++wordFrequency[wort];
      }
      wort.clear();
    }
  }
  // Zur Aufnahme des letzten Worts im string!
  if (!wort.empty() && !smallStopWords.contains(wort)) {
    ++wordFrequency[wort];
  }
  return wordFrequency;
}

std::string cleanText(const std::string &text) {
  std::string bereinigt = "";
  for (std::size_t i = 0; i < text.size(); ++i) {
    // Space-Zeichen als Leerzeichen darstellen
    if (isspace(text[i])) {
      if (!bereinigt.empty() && !isspace(bereinigt.back())) {
        bereinigt += ' ';
      }
      // Satzzeichen
    } else if (ispunct(text[i])) {
      // Leerzeichen vor Satzzeichen entfernen
      if (!bereinigt.empty() && isspace(bereinigt.back())) {
        bereinigt.pop_back();
      }
      // Mehrfache Satzzeichen weglassen
      if (!bereinigt.empty() && ispunct(bereinigt.back())) {
        continue;
      }
      bereinigt += text[i];
      // Restliche Zeichen dadrstellen
    } else {
      bereinigt += text[i];
    }
  }
  // letztes Leerzeichen entfernen
  if (!bereinigt.empty() && bereinigt.back() == ' ') {
    bereinigt.pop_back();
  }
  return bereinigt;
}

std::string cleanHTML(const std::string &text) {
  std::string bereinigt = "";
  bool imTag(false);

  for (std::size_t i = 0; i < text.length(); ++i) {
    if (text[i] == '<') {
      imTag = true;
    } else if (!imTag) {
      bereinigt += text[i];
    } else if (text[i] == '>') {
      imTag = false;
      bereinigt += ' ';
    }
  }
  return cleanText(bereinigt);
}

std::string cleanMarkdown(const std::string &text) {
  std::string bereinigt = "";
  bool zeilenanfang(true);
  bool imLinkText(false);
  bool inLinkURL(false);

  for (std::size_t i = 0; i < text.length(); ++i) {
    // öffnende Link-Klammer skippen
    if (text[i] == '[' && text.find("](", i) != std::string::npos) {
      imLinkText = true;
      continue;
      // schließende Linklklammer und URLklammer skippen ](
    } else if (imLinkText && text[i] == ']' && i + 1 < text.length() &&
               text[i + 1] == '(') {
      imLinkText = false;
      inLinkURL = true;
      ++i;
      continue;
      // schließende URLklammer skippen
    } else if (inLinkURL && text[i] == ')') {
      inLinkURL = false;
      continue;
      // URL skippen
    } else if (inLinkURL) {
      continue;
      // Zeilenanfang erkennen
    } else if (text[i] == '\n') {
      zeilenanfang = true;
      bereinigt += ' ';
      // Überschriften-# übersprpingen
    } else if (zeilenanfang && text[i] == '#') {
      continue;
      // Aufzählungen skippen
    } else if (zeilenanfang && (text[i] == '-' || text[i] == '*') &&
               (i + 1) < text.length() && text[i + 1] == ' ') {
      ++i;
      continue;
      // _ und * Formatierung skippen
    } else if (((text[i] == '_') || (text[i] == '*')) &&
               !(i > 0 && i + 1 < text.length() && isalpha(text[i - 1]) &&
                 isalpha(text[i + 1]))) {
      continue;
    } else if (text[i] == '`') {
      continue;
    } else {
      zeilenanfang = false;
      bereinigt += text[i];
    }
  }
  return cleanText(bereinigt);
}

std::vector<std::size_t> searchText(const std::string &text,
                                    const std::string &suchtext) {
  // Vektor für die gefundenen Stellen
  std::vector<std::size_t> fundPos;

  if (suchtext.empty()) {
    return fundPos;
  }
  // Erste Position finden
  std::size_t position = text.find(suchtext);
  // Fund in den Vektor pushen und weitersuchen
  while (position != std::string::npos) {
    fundPos.push_back(position);
    position = text.find(suchtext, position + 1);
  }
  return fundPos;
}

double averageWordLength(const std::string &text) {
  int wortAnzahl = countWords(text);
  double buchstabenAnzahl(0.0);
  for (std::size_t i = 0; i < text.length(); ++i) {
    if (isalpha(text[i])) {
      ++buchstabenAnzahl;
    }
  }
  if (wortAnzahl == 0) {
    return 0;
  }
  return buchstabenAnzahl / wortAnzahl;
}

double averageSentenceLength(const std::string &text) {
  int wortAnzahl = countWords(text);
  double satzzeichenAnzahl(0);
  // Text bereinigen, um doppelte Satzzeichen zu entfernen.
  std::string bereinigt = cleanText(text);
  for (std::size_t i = 0; i < bereinigt.length(); ++i) {
    if (bereinigt[i] == '!' || bereinigt[i] == '?' || bereinigt[i] == '.') {
      ++satzzeichenAnzahl;
    }
  }
  if (satzzeichenAnzahl == 0 || wortAnzahl == 0) {
    return 0.0;
  }
  return wortAnzahl / satzzeichenAnzahl;
}

std::map<std::pair<std::string, std::string>, int>
countCooccurrence(const std::string &text) {

  std::map<std::pair<std::string, std::string>, int> wortpaare;

  std::string vorherigesWort = "";
  std::string wort = "";

  for (std::size_t i = 0; i < text.length(); ++i) {
    if (isalpha(text[i])) {
      wort += std::tolower(text[i]);
    } else if (text[i] == '-' && i > 0 && i + 1 < text.length() &&
               isalpha(text[i - 1]) && isalpha(text[i + 1])) {
      wort += text[i];
    } else {
      if (!wort.empty()) {
        if (!vorherigesWort.empty()) {
          ++wortpaare[{vorherigesWort, wort}];
        }
        vorherigesWort = wort;
        wort.clear();
      }

      // Keine Wortpaare über Satzgrenzen
      if (text[i] == '.' || text[i] == '!' || text[i] == '?') {
        vorherigesWort.clear();
      }
    }
  }

  // Letztes Wort berücksichtigen
  if (!wort.empty() && !vorherigesWort.empty()) {
    ++wortpaare[{vorherigesWort, wort}];
  }

  return wortpaare;
}

int countSyllables(const std::string &wort) {
  int silbenAnzahl = 0;
  bool imVokal = false;

  for (std::size_t i = 0; i < wort.length(); ++i) {
    char buchstabe = std::tolower(wort[i]);

    if (buchstabe == 'a' || buchstabe == 'e' || buchstabe == 'i' ||
        buchstabe == 'o' || buchstabe == 'u') {
      if (!imVokal) {
        ++silbenAnzahl;
        imVokal = true;
      }
    } else {
      imVokal = false;
    }
  }

  return silbenAnzahl;
}

double fleschReadingEase(const std::string &text) {
  int wortAnzahl = countWords(text);
  int satzzeichenAnzahl = 0;
  int silbenAnzahl = 0;
  std::string wort = "";

  // Satzzeichen im bereinigten Text zählen
  std::string bereinigt = cleanText(text);

  for (char zeichen : bereinigt) {
    if (zeichen == '.' || zeichen == '!' || zeichen == '?') {
      ++satzzeichenAnzahl;
    }
  }

  if (wortAnzahl == 0 || satzzeichenAnzahl == 0) {
    return 0.0;
  }

  double satzLaenge =
      static_cast<double>(wortAnzahl) / satzzeichenAnzahl;

  for (std::size_t i = 0; i < text.length(); ++i) {
    if (isalpha(text[i])) {
      wort += std::tolower(text[i]);
    } else if (text[i] == '-' && i > 0 && i + 1 < text.length() &&
               isalpha(text[i - 1]) && isalpha(text[i + 1])) {
      wort += text[i];
    } else if (!wort.empty()) {
      silbenAnzahl += countSyllables(wort);
      wort.clear();
    }
  }

  // Letztes Wort berücksichtigen
  if (!wort.empty()) {
    silbenAnzahl += countSyllables(wort);
  }

  double durchschnittSilben = static_cast<double>(silbenAnzahl) / wortAnzahl;

  return 180.0 - satzLaenge - 58.5 * durchschnittSilben;
}
