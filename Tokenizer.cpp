#include "Tokenizer.h"
#include "fts3_tokenizer.h"
#include <unordered_map>
#include <memory>

namespace sqlite {

	struct InternalTokenizer
	{
		Tokenizer *real;
	};

	struct InternalCursor
	{
		InternalTokenizer *tokenizer;
		Token token;
	};

	std::unordered_map<std::string, std::shared_ptr<Tokenizer>> _tokenizers;

	static int xCreate(int argc, const char *const*argv,
		sqlite3_tokenizer **ppTokenizer
	)
	{
		if (argc != 1)
			return SQLITE_ERROR;

		auto tokenizer = (InternalTokenizer*)sqlite3_malloc(sizeof(InternalTokenizer));
		if (!tokenizer)
			return SQLITE_NOMEM;

		tokenizer->real = _tokenizers[argv[0]].get();
		*ppTokenizer = (sqlite3_tokenizer *)&tokenizer;
		return SQLITE_OK;
	}

	static int xDestroy(sqlite3_tokenizer *pTokenizer)
	{
		sqlite3_free(pTokenizer);
		return SQLITE_OK;
	}

	static int xOpen(
		sqlite3_tokenizer *pTokenizer,       /* Tokenizer object */
		const char *pInput, int nBytes,      /* Input buffer */
		sqlite3_tokenizer_cursor **ppCursor  /* OUT: Created tokenizer cursor */
	)
	{
		auto tokenizer = (InternalTokenizer *)pTokenizer;
		tokenizer->real->open(std::string(pInput, nBytes));
		auto cursor = (InternalCursor *)sqlite3_malloc(sizeof(InternalCursor));
		if (!cursor)
			return SQLITE_NOMEM;
		cursor->tokenizer = tokenizer;
		cursor->token.position = -1;
		return SQLITE_OK;
	}

	static int xClose(sqlite3_tokenizer_cursor *pCursor)
	{
		auto cursor = (InternalCursor *)pCursor;
		cursor->tokenizer->real->close();
		sqlite3_free(cursor);
		return SQLITE_OK;
	}

	static int xNext(
		sqlite3_tokenizer_cursor *pCursor,   /* Tokenizer cursor */
		const char **ppToken, int *pnBytes,  /* OUT: Normalized text for token */
		int *piStartOffset,  /* OUT: Byte offset of token in input buffer */
		int *piEndOffset,    /* OUT: Byte offset of end of token in input buffer */
		int *piPosition      /* OUT: Number of tokens returned before this one */
	)
	{
		auto cursor = (InternalCursor *)pCursor;
		auto tokenizer = cursor->tokenizer;
		tokenizer->real->next(cursor->token);
		*ppToken = cursor->token.token_str.c_str();
		*pnBytes = cursor->token.token_str.size();
		*piStartOffset = cursor->token.start;
		*piEndOffset = cursor->token.end;
		*piPosition = cursor->token.position;
		return SQLITE_OK;
	}

	static const sqlite3_tokenizer_module _module =
	{
		0,
		xCreate,
		xDestroy,
		xOpen,
		xClose,
		xNext,
		NULL
	};

}