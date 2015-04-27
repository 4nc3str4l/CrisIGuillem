\echo ---------- taula persona

CREATE TABLE persona (
	passaport TEXT PRIMARY KEY,
	nom TEXT NOT NULL,
	cognom TEXT NOT NULL,
	ciutat TEXT REFERENCES ciutat
		ON DELETE SET NULL
		ON UPDATE CASCADE
);