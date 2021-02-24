
#include <thread>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "SDBTime.h"

namespace sdblib {

int generateRandomMilliseconds() {
	return (std::chrono::high_resolution_clock::now().time_since_epoch() % sdblib::CONST_MILLISECONDS_IN_SECOND).count();
}

std::string now() {
	auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::ostringstream os;
	os << std::put_time(std::localtime(&now), "%Y%m%d %H:%M:%S");
	return os.str();
}

void sleep(unsigned int seconds) {
	std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

}




