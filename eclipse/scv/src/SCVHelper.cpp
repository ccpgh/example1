
#include <iostream>

#include <SDBConnectionFactory.h>
#include <SDBLogger.h>
#include <SDBResult.h>

#include "SCVHelper.h"

namespace scv {

int SCVHelper::displayHelp() noexcept {
	std::cout << std::endl
			<< "help:" << std::endl
			<< std::endl
			<< "************************************************************************************************************" << std::endl
			<< "* progname: scv                                                                                            *" << std::endl
			<< "* description: demo harness for Sybase DB lib - SDB                                                        *" << std::endl
			<< "************************************************************************************************************" << std::endl
			<< "* parameter name  |  mandatory  | boolean flag | description                     | environment alternative *" << std::endl
			<< "************************************************************************************************************" << std::endl
			<< "* -u              |  no         | no           | database user account name      | $SDBLIB_USERNAME        *" << std::endl
			<< "* -p              |  no         | no           | database user account password  | $SDBLIB_PASSWORD        *" << std::endl
			<< "* -db             |  no         | no           | database name                   | $SDBLIB_DBNAME          *" << std::endl
			<< "* -s              |  no         | no           | server name                     | $DSQUERY                *" << std::endl
			<< "* -h              |  no         | yes          | display this help menu          | N/A                     *" << std::endl
			<< "* -error          |  no         | yes          | output error information        | N/A                     *" << std::endl
			<< "* -log            |  no         | yes          | output log information           | N/A                     *" << std::endl
			<< "* -trace          |  no         | yes          | output trace information         | N/A                     *" << std::endl
			<< "* -debug          |  no         | yes          | output more information         | N/A                     *" << std::endl
			<< "* -env            |  no         | yes          | use env var fallback(s)         | N/A                     *" << std::endl
			<< "************************************************************************************************************" << std::endl
			<< "* notes:                                                                                                   *" << std::endl
			<< "*   parameters can be supplied either at the command line or via the indicated environment variable.       *" << std::endl
			<< "*   the values supplied at the command line override those in the environment, to use environment          *" << std::endl
			<< "*   setting you need to supply the -env parameter                                                          *" << std::endl
			<< "************************************************************************************************************" << std::endl
			<< "* examples:                                                                                                *" << std::endl
			<< "*   scv -h # display this help menu                                                                        *" << std::endl
			<< "*   scv -u sa -p password -s server1 -db db1 # run from command line                                       *" << std::endl
			<< "*   export SDBLIB_PASSWORD=pwd ; scv -u user -s server1 -db db1 -env # run from command line + environment *" << std::endl
			<< "************************************************************************************************************" << std::endl
			<< std::endl;

	std::cout.flush();
	return -1;
}

int SCVHelper::displayErrors(const std::list<std::string>& errors) {
	for(auto error : errors) {
		sdblib::error << "information: "
					<< error
					<< sdblib::endl
					<< sdblib::flush;
	}

	return 1;
}

void SCVHelper::generateParameterDefinitions(std::list<sdblib::SDBConfigurationParameterDefinition>& parameterDefinitions) {
	parameterDefinitions.push_back(sdblib::SDBConfigurationParameterDefinition(sdblib::SDBConnection::CONST_CONFIGURATION_PARAMETERNAME_USERNAME, "SDBLIB_USERNAME", true,  false));
	parameterDefinitions.push_back(sdblib::SDBConfigurationParameterDefinition(sdblib::SDBConnection::CONST_CONFIGURATION_PARAMETERNAME_PASSWORD, "SDBLIB_PASSWORD", true,  false));
	parameterDefinitions.push_back(sdblib::SDBConfigurationParameterDefinition(sdblib::SDBConnection::CONST_CONFIGURATION_PARAMETERNAME_SERVER,   "DSQUERY",         true,  false));
	parameterDefinitions.push_back(sdblib::SDBConfigurationParameterDefinition(sdblib::SDBConnection::CONST_CONFIGURATION_PARAMETERNAME_DATABASE, "SDBLIB_DBNAME",   true,  false));
}

int SCVHelper::initConfiguration(int argc, char *argv[]) {
	std::list<sdblib::SDBConfigurationParameterDefinition> parameterDefinitions;
	std::list<std::string> errors {};
	scv::SCVHelper::generateParameterDefinitions(parameterDefinitions);

	if(!sdblib::SDBConfiguration::initConfiguration(argc, argv, parameterDefinitions, errors)) {
		sdblib::error << "error: initConfiguration failed"
					<< sdblib::endl
					<< sdblib::flush;

		scv::SCVHelper::displayErrors(errors);
		return 1;
	}

	sdblib::SDBConfiguration* configuration(sdblib::SDBConfiguration::getConfiguration());
	if(configuration == nullptr) {
		sdblib::error << "error: getConfiguration failed - NULL"
					<< sdblib::endl
					<< sdblib::flush;

		return scv::SCVHelper::displayErrors(errors);

	} else if (configuration->getIsHelp()) {
		return scv::SCVHelper::displayHelp();

	} else if (!configuration->getIsValid()) {
		sdblib::error << "error: getConfiguration failed"
					<< sdblib::endl
					<< sdblib::flush;

		return scv::SCVHelper::displayErrors(errors);
	}

	return 0;
}

bool SCVHelper::initConfiguration(int argc, char *argv[], bool* exit) {
	sdblib::debug << "configuration: loading"
			<< sdblib::endl
			<< sdblib::flush;

	int status=scv::SCVHelper::initConfiguration(argc, argv);
	if (status>0) {
		sdblib::error  << "initConfiguration failed"
				<< sdblib::endl
				<< sdblib::flush;

		return false;
	} else if (status<0) {
		*exit=true;
		return true;
	}

	sdblib::SDBConfiguration* configuration(sdblib::SDBConfiguration::getConfiguration());
	if(configuration == nullptr) {
		sdblib::error << "error: getConfiguration was NULL"
				<< sdblib::endl;

		return false;
	}

	sdblib::debug << "configuration: loaded"
			<< sdblib::endl
			<< sdblib::flush;

	return true;
}

bool SCVHelper::getDBConnection(std::unique_ptr<sdblib::SDBConnection>& connection) {
	sdblib::debug << "database connection: creating"
			<< sdblib::endl
			<< sdblib::flush;

	connection=sdblib::SDBConnectionFactory::getDBConnection(sdblib::SDBConfiguration::getConfiguration());
	if(connection.get() == nullptr) {
		sdblib::error << "error: getDBConnection failed"
					<< sdblib::endl
					<< sdblib::flush;
		return false;
	}

	if(!connection->getIsValid()) {
		sdblib::error << "error: DBConnection is NOT valid"
					<< sdblib::endl
					<< sdblib::flush;

		scv::SCVHelper::displayErrors(connection->getErrors());
		return false;
	}

	sdblib::debug << "database connection: created server["
			<< connection->getServer()
			<< "] user["
			<< connection->getUsername()
			<< "]"
			<< sdblib::endl
			<< sdblib::flush;

	return true;
}

bool SCVHelper::executeSql(const std::string& sql, sdblib::SDBConnection* connection, const std::vector<sdblib::SDBColumnType>& columns) {
	sdblib::SDBResult result;
	const sdblib::SDBResult const_result;

	sdblib::SDBStatement statement{columns};
	statement << sql
			  << sdblib::execute(connection)
		      >> result;
	;
	if(!statement.getIsValid()) {
		sdblib::error << "statement error: "
				<< sql
				<< sdblib::endl
				<< sdblib::flush;

		scv::SCVHelper::displayErrors(statement.getErrors());
		return false;
	}

	if(!result.getIsValid()) {
		sdblib::error << "result error"
				<< sdblib::endl
				<< sdblib::flush;

		scv::SCVHelper::displayErrors(result.getErrors());
		return false;
	}

	for(sdblib::SDBResult::iterator i=result.begin(); i!=result.end(); i++) { // NYI - make this easier
		for (unsigned int x=0; x<columns.size(); x++) {
			if(x) {
				sdblib::info << ",";
			}
			sdblib::info << i.getCharValue(x);
		}
		sdblib::info << sdblib::endl << sdblib::flush;
	}

	sdblib::debug << "retrieved  "
			<< result.getRowCount()
			<< " rows"
			<< sdblib::endl
			<< sdblib::flush
			;
	return true;
}

}
