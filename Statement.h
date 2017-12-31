#pragma once

#include <string>
#include <tuple>
#include <cstdint>

struct sqlite3_value;
struct sqlite3_stmt;
struct sqlite3;

namespace sqlite {
    class Statement
    {
    public:
        enum State { BUSY, ROW, DONE, MISUSE };
        enum DataType { INTEGER, FLOAT, TEXT, BLOB, NUL };
        enum DestructorType { STATIC, TRANSIENT };

        Statement(sqlite3 *db, const std::string &sql);
        Statement(Statement &&other);
        ~Statement();

        void finalize();

        void bind(int idx, const void *blob, int n, DestructorType dtor);
        void bind(int idx, const void *blob, uint64_t n, DestructorType dtor);
        void bind(int idx, double value);
        void bind(int idx, int value);
        void bind(int idx, int64_t value);
        void bind(int idx);
        void bind(int idx, const std::string &text);
        void bind(int idx, const char *text, int n, DestructorType dtor);
        void bind(int idx, const char *text, uint64_t n, DestructorType dtor, unsigned char encoding);
        void bind(int idx, const sqlite3_value *value);
        void bind(int idx, void *ptr, const char *type, DestructorType dtor);
        void bind_zeroblob(int idx, int n);
        void bind_zeroblob64(int idx, uint64_t n);

        template <typename ...Args>
        void bind_all(Args...args)
        {
            _bind(1, args...);
        }

        State step();
        void reset();

        template <typename T>
        T column(int iCol);

        template <typename ...Args>
        std::tuple<Args...> column_all()
        {
            std::tuple<Args...> values;
            _column<0>(values);
            return values;
        }

        const char* column_name(int N);
        DataType column_type(int iCol);
        int column_bytes(int iCol);

    private:
        sqlite3 *_db;
        sqlite3_stmt *_stmt;
        const std::string _sql;

        void _bind(int) {}

        template <typename T, typename ...Args>
        void _bind(int idx, T value, Args...args)
        {
            bind(idx, value);
            _bind(idx + 1, args...);
        }

        template <size_t index, typename ...Args>
        typename std::enable_if<index == sizeof...(Args)>::type
        _column(std::tuple<Args...> &) {}

        template <size_t index, typename ...Args>
        typename std::enable_if<(index < sizeof...(Args))>::type
        _column(std::tuple<Args...> &values)
        {
            _column_value(index, std::get<index>(values));
            _column<index + 1>(values);
        }

        template <typename T>
        void _column_value(int iCol, T &value)
        {
            value = std::move(column<T>(iCol));
        }
    };
}
