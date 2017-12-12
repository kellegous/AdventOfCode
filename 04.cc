#include <stdio.h>

#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>

#include "util.h"
#include "status.h"

namespace {

void Split(std::vector<std::string>* results,
	const std::string& sub,
	const std::string& sep) {
	size_t beg = 0;
	size_t end = 0;
	results->clear();
	while ((end = sub.find(sep, beg)) != std::string::npos) {
		results->push_back(sub.substr(beg, end - beg));
		beg = end + 1;
	}
	results->push_back(sub.substr(beg, end));
}

bool IsValidPassphrase(const std::string& phrase) {
	std::vector<std::string> words;
	Split(&words, phrase, " ");

	std::unordered_set<std::string> set;
	for (int i = 0, n = words.size(); i < n; i++) {
		if (!set.insert(words[i]).second) {
			return false;
		}
	}

	return true;
}

Status Process(int* count, const char* filename) {
	*count = 0;

	std::ifstream file(filename);
	if (!file) {
		std::string err;
		util::StringFormat(&err, "unable to open %s", filename);
		return ERR(err.c_str());
	}

	int c = 0;
	std::string line;
	while (std::getline(file, line)) {
		if (IsValidPassphrase(line)) {
			c++;
		}
	}

	if (file.bad()) {
		return ERR("read error");
	}

	*count = c;
	return NoErr();
}

}

int main(int argc, char* argv[]) {
	const char* filename = (argc < 2)
		? "04.in.txt"
		: argv[1];

	int count = 0;
	Status did = Process(&count, filename);
	if (!did.ok()) {
		util::Fatal(did.what());
	}

	printf("%d\n", count);

	return 0;
}