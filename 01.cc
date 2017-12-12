#include <stdio.h>

#include <fstream>
#include <string>

#include "util.h"

namespace {

int ToInt(uint8_t c) {
    return c - 48;
}

int Solve(const std::string& data) {
    int sum = 0;
    for (int i = 0, n = data.size(); i < n; i++) {
        int j = (i + 1) % n;
        if (data[i] == data[j]) {
            sum += ToInt(data[i]);
        }
    }
    return sum;
}

Status ReadFile(std::string* data, const char* filename) {
    data->clear();

    std::ifstream file(filename);
    if (!file) {
        std::string err;
        util::StringFormat(&err, "unable to open %s", filename);
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

}

int main(int argc, char* argv[]) {
    const char* filename = (argc < 2)
        ? "01.in.txt"
        : argv[1];

    std::string data;
    Status did = ReadFile(&data, filename);
    if (!did.ok()) {
        util::Fatal(did.what());
    }

    printf("%d\n", Solve(data));
    return 0;
}
