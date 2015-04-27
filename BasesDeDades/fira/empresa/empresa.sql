\echo ---------- taula empresa

CREATE DOMAIN domini_activitat INTEGER
CHECK (VALUE >= 0 AND VALUE <  100);

CREATE TABLE empresa (
  nif TEXT PRIMARY KEY,
  nom TEXT,
  activitat domini_activitat
);
