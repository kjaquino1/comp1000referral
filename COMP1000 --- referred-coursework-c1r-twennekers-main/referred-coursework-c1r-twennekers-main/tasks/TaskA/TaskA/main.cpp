#include "TaskA.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

int main(int argc, char* argv[])
{
    cout << "Number of arguments: " << argc << endl;
    for (int i = 0; i < argc; ++i) {
        cout << "Argument " << i << ": " << argv[i] << endl;
    }

    if (argc == 3 || argc == 4 || argc == 5) {
        cout << "TaskA (c)2024" << endl;

        string fileName(argv[1]);
        string searchString(argv[2]);
        bool isRegex = false;
        int topN = 0;

        if (argc == 4) {
            if (string(argv[3]) == "-regex") {
                isRegex = true;
            }
            else {
                topN = stoi(argv[3]);
            }
        }
        else if (argc == 5) {
            if (string(argv[3]) == "-regex") {
                isRegex = true;
                topN = stoi(argv[4]);
            }
            else {
                cerr << "Usage: TaskA <filename> <search term> [-regex] [topN]" << endl;
                return EXIT_FAILURE;
            }
        }

        cout << "TaskA " << fileName << " " << searchString << (isRegex ? " -regex" : "") << endl;

        TextFile textFile;
        if (!textFile.load(fileName)) {
            return EXIT_FAILURE;
        }
        vector<string> lines = textFile.getLines();

        Search search(searchString, isRegex);
        search.execute(lines);
        search.displayResults();

        Statistics stats(lines);
        stats.displayWordCounts();

        double averageWordLength = stats.calculateAverageWordLength();
        cout << "Average Word Length: " << averageWordLength << endl;

        if (topN > 0) {
            vector<pair<string, int>> topWords = stats.getTopNWords(topN);
            cout << "Top " << topN << " Words:" << endl;
            for (const auto& entry : topWords) {
                cout << entry.first << ": " << entry.second << endl;
            }
        }

        int totalWords = 0;
        for (const auto& entry : stats.getWordCounts()) {
            totalWords += entry.second;
        }
        int totalSentences = Readability::countSentences(lines);
        int totalSyllables = 0;
        for (const auto& entry : stats.getWordCounts()) {
            totalSyllables += Readability::countSyllables(entry.first) * entry.second;
        }

        cout << "Total Words: " << totalWords << endl;
        cout << "Total Sentences: " << totalSentences << endl;
        cout << "Total Syllables: " << totalSyllables << endl;

        double fleschReadingEase = Readability::calculateFleschReadingEase(totalWords, totalSentences, totalSyllables);
        double fleschKincaidGradeLevel = Readability::calculateFleschKincaidGradeLevel(totalWords, totalSentences, totalSyllables);

        cout << "Flesch Reading Ease: " << fleschReadingEase << endl;
        cout << "Flesch-Kincaid Grade Level: " << fleschKincaidGradeLevel << endl;

        return EXIT_SUCCESS;
    }
    cerr << "Usage: TaskA <filename> <search term> [-regex] [topN]" << endl;
    return EXIT_FAILURE;
}