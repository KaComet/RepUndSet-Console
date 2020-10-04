#include "StudentSet.h"

StudentSet::StudentSet(unsigned int nChars) {
    for (unsigned int i = 0; i < nChars; i++)
        testableValues.push_back(false);
}

StudentSet::StudentSet(unsigned int nChars, const std::string &fileName) {
    for (unsigned int i = 0; i < nChars; i++)
        testableValues.push_back(false);

    std::ifstream file;
    file.open(fileName);
    if (file.is_open()) {
        std::string tmp;
        file >> tmp;
        while (!file.eof()) {
            unsigned int cNumber = std::stoi(tmp);
            setTestable(cNumber, true);
            file >> tmp;
        }
    } else {
        throw std::runtime_error("File could not be opened.");
    }
}

void StudentSet::save(const std::string &outputFileName) const {
    std::ofstream file;
    file.open(outputFileName, std::ofstream::trunc | std::ofstream::out);

    for (unsigned int i = 0; i < testableValues.size(); i++) {
        if (testableValues[i])
            file << std::to_string(i) << '\n';
    }
}

bool StudentSet::isTestable(unsigned int charNumber) const {
    if (charNumber >= testableValues.size())
        throw std::out_of_range("");
    else
        return testableValues[charNumber];
}

void StudentSet::setTestable(unsigned int charNumber, bool targetState) {
    if (charNumber >= testableValues.size())
        throw std::out_of_range("");
    else
        testableValues[charNumber] = targetState;
}