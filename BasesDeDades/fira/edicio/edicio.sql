\echo ---------- taula edicio

CREATE TABLE edicio (
  inici DATE PRIMARY KEY,
  fi DATE,
  expositors INTEGER DEFAULT 0,
  visitants INTEGER DEFAULT 0,
  esdeveniment TEXT REFERENCES esdeveniment
  ON DELETE SET NULL
  ON UPDATE CASCADE,
  UNIQUE(esdeveniment, inici)
);
