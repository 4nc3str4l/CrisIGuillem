\set ON_ERROR_STOP on
\echo escript principal per la base de dades fira
\echo -------------------------------------------

\c postgres
DROP DATABASE fira;
CREATE DATABASE fira;
\c fira
\i 'ubicacio/ubicacio.sql'
\i 'empresa/empresa.sql'
\i 'comercial/comercial.sql'
\i 'esdeveniment/esdeveniment.sql'
