\echo ---------- taula localitzada

CREATE TABLE localitzada (
	esdeveniment TEXT NOT NULL REFERENCES esdeveniment,
	edicio DATE NOT NULL REFERENCES edicio,
	ubicacio TEXT NOT NULL REFERENCES ubicacio,
	UNIQUE(esdeveniment,edicio,ubicacio)
);
