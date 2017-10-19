#pragma once

#include <string>

struct sqlite3_tokenizer_module;
struct sqlite3_tokenizer;
struct sqlite3_tokenizer_cursor;

namespace sqlite {

	struct Token
	{
		std::string token_str;
		int start;
		int end;
		int position;
	};

	class Tokenizer
	{
	public:
		Tokenizer() {}
		virtual ~Tokenizer() {}
		virtual int open(const std::string &input) = 0;
		virtual int close() = 0;
		virtual void next(Token &token) = 0;
	};
}