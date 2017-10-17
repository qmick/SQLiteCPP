#include "DB.h"
#include "Exception.h"
#include "sqlite3.h"

namespace sqlite {
	DB::DB(const std::string & filename)
	{
		if (sqlite3_open(filename.c_str(), &_db) != SQLITE_OK)
			throw Exception(_db, "cannot open db");
	}

	Statement DB::prepare(const std::string &sql)
	{
		return Statement(_db, sql);
	}

	void DB::exec(const std::string &sql, int(*callback)(void *, int, char **, char **), void *relay)
	{
		auto ret = sqlite3_exec(_db, sql.c_str(), callback, relay, NULL);
		if (ret != SQLITE_OK && ret != SQLITE_ABORT)
			throw Exception(_db, "cannot exec " );
	}

	bool DB::close()
	{
		auto ret = sqlite3_close(_db);
		if (ret == SQLITE_OK)
		{
			_db = NULL;
			return true;
		}
		else if (ret == SQLITE_BUSY)
			return false;
		else
			throw Exception(_db, "cannot close db");
	}

	DB::~DB()
	{
		if (_db)
			sqlite3_close(_db);
	}

}