\echo ---------- taula fa

CREATE TABLE fa (
	soci TEXT REFERENCES soci(persona),
	esport TEXT REFERENCES esport(nom),
	UNIQUE(soci,esport)
);