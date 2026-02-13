#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <sqlite3.h>
#include <string>

#include "statement.hpp"

class Database {
  public:
    explicit Database(const std::string& dbPfad);

    ~Database();

    [[nodiscard]] sqlite3* get() const;

    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    Database(Database&& other) noexcept;
    Database& operator=(Database&& other) noexcept;

    Statement prepare(const char* sql);

    void execute(const char* sql);

  private:
    sqlite3* db_;
};

#endif
