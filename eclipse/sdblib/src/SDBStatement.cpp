
#include <cassert>

#include "SDBStatement.h"
#include "SDBLogger.h"
#include "SDBConfiguration.h"
#include "SDBCommand.h"
#include "SDBColumn.h"

namespace sdblib {

SDBStatement::SDBStatement(const std::vector<sdblib::SDBColumnType>& columns) :
		_buffer{}, _errors{}, _rowCount{0}, _command{0}, _columns{columns} {
	assert(sizeof(SDBStatementExecuted) == sizeof(SDBStatement));
}

SDBStatementExecuted& SDBStatement::do_execute(SDBStatement& statement, SDBConnection* connection) noexcept {
	if (connection) {
		int rowCount{0};
		if(!statement.doexecute(connection, &statement, rowCount)) {
			statement._errors.push_back("connection method failed");
		} else {
			statement._rowCount=rowCount;
		}
	}

	return reinterpret_cast<SDBStatementExecuted&>(statement);
}

bool SDBStatement::doexecute(SDBConnection* connection, SDBStatement* statement, int& rowCount) {
	sdblib::info << "running sql: "
			<< statement->getBuffer().str()
			<< sdblib::endl
			<< sdblib::flush;

	this->_command=new sdblib::SDBCommand{statement, connection->getConnection()};
	if (!_command->getIsValid()) {
		this->_errors.push_back("SDBCommand not valid");
		std::copy(_command->getErrors().begin(), _command->getErrors().end(), std::back_insert_iterator<std::list<std::string> >(this->_errors));
		_command->clearErrors();
		return false;
	}

	return true;
}

SDBStatement::~SDBStatement() {
	if(_command) {
		delete _command;
	}
}

void SDBStatementExecuted::operator>>(SDBResult& result) noexcept {
	result.setResult(this->_columns, this->_command->releaseResults());
}


}
