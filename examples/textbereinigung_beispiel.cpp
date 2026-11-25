
#include "TextAnalysis.h"
#include <iostream>
#include <string>

int main() {
  const std::string html =
      "<h1>Textanalyse</h1><p>Die Software analysiert Texte.</p>";

  const std::string markdown =
      "# Textanalyse\n**Die Software** analysiert [Texte](https://example.com).";

  const std::string wort = "Analyse";

  // HTML bereinigen
  std::cout << "HTML-Original:\n"
            << html << "\n\n";

  std::cout << "HTML bereinigt:\n"
            << cleanHTML(html) << "\n\n";

  // Markdown bereinigen
  std::cout << "Markdown-Original:\n"
            << markdown << "\n\n";

  std::cout << "Markdown bereinigt:\n"
            << cleanMarkdown(markdown) << "\n\n";

  // Silbenanzahl bestimmen
  std::cout << "Geschaetzte Silbenanzahl von '"
            << wort << "': "
            << countSyllables(wort) << '\n';

  return 0;
}
