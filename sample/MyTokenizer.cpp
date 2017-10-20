#include "MyTokenizer.h"
#include <cppjieba/Jieba.hpp>
#include <iostream>
const char* const DICT_PATH = "../dict/jieba.dict.utf8";
const char* const HMM_PATH = "../dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "../dict/user.dict.utf8";
const char* const IDF_PATH = "../dict/idf.utf8";
const char* const STOP_WORD_PATH = "../dict/stop_words.utf8";

using TokenList = std::vector<cppjieba::Word>;

static cppjieba::QuerySegment querySegment(DICT_PATH, HMM_PATH, USER_DICT_PATH);
MyTokenizer::MyTokenizer()
{
}


MyTokenizer::~MyTokenizer()
{
}

int MyTokenizer::open(sqlite::Cursor &cursor)
{
	auto words = new TokenList;
	querySegment.Cut(cursor.input, *words);
	cursor.context = (void *)words;
	return 0;
}

int MyTokenizer::close(sqlite::Cursor &cursor)
{
	delete (TokenList *)cursor.context;
	return 0;
}

bool MyTokenizer::next(sqlite::Cursor &cursor)
{
	auto words = (TokenList *)cursor.context;
	//No token left, done
	if (cursor.position == words->size() - 1)
		return true;

	//Number of token returned
	cursor.position++;

	//Token string
	auto &token = (*words)[cursor.position];
	memcpy(cursor.token_str, token.word.c_str(), token.word.size() + 1);
	cursor.bytes = (int)token.word.size();

	//Token offset
	cursor.start = token.offset;
	cursor.end = cursor.start + cursor.bytes;

	return false;
}
