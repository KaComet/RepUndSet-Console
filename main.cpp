#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include "src/StudentSet.h"
#include "src/CharacterSet.h"

bool quiz(CharacterSet &characterSet, StudentSet &studentSet, unsigned int nCharacters, const std::string &exitString);

unsigned int lowerRandom(unsigned int min, unsigned int max);

void runQuizes(CharacterSet &characterSet, StudentSet &studentSet);

void printSet(CharacterSet &characterSet, StudentSet &studentSet, unsigned int tableWidth);

void clearTerm();

void settingMenu(CharacterSet &characterSet, StudentSet &studentSet, const std::string &studentFileName);

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cout << "Invalid input(s).\nExample usage: RepUndSet {Language File} {Student File}\n";
        exit(1);
    }
    std::string languageFilePath(argv[1]);
    std::string studentFilePath(argv[2]);

    CharacterSet languageSet(languageFilePath);
    StudentSet student(languageSet.length(), studentFilePath);

    bool hasExited = false;
    std::srand(time(nullptr));

    while (!hasExited) {
        std::cout << "-- RepUndSet\n"
                  << "-- 1) Begin Quizzing\n"
                  << "-- 2) Settings\n"
                  << "-- 3) Quit\n"
                  << "\n"
                  << ":";

        std::string tmp;
        std::cin >> tmp;
        unsigned int selection = 0;

        try {
            selection = std::stoi(tmp);
        } catch (...) {
            std::cout << "Please enter a number.\n";
            clearTerm();
            continue;
        }

        switch (selection) {
            case 1:
                runQuizes(languageSet, student);
                continue;
                break;
            case 2:
                settingMenu(languageSet, student, studentFilePath);
                continue;
                break;
            case 3:
                hasExited = true;
                continue;
                break;
            default:
                std::cout << "Please enter a valid menu entry.\n";
                clearTerm();
                continue;
        }
    }

    return 0;
}

unsigned int lowerRandom(unsigned int min, unsigned int max) {
    unsigned int a = (rand() % (max - min)) + min;
    unsigned int b = (rand() % (max - min)) + min;

    if (a > b)
        return b;
    else
        return a;
}

void runQuizes(CharacterSet &characterSet, StudentSet &studentSet) {
    while (quiz(characterSet, studentSet, lowerRandom(1, 10), "quit")) {
        clearTerm();
    }
}

bool quiz(CharacterSet &characterSet, StudentSet &studentSet, unsigned int nCharacters, const std::string &exitString) {
    std::vector<Character> questionData;

    for (unsigned int i = 0; i < nCharacters; i++) {
        bool foundValidChar = false;
        while (!foundValidChar) {
            unsigned int tmp = rand() % characterSet.length();
            if (studentSet.isTestable(tmp)) {
                questionData.push_back(characterSet.get(tmp));
                foundValidChar = true;
            }
        }
    }

    std::cout << "Translate the following:\n";

    for (auto &i : questionData)
        std::cout << i.character;

    std::cout << '\n';

    bool passed = true;
    for (auto &i : questionData) {
        std::string tmp;
        std::cin >> tmp;
        if (tmp == exitString)
            return false;

        if (tmp != i.latinSound)
            passed = false;
    }

    if (passed)
        std::cout << "Correct!\n";
    else
        std::cout << "Incorrect.\n";

    std::cout << "\n\n";

    return true;
}

void printSet(CharacterSet &characterSet, StudentSet &studentSet, unsigned int tableWidth) {
    unsigned int currentWidth = 0;
    for (unsigned int i = 0; i < characterSet.length(); i++) {
        if (currentWidth++ >= tableWidth) {
            std::cout << '\n';
            currentWidth = 0;
        }

        if (studentSet.isTestable(i))
            std::cout << "| " << "✔ " << i << ": " << characterSet.get(i).character << '\t';
        else
            std::cout << "| " << "✘ " << i << ": " << characterSet.get(i).character << '\t';
    }
}

void clearTerm() {
    std::cout << "\n\n";
}

void settingMenu(CharacterSet &characterSet, StudentSet &studentSet, const std::string &studentFileName) {
    bool hasExited = false;

    while (!hasExited) {
        printSet(characterSet, studentSet, 7);

        std::string tmp;
        std::cout << "\nWhat character do want to enable/disable?\nCharacter number: ";
        std::cin >> tmp;
        if (tmp == "quit") {
            std::cout << "\n\n";

            bool saveStateGiven = false;

            while (!saveStateGiven) {
                std::cout << "Save? (Y/N): ";
                std::string tmp2;
                std::cin >> tmp2;

                if (tmp2 == "Y") {
                    studentSet.save(studentFileName);
                    saveStateGiven = true;
                    continue;
                } else if (tmp2 == "N") {
                    saveStateGiven = true;
                    continue;
                }
            }

            hasExited = true;
            continue;
        }

        unsigned int i = std::stoi(tmp);
        if (i >= characterSet.length()) {
            std::cout << "Not a valid character.\n\n";
            continue;
        }

        studentSet.setTestable(i, !studentSet.isTestable(i));

        std::cout << "\n\n";
    }
}
