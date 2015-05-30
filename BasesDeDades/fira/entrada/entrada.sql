\echo ---------- taula entreda

CREATE TABLE entrada (
	esdeveniment TEXT NOT NULL REFERENCES esdeveniment
	ON DELETE SET NULL
  	ON UPDATE CASCADE,
	edicio DATE NOT NULL REFERENCES edicio
	ON DELETE SET NULL
  	ON UPDATE CASCADE,
	persona TEXT NOT NULL REFERENCES persona
	ON DELETE SET NULL
  	ON UPDATE CASCADE,

	venuda DATE,
	preu REAL DEFAULT 1.5,

	UNIQUE(esdeveniment, edicio, persona)
);
