
#ifndef SDBLIB_SDBLOGGER_H
#define SDBLIB_SDBLOGGER_H

#include "SDBTime.h"

namespace sdblib {

template <typename T>
class SDBLogger;

class SDBLogLevelError;
class SDBLogLevelInfo;
class SDBLogLevelDebug;
class SDBLogLevelTrace;
class SDBLogLevelDevelopment;

extern SDBLogger<SDBLogLevelError>& endl(SDBLogger<SDBLogLevelError>& logger);
extern SDBLogger<SDBLogLevelError>& flush(SDBLogger<SDBLogLevelError>& logger);
extern SDBLogger<SDBLogLevelInfo>& endl(SDBLogger<SDBLogLevelInfo>& logger);
extern SDBLogger<SDBLogLevelInfo>& flush(SDBLogger<SDBLogLevelInfo>& logger);
extern SDBLogger<SDBLogLevelDebug>& endl(SDBLogger<SDBLogLevelDebug>& logger);
extern SDBLogger<SDBLogLevelDebug>& flush(SDBLogger<SDBLogLevelDebug>& logger);
extern SDBLogger<SDBLogLevelTrace>& endl(SDBLogger<SDBLogLevelTrace>& logger);
extern SDBLogger<SDBLogLevelTrace>& flush(SDBLogger<SDBLogLevelTrace>& logger);
extern SDBLogger<SDBLogLevelDevelopment>& endl(SDBLogger<SDBLogLevelDevelopment>& logger);
extern SDBLogger<SDBLogLevelDevelopment>& flush(SDBLogger<SDBLogLevelDevelopment>& logger);


class SDBILogger {
private:
	SDBILogger(const SDBILogger&);
	SDBILogger(const SDBILogger&&);
	SDBILogger& operator=(const SDBILogger&);
	SDBILogger& operator=(const SDBILogger&&);

	void* _buffer; // done this way to demo how you can hide implementation from headers
	virtual const char* getPrefix() const noexcept=0;

protected:
	SDBILogger();
	virtual ~SDBILogger();
	bool flush();
	bool clear();

	bool stream(bool data);
	bool stream(const std::string& s);
	bool stream(const void* p);
	bool stream(const char* data);
	bool stream(long int data);
	bool stream(int data);
	bool stream(unsigned int data);
	bool stream(unsigned long data);

	bool _flush;

public:

};

template <typename T>
class SDBLogger :  private SDBILogger {
private:

	SDBLogger(const SDBLogger<T>&);
	SDBLogger(const SDBLogger<T>&&);
	SDBLogger<T>& operator=(const SDBLogger<T>&);
	SDBLogger<T>& operator=(const SDBLogger<T>&&);

	SDBLogger() :
		SDBILogger{} {
	}

	virtual ~SDBLogger();

	friend class SDBLoggerErrorAccess;
	friend class SDBLoggerInfoAccess;
	friend class SDBLoggerDebugAccess;
	friend class SDBLoggerTraceAccess;
	friend class SDBLoggerDevelopmentAccess;

	friend SDBLogger<SDBLogLevelError>& sdblib::endl(SDBLogger<SDBLogLevelError>& logger);
	friend SDBLogger<SDBLogLevelError>& flush(SDBLogger<SDBLogLevelError>& logger);

	friend SDBLogger<SDBLogLevelInfo>& sdblib::endl(SDBLogger<SDBLogLevelInfo>& logger);
	friend SDBLogger<SDBLogLevelInfo>& flush(SDBLogger<SDBLogLevelInfo>& logger);

	friend SDBLogger<SDBLogLevelDebug>& flush(SDBLogger<SDBLogLevelDebug>& logger);
	friend SDBLogger<SDBLogLevelDebug>& sdblib::endl(SDBLogger<SDBLogLevelDebug>& logger);

	friend SDBLogger<SDBLogLevelTrace>& flush(SDBLogger<SDBLogLevelTrace>& logger);
	friend SDBLogger<SDBLogLevelTrace>& sdblib::endl(SDBLogger<SDBLogLevelTrace>& logger);

	friend SDBLogger<SDBLogLevelDevelopment>& flush(SDBLogger<SDBLogLevelDevelopment>& logger);
	friend SDBLogger<SDBLogLevelDevelopment>& sdblib::endl(SDBLogger<SDBLogLevelDevelopment>& logger);

	static SDBLogger<T> logger;

public:
	template <typename U>
	inline SDBLogger<T>& operator<<(U u) {
		SDBILogger::stream((U) u);
		return (*this);
	}

	inline SDBLogger<T>& operator<<(SDBLogger<T>& (*fp)(SDBLogger<T>& logger)) {
		return fp(*this);
	}

	bool do_flush();
	virtual const char* getPrefix() const noexcept override;
};

extern SDBLogger<SDBLogLevelError>& error;
extern SDBLogger<SDBLogLevelInfo>& info;
extern SDBLogger<SDBLogLevelDebug>& debug;
extern SDBLogger<SDBLogLevelTrace>& trace;
extern SDBLogger<SDBLogLevelDevelopment>& development;

}

#endif // SDBLIB_SDBLOGGER_H
