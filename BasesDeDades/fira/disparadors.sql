\echo ----- Trigger comptador de visitants
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



\echo ----- Trigger comptador d''expositors
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



\echo ----- Trigger comprobacio comercials empresa
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


\echo ----- Trigger configuracio comercial
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



\echo ----- Trigger comprobacio comercial aten
CREATE OR REPLACE FUNCTION comproba_aten() RETURNS TRIGGER AS $$
DECLARE
    empresa_comercial TEXT;
BEGIN
    -- Evitar que un comercial atengui un estand d'una altre empresa
    PERFORM * FROM pertany p, comercial c WHERE
        p.empresa = c.empresa AND
        c.persona = NEW.comercial AND
        p.estand = NEW.estand;

    IF (NOT FOUND) THEN
        RAISE EXCEPTION 'Un comercial solament pot atendre estands de la seva empresa';
    END IF;

    -- Evitar que un comercial atengui dos estands
    PERFORM * FROM aten a WHERE
        a.esdeveniment = NEW.esdeveniment AND
        a.edicio = NEW.edicio AND
        a.comercial = NEW.comercial;

    IF (FOUND) THEN
        RAISE EXCEPTION 'Un comercial no pot atendre dos estands al mateix temps';
    END IF;

    -- Si l'estand no esta marcat com que pertany a l'empresa, fer-ho
    SELECT empresa INTO empresa_comercial FROM comercial WHERE
        persona = NEW.comercial;

    PERFORM * FROM pertany WHERE
        estand = NEW.estand AND
        empresa = empresa_comercial AND
        esdeveniment = NEW.esdeveniment AND
        edicio = NEW.edicio;

    IF (NOT FOUND) THEN
        INSERT INTO pertany VALUES
            (NEW.esdeveniment, NEW.edicio, NEW.estand, empresa_comercial);
    END IF;

    RETURN NEW;
END
$$
language 'plpgsql';

CREATE TRIGGER trigger_comproba_aten BEFORE INSERT ON aten
FOR EACH ROW EXECUTE PROCEDURE comproba_aten();
