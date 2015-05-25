\echo ---------- taula aten

CREATE TABLE aten (
	esdeveniment TEXT NOT NULL REFERENCES esdeveniment
	ON DELETE SET NULL
  	ON UPDATE CASCADE,
	edicio DATE NOT NULL REFERENCES edicio
	ON DELETE SET NULL
  	ON UPDATE CASCADE,
	estand TEXT NOT NULL REFERENCES	estand
	ON DELETE SET NULL
  	ON UPDATE CASCADE,
	comercial TEXT NOT NULL REFERENCES comercial
	ON DELETE SET NULL
  	ON UPDATE CASCADE,
	UNIQUE(esdeveniment,edicio,comercial,estand)
);