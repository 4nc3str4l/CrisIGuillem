\echo ---------- taula localitzada

CREATE TABLE pertany (
	esdeveniment TEXT NOT NULL REFERENCES esdeveniment
	ON DELETE SET NULL
  	ON UPDATE CASCADE,
	edicio DATE NOT NULL REFERENCES edicio
	ON DELETE SET NULL
  	ON UPDATE CASCADE,
	estand TEXT NOT NULL REFERENCES	estand
	ON DELETE SET NULL
  	ON UPDATE CASCADE,
	empresa TEXT NOT NULL REFERENCES empresa
	ON DELETE SET NULL
  	ON UPDATE CASCADE,
	UNIQUE(esdeveniment,edicio,estand)
);