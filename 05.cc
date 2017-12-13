#include <stdio.h>

#include <fstream>
#include <vector>

#include "status.h"
#include "util.h"

namespace {

Status ReadFile(
    std::vector<int>* res,
    const char* filename) {
    res->clear();
    std::ifstream file(filename);
    if (!file) {
        std::string err;
        util::StringFormat(&err, "unable to open %s", filename);
        return ERR(err.c_str());
    }

    std::string line;
    while (std::getline(file, line)) {
        int val;
        Status did = util::ParseInt(line, 10, &val);
        if (!did.ok()) {
            return did;
        }

        res->push_back(val);
    }

    if (file.bad()) {
        return ERR("read error");
    }

    return NoErr();
}

int Execute(std::vector<int>& insts) {
    int index = 0;
    int count = 0;
    int size = insts.size();

    while (true) {
        if (index < 0 || index >= size) {
            return count;
        }

        int off = insts[index];
        insts[index]++;
        count++;
        index += off;
    }
}

}

int main(int argc, char* argv[]) {
    const char* filename = (argc < 2)
        ? "05.in.txt"
        : argv[1];
    std::vector<int> insts;
    Status did = ReadFile(&insts, filename);
    if (!did.ok()) {
        util::Fatal(did.what());
    }

    printf("%d\n", Execute(insts));

    return 0;
}