\echo ---------- taula mails

CREATE DOMAIN domini_mail TEXT
	CHECK (VALUE LIKE '%_@%_.%__');

CREATE TABLE mails (
	persona TEXT NOT NULL REFERENCES persona(passaport),
	mail domini_mail NOT NULL,
	CONSTRAINT mail_ja_assignat UNIQUE(persona,mail)
);