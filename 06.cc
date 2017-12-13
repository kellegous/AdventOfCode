#include <stdio.h>

#include <fstream>
#include <vector>
#include <unordered_set>

#include "status.h"
#include "util.h"

namespace {

Status ReadFile(std::vector<int>* res, const char* filename) {
    res->clear();

    std::ifstream file(filename);
    if (!file) {
        std::string err;
        util::StringFormat(&err, "unable to open %s", filename);
        return ERR(err.c_str());
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<int> data;

        size_t beg = 0;
        size_t end = 0;
        int val;

        while ((end = line.find("\t", beg)) != std::string::npos) {
            Status did = util::ParseInt(line.substr(beg, end - beg), 10, &val);
            if (!did.ok()) {
                return did;
            }

            res->push_back(val);
            beg = end + 1;
        }

        Status did = util::ParseInt(line.substr(beg, end), 10, &val);
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

int FindLargest(const std::vector<int>& sectors) {
    int index = 0;
    for (int i = 1, n = sectors.size(); i < n; i++) {
        if (sectors[i] > sectors[index]) {
            index = i;
        }
    }
    return index;
}

void Shuffle(std::vector<int>& sectors) {
    int index = FindLargest(sectors);
    int count = sectors[index];
    sectors[index] = 0;
    for (int i = 0; i < count; i++) {
        index = (index + 1) % sectors.size();
        sectors[index]++;
    }
}

int Realloc(std::vector<int>& sectors) {
    int count = 0;
    std::unordered_set<std::string> set;

    while (true) {
        const char* data = reinterpret_cast<const char*>(sectors.data());
        std::string key(data, sectors.size() * sizeof(int));
        if (!set.insert(key).second) {
            return count;
        }

        Shuffle(sectors);
        count++;
    }
}

}

int main(int argc, char* argv[]) {
    const char* filename = (argc < 2)
        ? "06.in.txt"
        : argv[1];

    std::vector<int> sectors;
    Status did = ReadFile(&sectors, filename);
    if (!did.ok()) {
        util::Fatal(did.what());
    }

    printf("%d\n", Realloc(sectors));

    return 0;
}