#include "database.hpp"
#include <stdexcept>

namespace infrastructure::db {
Database::Database(const std::string& dbPfad) : db_(nullptr) {
    if (sqlite3_open(dbPfad.c_str(), &db_) != SQLITE_OK) {
        std::string fehler = sqlite3_errmsg(db_);
        sqlite3_close(db_);
        db_ = nullptr;
        throw std::runtime_error("Datenbank konnte nicht geöffnet werden: " + fehler);
    }
}

Database::~Database() {
    if (db_) {
        sqlite3_close(db_);
        db_ = nullptr;
    }
}

[[nodiscard]] sqlite3* Database::get() const {
    return db_;
}

Database::Database(Database&& other) noexcept : db_(other.db_) {
    other.db_ = nullptr;
}

Database& Database::operator=(Database&& other) noexcept {
    if (this != &other) {
        if (db_) {
            sqlite3_close(db_);
        }
        db_ = other.db_;
        other.db_ = nullptr;
    }
    return *this;
}

Statement Database::prepare(const char* sql) {
    sqlite3_stmt* stmt = nullptr;

    if (!db_) {
        throw std::logic_error("Datenbank nicht geöffnet.");
    }

    const int rc = sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        const std::string fehler = sqlite3_errmsg(db_);
        throw std::runtime_error("Prepare fehlgeschlagen: " + fehler);
    }

    return Statement(stmt);
}

void Database::execute(const char* sql) {
    if (db_ == nullptr) {
        throw std::logic_error("Datenbank nicht geöffnet.");
    }

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db_, sql, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        std::string fehler;

        if (errMsg != nullptr) {
            fehler = errMsg;
            sqlite3_free(errMsg);
        } else {
            fehler = sqlite3_errmsg(db_);
        }

        throw std::runtime_error("Execute fehlgeschlagen: " + fehler);
    }
}

int Database::changes() const {
    if (db_ == nullptr) {
        throw std::logic_error("Database is null in changes()");
    }

    return sqlite3_changes(db_);
}

} // namespace infrastructure::db
