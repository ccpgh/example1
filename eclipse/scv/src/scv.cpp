
#include <memory>
#include <list>
#include <thread>
#include <chrono>
#include <climits>

#include <SDBCommand.h>
#include <SDBConfiguration.h>
#include <SDBConfigurationParameterDefinition.h>
#include <SDBConfigurationParameterSetting.h>
#include <SDBConnection.h>
#include <SDBLogger.h>
#include <SDBResult.h>
#include <SDBTime.h>

#include "SCVHelper.h"

// NYI - tidy up logger lifecycle.. add LINE and FILE

int main(int argc, char *argv[]) {
	bool exit=false;
	if(!scv::SCVHelper::initConfiguration(argc, argv, &exit)) {
		return 1;
	} else if (exit) {
		return 0;
	}

	sdblib::info << "start"
			<< sdblib::endl
			<< sdblib::flush;

	std::unique_ptr<sdblib::SDBConnection> connection;
	if(!scv::SCVHelper::getDBConnection(connection)) {
		sdblib::error << "failed"
				<< sdblib::endl;

		return 1;
	}

	if(!scv::SCVHelper::executeSql(scv::SCVHelper::getSql1(), connection.get(), {})) {
		return 1;
	}

	if(!scv::SCVHelper::executeSql(scv::SCVHelper::getSql2(), connection.get(),
			{sdblib::SDBColumnType::COLUMNTYPE_STRING, sdblib::SDBColumnType::COLUMNTYPE_STRING, sdblib::SDBColumnType::COLUMNTYPE_STRING})) {
		return 1;
	}

	if(false)
	if(!scv::SCVHelper::executeSql(scv::SCVHelper::getSql3(), connection.get(),
			{sdblib::SDBColumnType::COLUMNTYPE_STRING, sdblib::SDBColumnType::COLUMNTYPE_STRING, sdblib::SDBColumnType::COLUMNTYPE_STRING})) {
		return 1;
	}

	if(!scv::SCVHelper::executeSql(scv::SCVHelper::getSql4(), connection.get(),
			{sdblib::SDBColumnType::COLUMNTYPE_STRING, sdblib::SDBColumnType::COLUMNTYPE_STRING, sdblib::SDBColumnType::COLUMNTYPE_STRING})) {
		return 1;
	}

	sdblib::info << "end"
			<< sdblib::endl
			<< sdblib::flush;

	return 0;
}


