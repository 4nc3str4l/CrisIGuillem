\echo ---------- taula estand
CREATE TABLE estand (
  nom TEXT PRIMARY KEY,
  te_aigua BOOLEAN,
  potencia_llum INTEGER,
  amplada INTEGER,
  llargada INTEGER,
  ubicacio TEXT REFERENCES ubicacio
  ON UPDATE CASCADE
  ON DELETE SET NULL
);
