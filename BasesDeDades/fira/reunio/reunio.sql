\echo ---------- taula reunio

CREATE TABLE reunio (
	comercial TEXT NOT NULL REFERENCES comercial
	ON DELETE SET NULL
  	ON UPDATE CASCADE,
	persona TEXT NOT NULL REFERENCES persona
	ON DELETE SET NULL
  	ON UPDATE CASCADE,
	data DATE,
	UNIQUE(comercial , data)
);