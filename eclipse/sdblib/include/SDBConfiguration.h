
#ifndef SDBLIB_SDBCONFIGURATION_H
#define SDBLIB_SDBCONFIGURATION_H

#include <mutex>
#include <tuple>
#include <list>
#include <cstring>

#include "SDBConfigurationParameterDefinition.h"
#include "SDBConfigurationParameterSetting.h"
#include "SDBString.h"

namespace sdblib {

class SDBConfiguration {
private:
	SDBConfiguration(const char* const programName) :
		_isValid{true}, _isHelp{false}, _isLogLevelDebug{false}, _isLogLevelError{false}, _isLogLevelTrace{false}, _isLogLevelInfo{false}, _isLogLevelDefault{false}, _isUseEnvironment{false}, _parameterSettings{}, _nullParameterSetting{"", "", false}, _programName{} {
		if (programName &&
				std::strlen(programName)) {
			this->_programName.append(SDBString::basename(programName));
		}
	}

	SDBConfiguration(const SDBConfiguration&);
	SDBConfiguration(const SDBConfiguration&&);
	SDBConfiguration& operator=(const SDBConfiguration&);
	SDBConfiguration& operator=(const SDBConfiguration&&);

	static SDBConfiguration* _configuration;
	static std::mutex _mutex; // not going to thread-proof everything now. would take too long. this is a DB/wrapper design demo. will do a separate multi-threading project next. will leave this in though

	bool _isValid;
	bool _isHelp;

	bool _isLogLevelDebug;
	bool _isLogLevelError;
	bool _isLogLevelTrace;
	bool _isLogLevelInfo;
	bool _isLogLevelDefault;

	bool _isUseEnvironment;
	std::list<SDBConfigurationParameterSetting> _parameterSettings;
	SDBConfigurationParameterSetting _nullParameterSetting;
	std::string _programName;

public:
	~SDBConfiguration() {}

	static SDBConfiguration* getConfiguration();

	static bool initConfiguration(int argc, char** argv,
			const std::list<sdblib::SDBConfigurationParameterDefinition>& parameterDefinitions, std::list<std::string>& errors);

	inline bool getIsHelp() const noexcept {
		return this->_isHelp;
	}

	inline bool getIsValid() const noexcept {
		return this->_isValid;
	}

	inline bool getUseEnvironment() const noexcept {
		return this->_isUseEnvironment;
	}

	inline const std::list<SDBConfigurationParameterSetting>& getParameterSettings() const noexcept {
		return this->_parameterSettings;
	}

	inline const SDBConfigurationParameterSetting& getParameterSetting(const std::string& parameterName) const noexcept {
		for (auto& parameterSetting : this->getParameterSettings()) {
			if (parameterSetting.getParameterName() == parameterName) {
				return parameterSetting;
			}
		}

		return this->_nullParameterSetting;
	}

	inline const std::string& getProgramName() const noexcept {
		return _programName;
	}

	inline bool getIsLogLevelTrace() const noexcept {
		return this->_isLogLevelTrace;
	}

	inline bool getIsLogLevelDebug() const noexcept {
		return this->_isLogLevelDebug || this->getIsLogLevelTrace();
	}

	inline bool getIsLogLevelInfo() const noexcept {
		return this->_isLogLevelInfo || this->getIsLogLevelDebug() || this->_isLogLevelDefault;
	}

	inline bool getIsLogLevelError() const noexcept {
		return this->_isLogLevelError || this->getIsLogLevelInfo() || this->_isLogLevelDefault;
	}

	inline bool getIsLogLevelDevelopment() const noexcept {
		return true;;
	}

private:
	inline void setIsValid(bool isValid) noexcept {
		this->_isValid=isValid;
	}

	void setIsHelp(int argc, char** argv);
	void setLoggingLevel(int argc, char** argv);
	void setIsUseEnvironment(int argc, char** argv);

};

}

#endif // SDBLIB_SDBCONFIGURATION_H
