#pragma once

#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

int countWords(const std::string &text);

std::map<std::string, int>
countWordFrequency(const std::string &text,
                   const std::set<std::string> &stopWords = {});

std::string cleanText(const std::string &text);

std::string cleanHTML(const std::string &text);

std::string cleanMarkdown(const std::string &text);

std::vector<std::size_t> searchText(const std::string &text,
                                    const std::string &suchtext);

double averageWordLength(const std::string &text);

double averageSentenceLength(const std::string &text);

std::map<std::pair<std::string, std::string>, int>
countCooccurrence(const std::string &text);

int countSyllables(const std::string &wort);

double fleschReadingEase(const std::string &text);
