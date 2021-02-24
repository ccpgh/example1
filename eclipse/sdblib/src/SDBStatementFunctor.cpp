
#include "SDBStatementFunctor.h"
#include "SDBStatement.h"

namespace sdblib {

SDBStatementFunctor execute(SDBConnection* connection) {
	return SDBStatementFunctor(SDBStatement::do_execute, connection);
}

}
