
#include <algorithm>

#include "SDBCommand.h"
#include "SDBLimits.h"
#include "SDBLogger.h"
#include "SDBStatement.h"
#include "SDBString.h"

namespace sdblib {

SDBCommand::SDBCommand(SDBStatement* statement, CS_CONNECTION* connection) :
		_command {nullptr}, _connection {connection}, _results{}, _formats{}, _buffers{}, _datalengths{}, _indicators{}, _sql{
				static_cast<CS_CHAR*>(SDBString::clone(statement->getBuffer().str()))}, _errors {}, _isCmdDrop{false},
				_isCtCancel{false} {
	try {
		CS_INT internalcode{CS_CMD_FAIL};
		CS_INT returncode{CS_FAIL};

		if (!this->_connection) {
			this->_errors.push_back("SDBCommand connection was NULL");
			return;
		}

		if (ct_cmd_alloc(this->_connection, &this->_command) != CS_SUCCEED) {
			this->_errors.push_back("error: ct_cmd_alloc failed");
			return;
		} else {
			this->_isCmdDrop = true;
			this->_isCtCancel = true;
		}

		if (ct_command(this->_command, CS_LANG_CMD, this->_sql.get(), CS_NULLTERM,
		CS_UNUSED) != CS_SUCCEED) {
			this->_errors.push_back("error: ct_command failed");
			return;
		}

		if (ct_send(this->_command) != CS_SUCCEED) {
			this->_errors.push_back("error: ct_send failed");
			return;
		}

		int columnCountCreate{0};
		int rowCount{0};
		CS_INT counter{static_cast<CS_INT>(0)};
		while ((returncode = ct_results(this->_command, &internalcode))
				== CS_SUCCEED ) {

			switch (static_cast<int>(internalcode)) {

			case CS_STATUS_RESULT:
				sdblib::trace << "CS_STATUS_RESULT"
					<< sdblib::endl
					<< sdblib::flush;

				break;

			case CS_ROW_RESULT:
				sdblib::trace << "CS_ROW_RESULT"
					<< sdblib::endl
					<< sdblib::flush;

				for (const sdblib::SDBColumnType column : statement->getColumns()) {
					_results.push_back(new std::vector<void*>{});
					++columnCountCreate;

					CS_CHAR* buffer{new CS_CHAR[SDBLimits::CONST_MAX_STRINGLENGTH + 1]};
					memset(buffer, 0, sizeof(CS_CHAR)*(SDBLimits::CONST_MAX_STRINGLENGTH + 1));
					_buffers.push_back(buffer);

					CS_INT* datalength{new CS_INT{static_cast<CS_INT>(0)}};
					_datalengths.push_back(datalength);

					CS_DATAFMT* format{new CS_DATAFMT{}};
					_formats.push_back(format);

					CS_SMALLINT* indicator{new CS_SMALLINT{static_cast<CS_SMALLINT>(0)}};
					_indicators.push_back(indicator);

					CS_INT columnreturncode{CS_FAIL};

					switch (column) {
					case sdblib::COLUMNTYPE_STRING: // NYI - extend types  (also extend deletion code)
						sdblib::trace << "create COLUMNTYPE_STRING["
							<< columnCountCreate
							<< "]"
							<< sdblib::endl
							<< sdblib::flush;

						format->format = CS_FMT_NULLTERM;
						format->datatype = CS_CHAR_TYPE;
						format->maxlength=SDBLimits::CONST_MAX_STRINGLENGTH;
						format->count = 1;
						format->locale = 0;

						columnreturncode = ct_bind(this->_command, columnCountCreate, format, buffer, datalength, indicator);
						if (columnreturncode != CS_SUCCEED) {
							this->_errors.push_back("error: ct_bind of column failed for COLUMNTYPE_STRING");
							return;
						}
						break;

					case sdblib::COLUMNTYPE_INTEGER:
						sdblib::trace << "create COLUMNTYPE_INTEGER"
							<< columnCountCreate
							<< "]"
							<< sdblib::endl
							<< sdblib::flush;

						this->_errors.push_back("error: invalid unhandled COLUMN type COLUMNTYPE_INTEGER - NYI");
						return;
						break;

					case sdblib::COLUMNTYPE_FLOAT:
						sdblib::trace << "create COLUMNTYPE_FLOAT"
							<< columnCountCreate
							<< "]"
							<< sdblib::endl
							<< sdblib::flush;

						this->_errors.push_back("error: invalid unhandled COLUMN type COLUMNTYPE_FLOAT - NYI");
						return;
						break;

					default:
						this->_errors.push_back("error: invalid unhandled COLUMN type UNKNOWN - NYI");
						return;
						break;
					}
				}

				while ((returncode = ct_fetch(this->_command, CS_UNUSED,
						CS_UNUSED,
						CS_UNUSED, &counter)) == CS_SUCCEED ) {
					rowCount++;

					int columnCountPopulate{0};
					for (const sdblib::SDBColumnType column : statement->getColumns()) {
						++columnCountPopulate;
						std::vector<void*>* result{nullptr};

						switch(column) {
						case sdblib::COLUMNTYPE_STRING:
							sdblib::trace << "populate COLUMNTYPE_STRING["
								<< rowCount
								<< ","
								<< columnCountPopulate
								<< "] - '"
								<< _buffers[columnCountPopulate-1]
								<< "'"
								<< sdblib::endl
								<< sdblib::flush;

							result=_results[columnCountPopulate-1];
							result->push_back(sdblib::SDBString::clone(_buffers[columnCountPopulate-1]));
							break;

						case sdblib::COLUMNTYPE_INTEGER:
							sdblib::trace << "populate COLUMNTYPE_INTEGER["
								<< rowCount
								<< ","
								<< columnCountPopulate
								<< "]"
								<< sdblib::endl
								<< sdblib::flush;

							this->_errors.push_back("error: invalid unhandled COLUMN type COLUMNTYPE_INTEGER - NYI");
							return;
							break;

						case sdblib::COLUMNTYPE_FLOAT:
							sdblib::trace << "populate COLUMNTYPE_FLOAT["
								<< rowCount
								<< ","
								<< columnCountPopulate
								<< "]"
								<< sdblib::endl
								<< sdblib::flush;

							this->_errors.push_back("error: invalid unhandled COLUMN type COLUMNTYPE_FLOAT - NYI");
							return;
							break;

						default:
							this->_errors.push_back("error: invalid unhandled COLUMN type UNKNOWN - NYI");
							return;
							break;
						}
					}
				}

				if (returncode != CS_END_DATA) {
					this->_errors.push_back("read fail condition CS_END_DATA");
					return;
				}

				break;

			case CS_CMD_FAIL:
				this->_errors.push_back("read fail condition CS_CMD_FAIL");
				return;
				break;

			case CS_CMD_SUCCEED :
				sdblib::trace << "CS_CMD_SUCCEED"
					<< sdblib::endl
					<< sdblib::flush;

				break;

			case CS_CMD_DONE:
				sdblib::trace << "CS_CMD_DONE"
					<< sdblib::endl
					<< sdblib::flush;

				break;

			default:
				this->_errors.push_back("read fail condition GENERAL");
				return;
				break;
			}

			if (internalcode != CS_CMD_DONE) {
				return;
			}

			if (returncode != CS_END_RESULTS) {
				return;
			} else {
				this->_isCtCancel = false;
			}
		}
	} catch(...) {
		std::ostringstream os;
		os << "SDBCommand constructor unkownn exception caught";

		this->_errors.push_back(os.str());
		sdblib::error << os.str()
			<< sdblib::endl
			<< sdblib::endl;

		exit(1); // NYI - improve
	}
}

SDBCommand::~SDBCommand() {
	try {
		if (this->_isCtCancel) {
			ct_cancel((CS_CONNECTION *) NULL, this->_command, CS_CANCEL_ALL);
		}

		if (this->_isCmdDrop) {
			ct_cmd_drop(this->_command);
		}

		std::transform(_results.begin(), _results.end(), _results.begin(),
				sdblib::SDBCommand::do_delete<std::vector<void*> *>);

		std::transform(_formats.begin(), _formats.end(), _formats.begin(),
				sdblib::SDBCommand::do_delete<CS_DATAFMT*>);

		std::transform(_buffers.begin(), _buffers.end(), _buffers.begin(),
				sdblib::SDBCommand::do_delete<char*>);

		std::transform(_datalengths.begin(), _datalengths.end(), _datalengths.begin(),
				sdblib::SDBCommand::do_delete<CS_INT*>);

		std::transform(_indicators.begin(), _indicators.end(), _indicators.begin(),
				sdblib::SDBCommand::do_delete<CS_SMALLINT*>);

	} catch (...) {
		std::ostringstream os;
		os << "SDBCommand destructor unkownn exception caught";

		this->_errors.push_back(os.str());
		sdblib::error << os.str()
			<< sdblib::endl
			<< sdblib::endl;

		exit(1); // NYI - improve
	}
}

}

