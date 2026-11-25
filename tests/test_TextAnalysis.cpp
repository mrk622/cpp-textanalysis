#include "TextAnalysis.h"
#include <cassert>
#include <iostream>

#include "TextAnalysis.h"
#include <cassert>
#include <cmath>
#include <iostream>

int main() {
  // 1. countWords()
  assert(countWords("Der Hund spielt.") == 3);
  assert(countWords("") == 0);
  assert(countWords("123 456") == 0);
  assert(countWords("Ein-Wort") == 1);
  assert(countWords("Hallo, Welt!") == 2);

  // 2. countWordFrequency()
  auto haeufigkeit = countWordFrequency("Hund Katze Hund");
  assert(haeufigkeit.size() == 2);
  assert(haeufigkeit.at("hund") == 2);
  assert(haeufigkeit.at("katze") == 1);

  auto grossKlein = countWordFrequency("Hund HUND hund");
  assert(grossKlein.at("hund") == 3);

  auto ohneStopWords =
      countWordFrequency("Der Hund und die Katze", {"der", "und", "die"});
  assert(ohneStopWords.size() == 2);
  assert(ohneStopWords.at("hund") == 1);
  assert(ohneStopWords.at("katze") == 1);
  assert(countWordFrequency("").empty());

  // 3. cleanText()
  assert(cleanText("Der   Hund   spielt.") == "Der Hund spielt.");
  assert(cleanText("  Hallo Welt  ") == "Hallo Welt");
  assert(cleanText("Hallo   , Welt  !") == "Hallo, Welt!");
  assert(cleanText("Hallo!!! Welt???") == "Hallo! Welt?");
  assert(cleanText("Hallo?! Welt") == "Hallo? Welt");
  assert(cleanText("Hallo\n\tWelt") == "Hallo Welt");
  assert(cleanText("").empty());
  assert(cleanText("   ").empty());

  // 4. cleanHTML()
  assert(cleanHTML("<p>Hallo Welt</p>") == "Hallo Welt");
  assert(cleanHTML("<h1>Titel</h1>") == "Titel");
  assert(cleanHTML("<p>Der <b>Hund</b> spielt.</p>") ==
         "Der Hund spielt.");
  assert(cleanHTML("<p>Hallo</p><p>Welt</p>") == "Hallo Welt");
  assert(cleanHTML("<div>  Hallo   Welt  </div>") == "Hallo Welt");
  assert(cleanHTML("<p></p>").empty());
  assert(cleanHTML("").empty());

  // 5. cleanMarkdown()
  assert(cleanMarkdown("# Titel") == "Titel");
  assert(cleanMarkdown("**Hallo** Welt") == "Hallo Welt");
  assert(cleanMarkdown("*Hallo* _Welt_") == "Hallo Welt");
  assert(cleanMarkdown("- Erster Punkt\n- Zweiter Punkt") ==
         "Erster Punkt Zweiter Punkt");
  assert(cleanMarkdown("[OpenAI](https://openai.com)") == "OpenAI");
  assert(cleanMarkdown("Das ist `Code`.") == "Das ist Code.");
  assert(cleanMarkdown("").empty());

  // 6. searchText()
  auto fundstellen = searchText("Hund Katze Hund", "Hund");
  assert(fundstellen.size() == 2);
  assert(fundstellen[0] == 0);
  assert(fundstellen[1] == 11);

  assert(searchText("Hallo Welt", "Katze").empty());
  assert(searchText("Hallo Welt", "").empty());
  assert(searchText("", "Hallo").empty());

  auto ueberlappend = searchText("aaaa", "aa");
  assert(ueberlappend.size() == 3);
  assert(ueberlappend[0] == 0);
  assert(ueberlappend[1] == 1);
  assert(ueberlappend[2] == 2);

  // 7. averageWordLength()
  assert(std::abs(averageWordLength("Hund Katze") - 4.5) < 0.0001);
  assert(std::abs(averageWordLength("Ein-Wort") - 7.0) < 0.0001);
  assert(averageWordLength("") == 0.0);
  assert(averageWordLength("123") == 0.0);

  // 8. averageSentenceLength()
  assert(std::abs(averageSentenceLength(
                      "Der Hund spielt. Die Katze rennt.") -
                  3.0) < 0.0001);
  assert(std::abs(averageSentenceLength("Hallo Welt!") - 2.0) <
         0.0001);
  assert(averageSentenceLength("Hallo Welt") == 0.0);
  assert(averageSentenceLength("") == 0.0);

  // 9. countCooccurrence()
  auto wortpaare =
      countCooccurrence("Der Hund spielt. Der Hund rennt!");

  assert(wortpaare.size() == 3);
  assert(wortpaare.at({"der", "hund"}) == 2);
  assert(wortpaare.at({"hund", "spielt"}) == 1);
  assert(wortpaare.at({"hund", "rennt"}) == 1);
  assert(countCooccurrence("").empty());
  assert(countCooccurrence("Hund").empty());

  // 10. countSyllables()
  assert(countSyllables("Hund") == 1);
  assert(countSyllables("Katze") == 2);
  assert(countSyllables("Auto") == 2);
  assert(countSyllables("Banane") == 3);
  assert(countSyllables("") == 0);
  assert(countSyllables("123") == 0);

  // 11. fleschReadingEase()
  assert(std::abs(fleschReadingEase(
                      "Der Hund spielt. Die Katze rennt.") -
                  108.75) < 0.0001);
  assert(fleschReadingEase("") == 0.0);
  assert(fleschReadingEase("Hallo Welt") == 0.0);

  std::cout << "Alle Unit-Tests erfolgreich.\n";

  return 0;
}
