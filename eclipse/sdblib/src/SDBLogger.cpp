
#include <iostream>
#include <sstream>

#include "SDBConfiguration.h"
#include "SDBLogger.h"

namespace sdblib {

class SDBLoggerErrorAccess {
public:
	static SDBLogger<SDBLogLevelError>& getError() {
		return SDBLogger<SDBLogLevelError>::logger;
	}
};

class SDBLoggerInfoAccess {
public:
	static SDBLogger<SDBLogLevelInfo>& getInfo() {
		return SDBLogger<SDBLogLevelInfo>::logger;
	}
};

class SDBLoggerDebugAccess {
public:
	static SDBLogger<SDBLogLevelDebug>& getDebug() {
		return SDBLogger<SDBLogLevelDebug>::logger;
	}
};

class SDBLoggerTraceAccess {
public:
	static SDBLogger<SDBLogLevelTrace>& getTrace() {
		return SDBLogger<SDBLogLevelTrace>::logger;
	}
};

class SDBLoggerDevelopmentAccess {
public:
	static SDBLogger<SDBLogLevelDevelopment>& getTrace() {
		return SDBLogger<SDBLogLevelDevelopment>::logger;
	}
};

template<typename T>
SDBLogger<T> SDBLogger<T>::logger;

SDBLogger<SDBLogLevelError>& error{SDBLoggerErrorAccess::getError()};
SDBLogger<SDBLogLevelInfo>& info{SDBLoggerInfoAccess::getInfo()};
SDBLogger<SDBLogLevelDebug>& debug{SDBLoggerDebugAccess::getDebug()};
SDBLogger<SDBLogLevelTrace>& trace{SDBLoggerTraceAccess::getTrace()};
SDBLogger<SDBLogLevelDevelopment>& development{SDBLoggerDevelopmentAccess::getTrace()};

template<>
bool SDBLogger<SDBLogLevelError>::do_flush() {
	SDBConfiguration* configuration {SDBConfiguration::getConfiguration()};
	if (configuration == nullptr) {
		this->clear();
	} else {
		if (configuration->getIsLogLevelError()) {
			this->flush();
		} else {
			this->clear();
		}
	}

	return true;
}

template<>
bool SDBLogger<SDBLogLevelInfo>::do_flush() {
	SDBConfiguration* configuration {SDBConfiguration::getConfiguration()};
	if (configuration == nullptr) {
		this->clear();
	} else {
		if (configuration->getIsLogLevelInfo()) {
			this->flush();
		} else {
			this->clear();
		}
	}

	return true;
}

template<>
bool SDBLogger<SDBLogLevelDebug>::do_flush() {
	SDBConfiguration* configuration {SDBConfiguration::getConfiguration()};
	if (configuration == nullptr) {
		this->clear();
	} else {
		if (configuration->getIsLogLevelDebug()) {
			this->flush();
		} else {
			this->clear();
		}
	}

	return true;
}

template<>
bool SDBLogger<SDBLogLevelTrace>::do_flush() {
	SDBConfiguration* configuration {SDBConfiguration::getConfiguration()};
	if (configuration == nullptr) {
		this->clear();
	} else {
		if (configuration->getIsLogLevelTrace()) {
			this->flush();
		} else {
			this->clear();
		}
	}

	return true;
}

template<>
bool SDBLogger<SDBLogLevelDevelopment>::do_flush() {
	SDBConfiguration* configuration {SDBConfiguration::getConfiguration()};
	if (configuration == nullptr) {
		this->clear();
	} else {
		if (configuration->getIsLogLevelDevelopment()) {
			this->flush();
		} else {
			this->clear();
		}
	}

	return true;
}

SDBLogger<SDBLogLevelError>& flush(SDBLogger<SDBLogLevelError>& logger) {
	logger.do_flush();
	return logger;
}

SDBLogger<SDBLogLevelError>& endl(SDBLogger<SDBLogLevelError>& logger) {
	std::ostringstream os;
	os << std::endl;
	logger << os.str();
	return logger;
}

SDBLogger<SDBLogLevelInfo>& flush(SDBLogger<SDBLogLevelInfo>& logger) {
	logger.do_flush();
	return logger;
}

SDBLogger<SDBLogLevelInfo>& endl(SDBLogger<SDBLogLevelInfo>& logger) {
	std::ostringstream os;
	os << std::endl;
	logger << os.str();
	return logger;
}

SDBLogger<SDBLogLevelDebug>& flush(SDBLogger<SDBLogLevelDebug>& logger) {
	logger.do_flush();
	return logger;
}

SDBLogger<SDBLogLevelDebug>& endl(SDBLogger<SDBLogLevelDebug>& logger) {
	std::ostringstream os;
	os << std::endl;
	logger << os.str();
	return logger;
}

SDBLogger<SDBLogLevelTrace>& flush(SDBLogger<SDBLogLevelTrace>& logger) {
	logger.do_flush();
	return logger;
}

SDBLogger<SDBLogLevelTrace>& endl(SDBLogger<SDBLogLevelTrace>& logger) {
	std::ostringstream os;
	os << std::endl;
	logger << os.str();
	return logger;
}

SDBLogger<SDBLogLevelDevelopment>& flush(SDBLogger<SDBLogLevelDevelopment>& logger) {
	logger.do_flush();
	return logger;
}

SDBLogger<SDBLogLevelDevelopment>& endl(SDBLogger<SDBLogLevelDevelopment>& logger) {
	std::ostringstream os;
	os << std::endl;
	logger << os.str();
	logger.do_flush();
	return logger;
}

template <>
const char* SDBLogger<SDBLogLevelDevelopment>::getPrefix() const noexcept {
	return "[dev  ]";
}

template <>
const char* SDBLogger<SDBLogLevelError>::getPrefix() const noexcept {
	return "[error]";
}

template <>
const char* SDBLogger<SDBLogLevelTrace>::getPrefix() const noexcept {
	return "[trace]";
}

template <>
const char* SDBLogger<SDBLogLevelInfo>::getPrefix() const noexcept {
	return "[info ]";
}

template <>
const char* SDBLogger<SDBLogLevelDebug>::getPrefix() const noexcept {
	return "[debug]";
}

template <>
SDBLogger<SDBLogLevelDevelopment>::~SDBLogger() {
}

template <>
SDBLogger<SDBLogLevelInfo>::~SDBLogger() {
}

template <>
SDBLogger<SDBLogLevelDebug>::~SDBLogger() {
	SDBConfiguration* configuration {SDBConfiguration::getConfiguration()};
	if (configuration == nullptr) {
		_flush=false;
	} else {
		if (!configuration->getIsLogLevelDebug()) {
			_flush=false;
		}
	}
}

template <>
SDBLogger<SDBLogLevelTrace>::~SDBLogger() {
	SDBConfiguration* configuration {SDBConfiguration::getConfiguration()};
	if (configuration == nullptr) {
		_flush=false;
	} else {
		if (!configuration->getIsLogLevelTrace()) {
			_flush=false;
		}
	}
}

template <>
SDBLogger<SDBLogLevelError>::~SDBLogger() {
	SDBConfiguration* configuration {SDBConfiguration::getConfiguration()};
	if (configuration == nullptr) {
		_flush=false;
	} else {
		if (!configuration->getIsLogLevelError()) {
			_flush=false;
		}
	}
}

SDBILogger::SDBILogger() :
	_buffer{new std::ostringstream()}, _flush{true} {
}

SDBILogger::~SDBILogger() {
	if (_flush) {
		if(static_cast<std::ostringstream*>(_buffer)->str().length()) {
			this->flush();
		}
	}
}

bool SDBILogger::flush() {
	if (static_cast<std::ostringstream*>(_buffer)->str().length()  > 0) {
		std::cout << sdblib::now()
			<< " "
			<< this->getPrefix()
			<< ": "
			<< static_cast<std::ostringstream*>(_buffer)->str();
		std::cout.flush();
	}

	return SDBILogger::clear();
}

bool SDBILogger::clear() {
	static_cast<std::ostringstream*>(_buffer)->clear();
	static_cast<std::ostringstream*>(_buffer)->str("");

	return true;
}

bool SDBILogger::stream(bool data) {
	*(static_cast<std::ostringstream*>(_buffer)) << data;
	return true;
}

bool SDBILogger::stream(const std::string& data) {
	*(static_cast<std::ostringstream*>(_buffer)) << data;
	return true;
}

bool SDBILogger::stream(const void* data) {
	*(static_cast<std::ostringstream*>(_buffer)) << data;
	return true;
}

bool SDBILogger::stream(const char* data) {
	*(static_cast<std::ostringstream*>(_buffer)) << data;
	return true;
}

bool SDBILogger::stream(long int data) {
	*(static_cast<std::ostringstream*>(_buffer)) << data;
	return true;
}

bool SDBILogger::stream(int data) {
	*(static_cast<std::ostringstream*>(_buffer)) << data;
	return true;
}

bool SDBILogger::stream(unsigned int data) {
	*(static_cast<std::ostringstream*>(_buffer)) << data;
	return true;
}

bool SDBILogger::stream(unsigned long data) {
	*(static_cast<std::ostringstream*>(_buffer)) << data;
	return true;
}

}

