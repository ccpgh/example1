
#include <thread>
#include <vector>
#include <tuple>
#include <sstream>
#include <cstring>

#include "SDBConfiguration.h"
#include "SDBTime.h"
#include "SDBString.h"

namespace sdblib {

SDBConfiguration* SDBConfiguration::_configuration {nullptr};
std::mutex SDBConfiguration::_mutex {};

bool SDBConfiguration::initConfiguration(int argc, char** argv,
		const std::list<sdblib::SDBConfigurationParameterDefinition>& parameterDefinitions,
		std::list<std::string>& errors) {

	std::lock_guard<std::mutex> guard{SDBConfiguration::_mutex};
	if (SDBConfiguration::_configuration != nullptr) {
		return true;
	}

	SDBConfiguration::_configuration = new SDBConfiguration(SDBString::basename(argv[0]).c_str());
	SDBConfiguration::_configuration->setIsHelp(argc, argv);
	SDBConfiguration::_configuration->setIsUseEnvironment(argc, argv);
	SDBConfiguration::_configuration->setLoggingLevel(argc, argv);
	if(SDBConfiguration::_configuration->getIsHelp()) {
		return true;
	}

	for (int x=1; x<argc; x++) {
		const std::string lhs{SDBString::trim(argv[x])};
		if (lhs == "-env") {
			continue;
		} else if (lhs == "-error" ||
				lhs == "-info" ||
				lhs == "-debug" ||
				lhs == "-trace") {
			continue;
		}

		bool found{false};
		for (sdblib::SDBConfigurationParameterDefinition parameterDefinition : parameterDefinitions) {
			if (parameterDefinition.getParameterName() == lhs) {
				found=true;
				for (auto parameterSetting : SDBConfiguration::_configuration->_parameterSettings) {
					if (parameterSetting.getParameterName() == lhs) {
						std::ostringstream os;
						os << "duplicate parameter '"
								<< lhs
								<< "'";

						errors.push_back(os.str());
						SDBConfiguration::_configuration->setIsValid(false);
						return false;
					}
				}

				if (parameterDefinition.getIsBoolean()) {
					SDBConfiguration::_configuration->_parameterSettings.push_back(sdblib::SDBConfigurationParameterSetting(lhs, "", true));
					break;

				} else if ((x+1)<argc &&
						std::strlen(argv[x+1]) &&
						argv[x+1][0]!='-') {
					const std::string rhs{SDBString::trim(argv[++x])};
					SDBConfiguration::_configuration->_parameterSettings.push_back(sdblib::SDBConfigurationParameterSetting(lhs, rhs, false));
					break;
				} else {
					std::ostringstream os;
					os << "missing value for parameter '"
							<< lhs
							<< "'";

					errors.push_back(os.str());
					SDBConfiguration::_configuration->setIsValid(false);
					return false;
				}
			}
		}

		if (!found) {
			std::ostringstream os;
			os << ( lhs.length() ? (lhs.at(0)=='-' ? "invalid parameter" : "indeterminate value") : "incorrect token" )
					<< " '"
					<< lhs
					<< "'";

			errors.push_back(os.str());
			SDBConfiguration::_configuration->setIsValid(false);
		}
	}

	for (sdblib::SDBConfigurationParameterDefinition parameterDefinition : parameterDefinitions) {
		bool found{false};

		for (SDBConfigurationParameterSetting parameterSetting : SDBConfiguration::_configuration->_parameterSettings) {
			if (parameterDefinition.getParameterName() == parameterSetting.getParameterName()) {
				found = true;
				break;
			}
		}

		if (!found &&
				SDBConfiguration::_configuration->getUseEnvironment()) {
			std::string environmentVariableName(parameterDefinition.getEnvironmentVariableName());
			if (environmentVariableName.length() > 0) {
				const char* environmentVariableValue {std::getenv(environmentVariableName.c_str())};
				if (environmentVariableValue) {
					found=true;
					std::string value{SDBString::trim(environmentVariableValue)};
					if (value.length() &&
							!parameterDefinition.getIsBoolean()) {
						SDBConfiguration::_configuration->_parameterSettings.push_back(sdblib::SDBConfigurationParameterSetting(parameterDefinition.getParameterName(), value, false));
					} else if (!value.length() &&
							parameterDefinition.getIsBoolean()) {
						SDBConfiguration::_configuration->_parameterSettings.push_back(sdblib::SDBConfigurationParameterSetting(parameterDefinition.getParameterName(), value, true));
					} else {
						std::ostringstream os;
						os << "environment parameter value does not match type environmentVariable '"
								<< environmentVariableName
								<< "' has value '"
								<< environmentVariableValue
								<< "'";

						errors.push_back(os.str());
						SDBConfiguration::_configuration->setIsValid(false);
					}
				}
			}
		}

		if (!found) {
			if (!parameterDefinition.getIsMandatory()) {
				found = true;
			}
		}

		if (!found) {
			std::ostringstream os;
			os << "mandatory parameter '"
					<< std::get<0>(parameterDefinition)
					<< "' missing";

			errors.push_back(os.str());
			SDBConfiguration::_configuration->setIsValid(false);
		}
	}

	return true;
}

SDBConfiguration* SDBConfiguration::getConfiguration() {
	if (SDBConfiguration::_mutex.try_lock()) {
		if (SDBConfiguration::_configuration != nullptr) {
			SDBConfiguration::_mutex.unlock();
			return SDBConfiguration::_configuration;
		}

		SDBConfiguration::_mutex.unlock();
	}

	return static_cast<SDBConfiguration*>(nullptr);
}

void SDBConfiguration::setIsHelp(int argc, char** argv) {
	for (int x=1; x<argc; x++) {
		if (!std::strcmp("-h", argv[x])) {
			this->_isHelp = true;
			break;
		}
	}
}

void SDBConfiguration::setIsUseEnvironment(int argc, char** argv) {
	for (int x=1; x<argc; x++) {
		if (!std::strcmp("-env", argv[x])) {
			this->_isUseEnvironment = true;
			break;
		}
	}
}

void SDBConfiguration::setLoggingLevel(int argc, char** argv) { // NYI - fix
	for (int x=1; x<argc; x++) {
		if (!std::strcmp("-trace", argv[x])) {
			this->_isLogLevelTrace = true;
			return;
		}
	}

	for (int x=1; x<argc; x++) {
		if (!std::strcmp("-debug", argv[x])) {
			this->_isLogLevelDebug = true;
			return;
		}
	}

	for (int x=1; x<argc; x++) {
		if (!std::strcmp("-info", argv[x])) {
			this->_isLogLevelInfo = true;
			return;
		}
	}

	for (int x=1; x<argc; x++) {
		if (!std::strcmp("-error", argv[x])) {
			this->_isLogLevelError = true;
			return;
		}
	}

	this->_isLogLevelDefault=true;
}

}



