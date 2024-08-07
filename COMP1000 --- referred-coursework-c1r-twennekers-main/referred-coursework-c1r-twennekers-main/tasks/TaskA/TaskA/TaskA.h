#ifndef TASKA_H
#define TASKA_H

#include <vector>
#include <string>
#include <map>
#include <regex>

class TextFile {
public:
    bool load(const std::string& fileName);
    std::vector<std::string> getLines() const;

private:
    std::vector<std::string> lines;
};

class Search {
public:
    Search(const std::string& searchTerm, bool isRegex);
    void execute(const std::vector<std::string>& lines);
    void displayResults() const;

private:
    std::string searchTerm;
    bool isRegex;
    std::vector<std::pair<int, int>> results; // line number, word number
};

class Statistics {
public:
    Statistics(const std::vector<std::string>& lines);
    void displayWordCounts() const;
    double calculateAverageWordLength() const;
    std::vector<std::pair<std::string, int>> getTopNWords(int N) const;
    std::map<std::string, int> getWordCounts() const;

private:
    std::map<std::string, int> wordCounts;
    void countWords(const std::vector<std::string>& lines);
};

class Readability {
public:
    static int countSyllables(const std::string& word);
    static int countSentences(const std::vector<std::string>& lines);
    static double calculateFleschReadingEase(int totalWords, int totalSentences, int totalSyllables);
    static double calculateFleschKincaidGradeLevel(int totalWords, int totalSentences, int totalSyllables);
};

#endif // TASKA_H
