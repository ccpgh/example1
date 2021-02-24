
#include <algorithm>
#include <memory>
#include <string>
#include <sstream>

#include "SDBConnection.h"
#include "SDBLogger.h"
#include "SDBString.h"
#include "SDBCommand.h"

#define ERR_CH stderr
#define OUT_CH stdout

namespace sdblib {

CS_RETCODE csmsg_callback(CS_CONTEXT* context, CS_CLIENTMSG* clientmsg);
CS_RETCODE clientmsg_callback(CS_CONTEXT* context,CS_CONNECTION* connection,CS_CLIENTMSG* clientmsg);
CS_RETCODE servermsg_callback(CS_CONTEXT* context, CS_CONNECTION* connection, CS_SERVERMSG* servermsg);

const std::string SDBConnection::CONST_CONFIGURATION_PARAMETERNAME_USERNAME{"-u"};
const std::string SDBConnection::CONST_CONFIGURATION_PARAMETERNAME_PASSWORD{"-p"};
const std::string SDBConnection::CONST_CONFIGURATION_PARAMETERNAME_DATABASE{"-db"};
const std::string SDBConnection::CONST_CONFIGURATION_PARAMETERNAME_SERVER{"-s"};

SDBConnection::SDBConnection(const std::string& username, const std::string& password, const std::string& server, const std::string& programName) :
	_isCtxDrop{false}, _isCtExit{false}, _isConDrop{false}, _isCtClose{false}, _context{nullptr}, _connection{nullptr}, _errors{},_username{username}, _password{password}, _server{server}, _programName{programName} {

	if (!username.length() ||
			!password.length() ||
			!server.length()) {
		this->_errors.push_back("error: empty argument in SDBConnection constructor");
		return;
	}

	if (cs_ctx_alloc(CS_CURRENT_VERSION, &this->_context) != CS_SUCCEED) {
		this->_errors.push_back("error: cs_ctx_alloc failed");
		return;
	} else {
		this->_isCtxDrop=true;
	}

	if (ct_init(this->_context, CS_CURRENT_VERSION) != CS_SUCCEED) {
		this->_errors.push_back("error: ct_init failed");
		return;
	} else {
		this->_isCtExit =true;
	}

	if(cs_config(this->_context, CS_SET, CS_MESSAGE_CB, (CS_VOID*)csmsg_callback, CS_UNUSED, NULL) != CS_SUCCEED) {
		this->_errors.push_back("error: cs_config failed");
		return;
	}

	if(ct_callback(this->_context, NULL, CS_SET, CS_CLIENTMSG_CB, (CS_VOID*)clientmsg_callback) != CS_SUCCEED) {
		this->_errors.push_back("error: ct_callback failed");
		return;
	}

	if(ct_callback(this->_context, NULL, CS_SET, CS_SERVERMSG_CB, (CS_VOID*)servermsg_callback) != CS_SUCCEED) {
		this->_errors.push_back("error: ct_callback failed");
		return;
	}

	if (ct_con_alloc(this->_context, &_connection) != CS_SUCCEED) {
		this->_errors.push_back("error: ct_con_alloc failed");
		return;
	} else {
		this->_isConDrop=true;
	}

	if (ct_con_props(this->_connection,
			CS_SET,
			CS_USERNAME,
			(CS_VOID*) username.c_str(),
			CS_NULLTERM,
			(CS_INT*)NULL) != CS_SUCCEED) {
		this->_errors.push_back("CS_USERNAME failed");
		return;
	}

	if (ct_con_props(this->_connection,
			CS_SET,
			CS_PASSWORD,
			(CS_VOID*) password.c_str(),
			CS_NULLTERM,
			(CS_INT*)NULL) != CS_SUCCEED) {
		this->_errors.push_back("CS_PASSWORD failed");
		return;
	}

	if (ct_con_props(this->_connection,
			CS_SET,
			CS_APPNAME,
			(CS_VOID*) programName.c_str(),
			CS_NULLTERM,
			(CS_INT*)NULL) != CS_SUCCEED) {
		this->_errors.push_back("CS_APPNAME failed");
		return;
	}

	if (ct_connect(this->_connection,
			(CS_CHAR*) server.c_str(),
			CS_NULLTERM) != CS_SUCCEED) {
		this->_errors.push_back("ct_connect failed");
		return;
	} else {
		this->_isCtClose=true;
	}
}

SDBConnection::~SDBConnection() {
	if(this->_isCtClose) {
		ct_close(this->_connection, CS_UNUSED);
	}

	if(this->_isConDrop) {
		ct_con_drop(this->_connection);
	}

	if(this->_isCtExit) {
		ct_exit(this->_context, CS_FORCE_EXIT);
	}

	if (this->_isCtxDrop) {
		cs_ctx_drop(this->_context);
	}
}

// Client-Library will call this routine when it receives a message from the server.
CS_RETCODE
servermsg_callback(CS_CONTEXT *cp, CS_CONNECTION *chp, CS_SERVERMSG *msgp) {
	if (msgp->severity == 10 && // NYI - fix
			msgp->msgnumber == 5701) {
		return CS_SUCCEED;
	}

	sdblib::error << "server message: number("
		<< msgp->msgnumber
		<< ") severity("
		<< msgp->severity
		<< ") state("
		<< msgp->state
		<< ") line("
		<< msgp->line
		<< ")";

	if (msgp->svrnlen > 0) {
		sdblib::error << " server name: "
			<< msgp->svrname
			<< " ";
	}

	if (msgp->proclen > 0) {
		sdblib::error << " procedure name: "
				<< msgp->proc
				<< " ";
	}

	std::string text{msgp->text};
	std::replace(text.begin(), text.end(),'\n', ' ');
	std::transform(text.begin(), text.end(), text.begin(), ::tolower);
	sdblib::error << text
			<< sdblib::endl
			<< sdblib::flush;

	return CS_SUCCEED;
}

// This function will be invoked when a Client-Library has detected an error.
// Before Client-Library routines return CS_FAIL, this handler will be called with additional error information.
CS_RETCODE
clientmsg_callback(CS_CONTEXT *context, CS_CONNECTION *conn, CS_CLIENTMSG *emsgp) {
	sdblib::error << "client library error: severity("
			<< emsgp->severity
			<< ") number("
			<< emsgp->msgnumber
			<< ") origin("
			<< emsgp->msgnumber
			<< ") layer("
			<< emsgp->msgnumber
			<< ") ";

	std::string text{emsgp->msgstring};
	std::replace(text.begin(), text.end(),'\n', ' ');
	std::transform(text.begin(), text.end(), text.begin(), ::tolower);
	sdblib::error << text;

	if (emsgp->osstringlen > 0) {
		std::string text{emsgp->osstring};
		std::replace(text.begin(), text.end(),'\n', ' ');
		std::transform(text.begin(), text.end(), text.begin(), ::tolower);

		sdblib::error << " operating system error number("
				<< emsgp->osnumber
				<< ") "
				<< text;
	}

	sdblib::error << sdblib::endl
			<< sdblib::flush;

	return CS_SUCCEED;
}

// CS-Library error handler. This function will be invoked when CS-Library has detected an error.
CS_RETCODE
csmsg_callback(CS_CONTEXT *context, CS_CLIENTMSG *emsgp) {
	sdblib::error << "cs-library error: severity("
			<< emsgp->msgnumber
			<< ") layer("
			<< emsgp->msgnumber
			<< ") origin("
			<< emsgp->msgnumber
			<< ") number("
			<< emsgp->msgnumber
			<< ") ";

	std::string text{emsgp->msgstring};
	std::replace(text.begin(), text.end(),'\n', ' ');
	std::transform(text.begin(), text.end(), text.begin(), ::tolower);
	sdblib::error << text;

	if (emsgp->osstringlen > 0) {
		std::string text{emsgp->osstring};
		std::replace(text.begin(), text.end(),'\n', ' ');
		std::transform(text.begin(), text.end(), text.begin(), ::tolower);

		sdblib::error << " operating system error number("
				<< emsgp->osnumber
				<< ") "
				<< text;
	}

	sdblib::error << sdblib::endl
			<< sdblib::flush;

	return CS_SUCCEED;
}

}


