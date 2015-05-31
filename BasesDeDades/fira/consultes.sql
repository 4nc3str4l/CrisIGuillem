\echo ----- Seleccionar edicions
SELECT
    es.nom,
    EXTRACT(YEAR FROM ed.inici) as edicio,
    ed.visitants,
    (
        SELECT
            SUM(en.preu)
        FROM
            entrada en
        WHERE
            en.esdeveniment = ed.esdeveniment AND
            en.edicio = ed.inici
    ) as preu,
    ed.expositors
FROM
    esdeveniment es,
    edicio ed
WHERE
    ed.esdeveniment = es.nom
ORDER BY
    es.nom,
    ed.inici;


\echo ----- Seleccionar la edició amb més preu
SELECT
    es.nom,
    EXTRACT(YEAR FROM ed.inici) as edicio,
    ed.visitants,
    (
        SELECT
            SUM(en.preu)
        FROM
            entrada en
        WHERE
            en.esdeveniment = ed.esdeveniment AND
            en.edicio = ed.inici
    ) as preu,
    ed.expositors
FROM
    esdeveniment es,
    edicio ed
WHERE
    ed.esdeveniment = es.nom
ORDER BY
    preu DESC
LIMIT
    1;


\echo ----- Seleccionar edicio amb visitants maxim de cada esdeveniment
SELECT tmp.nom, EXTRACT(YEAR FROM ed.inici) as edicio, tmp.visitants as visitants
FROM
    (
        SELECT
           es.nom,
           MAX(ed.visitants) as visitants
        FROM
           esdeveniment es,
           edicio ed
        WHERE
           ed.esdeveniment = es.nom
        GROUP BY
            es.nom
    ) as tmp,
    edicio ed
WHERE
    ed.esdeveniment = tmp.nom AND
    ed.visitants = tmp.visitants;


\echo ----- Visitants per esdeveniment
SELECT
   es.nom,
   SUM(ed.visitants) as visitants
FROM
   esdeveniment es,
   edicio ed
WHERE
   ed.esdeveniment = es.nom
GROUP BY
    es.nom;


\echo ----- Runions i persones ateses per comercial en total
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
            r.comercial = p.nif
    ) as reunions_tancades
FROM
    comercial c,
    persona p
WHERE
    p.nif = c.persona;
