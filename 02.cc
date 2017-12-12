#include <stdio.h>

#include <climits>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "util.h"

namespace {

bool ParseInt(const std::string& s, int base, int* v) {
    char* end = nullptr;
    *v = strtol(s.c_str(), &end, base);
    return *end ? false : true;
}

bool LineToInts(std::vector<int>* result, const std::string& line) {
    result->clear();

    size_t beg = 0;
    size_t end = 0;
    int val;

    while ((end = line.find("\t", beg)) != std::string::npos) {
        if (!ParseInt(line.substr(beg, end - beg), 10, &val)) {
            return false;
        }
        result->push_back(val);
        beg = end + 1;
    }

    if (!ParseInt(line.substr(beg, end), 10, &val)) {
        return false;
    }
    result->push_back(val);
    return true;
}

bool Sum(int* val, const char* filename) {
    *val = 0;

    std::ifstream file(filename);
    if (!file) {
        return false;
    }

    std::string line;
    std::vector<int> vals;
    while (std::getline(file, line)) {
        if (!LineToInts(&vals, line)) {
            return false;
        }

        int min = INT_MAX;
        int max = INT_MIN;

        for (int i = 0, n = vals.size(); i < n; i++) {
            if (vals[i] > max) {
                max = vals[i];
            }
            if (vals[i] < min) {
                min = vals[i];
            }
        }

        *val += max - min;
    }

    return true;
}

}

int main(int argc, char* argv[]) {
    const char* filename = (argc < 2)
        ? "02.in.txt"
        : argv[1];
    int sum;
    if (!Sum(&sum, filename)) {
        util::Fatal("sum failed");
    }
    printf("%d\n", sum);
}