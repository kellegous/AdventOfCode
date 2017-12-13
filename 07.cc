#include <stdio.h>

#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "status.h"
#include "util.h"

namespace {

class Node {
 public:
    Node() : value_(0) {}

    int value() {
        return value_;
    }

    const std::string& name() {
        return name_;
    }

    const std::vector<std::string>& children() {
        return children_;
    }

    Status Parse(const std::string& str) {
        size_t end = str.find(" ");
        if (end == std::string::npos) {
            std::string err;
            util::StringFormat(&err, "invalid line: %s", str.c_str());
            return ERR(err.c_str());
        }

        name_ = str.substr(0, end);

        size_t beg = end + 2;
        if (beg >= str.size() || str[beg-1] != '(') {
            std::string err;
            util::StringFormat(&err, "invalid line: %s", str.c_str());
            return ERR(err.c_str());
        }

        end = str.find(")", beg);
        if (end == std::string::npos) {
            std::string err;
            util::StringFormat(&err, "invalid line: %s", str.c_str());
            return ERR(err.c_str());
        }

        Status did = util::ParseInt(
            str.substr(beg, end - beg),
            10,
            &value_);
        if (!did.ok()) {
            std::string err;
            util::StringFormat(&err,
                "invalid line (%s): %s",
                did.what(),
                str.c_str());
            return ERR(err.c_str());
        }

        beg = str.find("->", end);
        if (beg != std::string::npos) {
            util::StringSplit(
                &children_,
                str.substr(beg+2),
                ", ");
        }

        return NoErr();
    }
 private:

    void Reset() {
        name_.clear();
        value_ = 0;
        children_.clear();
    }

    std::string name_;

    int value_;

    std::vector<std::string> children_;
};

Status ReadFile(
    std::vector<std::unique_ptr<Node>>* nodes,
    const char* filename) {

    std::ifstream file(filename);
    if (!file) {
        std::string err;
        util::StringFormat(&err, "unable to open %s", filename);
        return ERR(err.c_str());
    }

    std::string line;
    while (std::getline(file, line)) {
        std::unique_ptr<Node> node(new Node());

        Status did = node->Parse(line);
        if (!did.ok()) {
            return did;
        }

        nodes->push_back(
            std::unique_ptr<Node>(node.release()));
    }

    if (file.bad()) {
        return ERR("read error");
    }

    return NoErr();
}

}

int main(int argc, char* argv[]) {
    const char* filename = (argc < 2)
        ? "07.in.txt"
        : argv[1];

    std::vector<std::unique_ptr<Node>> nodes;
    Status did = ReadFile(&nodes, filename);
    if (!did.ok()) {
        util::Fatal(did.what());
    }

    for (int i = 0, n = nodes.size(); i < n; i++) {
        printf("%s,%d\n", nodes[i]->name().c_str(), nodes[i]->value());
        const std::vector<std::string>& children = nodes[i]->children();
        for (int j = 0, m = children.size(); j < m; j++) {
            printf("    %s\n", children[j].c_str());
        }
    }

    return 0;
}