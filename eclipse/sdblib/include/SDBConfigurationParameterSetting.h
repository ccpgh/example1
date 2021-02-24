
#ifndef SDBLIB_SDBCONFIGURATIONPARAMETERSETTING_H
#define SDBLIB_SDBCONFIGURATIONPARAMETERSETTING_H

#include <tuple>

namespace sdblib {

class SDBConfigurationParameterSetting : public std::tuple <std::string, std::string, bool> {
public:
	SDBConfigurationParameterSetting(const std::string& parameterName, const std::string& parameterValue, bool isBoolean) :
		std::tuple<std::string, std::string, bool>{parameterName, parameterValue, isBoolean}
	{}

	SDBConfigurationParameterSetting(const SDBConfigurationParameterSetting& configurationParameterSetting)  :
		std::tuple<std::string, std::string, bool>{configurationParameterSetting.getParameterName(),
				configurationParameterSetting.getParameterValue(),
				configurationParameterSetting.getIsBoolean()}
	{}

	SDBConfigurationParameterSetting(const SDBConfigurationParameterSetting&& configurationParameterSetting)  :
				std::tuple<std::string, std::string, bool>{configurationParameterSetting.getParameterName(),
						configurationParameterSetting.getParameterValue(),
						configurationParameterSetting.getIsBoolean()}
   {}

	SDBConfigurationParameterSetting& operator=(const SDBConfigurationParameterSetting& configurationParameterSetting)  {
		std::tuple<std::string, std::string, bool>::operator=(configurationParameterSetting);
		return (*this);
	}

	SDBConfigurationParameterSetting& operator=(const SDBConfigurationParameterSetting&& configurationParameterSetting)  {
		std::tuple<std::string, std::string, bool>::operator=(configurationParameterSetting);
		return (*this);
	}

	virtual ~SDBConfigurationParameterSetting() {}

	inline const std::string& getParameterName() const noexcept {
		return std::get<0>(*this);
	}

	inline const std::string& getParameterValue() const noexcept {
		return std::get<1>(*this);
	}

	inline bool getIsBoolean() const noexcept {
		return std::get<2>(*this);
	}
};

}

#endif // SDBLIB_SDBCONFIGURATIONPARAMETERSETTING_H
