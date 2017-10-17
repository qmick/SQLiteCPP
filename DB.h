#pragma once

#include "Statement.h"
#include <string>


struct sqlite3;

namespace sqlite {
	class DB
	{
	public:
		DB(const std::string &filename);
		~DB();

		Statement prepare(const std::string &sql);
		void exec(const std::string &sql, int(*callback)(void*, int, char**, char**), void *relay);
		bool close();

	private:
		sqlite3 *_db;
	};

}

