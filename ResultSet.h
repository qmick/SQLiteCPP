#pragma once

#include <tuple>
#include "Statement.h"

namespace sqlite {
    class ResultSet
    {
    public:
        ResultSet(ResultSet &&other)
            :_stmt(std::move(other._stmt)) {}
        explicit ResultSet(Statement &stmt)
            :_stmt(std::move(stmt)) {}
        ~ResultSet() {}
        bool next()
        {
            return _stmt.step() == Statement::ROW;
        }

        template <typename...Args>
        std::tuple<Args...> get()
        {
            return _stmt.column_all<Args...>();
        }

    private:
        Statement _stmt;
    };

}
