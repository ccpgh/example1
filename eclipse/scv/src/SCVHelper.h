
#ifndef SCV_SCVSETUP_H
#define SCV_SCVSETUP_H

#include <list>
#include <memory>
#include <string>
#include <vector>

#include <SDBConnection.h>
#include <SDBConfiguration.h>
#include <SDBConfigurationParameterDefinition.h>
#include <SDBConfigurationParameterSetting.h>
#include <SDBStatement.h>
#include <SDBResult.h>

namespace scv {

class SCVHelper {
private:
	SCVHelper();
	SCVHelper(const SCVHelper&);
	SCVHelper(const SCVHelper&&);
	SCVHelper& operator=(const SCVHelper&);
	SCVHelper& operator=(const SCVHelper&&);
	~SCVHelper();

public:
	static int displayHelp() noexcept;
	static int displayErrors(const std::list<std::string>& errors);
	static int initConfiguration(int argc, char *argv[]);
	static void generateParameterDefinitions(std::list<sdblib::SDBConfigurationParameterDefinition>& parameterDefinitions);
	static bool executeSql(const std::string& sql, sdblib::SDBConnection* connection, const std::vector<sdblib::SDBColumnType>& columns);
	static bool initConfiguration(int argc, char *argv[], bool* exit);
	static bool getDBConnection(std::unique_ptr<sdblib::SDBConnection>& connection);

	constexpr static const char* getSql1() noexcept  {
		const char* const sql {"use scv"};
		return sql;
	}

	constexpr static const char* getSql2() noexcept {
		//const char* const sql {"select account_name, account_name aaa, first_name, last_name from scvACCOUNT"};
		const char* const sql {"select account_name, first_name, last_name from scvACCOUNT"};
		return sql;
	}

	constexpr static const char* getSql3() noexcept {
		const char* const sql {"select account_name, first_name, last_name from scvACCOUNT and jksjs "};
		return sql;
	}

	constexpr static const char* getSql4() noexcept {
		const char* const sql {"select account_name, first_name, last_name from scvACCOUNT where last_name='haha' "};
		return sql;
	}

};

}

#endif // SCV_SCVSETUP_H
