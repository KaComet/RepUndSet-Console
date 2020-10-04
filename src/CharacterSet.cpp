#include "CharacterSet.h"

CharacterSet::CharacterSet(const std::string &fileName) {
    std::ifstream file;
    file.open(fileName);
    if (file.is_open()) {
        std::string tmp;
        file >> tmp;
        while (!file.eof()) {
            Character tmpChar;
            tmpChar.character = tmp;
            file >> tmp;
            tmpChar.latinSound = tmp;
            alphabet.push_back(tmpChar);
            file >> tmp;
        }
    } else {
        throw std::runtime_error("File could not be opened.");
    }
}

Character CharacterSet::get(unsigned int charNumber) const {
    if (charNumber >= alphabet.size())
        throw std::out_of_range("");
    else
        return alphabet[charNumber];
}

unsigned int CharacterSet::length() const noexcept {
    return alphabet.size();
}