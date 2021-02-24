
#include <algorithm>
#include <functional>
#include <cstring>
#include <memory>

#include "SDBString.h"

namespace sdblib {

std::string SDBString::ltrim(const std::string& string) {
	std::string s{string};
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

std::string SDBString::rtrim(const std::string& string) {
	std::string s{string};
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());

    return s;
}

std::string SDBString::trim(const std::string& string) {
	std::string s{string};
    return ltrim(rtrim(s));
}

std::string SDBString::trim(const char* const string) {
	std::string s{string};
	return trim(s);
}

std::string SDBString::basename(const char* const string) {
	if (!string ||
			!std::strlen(string)) {
		return "";
	}

	std::string s{string};
	size_t position{s.find_last_of('/')};
	if (position == std::string::npos) {
		return s;
	}

	return s.substr(position + 1);
}

char* SDBString::clone(const std::string& string) {
	const size_t length{string.length()};
	char* buffer{new char[length+1]};
	memset(buffer, 0, sizeof(char)*(length+1));
	strncpy(buffer, string.c_str(), length);
	return buffer;
}

}

