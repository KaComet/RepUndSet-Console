#ifndef REPUNDSET_STUDENTSET_H
#define REPUNDSET_STUDENTSET_H

#include <exception>
#include <string>
#include <fstream>
#include <vector>

class StudentSet {
public:
    StudentSet() = delete;

    StudentSet(unsigned int nChars);

    StudentSet(unsigned int nChars, const std::string &fileName);

    void save(const std::string &outputFileName) const;

    bool isTestable(unsigned int charNumber) const;

    void setTestable(unsigned int charNumber, bool targetState);

private:
    std::vector<bool> testableValues;
};


#endif //REPUNDSET_STUDENTSET_H
