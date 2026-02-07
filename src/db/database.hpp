#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <sqlite3.h>
#include <string>

#include "statement.hpp"

class Database {
public:
    // Öffnet bestehende Datenbank oder erstellt neue Datei
    explicit Database(const std::string& dbPfad);

    // Schließt die Datenbank automatisch
    ~Database();

    // Zugriff auf rohe SQLite Verbindung
    [[nodiscard]] sqlite3* get() const;

    // Kopieren verbieten
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    // Verschieben erlauben
    Database(Database&& other) noexcept;
    Database& operator=(Database&& other) noexcept;

    Statement prepare(const char *sql);

    void execute(const char* sql);

private:
    sqlite3* db_;
};


#endif
