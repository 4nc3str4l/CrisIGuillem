\echo ----- vista 1 (ativitat a la fira)

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
            r.comercial = CURRENT_USER
    ) as reunions_tancades
FROM
    comercial c,
    persona p
WHERE
    p.nif = c.persona;
