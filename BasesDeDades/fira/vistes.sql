\echo ----- vista 1 (ativitat a la fira)

CREATE OR REPLACE FUNCTION username_nif(username TEXT) RETURNS TEXT AS
$$
BEGIN
    RETURN REPLACE(SUBSTRING(username, 2), '-', '');
END
$$
language 'plpgsql';

CREATE VIEW pagaments_soci AS
SELECT
    p.nom,
    p.nif,
    c.visitants_atesos,
    (
        SELECT
            COUNT(*)
        FROM
            reunio r
        WHERE
            r.comercial = username_nif(CURRENT_USER)
    ) as reunions_tancades
FROM
    comercial c,
    persona p
WHERE
    p.nif = c.persona AND
    c.persona = username_nif(CURRENT_USER);

GRANT SELECT ON pagaments_soci TO GROUP comercials;

\echo ----- vista 2 (llista expositors)
CREATE VIEW llista_edicions AS
    SELECT
        es.nom,
        ed.inici
    FROM
        esdeveniment es,
        edicio ed
    WHERE
        ed.esdeveniment = es.nom
    ORDER BY
        es.nom,
        ed.inici;

GRANT SELECT ON llista_edicions TO GROUP comercials;

CREATE OR REPLACE FUNCTION edicio_actual(esd TEXT, edi DATE) RETURNS VOID AS
$$
BEGIN
    PERFORM * FROM edicio e WHERE e.esdeveniment = esd AND
        e.inici = edi;

    IF (NOT FOUND) THEN
        RAISE EXCEPTION 'Esdeveniment i edicio no trobada';
    END IF;

    UPDATE _config SET esdeveniment = esd, edicio = edi
    WHERE nif = username_nif(CURRENT_USER);
END
$$
language 'plpgsql';

GRANT EXECUTE ON FUNCTION edicio_actual(esdeveniment TEXT, edicio DATE) TO GROUP comercials;
GRANT SELECT ON edicio TO GROUP comercials;
GRANT SELECT, UPDATE ON _config TO GROUP comercials;

\echo ----- vista 2 (llista expositors)
CREATE VIEW llista_expositors AS
SELECT
    d.nom as estand,
    e.nom as empresa
FROM
    empresa e,
    estand d,
    pertany p
WHERE
    e.nif = p.empresa AND
    d.nom = p.estand AND
    p.esdeveniment = (SELECT esdeveniment FROM _config WHERE nif = username_nif(CURRENT_USER)) AND
    p.edicio = (SELECT edicio FROM _config WHERE nif = username_nif(CURRENT_USER));


GRANT SELECT ON llista_expositors TO GROUP comercials;


CREATE VIEW llista_comercials_autoritzats AS
SELECT
    c.nom as comercial,
    d.nom as estand,
    e.nom as empresa
FROM
    persona c,
    comercial k,
    empresa e,
    estand d,
    pertany p,
    aten a
WHERE
    c.nif = a.comercial AND
    c.nif = k.persona AND
    k.empresa = p.empresa AND
    e.nif = p.empresa AND
    d.nom = p.estand AND
    p.esdeveniment = (SELECT esdeveniment FROM _config WHERE nif = username_nif(CURRENT_USER)) AND
    p.edicio = (SELECT edicio FROM _config WHERE nif = username_nif(CURRENT_USER)) AND
    a.esdeveniment = (SELECT esdeveniment FROM _config WHERE nif = username_nif(CURRENT_USER)) AND
    a.edicio = (SELECT edicio FROM _config WHERE nif = username_nif(CURRENT_USER));

GRANT SELECT ON llista_comercials_autoritzats TO GROUP comercials;


CREATE OR REPLACE FUNCTION llista_entrades(inici TEXT, fi TEXT) RETURNS TABLE(esdeveniment TEXT, edicio DATE, venuda DATE, preu REAL) AS
$$
    SELECT
        esdeveniment, edicio, venuda, preu
    FROM
        entrada
    WHERE
        venuda >= CAST(inici AS DATE) AND
        venuda <= CAST(fi AS DATE)
    ORDER BY
        venuda ASC;
$$
language 'sql';

GRANT EXECUTE ON FUNCTION llista_entrades(inici TEXT, fi TEXT) TO GROUP administradors;



CREATE OR REPLACE FUNCTION crear_comercial(nif TEXT, nom TEXT, telefon TEXT, empresa TEXT)
RETURNS VOID AS
$$
BEGIN
    INSERT INTO persona VALUES (nif, nom, telefon);
    INSERT INTO comercial VALUES (nif, 0, empresa);
END
$$
language 'plpgsql';

GRANT EXECUTE ON FUNCTION crear_comercial(nif TEXT, nom TEXT, telefon TEXT, empresa TEXT)
    TO GROUP empreses;
GRANT SELECT,INSERT ON persona TO GROUP empreses;
GRANT SELECT,INSERT ON comercial TO GROUP empreses;
