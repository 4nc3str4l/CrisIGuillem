DROP GROUP IF EXISTS comercials;
CREATE GROUP comercials;

CREATE OR REPLACE FUNCTION comptador_visitants() RETURNS TRIGGER AS $$
BEGIN
    UPDATE edicio SET visitants = visitants + 1 WHERE
        inici = NEW.edicio AND
        esdeveniment = NEW.esdeveniment;

    RETURN NEW;
END
$$
LANGUAGE 'plpgsql';

CREATE TRIGGER trigger_comptador_visitants AFTER INSERT ON entrada
FOR EACH ROW EXECUTE PROCEDURE comptador_visitants();




CREATE OR REPLACE FUNCTION comptador_expositors() RETURNS TRIGGER AS $$
BEGIN
    UPDATE edicio SET expositors = expositors + 1 WHERE
        inici = NEW.edicio AND
        esdeveniment = NEW.esdeveniment;

    RETURN NEW;
END
$$
LANGUAGE 'plpgsql';

CREATE TRIGGER trigger_comptador_expositors AFTER INSERT ON pertany
FOR EACH ROW EXECUTE PROCEDURE comptador_expositors();


CREATE OR REPLACE FUNCTION comproba_empresa() RETURNS TRIGGER AS $$
DECLARE
    total INTEGER;
BEGIN
    SELECT COUNT(*) INTO total FROM comercial WHERE
        empresa = NEW.empresa;

    IF (total >= 5) THEN
        RAISE EXCEPTION 'Una empresa no pot tenir mes de 5 comercials';
    END IF;

    RETURN NEW;
END
$$
language 'plpgsql';

CREATE TRIGGER trigger_comproba_empresa BEFORE INSERT ON comercial
FOR EACH ROW EXECUTE PROCEDURE comproba_empresa();



CREATE OR REPLACE FUNCTION comercial_config() RETURNS TRIGGER AS $$
BEGIN
    INSERT INTO _config VALUES (
        NEW.persona,
        (SELECT nom FROM esdeveniment LIMIT 1),
        (SELECT inici FROM edicio WHERE esdeveniment =
            (SELECT nom FROM esdeveniment LIMIT 1)
            LIMIT 1
        )
    );

    EXECUTE 'DROP USER IF EXISTS u' || NEW.persona;
    EXECUTE 'CREATE USER u' || NEW.persona || ' WITH PASSWORD ''db1''';
    EXECUTE 'ALTER GROUP comercials ADD USER u' || NEW.persona;

    RETURN NEW;
END
$$
LANGUAGE 'plpgsql';

CREATE TRIGGER trigger_comercial_config AFTER INSERT ON comercial
FOR EACH ROW EXECUTE PROCEDURE comercial_config();
