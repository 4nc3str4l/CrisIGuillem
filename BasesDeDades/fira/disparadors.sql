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
