
#ifndef SDBLIB_SDBLIMITS_H
#define SDBLIB_SDBLIMITS_H

namespace sdblib {

class SDBLimits {
private:
	SDBLimits();
	SDBLimits(const SDBLimits&);
	SDBLimits(const SDBLimits&&);
	SDBLimits& operator=(const SDBLimits&);
	SDBLimits& operator=(const SDBLimits&&);
	~SDBLimits() {}

public:
	constexpr static int CONST_MAX_STRINGLENGTH {256};
};

}

#endif // SDBLIB_SDBLIMITS_H
