
#ifndef SDBLIB_SDBCONFIGURATIONPARAMETERDEFINITION_H
#define SDBLIB_SDBCONFIGURATIONPARAMETERDEFINITION_H

#include <tuple>

namespace sdblib {

class SDBConfigurationParameterDefinition : public std::tuple <std::string, std::string, bool, bool> {
public:
	SDBConfigurationParameterDefinition(const std::string& parameterName, const std::string& environmentVariableName, bool isMandatory, bool isBoolean) :
		std::tuple <std::string, std::string, bool, bool>{parameterName, environmentVariableName, isMandatory, isBoolean}
	{}

	SDBConfigurationParameterDefinition(const SDBConfigurationParameterDefinition& configurationParameterDefinition) :
		std::tuple <std::string, std::string, bool, bool>{configurationParameterDefinition.getParameterName(),
				configurationParameterDefinition.getEnvironmentVariableName(),
				configurationParameterDefinition.getIsMandatory(),
				configurationParameterDefinition.getIsBoolean()}
	{}

	SDBConfigurationParameterDefinition(const SDBConfigurationParameterDefinition&& configurationParameterDefinition) :
		std::tuple <std::string, std::string, bool, bool>{configurationParameterDefinition.getParameterName(),
				configurationParameterDefinition.getEnvironmentVariableName(),
				configurationParameterDefinition.getIsMandatory(),
				configurationParameterDefinition.getIsBoolean()}
	{}

	SDBConfigurationParameterDefinition& operator=(const SDBConfigurationParameterDefinition& configurationParameterDefinition)  {
		std::tuple <std::string, std::string, bool, bool>::operator=(configurationParameterDefinition);
		return (*this);
	}

	SDBConfigurationParameterDefinition& operator=(const SDBConfigurationParameterDefinition&& configurationParameterDefinition)  {
		std::tuple <std::string, std::string, bool, bool>::operator=(configurationParameterDefinition);
		return (*this);
	}

	virtual ~SDBConfigurationParameterDefinition() {}

	inline const std::string& getParameterName() const noexcept {
		return std::get<0>(*this);
	}

	inline const std::string& getEnvironmentVariableName() const noexcept {
		return std::get<1>(*this);
	}

	inline const bool getIsMandatory() const noexcept {
		return std::get<2>(*this);
	}

	inline const bool getIsBoolean() const noexcept {
		return std::get<3>(*this);
	}
};

}

#endif // SDBLIB_SDBCONFIGURATIONPARAMETERDEFINITION_H



