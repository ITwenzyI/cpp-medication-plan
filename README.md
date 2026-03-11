# 💊 Medication Plan (C++ · SQLite)

A modern C++20 command line application for managing patients, medications, and intake plans.

This project focuses on clean layering, explicit error handling, and a practical SQLite-backed persistence flow.

---

## 🧠 Architecture

The project follows a layered structure:

- `domain` -> entities and enums
- `application::ports` -> repository interfaces
- `infrastructure::db` -> SQLite database and statement RAII wrappers
- `infrastructure::persistence::sqlite` -> repository implementations and mapping helpers
- `common` -> result type, validation, error codes, string helpers
- `ui::cli` -> command line menus, input handling, printers, error rendering
- `tests` -> smoke tests against in-memory SQLite

Namespaces reflect the architectural boundaries.

---

## 🧱 Current Features

### Patient Management

- Create patients
- List all patients
- Find a patient by ID
- Delete a patient by ID
- Update patient name
- Update patient birth date
- Update patient nationality
- Support optional birth date and nationality fields

### Medication Management

- Create medications
- List all medications
- Find a medication by ID
- Delete a medication by ID
- Update medication name
- Update medication strength
- Update medication warnings

### Intake Plan Management

- Create intake plans
- List intake plans by patient ID
- List intake plans by medication ID
- Delete an intake plan by ID
- Update intake plans in the repository layer

### Persistence and Error Handling

- SQLite integration via prepared statements only
- RAII wrappers for `sqlite3` and `sqlite3_stmt`
- Foreign key enforcement enabled per connection
- Unique constraint for `(patient_id, medication_id, time_of_day)`
- Explicit `Result<T>`-based error propagation
- Mapped error categories such as `NotFound`, `InvalidArgument`, `Conflict`, and `ForeignKeyViolation`

---

## Domain Model

- `Patient`
  - `id`
  - `name`
  - `birth_date`
  - `nationality`
- `Medication`
  - `id`
  - `name`
  - `strength`
  - `warnings`
- `IntakePlan`
  - `id`
  - `patient_id`
  - `medication_id`
  - `dose`
  - `time_of_day`
  - `notes`

Enums currently used in the model:

- `Nationality`
- `TimeOfDay`

---

## ✅ Validation

The project already validates several important input and persistence rules:

- positive IDs
- non-empty names and strengths
- ISO birth date format `YYYY-MM-DD`
- calendar-aware birth date checks, including leap years
- supported nationality codes
- supported intake plan times of day

---

## Build

### Requirements

- CMake 3.20 or newer
- a C++20 compiler

### Configure and Build

```bash
cmake -S . -B build
cmake --build build
```

---

## Run

Start the CLI application:

```bash
./build/medication_plan.exe
```


---

## Tests

The project currently includes smoke tests for the SQLite repositories:

```bash
./build/smoke_patients.exe
./build/smoke_medications.exe
./build/smoke_intake_plans.exe
```

These tests cover:

- create, read, update, delete flows
- optional field handling
- unique constraint behavior
- foreign key violations
- cascade delete behavior
- intake plan update behavior

---

## Project Layout

```text
src/
  application/
  common/
  domain/
  infrastructure/
  ui/
tests/
sql/
external/sqlite/
```

---

## ⚙️ Tech Stack

- C++20
- SQLite C API
- CMake
- RAII wrappers
- layered architecture
- explicit result-based error handling

---

## 📌 Status

The project is now in a solid first-release state:

- core CRUD flows are implemented
- smoke tests exist for all three repository areas
- several pre-release validation and CLI stability issues have been fixed

---

## 🔜 Possible Next Steps

- improve CLI ergonomics and navigation flow
- add dedicated unit tests for validation and mappers
- introduce transaction support for larger workflows
- add richer update flows for intake plans directly in the CLI
