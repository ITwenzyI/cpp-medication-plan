#include "database.hpp"


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
