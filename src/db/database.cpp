#include "database.hpp"
#include <stdexcept>


Database::Database(const std::string& dbPfad) : db(nullptr) {
    if (sqlite3_open(dbPfad.c_str(), &db) != SQLITE_OK) {
        std::string fehler = sqlite3_errmsg(db);
        sqlite3_close(db);
        db = nullptr;
        throw std::runtime_error("Datenbank konnte nicht geöffnet werden: " + fehler);
    }
}


Database::~Database() {
        if (db) {
            sqlite3_close(db);
            db = nullptr;
        }
}


[[nodiscard]] sqlite3 *Database::get() const{
        return db;
}

Database::Database(Database &&other) noexcept : db(other.db) {
        other.db = nullptr;
}

Database &Database::operator=(Database &&other) noexcept {
        if (this != &other) {
            if (db) {
                sqlite3_close(db);
            }
            db = other.db;
            other.db = nullptr;
        }
        return *this;
}


Statement Database::prepare(const char *sql) {
    sqlite3_stmt* stmt = nullptr;

    if (!db) {
        throw std::logic_error("Datenbank nicht geöffnet.");
    }


    const int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        const std::string fehler = sqlite3_errmsg(db);
        throw std::runtime_error("Prepare fehlgeschlagen: " + fehler);
    }

    return Statement(stmt);
}

void Database::execute(const char *sql) {
    if (db == nullptr) {
        throw std::logic_error("Datenbank nicht geöffnet.");
    }

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        std::string fehler;

        if (errMsg != nullptr) {
            fehler = errMsg;
            sqlite3_free(errMsg);
        } else {
            fehler = sqlite3_errmsg(db);
        }

        throw std::runtime_error("Execute fehlgeschlagen: " + fehler);
    }
}
