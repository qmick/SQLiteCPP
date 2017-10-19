#pragma once

#include <string>

struct sqlite3_tokenizer_module;
struct sqlite3_tokenizer;
struct sqlite3_tokenizer_cursor;

namespace sqlite {

	struct Token
	{
		char token_str[128];
		char bytes;
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
		virtual bool next(Token &token) = 0;
	};
}