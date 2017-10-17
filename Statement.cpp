#include "Statement.h"
#include "DB.h"
#include "Exception.h"
#include "sqlite3.h"

namespace sqlite {

	Statement::Statement(sqlite3 *db, const std::string &sql)
		: _db(db), _sql(sql)
	{
		if (sqlite3_prepare_v2(db, sql.c_str(), sql.size(), &_stmt, NULL) != SQLITE_OK)
			throw Exception(db, std::string("cannot prepare ") + sql);
	}

	Statement::~Statement()
	{
		finalize();
	}

	void Statement::finalize()
	{
		sqlite3_finalize(_stmt);
		_stmt = NULL;
	}

	void Statement::bind(int idx, const void *blob, int n, void(*dtor)(void *))
	{
		if (sqlite3_bind_blob(_stmt, idx, blob, n, dtor) != SQLITE_OK)
			throw Exception(_db, "cannot bind");
	}

	void Statement::bind(int idx, const void *blob, uint64_t n, void(*dtor)(void *))
	{
		if (sqlite3_bind_blob64(_stmt, idx, blob, n, dtor) != SQLITE_OK)
			throw Exception(_db, "cannot bind");
	}

	void Statement::bind(int idx, double value)
	{
		if (sqlite3_bind_double(_stmt, idx, value) != SQLITE_OK)
			throw Exception(_db, "cannot bind");
	}

	void Statement::bind(int idx, int value)
	{
		if (sqlite3_bind_int(_stmt, idx, value) != SQLITE_OK)
			throw Exception(_db, "cannot bind");
	}

	void Statement::bind(int idx, int64_t value)
	{
		if (sqlite3_bind_int64(_stmt, idx, value) != SQLITE_OK)
			throw Exception(_db, "cannot bind");
	}

	void Statement::bind(int idx)
	{
		if (sqlite3_bind_null(_stmt, idx) != SQLITE_OK)
			throw Exception(_db, "cannot bind");
	}

	void Statement::bind(int idx, const std::string &text)
	{
		if (sqlite3_bind_text(_stmt, idx, text.c_str(), text.size(), SQLITE_TRANSIENT) != SQLITE_OK)
			throw Exception(_db, "cannot bind");
	}

	void Statement::bind(int idx, const char *text, int n, void(*dtor)(void *))
	{
		if (sqlite3_bind_text(_stmt, idx, text, n, dtor) != SQLITE_OK)
			throw Exception(_db, "cannot bind");
	}

	void Statement::bind(int idx, const char *text, uint64_t n, void(*dtor)(void *), unsigned char encoding)
	{
		if (sqlite3_bind_text64(_stmt, idx, text, n, dtor, encoding) != SQLITE_OK)
			throw Exception(_db, "cannot bind");
	}

	void Statement::bind(int idx, const sqlite3_value *value)
	{
		if (sqlite3_bind_value(_stmt, idx, value) != SQLITE_OK)
			throw Exception(_db, "cannot bind");
	}

	void Statement::bind(int idx, void *ptr, const char *type, void(*dtor)(void *))
	{
		if (sqlite3_bind_pointer(_stmt, idx, ptr, type, dtor) != SQLITE_OK)
			throw Exception(_db, "cannot bind");
	}

	//void Statement::bind_text16(int idx, const void * text, int n, void(*dtor)(void *))
	//{
	//	if (sqlite3_bind_text16(_stmt, idx, text, n, dtor) != SQLITE_OK)
	//		throw Exception(_db, "cannot bind");
	//}

	void Statement::bind_zeroblob(int idx, int n)
	{
		if (sqlite3_bind_zeroblob(_stmt, idx, n) != SQLITE_OK)
			throw Exception(_db, "cannot bind");
	}

	void Statement::bind_zeroblob64(int idx, uint64_t n)
	{
		if (sqlite3_bind_zeroblob64(_stmt, idx, n) != SQLITE_OK)
			throw Exception(_db, "cannot bind");
	}

	Statement::State Statement::step()
	{
		auto ret = sqlite3_step(_stmt);
		switch (ret)
		{
		case SQLITE_ROW:
			return ROW;
		case SQLITE_DONE:
			return DONE;
		case SQLITE_BUSY:
			return BUSY;
		case SQLITE_MISUSE:
			return MISUSE;
		default:
			throw Exception(_db, "cannot step");
		}
	}

	void Statement::reset()
	{
		if (sqlite3_reset(_stmt) != SQLITE_OK)
			throw Exception(_db, "cannot reset");
	}

	template<> const void* Statement::column<const void*>(int iCol)
	{
		return sqlite3_column_blob(_stmt, iCol);
	}

	template<> double Statement::column<double>(int iCol)
	{
		return sqlite3_column_double(_stmt, iCol);
	}

	template<> int Statement::column<int>(int iCol)
	{
		return sqlite3_column_int(_stmt, iCol);
	}

	template<> int64_t Statement::column<int64_t>(int iCol)
	{
		return sqlite3_column_int64(_stmt, iCol);
	}

	template<> std::string Statement::column<std::string>(int iCol)
	{
		auto len = column_bytes(iCol);
		return std::string(reinterpret_cast<const char*>(sqlite3_column_text(_stmt, iCol)), len);
	}

	template<> const unsigned char* Statement::column<const unsigned char*>(int iCol)
	{
		return sqlite3_column_text(_stmt, iCol);
	}

	template<> sqlite3_value* Statement::column<sqlite3_value*>(int iCol)
	{
		return sqlite3_column_value(_stmt, iCol);
	}

	const char * Statement::column_name(int N)
	{
		return sqlite3_column_name(_stmt, N);
	}

	//const void* Statement::column_text16(int iCol)
	//{
	//	return sqlite3_column_text16(_stmt, iCol);
	//}

	Statement::DataType Statement::column_type(int iCol)
	{
		return (DataType)sqlite3_column_type(_stmt, iCol);
	}

	int Statement::column_bytes(int iCol)
	{
		return sqlite3_column_bytes(_stmt, iCol);
	}

 //   int Statement::column_bytes16(int iCol)
	//{
	//	return sqlite3_column_bytes16(_stmt, iCol);
	//}
}
