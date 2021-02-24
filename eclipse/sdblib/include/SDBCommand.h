

#ifndef SDBLIB_SDBCOMMAND_H
#define SDBLIB_SDBCOMMAND_H

#include <list>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>

#include <ctpublic.h>
#include <cspublic.h>

#include "SDBStatement.h"

namespace sdblib {

class SDBConnection;

class SDBCommand {
private:
	SDBCommand(const SDBCommand&);
	SDBCommand(const SDBCommand&&);
	SDBCommand& operator=(const SDBCommand&);
	SDBCommand& operator=(const SDBCommand&&);

	CS_COMMAND* _command;
	CS_CONNECTION* _connection;

	std::vector<std::vector<void*> *> _results;
	std::vector<CS_DATAFMT*> _formats;
	std::vector<char*> _buffers;
	std::vector<CS_INT*> _datalengths;
	std::vector<CS_SMALLINT*> _indicators;

	std::unique_ptr<CS_CHAR> _sql;
	std::list<std::string> _errors;
	bool _isCmdDrop;
	bool _isCtCancel;

	SDBCommand(SDBStatement* statement, CS_CONNECTION* connection);

	friend class SDBStatement;
	friend class SDBStatementExecuted;

	inline std::vector<std::vector<void*> *>* releaseResults() {
		std::vector<std::vector<void*> *>* results(new std::vector<std::vector<void*> *>());
		std::copy(this->_results.begin(), this->_results.end(), std::back_inserter(*results));
		this->_results.clear();
		return results;
	}

public:
	~SDBCommand();

	inline bool getIsValid() const noexcept {
		return this->_errors.size() == 0;
	}

	inline void clearErrors() noexcept {
		this->_errors.clear();
	}

	inline const std::list<std::string>& getErrors() const noexcept {
		return this->_errors;
	}

	template <typename T>
	static T do_delete (T t) {
		if (t == nullptr) {
			return t;
		}

		if(typeid(*t)==typeid(std::vector<void*>)) {
			std::vector<void*>* ts{reinterpret_cast<std::vector<void*>*>(t)};
			for (std::vector<void*>::iterator i=ts->begin();  i!=ts->end(); i++) {
				delete static_cast<char*>(*i); // NYI - extend types (also extend creation code)
			}
		}

		delete t;
		return static_cast<T>(nullptr);
	}
};

}

#endif // SDBLIB_SDBCOMMAND_H
