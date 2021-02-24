#ifndef SDBLIB_SDBSTATEMENTFUNCTOR_H
#define SDBLIB_SDBSTATEMENTFUNCTOR_H

#include "SDBConnection.h"

namespace sdblib {

class SDBStatementExecuted;

class SDBStatementFunctor {
private:
	SDBConnection* _connection;
	SDBStatementExecuted& (*_fp) (SDBStatement&, SDBConnection* connection);

private:
	SDBStatementFunctor(SDBStatementExecuted& (*fp)(SDBStatement&, SDBConnection* connection), SDBConnection* connection) :
    	_connection(connection),_fp{fp}
    {}

    SDBStatementFunctor(const SDBStatementFunctor&);
	SDBStatementFunctor(const SDBStatementFunctor&&);
	SDBStatementFunctor& operator=(const SDBStatementFunctor&);
	SDBStatementFunctor& operator=(const SDBStatementFunctor&&);

	friend SDBStatementFunctor execute(SDBConnection* connection);
	friend SDBStatement;

public:
	~SDBStatementFunctor() {}
};

extern SDBStatementFunctor execute(SDBConnection* connection);

}

#endif // SDBLIB_SDBSTATEMENTFUNCTOR_H
