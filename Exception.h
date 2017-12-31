#pragma once

#include <stdexcept>

struct sqlite3;

namespace sqlite {
    class Exception : public virtual std::runtime_error
    {
    public:
        Exception(sqlite3 *db, const std::string &message);
        ~Exception();
    };

}

