

#ifndef SDBLIB_SDBRESULT_H
#define SDBLIB_SDBRESULT_H

#include <string>
#include <list>
#include <vector>
#include <cassert>
#include <climits>

#include "SDBColumn.h"
#include "SDBLogger.h"

namespace sdblib {

class SDBResult {
private:
	SDBResult(const SDBResult&);
	SDBResult(const SDBResult&&);
	SDBResult& operator=(const SDBResult&);
	SDBResult& operator=(const SDBResult&&);

	std::list<std::string> _errors;
	std::vector<std::vector<void*> *>* _results;
	std::vector<sdblib::SDBColumnType> _columns;
	int _columnCount;
	int _rowCount;

	template<typename T1, typename UnqualifiedType = std::remove_cv_t<T1>>
    class iterator_base : public std::iterator<std::forward_iterator_tag,
    															UnqualifiedType,
																std::ptrdiff_t,
																T1*,
																T1&>
    {
        public:
	    	iterator_base(int index, const SDBResult* result)
	        	: _index(index), _result(const_cast<SDBResult*>(result)) {
	    	}

    	    ~iterator_base() {}

    	    iterator_base(const iterator_base<T1>& i)
	        	: _index(i._index), _result(i._result)
	    	{}

    	    template<typename T2>
	    	bool operator==(const iterator_base<T2>& i) const {
	    		return this->_index==i._index;
	    	}

	    	template<typename T2>
	    	bool operator!=(const iterator_base<T2>& i) const {
	    		return this->_index!=i._index;
	    	}

	    	iterator_base<T1> operator++() {
	    		this->_index++;
	    		return (*this);
	    	}

	    	iterator_base<T1> operator++(int ignored) {
	    		iterator_base<T1> saved(*this);
	    		this->_index++;
	    		return saved;
	    	}

	    	auto operator*() const {
	    		return this;
	    	}

	    	auto operator->() const {
	    		return this;
	    	}

	    	char* getCharValue(unsigned int columnIndex) {
	    		assert(this->_result != nullptr && "invalid operator-> on null iterator");
	    		assert(this->_result->_results->size() >= columnIndex && "invalid operator-> overflow on column count for iterator");

	    		std::vector<void*> *v=this->_result->_results->operator[](columnIndex);
	    		assert(v->size() >=this->_index && "invalid operator-> overflow on row count for iterator");

	    		return reinterpret_cast<char*>(v->operator[](this->_index));
	    	}

	        void swap(iterator_base<T1>& i) {
	            using std::swap;
	            swap(this->_index, i._index);
	        }

    		unsigned int _index;

        private:
    		SDBResult* _result;
    };

public:
	typedef iterator_base<std::vector<void*>*> iterator;
	typedef iterator_base<const std::vector<void*>*> const_iterator;

	iterator begin() {
		return iterator(0, this);
	}

	const_iterator begin() const {
		return const_iterator(0, this);
	}

	iterator end() {
		if (!this->_results &&
				this->_results->size()) {
			return iterator(0, this);
		}

		return iterator(this->_rowCount, this);
	}

	const_iterator end() const {
		if (!this->_results) {
			return const_iterator(0, this);
		}

		return const_iterator(this->_rowCount, this);
	}

public:
	SDBResult() :
		_errors{}, _results{nullptr}, _columns{}, _columnCount{0},  _rowCount{0}
		{}

	~SDBResult()
	{}

	inline bool getIsValid() const noexcept {
		return this->_errors.size() == 0;
	}

	inline const std::list<std::string>& getErrors() const noexcept {
		return this->_errors;
	}

	inline bool setResult(const std::vector<sdblib::SDBColumnType>& columns, std::vector<std::vector<void*> *>* results) noexcept {
		this->_columns.insert(std::end(this->_columns), std::begin(columns), std::end(columns));
		_results=results;

		if(this->_results == nullptr) {
			this->_errors.push_back("results structure is NULL");
			return false;
		}

		if(this->_columns.size() != this->_results->size()) {
			this->_errors.push_back("results structure mismatch");
			return false;
		}

		int columnCount{0};
		unsigned int rowCount{0};
		for (std::vector<std::vector<void*> *>::iterator i=this->_results->begin(); i!=this->_results->end(); i++) {
			++columnCount;
			if (rowCount == 0) {
				rowCount = (*i)->size();
			} else if (rowCount != (*i)->size()) {
				this->_errors.push_back("results structure result counts mismatch");
				return false;
			}
		}

		this->_columnCount=columnCount;
		this->_rowCount=rowCount;

		return true;
	}

	inline int getColumnCount() const noexcept {
		if (!_results) {
			return 0;
		}

		return this->_columnCount;
	}

	inline int getRowCount() const noexcept {
		if (!_results) {
			return 0;
		}

		return this->_rowCount;
	}

};

}

#endif // SDBLIB_SDBRESULT_H

