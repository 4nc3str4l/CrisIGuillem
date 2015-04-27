\echo ---------- taula coneix

CREATE TABLE coneix (
	coneix TEXT NOT NULL REFERENCES persona,
	es_coneguda TEXT NOT NULL REFERENCES persona,
	UNIQUE(coneix,es_coneguda)
);