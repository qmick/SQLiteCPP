#pragma once

#include "Statement.h"
#include "Tokenizer.h"
#include "ResultSet.h"
#include <string>
#include <memory>

struct sqlite3;
struct sqlite3_context;
struct sqlite3_value;

namespace sqlite {
	class DB
	{
	public:
		DB(const std::string &filename);
		~DB();

		Statement prepare(const std::string &sql);

		template <typename...Args>
		ResultSet query(const std::string &sql, Args...args)
		{
			auto stmt = prepare(sql);
			if (sizeof...(Args) > 0)
				stmt.bind_all(args...);
			return ResultSet(stmt);
		}

		template <typename...Args>
		bool update(const std::string &sql, Args...args)
		{
			auto stmt = prepare(sql);
			if (sizeof...(Args) > 0)
				stmt.bind_all(args...);
			return stmt.step() == Statement::DONE;
		}

		bool create_function(const std::string &name, int nArg, 
			                 void(*xFunc)(sqlite3_context*, int, sqlite3_value**));

		void exec(const std::string &sql, int(*callback)(void*, int, char**, char**), void *relay);
		bool close();

		bool add_tokenizer(const std::string &name, std::shared_ptr<Tokenizer> tokenizer);
		
	private:
		sqlite3 *_db;
	};


}

