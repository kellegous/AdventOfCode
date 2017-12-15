#include <stdio.h>

#include <fstream>
#include <vector>

#include "status.h"
#include "util.h"

namespace {

Status ReadFile(int* size, std::vector<int>* data, const char* filename) {
    data->clear();
    *size = 0;

    std::ifstream file(filename);
    if (!file) {
        std::string err;
        util::StringFormat(&err, "unable to open %s", filename);
        return ERR(err.c_str());
    }

    std::string cnts;

    file.seekg(0, std::ios::end);
    if (file.bad()) {
        return ERR("seek error");
    }
    cnts.reserve(file.tellg());
    file.seekg(0, std::ios::beg);
    if (file.bad()) {
        return ERR("seek error");
    }

    cnts.assign(
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>());

    std::vector<std::string> items;
    util::StringSplit(&items, cnts, ",");
    if (items.empty()) {
        return ERR("invalid empty file");
    }

    Status did = util::ParseInt(items[0], 10, size);
    if (!did.ok()) {
        return did;
    }

    data->reserve(items.size() - 1);
    for (int i = 1, n = items.size(); i < n; i++) {
        int val;
        Status did = util::ParseInt(items[i], 10, &val);
        if (!did.ok()) {
            return did;
        }
        data->push_back(val);
    }

    return NoErr();
}

void Init(std::vector<int>* data, int size) {
    data->clear();
    data->reserve(size);
    for (int i = 0; i < size; i++) {
        data->push_back(i);
    }
}

void Reverse(std::vector<int>* list, int ix, int n) {
    int ln = list->size();
    int tmp;
    for (int i = 0, m = n / 2; i < m; i++) {
        int bg = (ix + i) % ln;
        int en = (ix + n - i - 1) % ln;
        tmp = (*list)[bg];
        (*list)[bg] = (*list)[en];
        (*list)[en] = tmp;
    }
}

void Hash(std::vector<int>* list, const std::vector<int>& lens) {
    int ix = 0;
    int skip_len = 0;
    for (int i = 0, n = lens.size(); i < n; i++) {
        Reverse(list, ix, lens[i]);
        ix = (ix + lens[i] + skip_len) % list->size();
        skip_len++;
    }
}

}

int main(int argc, char* argv[]) {
    const char* filename = (argc < 2)
        ? "10.in.txt"
        : argv[1];
    int size;
    std::vector<int> lens;
    Status did = ReadFile(&size, &lens, filename);
    if (!did.ok()) {
        util::Fatal(did.what());
    }

    std::vector<int> list;
    Init(&list, size);
    Hash(&list, lens);

    printf("%d\n", list[0]*list[1]);

    return 0;
}