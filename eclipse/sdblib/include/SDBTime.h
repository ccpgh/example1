
#ifndef SDBLIB_SDBTIME_H
#define SDBLIB_SDBTIME_H

namespace sdblib {
	constexpr int CONST_MILLISECONDS_IN_MINUTE {60000};
	constexpr int CONST_MILLISECONDS_IN_SECOND {1000};

	int generateRandomMilliseconds();
	std::string now();
	void sleep(unsigned int seconds);
}

#endif // SDBLIB_SDBTIME_H
