

#ifndef SDBLIB_SDBSTATEMENT_H
#define SDBLIB_SDBSTATEMENT_H

#include <sstream>
#include <vector>

#include "SDBColumn.h"
#include "SDBConnection.h"
#include "SDBResult.h"
#include "SDBStatementFunctor.h"

namespace sdblib {

class SDBCommand;
class SDBStatement;
class SDBStatementExecuted;

class SDBStatement  {
private:
	std::ostringstream _buffer;
	std::list<std::string> _errors;
	int _rowCount;

	SDBStatement(const SDBStatement&);
	SDBStatement(const SDBStatement&&);
	SDBStatement& operator=(const SDBStatement&);
	SDBStatement& operator=(const SDBStatement&&);

	friend SDBStatementFunctor execute(SDBConnection* connection);

	static SDBStatementExecuted& do_execute(SDBStatement& statement, SDBConnection* connection) noexcept;
	bool doexecute(SDBConnection* connection, SDBStatement* statement, int& rowCount);

protected:
	SDBCommand* _command;
	std::vector<sdblib::SDBColumnType> _columns;

public:
	SDBStatement(const std::vector<sdblib::SDBColumnType>& columns);
	virtual ~SDBStatement();

public:
	inline SDBStatement& operator<<(const char* const s) noexcept {
		this->_buffer << s;
		return (*this);
	}

	inline SDBStatement& operator<<(const std::string& s) noexcept {
		this->_buffer << s;
		return (*this);
	}

	inline SDBStatementExecuted& operator<<(const SDBStatementFunctor& functor) noexcept {
		return functor._fp(*this, functor._connection);
	}

	inline bool getIsValid() const noexcept {
		return _errors.size() == 0;
	}

	inline const std::list<std::string>& getErrors() const noexcept {
		return this->_errors;
	}

	inline void clearErrors() noexcept {
		this->_errors.clear();
	}

	inline int getRowCount() const noexcept {
		return _rowCount;
	}

	inline const std::ostringstream& getBuffer() const noexcept {
		return _buffer;
	}

	inline const std::vector<sdblib::SDBColumnType>& getColumns() const noexcept {
		return _columns;
	}
};

class SDBStatementExecuted : private SDBStatement {
private:
	SDBStatementExecuted(const SDBStatementExecuted&);
	SDBStatementExecuted(const SDBStatementExecuted&&);
	SDBStatementExecuted& operator=(const SDBStatementExecuted&);
	SDBStatementExecuted& operator=(const SDBStatementExecuted&&);

public:
	SDBStatementExecuted(const std::vector<sdblib::SDBColumnType>& columns) :
		SDBStatement(columns)
		{}

	virtual ~SDBStatementExecuted() {}

	void operator>>(SDBResult& result) noexcept;
};

}

#endif // SDBLIB_SDBSTATEMENT_H
