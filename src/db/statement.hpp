#ifndef STATEMENT_HPP
#define  STATEMENT_HPP

#include <sqlite3.h>

class Statement {
public:
    explicit Statement(sqlite3_stmt* stmt = nullptr);
    ~Statement();

    Statement(const Statement&) = delete;
    Statement& operator=(const Statement&) = delete;

    Statement(Statement&& other) noexcept;
    Statement& operator=(Statement&& other) noexcept;

    [[nodiscard]] sqlite3_stmt* get() const;

    [[nodiscard]] int stepRaw();

private:
    sqlite3_stmt* stmt_;
};


#endif
