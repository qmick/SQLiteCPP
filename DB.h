#pragma once

#include "Statement.h"
#include "Tokenizer.h"
#include "ResultSet.h"
#include <string>
#include <memory>

struct sqlite3;

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
		int update(const std::string &sql, Args...args)
		{
			auto stmt = prepare(sql);
			if (sizeof...(Args) > 0)
				stmt.bind_all(args);
			return stmt.step() == Statement::DONE;
		}

		void exec(const std::string &sql, int(*callback)(void*, int, char**, char**), void *relay);
		bool close();

		bool add_tokenizer(const std::string &name, std::shared_ptr<Tokenizer> tokenizer);
		
	private:
		sqlite3 *_db;
	};


}

