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

	int open(const std::string & input) override;
	int close() override;
	bool next(sqlite::Token &token) override;

private:
	std::vector<cppjieba::Word> _words;
	cppjieba::Jieba _jieba;
};

