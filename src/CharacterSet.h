#ifndef REPUNDSET_CHARACTERSET_H
#define REPUNDSET_CHARACTERSET_H

#include <exception>
#include <string>
#include <vector>
#include <fstream>

struct Character {
    std::string character, latinSound;

    bool operator==(const Character &b) const {
        return (b.latinSound == latinSound) && (b.character == character);
    }
};

class CharacterSet {
public:
    CharacterSet() = delete;

    explicit CharacterSet(const std::string &fileName);

    Character get(unsigned int charNumber) const;

    unsigned int length() const noexcept;

private:
    std::vector<Character> alphabet;
};


#endif //REPUNDSET_CHARACTERSET_H
