#!/usr/local/bin/psqlsh

\timing off
\t on
\a

DO $$
DECLARE
	cnt int;
BEGIN
	SELECT INTO cnt count(*) FROM pg_class WHERE relname = 't1';
	IF cnt = 0 THEN
		EXECUTE 'CREATE TABLE t1(id serial, name varchar)';
		INSERT INTO t1 (name) VALUES('Bob'),('Mary'),('Tom');
	END IF;
END;
$$;

INSERT INTO t1 (name) VALUES('hoge');

SELECT 'Content-type: text/html;';
SELECT '';
SELECT '<html><head><title>Hello psql CGI!</title>';
SELECT '<body>';
\html
SELECT * from t1;
\html
SELECT '</body></html>';
