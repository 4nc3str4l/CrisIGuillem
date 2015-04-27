\echo ---------- taula treballador

CREATE DOMAIN domini_departament TEXT
	CHECK (VALUE IN ('administraciÃ³','comercial','entrenador'));

CREATE TABLE treballador (
	persona TEXT PRIMARY KEY REFERENCES persona(passaport),
	departament domini_departament,
	obeeix TEXT REFERENCES treballador(persona)
);