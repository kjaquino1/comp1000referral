#include "TaskA.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

bool TextFile::load(const std::string& fileName) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << fileName << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        lines.push_back(line);
    }

    file.close();
    return true;
}

vector<string> TextFile::getLines() const {
    return lines;
}

Search::Search(const std::string& searchTerm, bool isRegex) : searchTerm(searchTerm), isRegex(isRegex) {}

void Search::execute(const std::vector<std::string>& lines) {
    if (isRegex) {
        regex searchPattern(searchTerm);
        for (size_t lineNumber = 0; lineNumber < lines.size(); ++lineNumber) {
            string line = lines[lineNumber];
            smatch matches;
            string::const_iterator searchStart(line.cbegin());
            while (regex_search(searchStart, line.cend(), matches, searchPattern)) {
                istringstream iss(line);
                string word;
                int wordNumber = 0;
                size_t matchPos = matches.position(0);
                size_t currentPos = 0;
                while (iss >> word) {
                    size_t wordStart = line.find(word, currentPos);
                    if (wordStart == string::npos) break;
                    wordNumber++;
                    if (matchPos >= wordStart && matchPos < wordStart + word.length()) {
                        results.push_back({ lineNumber + 1, wordNumber });
                        break;
                    }
                    currentPos = wordStart + word.length();
                }
                searchStart = matches.suffix().first;
            }
        }
    }
    else {
        for (size_t lineNumber = 0; lineNumber < lines.size(); ++lineNumber) {
            string line = lines[lineNumber];
            size_t pos = line.find(searchTerm);
            while (pos != string::npos) {
                istringstream iss(line);
                string word;
                int wordNumber = 0;
                size_t wordPos = 0;
                while (iss >> word) {
                    size_t wordStart = line.find(word, wordPos);
                    wordNumber++;
                    if (wordStart == pos) {
                        results.push_back({ lineNumber + 1, wordNumber });
                        break;
                    }
                    wordPos = wordStart + word.length();
                }
                pos = line.find(searchTerm, pos + 1);
            }
        }
    }
}

void Search::displayResults() const {
    for (const auto& result : results) {
        cout << "Match found at line " << result.first << ", word " << result.second << endl;
    }
}

Statistics::Statistics(const std::vector<std::string>& lines) {
    countWords(lines);
}

void Statistics::countWords(const std::vector<std::string>& lines) {
    for (const string& line : lines) {
        istringstream iss(line);
        string word;
        while (iss >> word) {
            transform(word.begin(), word.end(), word.begin(), ::tolower);
            word.erase(remove_if(word.begin(), word.end(), [](char c) { return !isalnum(c); }), word.end());
            wordCounts[word]++;
        }
    }
}

void Statistics::displayWordCounts() const {
    cout << "Word Counts:" << endl;
    for (const auto& entry : wordCounts) {
        cout << entry.first << ": " << entry.second << endl;
    }
}

double Statistics::calculateAverageWordLength() const {
    int totalLength = 0;
    int wordCount = 0;
    for (const auto& entry : wordCounts) {
        totalLength += entry.first.length() * entry.second;
        wordCount += entry.second;
    }
    return wordCount == 0 ? 0 : static_cast<double>(totalLength) / wordCount;
}

vector<pair<string, int>> Statistics::getTopNWords(int N) const {
    vector<pair<string, int>> words(wordCounts.begin(), wordCounts.end());
    sort(words.begin(), words.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return b.second < a.second;
        });
    if (words.size() > N) {
        words.resize(N);
    }
    return words;
}

map<string, int> Statistics::getWordCounts() const {
    return wordCounts;
}

int Readability::countSyllables(const std::string& word) {
    int syllables = 0;
    bool lastWasVowel = false;
    for (char c : word) {
        c = tolower(c);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'y') {
            if (!lastWasVowel) {
                syllables++;
                lastWasVowel = true;
            }
        }
        else {
            lastWasVowel = false;
        }
    }
    if (!word.empty() && (word.back() == 'e' || word.back() == 'E') && syllables > 1) {
        syllables--;
    }
    return syllables;
}

int Readability::countSentences(const std::vector<std::string>& lines) {
    int sentences = 0;
    for (const std::string& line : lines) {
        sentences += std::count_if(line.begin(), line.end(), [](char c) {
            return c == '.' || c == '!' || c == '?';
            });
    }
    return sentences;
}

double Readability::calculateFleschReadingEase(int totalWords, int totalSentences, int totalSyllables) {
    if (totalWords == 0 || totalSentences == 0) return 0;
    return 206.835 - 1.015 * (static_cast<double>(totalWords) / totalSentences) - 84.6 * (static_cast<double>(totalSyllables) / totalWords);
}

double Readability::calculateFleschKincaidGradeLevel(int totalWords, int totalSentences, int totalSyllables) {
    if (totalWords == 0 || totalSentences == 0) return 0;
    return 0.39 * (static_cast<double>(totalWords) / totalSentences) + 11.8 * (static_cast<double>(totalSyllables) / totalWords) - 15.59;
}