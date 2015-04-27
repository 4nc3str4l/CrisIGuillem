\echo ---------- taula esdeveniment

CREATE DOMAIN domini_periodicitat TEXT
CHECK (VALUE in ("anual, bianual"));

CREATE TABLE esdeveniment (
  nom TEXT PRIMARY KEY,
  periodicitat domini_periodicitat,
  empresa TEXT REFERENCES empresa
  ON DELETE SET NULL
  ON UPDATE CASCADE
);
