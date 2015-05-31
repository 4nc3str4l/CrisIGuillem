\echo ----- Funcio username a nif
CREATE OR REPLACE FUNCTION username_nif(username TEXT) RETURNS TEXT AS
$$
BEGIN
    RETURN REPLACE(SUBSTRING(username, 2), '-', '');
END
$$
language 'plpgsql';

\echo ----- Vista pagaments soci
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


\echo ----- Vita llista edicions
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


\echo ----- Funcio edicio actual
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


\echo ----- Vista llista expositors
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


\echo ----- Vista llista comercials autoritzats
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


\echo ----- Funcio llista entrades
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


\echo ----- Funcio crear comercial
CREATE OR REPLACE FUNCTION crear_comercial(comercial_nif TEXT, nom TEXT, telefon TEXT, comercial_empresa TEXT)
RETURNS VOID AS
$$
BEGIN
    -- Cal comprobar que l'empresa existeixi primer
    PERFORM * FROM empresa WHERE nif = comercial_empresa;
    IF (NOT FOUND) THEN
        RAISE EXCEPTION 'Empresa no trobada';
    END IF;

    -- Insertem
    INSERT INTO persona VALUES (comercial_nif, nom, telefon);
    INSERT INTO comercial VALUES (comercial_nif, 0, comercial_empresa);
END
$$
language 'plpgsql';

GRANT EXECUTE ON FUNCTION crear_comercial(comercial_nif TEXT, nom TEXT, telefon TEXT, comercial_empresa TEXT)
    TO GROUP treballadors;
GRANT SELECT,INSERT ON persona TO GROUP treballadors;
GRANT SELECT,INSERT ON comercial TO GROUP treballadors;



\echo ----- Funcio crear visitant
CREATE OR REPLACE FUNCTION crear_visitant(nif TEXT, nom TEXT, telefon TEXT, esd TEXT, edi TEXT)
RETURNS VOID AS
$$
BEGIN
    -- Cal comprobar que existeixi l'esdeveniment i la edicio
    PERFORM * FROM edicio WHERE esdeveniment = esd AND inici = CAST(edi AS DATE);

    IF (NOT FOUND) THEN
        RAISE EXCEPTION 'Esdeveniment i edicio no trobats';
    END IF;

    INSERT INTO persona VALUES (nif, nom, telefon);
    INSERT INTO entrada VALUES (esd, CAST(edi AS DATE), nif, CAST(NOW() AS DATE));
END
$$
language 'plpgsql';

GRANT EXECUTE ON FUNCTION crear_visitant(nif TEXT, nom TEXT, telefon TEXT, esd TEXT, edi TEXT)
    TO GROUP treballadors;
GRANT SELECT,INSERT ON persona TO GROUP treballadors;
GRANT SELECT,INSERT ON entrada TO GROUP treballadors;



\echo ----- Funcio crear empresa
CREATE OR REPLACE FUNCTION crear_empresa(nif TEXT, nom TEXT, activitat domini_activitat)
RETURNS VOID AS
$$
BEGIN
    INSERT INTO empresa VALUES (nif, nom, activitat);
END
$$
language 'plpgsql';

GRANT EXECUTE ON FUNCTION crear_empresa(nif TEXT, nom TEXT, activitat domini_activitat)
    TO GROUP treballadors;
GRANT SELECT,INSERT ON empresa TO GROUP treballadors;



\echo ----- Funcio crear edicio
CREATE OR REPLACE FUNCTION crear_edicio(esd TEXT)
RETURNS VOID AS
$$
DECLARE
    periode domini_periodicitat;
    increment INTERVAL;
    inici_ultima DATE;
    fi_ultima DATE;
BEGIN
    SELECT periodicitat INTO periode FROM esdeveniment WHERE
        nom = esd;

    IF (periode = CAST('anual' as domini_periodicitat)) THEN
        SELECT CAST('1 year' AS INTERVAL) INTO increment;
    ELSE
        SELECT CAST('2 years' AS INTERVAL) INTO increment;
    END IF;

    SELECT inici, fi INTO inici_ultima, fi_ultima FROM edicio WHERE
        esdeveniment = esd ORDER BY inici DESC LIMIT 1;

    INSERT INTO edicio VALUES
        (inici_ultima + increment, fi_ultima + increment, 0, 0, esd);
END
$$
language 'plpgsql';

GRANT EXECUTE ON FUNCTION crear_edicio(esd TEXT)
    TO GROUP treballadors;
GRANT SELECT,INSERT ON edicio TO GROUP treballadors;


\echo ----- Funcio donar alta estand
CREATE OR REPLACE FUNCTION donar_alta_estand(esd TEXT, edi TEXT, estand TEXT, empresa TEXT)
RETURNS VOID AS
$$
BEGIN
    -- Pertany aquest estand a aquesta edicio? Es a dir, esta en la mateixa ubicacio?
    PERFORM * FROM localitzada l WHERE
        l.ubicacio = (SELECT ubicacio FROM estand e WHERE e.nom = estand) AND
        l.esdeveniment = esd AND
        l.edicio = CAST(edi AS DATE);

    IF (NOT FOUND) THEN
        RAISE EXCEPTION 'Aquest estand no pertany a cap ubicacio d''aquesta edicio';
    END IF;

    INSERT INTO pertany VALUES
        (esd, CAST(edi AS DATE), estand, empresa);
END
$$
language 'plpgsql';

GRANT EXECUTE ON FUNCTION donar_alta_estand(esd TEXT, edi TEXT, estand TEXT, empresa TEXT)
    TO GROUP treballadors;
GRANT SELECT,INSERT ON empresa TO GROUP treballadors;


\echo ----- Funcio registrar visita
CREATE OR REPLACE FUNCTION registrar_visita(per TEXT) RETURNS VOID AS
$$
BEGIN
    -- Comprobar si existeix la persona
    PERFORM * FROM persona WHERE nif = per;

    IF (NOT FOUND) THEN
        RAISE EXCEPTION 'Aquesta persona no es part de l''esdeveniment';
    END IF;

    UPDATE comercial SET visitants_atesos = visitants_atesos + 1 WHERE
        persona = username_nif(CURRENT_USER);
END
$$
language 'plpgsql';

GRANT EXECUTE ON FUNCTION registrar_visita(per TEXT) TO GROUP comercials;
GRANT SELECT, UPDATE ON comercial TO GROUP comercials;



\echo ----- Funcio tancar reunio
CREATE OR REPLACE FUNCTION tancar_reunio(per TEXT) RETURNS VOID AS
$$
BEGIN
    INSERT INTO reunio VALUES
        (username_nif(CURRENT_USER), per, CAST(NOW() AS DATE));
END
$$
language 'plpgsql';

GRANT EXECUTE ON FUNCTION tancar_reunio(per TEXT) TO GROUP comercials;
GRANT SELECT, UPDATE ON reunio TO GROUP comercials;
