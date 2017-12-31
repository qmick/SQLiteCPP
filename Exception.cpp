#include "Exception.h"

#include "sqlite3.h"

namespace sqlite {
    Exception::Exception(sqlite3 *db, const std::string &message)
        : std::runtime_error(message + ": " + sqlite3_errmsg(db))
    {

    }

    Exception::~Exception()
    {
    }

}