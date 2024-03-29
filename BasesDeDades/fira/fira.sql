\echo escript principal per la base de dades fira
\echo -------------------------------------------

\c postgres
DROP DATABASE fira;
CREATE DATABASE fira;
\c fira

SET client_min_messages TO WARNING;

\i 'ubicacio/ubicacio.sql'
\i 'empresa/empresa.sql'
\i 'persona/persona.sql'
\i 'esdeveniment/esdeveniment.sql'
\i 'edicio/edicio.sql'
\i 'comercial/comercial.sql'
\i 'localitzada/localitzada.sql'
\i 'estand/estand.sql'
\i 'pertany/pertany.sql'
\i 'aten/aten.sql'
\i 'reunio/reunio.sql'
\i 'entrada/entrada.sql'
\i 'config/config.sql'

\i 'grups.sql'
\i 'disparadors.sql'

\i 'ubicacio/insert.sql'
\i 'empresa/insert.sql'
\i 'persona/insert.sql'
\i 'esdeveniment/insert.sql'
\i 'edicio/insert.sql'
\i 'comercial/insert.sql'
\i 'localitzada/insert.sql'
\i 'estand/insert.sql'
\i 'pertany/insert.sql'
\i 'aten/insert.sql'
\i 'reunio/insert.sql'
\i 'entrada/insert.sql'

\i 'vistes.sql'
\i 'funcio.sql'

-- TODO: Al donar d'alta un comercial hem de crear-li un usuari per el postgres .
-- TODO: Crear grups, tralali, tralala comercials,
-- CREATE GROUP comercial, grand SELECT on vista comercial to group comercial
-- Pistes : SELECT * from visita_comercial where dni = current_user
-- Vol crear un usuari des de dintre de un trigger, diu que li posa palote. (s'ha de dir alta comercial o algo aixi)
