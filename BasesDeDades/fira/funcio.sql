CREATE OR REPLACE FUNCTION resum_activitat(in_inici TEXT, in_fi TEXT) RETURNS TABLE(
    data_inici DATE,
    data_fi DATE,
    total_esdeveniments INTEGER,
    total_edicions INTEGER,
    total_visitants REAL,
    total_recaptació REAL,
    mitjana_visitants REAL,
    mitjana_recaptació REAL
) AS
$$
DECLARE
    d_inici DATE;
    d_fi DATE;
BEGIN

    SELECT CAST(in_inici AS DATE) INTO d_inici;
    SELECT CAST(in_fi AS DATE) INTO d_fi;

    PERFORM * FROM information_schema.tables
        WHERE table_name = 'informe';

    IF (NOT FOUND) THEN
        --CREATE TEMP TABLE IF NOT EXISTS Crea un warning
        CREATE TEMP TABLE informe(
            inici DATE,
            fi DATE,
            total_esdeveniments INTEGER,
            total_edicions INTEGER,
            total_visitants REAL,
            total_recaptació REAL,
            mitjana_visitants REAL,
            mitjana_recaptació REAL
        );
    END IF;

    INSERT INTO informe VALUES(
        d_inici,
        d_fi,
        (
            SELECT COUNT(*) FROM
            (
                SELECT
                    esdeveniment
                FROM
                    edicio
                WHERE
                    inici >= d_inici AND
                    fi <= d_fi
                GROUP BY
                    esdeveniment
            ) as tmp
        ),
        (
            SELECT
                COUNT(*)
            FROM
                edicio
            WHERE
                inici >= d_inici AND
                fi <= d_fi
        ),
        (
            SELECT
                SUM(visitants)
            FROM
                edicio
            WHERE
                inici >= d_inici AND
                fi <= d_fi
        ),
        (
            SELECT
                SUM(en.preu)
            FROM
                entrada en
            WHERE
                venuda >= d_inici AND
                venuda <= d_fi
        ),
        (
            SELECT
                AVG(visitants)
            FROM
                edicio
            WHERE
                inici >= d_inici AND
                fi <= d_fi
        ),
        (
            SELECT
                AVG(en.preu)
            FROM
                entrada en
            WHERE
                venuda >= d_inici AND
                venuda <= d_fi
        )
    );

    RETURN QUERY SELECT * FROM informe;
END
$$
language 'plpgsql';

GRANT EXECUTE ON FUNCTION resum_activitat(in_inici TEXT, in_fi TEXT) TO GROUP comercials;
