
#ifndef SDBLIB_SDBCONNECTION_H
#define SDBLIB_SDBCONNECTION_H

#include <list>
#include <string>
#include <memory>

#include <ctpublic.h>
#include <cspublic.h>

namespace sdblib {

class SDBStatement;

class SDBConnection {
private:
	SDBConnection(const SDBConnection&);
	SDBConnection(const SDBConnection&&);
	SDBConnection& operator=(const SDBConnection&);
	SDBConnection& operator=(const SDBConnection&&);

	bool _isCtxDrop;
	bool _isCtExit;
	bool _isConDrop;
	bool _isCtClose;

	CS_CONTEXT* _context;
	CS_CONNECTION* _connection;

	std::list<std::string> _errors;

	const std::string _username;
	const std::string _password;
	const std::string _server;
	const std::string _programName;

	CS_CONTEXT* getContext() {
		return _context;
	}

public:
	SDBConnection(const std::string& username, const std::string& password, const std::string& server, const std::string& programName);
	~SDBConnection();

	static const std::string CONST_CONFIGURATION_PARAMETERNAME_USERNAME;
	static const std::string CONST_CONFIGURATION_PARAMETERNAME_PASSWORD;
	static const std::string CONST_CONFIGURATION_PARAMETERNAME_DATABASE;
	static const std::string CONST_CONFIGURATION_PARAMETERNAME_SERVER;

	inline const std::list<std::string>& getErrors() const noexcept {
		return this->_errors;
	}

	inline const std::string& getUsername() const noexcept {
		return this->_username;
	}

	inline const std::string& getPassword() const noexcept {
		return this->_password;
	}

	inline const std::string& getServer() const noexcept {
		return this->_server;
	}

	inline const std::string& getProgramName() const noexcept {
		return _programName;
	}

	inline bool getIsValid() const noexcept {
		return this->_errors.size() == 0;
	}

	inline void clearErrors() noexcept {
		this->_errors.clear();
	}

	inline CS_CONNECTION* getConnection() const noexcept {
		return _connection;
	}

};

}


#endif // SDBLIB_SDBCONNECTION_H
