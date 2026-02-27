# 💊 Medication Plan (C++ · SQLite)

A modern C++20 command line application for managing patients, medications, and intake plans.

This project focuses on clean architecture, strong domain modeling, and explicit error handling.

---

## 🧠 Architecture

The project follows a layered architecture:

- `domain` → Entities and Value Objects  
- `application::ports` → Repository interfaces (abstractions)  
- `infrastructure::db` → SQLite database and statement RAII wrappers  
- `infrastructure::persistence::sqlite` → Repository implementations and persistence mapping
- `common` → Result type, validation, error codes  
- `ui::cli` → Command line interface  
- `tests` → Smoke tests

Namespaces reflect the architectural boundaries.

---

## 🧱 Current Implementation Status

### ✅ Database Layer

- RAII-based `Database` and `Statement` wrappers
- Prepared statements only (no raw SQL execution)
- SQLite C API integration
- `sqlite3_changes()` support

### ✅ Error Handling

- Custom `common::result::Result<T>`
- Central `ErrorCode` enum
- No exceptions for domain/application errors
- Explicit failure propagation

### ✅ Domain Modeling

- `Patient` entity
- `Medication` entity
- `Intake Plan` entity
- Birth date validation (ISO format YYYY-MM-DD)
- Validation utilities in `common::validation`

### ✅ Persistence

- `PatientRepositorySqlite`
  - createPatient
  - getAllPatients
  - findPatientById
  - deletePatientById
  - updatePatientName
  - updatePatientBirthdate
  - updatePatientNationality
- `MedicationRepositorySqlite`
  - createMedication
  - getAllMedications
  - findMedicationById
  - deleteMedicationById
  - updateMedicationName
  - updateMedicationStrength
  - updateMedicationWarnings
- `IntakePlanRepositorySqlite`
  - createIntakePlan
  - getIntakePlansByPatientId
  - getIntakePlansByMedicationId
  - deleteIntakePlanById
  - updateIntakePlan
- Smoke tests for Patient and Medication using in-memory SQLite

---

## 🚧 Work In Progress

- IntakePlan functions
- IntakePlan Smoke test
- Refactoring to consistently use `Result<T>` everywhere
- Include BirthDate and Nationality class in Patient

---

## 🔜 Planned Next Steps

- Improve CLI structure
- Add more structured tests
- Introduce transaction support
- Add FTXUI-based TUI

---

## 🎯 Goals of This Project

- Demonstrate modern C++ design principles
- Show clean separation of layers
- Use SQLite professionally via prepared statements
- Practice explicit error handling instead of hidden control flow
- Build a realistic persistence-heavy CLI system

---

## ⚙️ Tech Stack

- C++20
- SQLite (C API)
- CMake
- RAII wrappers
- Clean Architecture principles
- Conventional Commits workflow

---

## 📌 Status

Actively developed.
