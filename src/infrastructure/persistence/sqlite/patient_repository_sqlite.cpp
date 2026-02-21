#include "patient_repository_sqlite.hpp"
#include <stdexcept>

namespace infrastructure::persistence::sqlite {
static domain::Patient mapPatient(const db::Statement& stmt) {
    domain::Patient temp;
    temp.id = stmt.getInt(0);
    temp.name = stmt.getText(1);
    temp.birth_date = stmt.getText(2);
    temp.nationality = stmt.getText(3);

    return temp;
}

PatientRepositorySqlite::PatientRepositorySqlite(db::Database& db) : db_(db) {
}

domain::Patient PatientRepositorySqlite::createPatient(const domain::Patient& p) {
    auto stmt =
        db_.prepare("INSERT INTO patients (name, birth_date, nationality) VALUES (?, ?, ?);");

    stmt.bindText(1, p.name);

    if (!p.birth_date.empty()) {
        stmt.bindText(2, p.birth_date);
    } else {
        stmt.bindNull(2);
    }

    if (!p.nationality.empty()) {
        stmt.bindText(3, p.nationality);
    } else {
        stmt.bindNull(3);
    }

    int rc = stmt.step();
    if (rc != SQLITE_DONE) {
        throw std::runtime_error("INSERT fehlgeschlagen.");
    }

    int lastInsertRowID = static_cast<int>(sqlite3_last_insert_rowid(db_.get()));

    domain::Patient newPatient = p;
    newPatient.id = lastInsertRowID;

    return newPatient;
}

std::vector<domain::Patient> PatientRepositorySqlite::getAllPatients() {
    auto stmt =
        db_.prepare("SELECT id, name, birth_date, nationality FROM patients ORDER BY id ASC;");

    std::vector<domain::Patient> result;

    while (true) {
        int rc = stmt.step();

        if (rc == SQLITE_ROW) {
            result.push_back(mapPatient(stmt));
            continue;
        } else if (rc == SQLITE_DONE) {
            break;
        } else {
            throw std::runtime_error("STEP fehlgeschlagen.");
        }
    }

    return result;
}
} // namespace infrastructure::persistence::sqlite
