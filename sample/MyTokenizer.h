#pragma once

#include "../Tokenizer.h"
#include <cppjieba/Jieba.hpp>
#include <memory>
#include <vector>
#include <string>


// Custom tokenizer using Jieba Tokenizer
class MyTokenizer :
	public sqlite::Tokenizer
{
public:
	MyTokenizer();
	~MyTokenizer() override;

	int open(sqlite::Cursor &cursor) override;
	int close(sqlite::Cursor &cursor) override;
	bool next(sqlite::Cursor &cursor) override;

private:
	//std::vector<cppjieba::Word> _words;
	//cppjieba::Jieba _jieba;
};

