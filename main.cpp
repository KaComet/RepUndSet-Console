#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <optional>
#include "src/StudentSet.h"
#include "src/CharacterSet.h"

bool quiz(CharacterSet &characterSet, StudentSet &studentSet, unsigned int nCharacters, const std::string &exitString);

unsigned int lowerRandom(unsigned int min, unsigned int max);

void runQuizes(CharacterSet &characterSet, StudentSet &studentSet);

void printSet(CharacterSet &characterSet, StudentSet &studentSet, unsigned int tableWidth);

void clearTerm();

void settingMenu(CharacterSet &characterSet, StudentSet &studentSet, const std::string &studentFileName);

template<class T>
void uniqueAddToVector(std::vector<T> &vector, T &element) {
    bool notFound = true;
    for (auto i : vector) {
        if (i == element) {
            notFound = false;
            break;
        }
    }

    if (notFound)
        vector.push_back(element);
}

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cout << "Invalid input(s).\nExample usage: RepUndSet-Console {Language File} {Student File}\n";
        exit(1);
    }

    std::string languageFilePath(argv[1]);
    std::string studentFilePath(argv[2]);

    std::optional<CharacterSet> languageSet = std::nullopt;
    std::optional<StudentSet> student = std::nullopt;

    try {
        languageSet = CharacterSet(languageFilePath);
    } catch (std::runtime_error &e) {
        std::cout << "Could not load character set. Exiting...\n";
        exit(1);
    }

    try {
        student = StudentSet(languageSet.value().length(), studentFilePath);
    } catch (std::runtime_error &e) {
        std::cout << "Could not load student file. Exiting...\n";
        exit(1);
    }

    bool hasExited = false;
    std::srand(time(nullptr));

    while (!hasExited) {
        std::cout << "-- RepUndSet-Console\n"
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
                runQuizes(languageSet.value(), student.value());
                continue;
                break;
            case 2:
                settingMenu(languageSet.value(), student.value(), studentFilePath);
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

    bool hasCorrectlyAnswered = false;

    while (!hasCorrectlyAnswered) {
        std::vector<Character> wrongAnswers;

        std::cout << "Translate the following:\n";

        for (auto &i : questionData)
            std::cout << i.character;

        std::cout << '\n';

        bool passed = true;
        for (auto &i : questionData) {
            std::string tmp;
            std::cin >> tmp;
            if (tmp == exitString) {
                std::cout << "\n\n";
                return false;
            }

            if (tmp != i.latinSound) {
                uniqueAddToVector<Character>(wrongAnswers, i);
                passed = false;
            }
        }

        if (passed) {
            std::cout << "Correct!\n";
            hasCorrectlyAnswered = true;
        } else {
            std::cout << "Incorrect.\n\n";
            for (Character c : wrongAnswers)
                std::cout << c.character << ": " << c.latinSound << '\n';
            std::cout << '\n';
            std::shuffle(questionData.begin(), questionData.end(), std::mt19937(std::random_device()()));
            continue;
        }

        std::cout << "\n\n";
    }

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
        printSet(characterSet, studentSet, 5);

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
        unsigned int i;

        try {
            i = std::stoi(tmp);
        } catch (std::invalid_argument &e) {
            continue;
        }

        if (i >= characterSet.length()) {
            std::cout << "Not a valid character.\n\n";
            continue;
        }

        studentSet.setTestable(i, !studentSet.isTestable(i));

        std::cout << "\n\n";
    }
}
