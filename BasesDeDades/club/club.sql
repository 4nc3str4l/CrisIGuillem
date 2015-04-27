\set ON_ERROR_STOP on
\echo escript principal per la base de dades club
\echo -------------------------------------------

\c postgres
DROP DATABASE club;
CREATE DATABASE club;
\c club
\i 'comarca/comarca.sql'
\i 'ciutat/ciutat.sql'
\i 'persona/persona.sql'
\i 'mails/mails.sql'
\i 'coneix/coneix.sql'
\i 'soci/soci.sql'
\i 'treballador/treballador.sql'
\i 'esport/esport.sql'
\i 'fa/fa.sql'
\i 'nomines/nomines.sql'
\i 'inserts.sql'







