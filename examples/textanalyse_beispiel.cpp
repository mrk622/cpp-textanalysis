
#include "TextAnalysis.h"
#include <iostream>
#include <set>
#include <string>

int main() {
  const std::string text = "Die Software analysiert Texte. "
                           "Die Analyse erkennt wichtige Begriffe. "
                           "Software verbessert die Analyse.";

  const std::set<std::string> stopWords = {"die", "der", "das"};

  // Grundlegende Textanalyse
  std::cout << "Originaltext:\n" << text << "\n\n";

  std::cout << "Bereinigter Text:\n" << cleanText(text) << "\n\n";

  std::cout << "Anzahl Woerter: " << countWords(text) << '\n';

  std::cout << "Durchschnittliche Wortlaenge: " << averageWordLength(text)
            << '\n';

  std::cout << "Durchschnittliche Satzlaenge: " << averageSentenceLength(text)
            << '\n';

  std::cout << "Flesch-Reading-Ease: " << fleschReadingEase(text) << "\n\n";

  // Worthaeufigkeiten ohne Stop-Woerter
  std::cout << "Worthaeufigkeiten:\n";

  const auto haeufigkeiten = countWordFrequency(text, stopWords);

  for (const auto &[wort, anzahl] : haeufigkeiten) {
    std::cout << wort << ": " << anzahl << '\n';
  }

  // Textsuche
  std::cout << "\nFundstellen von 'Analyse':\n";

  const auto positionen = searchText(text, "Analyse");

  for (std::size_t position : positionen) {
    std::cout << "Position: " << position << '\n';
  }

  // Benachbarte Wortpaare
  std::cout << "\nWortpaare:\n";

  const auto wortpaare = countCooccurrence(text);

  for (const auto &[paar, anzahl] : wortpaare) {
    std::cout << paar.first << " + " << paar.second << ": " << anzahl << '\n';
  }

  return 0;
}
