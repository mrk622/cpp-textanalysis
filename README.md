# Textanalyse-Bibliothek

Diese Bibliothek wurde mit C++20 erstellt und kann Texte untersuchen und bereinigen. Sie verwendet nur die C++-Standardbibliothek, zusätzliche Pakete sind nicht nötig.

## Funktionen

| Funktion | Was sie macht |
|---|---|
| `countWords(text)` | Zählt Wörter. Bindestriche innerhalb von Wörtern sind erlaubt, reine Zahlen zählen nicht. |
| `countWordFrequency(text, stopWords)` | Zählt, wie oft jedes Wort vorkommt. Groß-/Kleinschreibung wird ignoriert; optionale Stop-Wörter werden ausgelassen. Das Ergebnis ist eine alphabetisch sortierte `std::map`. |
| `cleanText(text)` | Fasst Leerzeichen und Zeilenumbrüche zusammen, entfernt Leerzeichen vor Satzzeichen und lässt bei mehreren direkt aufeinanderfolgenden Satzzeichen nur das erste stehen. |
| `cleanHTML(text)` | Entfernt einfache HTML-Tags zwischen `<` und `>` und bereinigt den Text mit `cleanText()`. Kein vollständiger HTML-Parser. |
| `cleanMarkdown(text)` | Entfernt einfache Markdown-Formatierungen wie Überschriften, Listenzeichen, Hervorhebungen und Inline-Code. Bei Links bleibt der sichtbare Text erhalten; danach folgt `cleanText()`. |
| `searchText(text, suchtext)` | Gibt die Startpositionen aller Treffer als `std::vector<std::size_t>` zurück (ab 0, auch überlappende Treffer). Groß-/Kleinschreibung wird beachtet. |
| `averageWordLength(text)` | Berechnet die durchschnittliche Zahl alphabetischer Zeichen pro Wort. |
| `averageSentenceLength(text)` | Berechnet Wörter pro Satz; `.`, `!` und `?` gelten als Satzende. |
| `countCooccurrence(text)` | Zählt benachbarte Wortpaare innerhalb eines Satzes, ohne Groß-/Kleinschreibung zu unterscheiden. Ergebnis: `std::map` mit Wortpaaren und Häufigkeiten. |
| `countSyllables(wort)` | Schätzt die Silben anhand zusammenhängender Vokalgruppen (`a`, `e`, `i`, `o`, `u`). |
| `fleschReadingEase(text)` | Schätzt, wie leicht ein deutscher Text zu lesen ist. Höhere Werte bedeuten leichtere Lesbarkeit. |

Die Funktionen nehmen Texte als `const std::string &` entgegen. `countWords()` und `countSyllables()` geben `int` zurück, die Durchschnitts- und Lesbarkeitsfunktionen `double`, die Bereinigungsfunktionen `std::string`. `countWordFrequency()` nimmt zusätzlich eine optionale `const std::set<std::string> &` für Stop-Wörter entgegen. `countCooccurrence()` gibt eine `std::map<std::pair<std::string, std::string>, int>` zurück. Bei fehlenden Wörtern bzw. Satzenden liefern die betreffenden Durchschnitts- und Lesbarkeitsfunktionen `0.0`.

Der Lesbarkeitswert wird vereinfacht berechnet:

`180 - durchschnittliche Satzlänge - 58,5 × durchschnittliche Silbenanzahl pro Wort`

Da die Silbenzählung nur Vokalgruppen betrachtet, ist der Wert eine Schätzung.

## Aufbau und Verwendung

Die wichtigsten Dateien sind:

- `include/TextAnalysis.h` – Funktionsdeklarationen, die andere Programme einbinden.
- `src/TextAnalysis.cpp` – eigentliche Umsetzung der Funktionen.
- `tests/test_TextAnalysis.cpp` – automatische Tests mit `assert()`.
- `examples/textanalyse_beispiel.cpp` – zeigt Textbereinigung, Wortzählung, Wort- und Satzlänge, Lesbarkeitswert, Worthäufigkeiten mit Stop-Wörtern, Textsuche und Wortpaare.
- `examples/textbereinigung_beispiel.cpp` – zeigt HTML-/Markdown-Bereinigung und Silbenzählung.

Für andere Projekte reichen `TextAnalysis.h` und `TextAnalysis.cpp`. Im eigenen Programm wird der Header mit `#include "TextAnalysis.h"` eingebunden. Der Schalter `-Iinclude` sagt dem Compiler, wo er den Header findet; `src/TextAnalysis.cpp` wird mitkompiliert.

Beide Beispiele lassen sich im Projektordner so erstellen und starten:

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Iinclude examples/textanalyse_beispiel.cpp src/TextAnalysis.cpp -o build/textanalyse_beispiel
./build/textanalyse_beispiel

g++ -std=c++20 -Wall -Wextra -Wpedantic -Iinclude examples/textbereinigung_beispiel.cpp src/TextAnalysis.cpp -o build/textbereinigung_beispiel
./build/textbereinigung_beispiel
```

Die Unit-Tests werden so ausgeführt:

```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -Iinclude tests/test_TextAnalysis.cpp src/TextAnalysis.cpp -o build/test_textanalyse
./build/test_textanalyse
```

Bei Erfolg erscheint `Alle Unit-Tests erfolgreich.`

## Was ich gelernt habe und was noch verbessert werden könnte

Am schwierigsten waren die Worterkennung, die Textbereinigung und der Umgang mit Sonderfällen. Durch die Aufteilung in Header und Quelldatei habe ich gelernt, Funktionen so aufzubauen, dass sie auch in anderen Programmen genutzt werden können. Die Unit-Tests mit `assert()` haben geholfen, Fehler zu finden. Außerdem habe ich eine Analyse mit `std::async()` und `std::future` asynchron ausgeführt und Laufzeiten mit `std::chrono::steady_clock` gemessen.

Bei einem Vergleich mit einer Wortzählung über `std::istringstream` brauchte meine Funktion für 600.000 Wörter durchschnittlich **11,26 ms**, die andere **22,58 ms**. Bei 6.000 Wörtern waren es **0,098 ms gegenüber 0,192 ms**, bei 60.000 **1,224 ms gegenüber 2,523 ms**. Meine Variante war bei diesen Tests ungefähr doppelt so schnell. Weil beide Methoden Wörter unterschiedlich erkennen, ist das aber kein allgemeingültiger Geschwindigkeitsvergleich.

**Bekannte Einschränkung:** Die Worterkennung verarbeitet UTF-8-Zeichen wie `ä`, `ö`, `ü` und `ß` nicht richtig. Das kann Wortzahlen und weitere Auswertungen verfälschen. Auch die Silbenzählung ist nur eine einfache Näherung.

Später könnte ich deshalb die Unicode-Unterstützung und Silbenzählung verbessern, eine Suche ohne Groß-/Kleinschreibung oder mit regulären Ausdrücken ergänzen und weitere Statistiken wie Wortlängenverteilungen oder Auswertungen häufiger Wortpaare hinzufügen. Dafür würde ich die neuen Funktionen in `TextAnalysis.h` deklarieren, in `TextAnalysis.cpp` umsetzen, Tests ergänzen und README sowie Beispiele anpassen. Die bestehende Projektstruktur kann dabei bleiben.

## Ergebnis und Nutzen

Die Bibliothek bietet **elf Funktionen**, die in den vorhandenen Unit-Tests erfolgreich geprüft und in zwei Beispielprogrammen gezeigt wurden. Sie erfüllt damit die wesentlichen Projektziele und lässt sich für einfache Textauswertungen, Statistiken sowie die Bereinigung von HTML- und Markdown-Texten wiederverwenden. Für eine zuverlässige Verarbeitung mehrsprachiger Texte müsste vor allem die Unicode-Unterstützung ergänzt werden.
