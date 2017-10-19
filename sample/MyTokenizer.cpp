#include "MyTokenizer.h"
#include <cppjieba/Jieba.hpp>
#include <iostream>
const char* const DICT_PATH = "../dict/jieba.dict.utf8";
const char* const HMM_PATH = "../dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "../dict/user.dict.utf8";
const char* const IDF_PATH = "../dict/idf.utf8";
const char* const STOP_WORD_PATH = "../dict/stop_words.utf8";

MyTokenizer::MyTokenizer()
	:_jieba(cppjieba::Jieba(DICT_PATH, HMM_PATH,
							USER_DICT_PATH,
							IDF_PATH,
							STOP_WORD_PATH))
{
}


MyTokenizer::~MyTokenizer()
{
}

int MyTokenizer::open(const std::string & input)
{
	_jieba.CutForSearch(input, _words, true);
	return 0;
}

int MyTokenizer::close()
{
	_words.clear();
	return 0;
}

bool MyTokenizer::next(sqlite::Token &token)
{
	//No token left, done
	if (token.position == _words.size() - 1)
		return true;

	//Number of token returned
	token.position++;

	//Token string
	memcpy(token.token_str, _words[token.position].word.c_str(), _words[token.position].word.size() + 1);
	token.bytes = (int)_words[token.position].word.size();

	//Token offset
	token.start = _words[token.position].offset;
	token.end = token.start + token.bytes;

	return false;
}
