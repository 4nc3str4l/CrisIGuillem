\echo ---------- taula comercial

CREATE TABLE comercial (
	persona TEXT PRIMARY KEY REFERENCES persona
	ON DELETE SET NULL
  	ON UPDATE CASCADE,
  	visitants_atesos INTEGER DEFAULT 0,
  	empresa TEXT REFERENCES empresa
  	ON DELETE SET NULL
  	ON UPDATE CASCADE
);
