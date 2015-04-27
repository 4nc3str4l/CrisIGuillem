\echo ---------- taula nomines

CREATE TABLE nomines (
	treballador TEXT REFERENCES treballador,
	periode TEXT,
	sou_base DECIMAL(6,2),
	retencio DECIMAL (4,2),
	PRIMARY KEY(treballador,periode)
);