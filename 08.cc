#include <stdio.h>

#include <climits>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "status.h"
#include "util.h"

namespace {

enum Op {
    EqualTo,
    NotEqualTo,
    GreaterThan,
    LessThan,
    GreaterThanOrEqual,
    LessThanOrEqual
};

bool ParseMod(
    int* val,
    const std::string& opstr,
    const std::string& valstr) {
    int v;
    Status did = util::ParseInt(valstr, 10, &v);
    if (!did.ok()) {
        return false;
    }

    if (opstr.compare("dec") == 0) {
        *val = -v;
    } else if (opstr.compare("inc") == 0) {
        *val = v;
    } else {
        return false;
    }

    return true;
}

bool ParseCond(
    Op* op,
    int* val,
    const std::string& opstr,
    const std::string& valstr) {
    Status did = util::ParseInt(valstr, 10, val);
    if (!did.ok()) {
        return false;
    }

    if (opstr.compare("==") == 0) {
        *op = EqualTo;
    } else if (opstr.compare("!=") == 0) {
        *op = NotEqualTo;
    } else if (opstr.compare(">") == 0) {
        *op = GreaterThan;
    } else if (opstr.compare("<") == 0) {
        *op = LessThan;
    } else if (opstr.compare(">=") == 0) {
        *op = GreaterThanOrEqual;
    } else if (opstr.compare("<=") == 0) {
        *op = LessThanOrEqual;
    } else {
        return false;
    }

    return true;
}

bool ConditionIsMet(Op op, int a, int b) {
    switch (op) {
    case EqualTo:
        return a == b;
    case NotEqualTo:
        return a != b;
    case GreaterThan:
        return a > b;
    case LessThan:
        return a < b;
    case GreaterThanOrEqual:
        return a >= b;
    case LessThanOrEqual:
        return a <= b;
    }

    util::Fatal("invalid op");
    return false;
}

class Stmt {
 public:
    Stmt()
        : mod_val_(0)
        , cond_op_(GreaterThan)
        , cond_val_(0) {}

    Status Parse(const std::string& str) {
        std::vector<std::string> tokens;
        util::StringSplit(&tokens, str, " ");

        if (tokens.size() != 7) {
            std::string err;
            util::StringFormat(&err, "invalid stmt: %s", str.c_str());
            return ERR(err.c_str());
        }

        mod_reg_ = tokens[0];
        if (!ParseMod(&mod_val_, tokens[1], tokens[2])) {
            std::string err;
            util::StringFormat(&err, "invalid stmt: %s", str.c_str());
            return ERR(err.c_str());
        }

        // TODO(knorton): Check for "if" in tokens[3].

        cond_reg_ = tokens[4];
        if (!ParseCond(&cond_op_, &cond_val_, tokens[5], tokens[6])) {
            std::string err;
            util::StringFormat(&err, "invalid stmt: %s", str.c_str());
            return ERR(err.c_str());
        }

        return NoErr();
    }

    void Apply(std::unordered_map<std::string, int>* regs) {
        int cond_val = 0;
        std::unordered_map<std::string, int>::const_iterator cond_it = regs->find(cond_reg_);
        if (cond_it != regs->end()) {
            cond_val = cond_it->second;
        }

        int mod_val = 0;
        std::unordered_map<std::string, int>::const_iterator mod_it = regs->find(mod_reg_);
        if (mod_it != regs->end()) {
            mod_val = mod_it->second;
        }

        if (ConditionIsMet(cond_op_, cond_val, cond_val_)) {
            (*regs)[mod_reg_] = mod_val + mod_val_;
        }
    }

    void ToString(std::string* str) {
        str->clear();
        util::StringFormat(
            str,
            "%s %d, %s %d %d",
            mod_reg_.c_str(),
            mod_val_,
            cond_reg_.c_str(),
            cond_op_,
            cond_val_);
    }
 private:
    std::string mod_reg_;
    int mod_val_;

    Op cond_op_;
    std::string cond_reg_;
    int cond_val_;
};

Status ReadFile(
    std::vector<std::unique_ptr<Stmt>>* stmts,
    const char* filename) {
    std::ifstream file(filename);
    if (!file) {
        std::string err;
        util::StringFormat(&err, "unable to open %s", filename);
        return ERR(err.c_str());
    }

    std::string line;
    while (std::getline(file, line)) {
        std::unique_ptr<Stmt> stmt(new Stmt());
        Status did = stmt->Parse(line);
        if (!did.ok()) {
            return did;
        }

        stmts->push_back(
            std::unique_ptr<Stmt>(stmt.release()));
    }

    if (file.bad()) {
        return ERR("read error");
    }

    return NoErr();
}

int FindMax(const std::unordered_map<std::string, int>& regs) {
    int max = INT_MIN;
    for (auto it = regs.begin(); it != regs.end(); ++it) {
        if (it->second > max) {
            max = it->second;
        }
    }
    return max;
}

}

int main(int argc, char* argv[]) {
    const char* filename = (argc < 2)
        ? "08.in.txt"
        : argv[1];
    std::vector<std::unique_ptr<Stmt>> stmts;
    Status did = ReadFile(&stmts, filename);
    if (!did.ok()) {
        util::Fatal(did.what());
    }

    std::unordered_map<std::string, int> regs;
    for (int i = 0, n = stmts.size(); i < n; i++) {
        stmts[i]->Apply(&regs);
    }

    printf("%d\n", FindMax(regs));

    return 0;
}