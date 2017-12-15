#include <stdio.h>

#include <fstream>
#include <string>

#include "status.h"
#include "util.h"

namespace {

Status ReadFile(std::string* data, const char* filename) {
    std::ifstream file(filename);
    if (!file) {
        std::string err;
        util::StringFormat(&err, "unable to open file %s", filename);
        return ERR(err.c_str());
    }

    file.seekg(0, std::ios::end);
    if (file.bad()) {
        return ERR("seek error");
    }
    data->reserve(file.tellg());
    file.seekg(0, std::ios::beg);
    if (file.bad()) {
        return ERR("seek error");
    }

    data->assign(
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>());
    
    return NoErr();
}

int ConsumeGarbage(const char* data, int n) {
    bool esc = false;
    for (int i = 1; i < n; i++) {
        if (esc) {
            esc = false;
            continue;
        }

        switch (data[i]) {
        case '>':
            return i;
        case '!':
            esc = true;
            break;
        }
    }

    return n;
}

int Evaluate(const char* data, int n) {
    int score = 0;
    int level = 1;
    for (int i = 0; i < n; i++) {
        switch (data[i]) {
        case '{':
            score += level;
            level++;
            break;
        case '}':
            level--;
            break;
        case ',':
            break;
        case '<':
            i += ConsumeGarbage(&data[i], n - i);
            break;
        }
    }

    return score;
}

}

int main(int argc, char* argv[]) {
    const char* filename = (argc < 2)
        ? "09.in.txt"
        : argv[1];

    std::string data;
    Status did = ReadFile(&data, filename);
    if (!did.ok()) {
        util::Fatal(did.what());
    }

    printf("%d\n", Evaluate(data.c_str(), data.size()));

    return 0;
}