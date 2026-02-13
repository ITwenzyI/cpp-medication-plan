/*
 * File: patient_repository
 * Author: Kilian | ItwenzyI
 * Created: 07.02.2026
 * Description: 
 */
//

#include "patient_repository.hpp"
#include <stdexcept>

PatientRepository::PatientRepository(Database& db) : db_(db) {
}

Patient PatientRepository::createPatient(const Patient& p) {
    auto stmt =
        db_.prepare("INSERT INTO patients (name, birth_date, nationality) VALUES (?, ?, ?);");

    stmt.bindText(1, p.name);

    if (p.birth_date.empty()) {
        stmt.bindText(2, p.birth_date);
    } else {
        stmt.bindNull(2);
    }

    if (p.nationality.empty()) {
        stmt.bindText(3, p.nationality);
    } else {
        stmt.bindNull(3);
    }

    int rc = stmt.step();
    if (rc != SQLITE_DONE) {
        throw std::runtime_error("INSERT fehlgeschlagen.");
    }

    int lastInsertRowID = static_cast<int>(sqlite3_last_insert_rowid(db_.get()));

    Patient newPatient = p;
    newPatient.id = lastInsertRowID;

    return newPatient;
}

std::vector<Patient> PatientRepository::getAllPatients() {
}