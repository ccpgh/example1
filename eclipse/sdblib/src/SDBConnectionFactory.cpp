
#include "SDBConnectionFactory.h"
#include "SDBLogger.h"

namespace sdblib {
	std::unique_ptr<SDBConnection> SDBConnectionFactory::getDBConnection(sdblib::SDBConfiguration* configuration) {

		if (!configuration) {
			return std::unique_ptr<SDBConnection>(static_cast<SDBConnection*>(0));
		}

		sdblib::trace << "extracting parameters"
				<< sdblib::endl
				<< sdblib::flush;

		std::string username=configuration->getParameterSetting(sdblib::SDBConnection::CONST_CONFIGURATION_PARAMETERNAME_USERNAME).getParameterValue();
		std::string password=configuration->getParameterSetting(sdblib::SDBConnection::CONST_CONFIGURATION_PARAMETERNAME_PASSWORD).getParameterValue();
		std::string database=configuration->getParameterSetting(sdblib::SDBConnection::CONST_CONFIGURATION_PARAMETERNAME_DATABASE).getParameterValue();
		std::string server=configuration->getParameterSetting(sdblib::SDBConnection::CONST_CONFIGURATION_PARAMETERNAME_SERVER).getParameterValue();

		sdblib::trace << "parameter username: '"
					<< username
					<< "'"
					<< sdblib::endl
					<< sdblib::flush;

		sdblib::trace << "parameter password: '"
					<< password
					<< "'"
					<< sdblib::endl
					<< sdblib::flush;

		sdblib::trace << "parameter database: '"
					<< database
					<< "'"
					<< sdblib::endl
					<< sdblib::flush;

		sdblib::trace << "parameter server:   '"
					<< server
					<< "'"
					<< sdblib::endl
					<< sdblib::flush;

		if (!username.length() ||
				!password.length() ||
				!database.length() ||
				!server.length()) {

			sdblib::error << "error: empty parameter"
					<< sdblib::endl
					<< sdblib::flush;

			return std::unique_ptr<SDBConnection>(static_cast<SDBConnection*>(0));
		}

		return std::unique_ptr<SDBConnection>(new sdblib::SDBConnection(username, password, server, configuration->getProgramName()));
	}
}
