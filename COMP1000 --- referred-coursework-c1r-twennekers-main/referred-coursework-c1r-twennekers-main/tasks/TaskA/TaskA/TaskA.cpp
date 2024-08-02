#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <regex>
#include <map>
#include <string>
using namespace std;

//See bottom of main
int findArg(int argc, char* argv[], string pattern);


/*
 *
 * The user can launch application as follows:
 *
 * TaskA <filename> <search term> [-regex]
 * 
 * <database file>              REQUIRED. Specifies the file to search (required). This is ALWAYS the first parameter. The file must exist in order to be searched
 * <search term>                REQUIRED. The search term as either a single word or a regular expression. 
                                This is a single word, made up of alpha-numeric characters only.
 * -regex                       OPTIONAL. If this flag is present, then the search term is a regular expression (as opposed to a single word). 
                                It must be a valid regex expression.
 *
 * ****************
 * *** EXAMPLES ***
 * ****************
 *
 * TaskA lorum.txt comp1000             Searches for the string comp1000 in the file lorum.txt
 * TaskA lorum.txt "^(\\d)\\d" -regex   Searches the file lorum.txt for all patterns that match the regular expression "^(\\d)\\d" 
 * TaskA lorum.txt -regex "^(\\d)\\d"   Searches the file lorum.txt for all patterns that match the regular expression "^(\\d)\\d" 
 * TaskA lorum.txt                      Error - search expression provided
 * 
 * *************
 * *** NOTES ***
 * *************
 *
 * o Try to write your code such that is can be reused in other tasks.
 * o Code should be consistently indented and commented
 * o Consider error conditions, such as missing parameters or non-existent files
*/


int main(int argc, char* argv[])
{
    // argv is an array of strings, where argv[0] is the path to the program, argv[1] is the first parameter, ...
    // argc is the number of strings in the array argv
    // These are passed to the application as command line arguments
    // Return value should be EXIT_FAILURE if the application exited with any form of error, or EXIT_SUCCESS otherwise

    cout << "Number of arguments: " << argc << endl;
    for (int i = 0; i < argc; ++i) {
        cout << "Argument " << i << ": " << argv[i] << endl;
    }

    if (argc == 3 || argc == 4) {
        //Welcome message
        cout << "TaskA (c)2024" << endl;

        //BASIC EXAMPLE: Get parameters for the simple case
        string fileName(argv[1]);
        string searchString(argv[2]);
        bool isRegex = false;

        if (argc == 4 && string(argv[3]) == "-regex") {
            isRegex = true;
        }

        //Confirm
        cout << "TaskA " << fileName << " " << searchString << (isRegex ? " -regex" : "") << endl;

        ifstream file(fileName);
		if (!file.is_open()) {
			cerr << "Error: Could not open file " << fileName << endl;
			return EXIT_FAILURE;
        }

        string line;
        int lineNumber = 0;
        regex searchPattern(searchString);
        while (getline(file, line)) {
            lineNumber++;
            if (isRegex) {
                smatch matches;
                string::const_iterator searchStart(line.cbegin());
                while (regex_search(searchStart, line.cend(), matches, searchPattern)) {
                    // Find the word number
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
                            cout << "Match found at line " << lineNumber << ", word " << wordNumber << ": " << matches[0] << endl;
                            break;
                        }
                        currentPos = wordStart + word.length();
                    }
                    searchStart = matches.suffix().first;
                }
            }
            else {
                size_t pos = line.find(searchString);
                while (pos != string::npos) {
                    // Find the word number
                    istringstream iss(line);
                    string word;
                    int wordNumber = 0;
                    size_t wordPos = 0;
                    while (iss >> word) {
                        size_t wordStart = line.find(word, wordPos);
                        wordNumber++;
                        if (wordStart == pos) {
                            cout << "Match found at line " << lineNumber << ", word " << wordNumber << ": " << searchString << endl;
                            break;
                        }
                        wordPos = wordStart + word.length();
                    }
                    pos = line.find(searchString, pos + 1);
                }
            }
        }
        
        file.close();
        
        //Done
        return EXIT_SUCCESS;
    }

    //EXAMPLE: Scan command line for -regex switch
    int p = findArg(argc, argv, "-regex");
    if (p) {
        cout << "The search term is a regular expression. See https://www.softwaretestinghelp.com/regex-in-cpp/ for examples of how to do this " << endl;
    }

    //**************************************************************
    //You could continue here :)
    //**************************************************************

    return EXIT_SUCCESS;
}

// Find an argument on the command line and return the location
int findArg(int argc, char* argv[], string pattern)
{
    for (int n = 1; n < argc; n++) {
        string s1(argv[n]);
        if (s1 == pattern) {
            return n;
        }
    }
    return 0;
}
