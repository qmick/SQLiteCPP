#pragma once

#include "Statement.h"
#include "Tokenizer.h"
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
		void exec(const std::string &sql, int(*callback)(void*, int, char**, char**), void *relay);
		bool close();

		bool add_tokenizer(const std::string &name, std::shared_ptr<Tokenizer> tokenizer);
		
	private:
		sqlite3 *_db;
	};

}

