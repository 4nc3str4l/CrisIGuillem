\echo ---------- taula comercial

CREATE TABLE comercial (
  nif TEXT PRIMARY KEY,
  nom TEXT,
  telefon TEXT,
  empresa TEXT REFERENCES empresa
  ON DELETE SET NULL
  ON UPDATE CASCADE
);
