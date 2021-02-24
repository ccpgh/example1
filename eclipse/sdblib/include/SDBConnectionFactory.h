
#ifndef SDBLIB_SDBCONNECTIONFACTORY_H
#define SDBLIB_SDBCONNECTIONFACTORY_H

#include <memory>

#include "SDBConnection.h"
#include "SDBConfiguration.h"

namespace sdblib {

class SDBConnectionFactory {
private:
	SDBConnectionFactory();
	SDBConnectionFactory(const SDBConnectionFactory&);
	SDBConnectionFactory(const SDBConnectionFactory&&);
	SDBConnectionFactory& operator=(const SDBConnectionFactory&);
	SDBConnectionFactory& operator=(const SDBConnectionFactory&&);
	~SDBConnectionFactory();

public:
	static std::unique_ptr<SDBConnection> getDBConnection(sdblib::SDBConfiguration* configuration);
};

}

#endif // SDBLIB_SDBCONNECTIONFACTORY_H

