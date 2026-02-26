PRAGMA foreign_keys = ON;

CREATE TABLE
  IF NOT EXISTS patients (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    birth_date TEXT,
    nationality TEXT
  );

CREATE TABLE
  IF NOT EXISTS medications (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    strength TEXT NOT NULL,
    warnings TEXT
  );

CREATE TABLE
  IF NOT EXISTS intake_plans (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    patient_id INTEGER NOT NULL,
    medication_id INTEGER NOT NULL,
    dose TEXT NOT NULL,
    time_of_day TEXT NOT NULL,
    notes TEXT,
    FOREIGN KEY (patient_id) REFERENCES patients (id) ON DELETE CASCADE,
    FOREIGN KEY (medication_id) REFERENCES medications (id) ON DELETE CASCADE,
    UNIQUE (patient_id, medication_id, time_of_day)
  );

CREATE INDEX IF NOT EXISTS idx_intake_plans_patient_id ON intake_plans (patient_id);

CREATE INDEX IF NOT EXISTS idx_intake_plans_medication_id ON intake_plans (medication_id);