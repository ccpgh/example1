
#ifndef SDBLIB_SDBSTRING_H
#define SDBLIB_SDBSTRING_H

#include <string>

namespace sdblib {

class SDBString {
private:
	SDBString();
	SDBString(const SDBString&);
	SDBString(const SDBString&&);
	SDBString& operator=(const SDBString&);
	SDBString& operator=(const SDBString&&);
	~SDBString() {}

	static std::string ltrim(const std::string& string);
	static std::string rtrim(const std::string& string);

public:
	static std::string trim(const std::string& string);
	static std::string trim(const char* const string);
	static std::string basename(const char* const string);
	static char* clone(const std::string& string);
};

}

#endif // SDBLIB_SDBSTRING_H
