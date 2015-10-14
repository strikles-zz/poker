CREATE DATABASE kasper_knn WITH TEMPLATE = template0 ENCODING = 'UTF8' LC_COLLATE = 'C' LC_CTYPE = 'C';
\connect kasper_knn

SET statement_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;

INSERT INTO "br1_0" VALUES (DEFAULT, 10, 44, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_0" VALUES (DEFAULT, 9, 43, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_0" VALUES (DEFAULT, 8, 42, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_0" VALUES (DEFAULT, 7, 39, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_0" VALUES (DEFAULT, 6, 36, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_0" VALUES (DEFAULT, 5, 33, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_0" VALUES (DEFAULT, 4, 30, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_0" VALUES (DEFAULT, 3, 27, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_0" VALUES (DEFAULT, 2, 24, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_0" VALUES (DEFAULT, 1, 21, 1, 1, 1, 2, 3, 1, 2, 0);

INSERT INTO "br1_1" VALUES (DEFAULT, 10, 10, 10, 44, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_1" VALUES (DEFAULT, 9, 9, 9, 43, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_1" VALUES (DEFAULT, 8, 8, 8, 42, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_1" VALUES (DEFAULT, 7, 7, 7, 39, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_1" VALUES (DEFAULT, 6, 6, 6, 36, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_1" VALUES (DEFAULT, 5, 5, 5, 33, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_1" VALUES (DEFAULT, 4, 4, 4, 30, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_1" VALUES (DEFAULT, 3, 3, 3, 27, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_1" VALUES (DEFAULT, 2, 2, 2, 24, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_1" VALUES (DEFAULT, 1, 1, 1, 21, 1, 1, 1, 2, 3, 1, 2, 0);

INSERT INTO "br1_2" VALUES (DEFAULT, 10, 10, 10, 10, 44, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_2" VALUES (DEFAULT, 9, 9, 9, 9, 43, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_2" VALUES (DEFAULT, 8, 8, 8, 8, 42, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_2" VALUES (DEFAULT, 7, 7, 7, 7, 39, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_2" VALUES (DEFAULT, 6, 6, 6, 6, 36, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_2" VALUES (DEFAULT, 5, 5, 5, 5, 33, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_2" VALUES (DEFAULT, 4, 4, 4, 4, 30, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_2" VALUES (DEFAULT, 3, 3, 3, 3, 27, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_2" VALUES (DEFAULT, 2, 2, 2, 2, 24, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_2" VALUES (DEFAULT, 1, 1, 1, 1, 21, 1, 1, 1, 2, 3, 1, 2, 0);

INSERT INTO "br1_3" VALUES (DEFAULT, 10, 10, 10, 10, 10, 44, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_3" VALUES (DEFAULT, 9, 9, 9, 9, 9, 43, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_3" VALUES (DEFAULT, 8, 8, 8, 8, 8, 42, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_3" VALUES (DEFAULT, 7, 7, 7, 7, 7, 39, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_3" VALUES (DEFAULT, 6, 6, 6, 6, 6, 36, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_3" VALUES (DEFAULT, 5, 5, 5, 5, 5, 33, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_3" VALUES (DEFAULT, 4, 4, 4, 4, 4, 30, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_3" VALUES (DEFAULT, 3, 3, 3, 3, 3, 27, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_3" VALUES (DEFAULT, 2, 2, 2, 2, 2, 24, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br1_3" VALUES (DEFAULT, 1, 1, 1, 1, 1, 21, 1, 1, 1, 2, 3, 1, 2, 0);

INSERT INTO "br2_0" VALUES (DEFAULT, 10, 10, 100, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_0" VALUES (DEFAULT, 9, 9, 90, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_0" VALUES (DEFAULT, 8, 8, 85, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_0" VALUES (DEFAULT, 7, 7, 80, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_0" VALUES (DEFAULT, 6, 6, 75, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_0" VALUES (DEFAULT, 5, 5, 70, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_0" VALUES (DEFAULT, 4, 4, 65, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_0" VALUES (DEFAULT, 3, 3, 60, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_0" VALUES (DEFAULT, 2, 2, 55, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_0" VALUES (DEFAULT, 1, 1, 50, 1, 1, 1, 2, 3, 1, 2, 0);

INSERT INTO "br2_1" VALUES (DEFAULT, 10, 10, 10, 100, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_1" VALUES (DEFAULT, 9, 9, 9, 90, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_1" VALUES (DEFAULT, 8, 8, 8, 85, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_1" VALUES (DEFAULT, 7, 7, 7, 80, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_1" VALUES (DEFAULT, 6, 6, 6, 75, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_1" VALUES (DEFAULT, 5, 5, 5, 70, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_1" VALUES (DEFAULT, 4, 4, 4, 65, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_1" VALUES (DEFAULT, 3, 3, 3, 60, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_1" VALUES (DEFAULT, 2, 2, 2, 55, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_1" VALUES (DEFAULT, 1, 1, 1, 50, 1, 1, 1, 2, 3, 1, 2, 0);

INSERT INTO "br2_2" VALUES (DEFAULT, 10, 10, 10, 10, 100, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_2" VALUES (DEFAULT, 9, 9, 9, 9, 90, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_2" VALUES (DEFAULT, 8, 8, 8, 8, 85, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_2" VALUES (DEFAULT, 7, 7, 7, 7, 80, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_2" VALUES (DEFAULT, 6, 6, 6, 6, 75, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_2" VALUES (DEFAULT, 5, 5, 5, 5, 70, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_2" VALUES (DEFAULT, 4, 4, 4, 4, 65, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_2" VALUES (DEFAULT, 3, 3, 3, 3, 60, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_2" VALUES (DEFAULT, 2, 2, 2, 2, 55, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_2" VALUES (DEFAULT, 1, 1, 1, 1, 50, 1, 1, 1, 2, 3, 1, 2, 0);

INSERT INTO "br2_3" VALUES (DEFAULT, 10, 10, 10, 10, 10, 100, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_3" VALUES (DEFAULT, 9, 9, 9, 9, 9, 90, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_3" VALUES (DEFAULT, 8, 8, 8, 8, 8, 85, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_3" VALUES (DEFAULT, 7, 7, 7, 7, 7, 80, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_3" VALUES (DEFAULT, 6, 6, 6, 6, 6, 75, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_3" VALUES (DEFAULT, 5, 5, 5, 5, 5, 70, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_3" VALUES (DEFAULT, 4, 4, 4, 4, 4, 65, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_3" VALUES (DEFAULT, 3, 3, 3, 3, 3, 60, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_3" VALUES (DEFAULT, 2, 2, 2, 2, 2, 55, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br2_3" VALUES (DEFAULT, 1, 1, 1, 1, 1, 50, 1, 1, 1, 2, 3, 1, 2, 0);

INSERT INTO "br3_0" VALUES (DEFAULT, 10, 10, 100, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_0" VALUES (DEFAULT, 9, 9, 90, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_0" VALUES (DEFAULT, 8, 8, 85, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_0" VALUES (DEFAULT, 7, 7, 80, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_0" VALUES (DEFAULT, 6, 6, 75, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_0" VALUES (DEFAULT, 5, 5, 70, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_0" VALUES (DEFAULT, 4, 4, 65, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_0" VALUES (DEFAULT, 3, 3, 60, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_0" VALUES (DEFAULT, 2, 2, 55, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_0" VALUES (DEFAULT, 1, 1, 50, 1, 1, 1, 2, 3, 1, 2, 0);

INSERT INTO "br3_1" VALUES (DEFAULT, 10, 10, 10, 100, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_1" VALUES (DEFAULT, 9, 9, 9, 90, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_1" VALUES (DEFAULT, 8, 8, 8, 85, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_1" VALUES (DEFAULT, 7, 7, 7, 80, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_1" VALUES (DEFAULT, 6, 6, 6, 75, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_1" VALUES (DEFAULT, 5, 5, 5, 70, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_1" VALUES (DEFAULT, 4, 4, 4, 65, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_1" VALUES (DEFAULT, 3, 3, 3, 60, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_1" VALUES (DEFAULT, 2, 2, 2, 55, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_1" VALUES (DEFAULT, 1, 1, 1, 50, 1, 1, 1, 2, 3, 1, 2, 0);

INSERT INTO "br3_2" VALUES (DEFAULT, 10, 10, 10, 10, 100, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_2" VALUES (DEFAULT, 9, 9, 9, 9, 90, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_2" VALUES (DEFAULT, 8, 8, 8, 8, 85, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_2" VALUES (DEFAULT, 7, 7, 7, 7, 80, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_2" VALUES (DEFAULT, 6, 6, 6, 6, 75, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_2" VALUES (DEFAULT, 5, 5, 5, 5, 70, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_2" VALUES (DEFAULT, 4, 4, 4, 4, 65, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_2" VALUES (DEFAULT, 3, 3, 3, 3, 60, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_2" VALUES (DEFAULT, 2, 2, 2, 2, 55, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_2" VALUES (DEFAULT, 1, 1, 1, 1, 50, 1, 1, 1, 2, 3, 1, 2, 0);

INSERT INTO "br3_3" VALUES (DEFAULT, 10, 10, 10, 10, 10, 100, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_3" VALUES (DEFAULT, 9, 9, 9, 9, 9, 90, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_3" VALUES (DEFAULT, 8, 8, 8, 8, 8, 85, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_3" VALUES (DEFAULT, 7, 7, 7, 7, 7, 80, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_3" VALUES (DEFAULT, 6, 6, 6, 6, 6, 75, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_3" VALUES (DEFAULT, 5, 5, 5, 5, 5, 70, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_3" VALUES (DEFAULT, 4, 4, 4, 4, 4, 65, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_3" VALUES (DEFAULT, 3, 3, 3, 3, 3, 60, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_3" VALUES (DEFAULT, 2, 2, 2, 2, 2, 55, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br3_3" VALUES (DEFAULT, 1, 1, 1, 1, 1, 50, 1, 1, 1, 2, 3, 1, 2, 0);

INSERT INTO "br4_0" VALUES (DEFAULT, 10, 10, 100, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_0" VALUES (DEFAULT, 9, 9, 90, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_0" VALUES (DEFAULT, 8, 8, 85, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_0" VALUES (DEFAULT, 7, 7, 80, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_0" VALUES (DEFAULT, 6, 6, 75, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_0" VALUES (DEFAULT, 5, 5, 70, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_0" VALUES (DEFAULT, 4, 4, 65, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_0" VALUES (DEFAULT, 3, 3, 60, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_0" VALUES (DEFAULT, 2, 2, 55, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_0" VALUES (DEFAULT, 1, 1, 50, 1, 1, 1, 2, 3, 1, 2, 0);

INSERT INTO "br4_1" VALUES (DEFAULT, 10, 10, 10, 100, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_1" VALUES (DEFAULT, 9, 9, 9, 90, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_1" VALUES (DEFAULT, 8, 8, 8, 85, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_1" VALUES (DEFAULT, 7, 7, 7, 80, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_1" VALUES (DEFAULT, 6, 6, 6, 75, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_1" VALUES (DEFAULT, 5, 5, 5, 70, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_1" VALUES (DEFAULT, 4, 4, 4, 65, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_1" VALUES (DEFAULT, 3, 3, 3, 60, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_1" VALUES (DEFAULT, 2, 2, 2, 55, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_1" VALUES (DEFAULT, 1, 1, 1, 50, 1, 1, 1, 2, 3, 1, 2, 0);

INSERT INTO "br4_2" VALUES (DEFAULT, 10, 10, 10, 10, 100, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_2" VALUES (DEFAULT, 9, 9, 9, 9, 90, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_2" VALUES (DEFAULT, 8, 8, 8, 8, 85, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_2" VALUES (DEFAULT, 7, 7, 7, 7, 80, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_2" VALUES (DEFAULT, 6, 6, 6, 6, 75, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_2" VALUES (DEFAULT, 5, 5, 5, 5, 70, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_2" VALUES (DEFAULT, 4, 4, 4, 4, 65, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_2" VALUES (DEFAULT, 3, 3, 3, 3, 60, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_2" VALUES (DEFAULT, 2, 2, 2, 2, 55, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_2" VALUES (DEFAULT, 1, 1, 1, 1, 50, 1, 1, 1, 2, 3, 1, 2, 0);

INSERT INTO "br4_3" VALUES (DEFAULT, 10, 10, 10, 10, 10, 100, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_3" VALUES (DEFAULT, 9, 9, 9, 9, 9, 90, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_3" VALUES (DEFAULT, 8, 8, 8, 8, 8, 85, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_3" VALUES (DEFAULT, 7, 7, 7, 7, 7, 80, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_3" VALUES (DEFAULT, 6, 6, 6, 6, 6, 75, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_3" VALUES (DEFAULT, 5, 5, 5, 5, 5, 70, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_3" VALUES (DEFAULT, 4, 4, 4, 4, 4, 65, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_3" VALUES (DEFAULT, 3, 3, 3, 3, 3, 60, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_3" VALUES (DEFAULT, 2, 2, 2, 2, 2, 55, 1, 1, 1, 2, 3, 1, 2, 0);
INSERT INTO "br4_3" VALUES (DEFAULT, 1, 1, 1, 1, 1, 50, 1, 1, 1, 2, 3, 1, 2, 0);

INSERT INTO "profits" VALUES (1, 1);
INSERT INTO "profits" VALUES (2, 2);
INSERT INTO "profits" VALUES (3, 3);
INSERT INTO "profits" VALUES (4, 4);
INSERT INTO "profits" VALUES (5, 5);
INSERT INTO "profits" VALUES (6, 6);
INSERT INTO "profits" VALUES (7, 7);
INSERT INTO "profits" VALUES (8, 8);
INSERT INTO "profits" VALUES (9, 9);
INSERT INTO "profits" VALUES (10, 10);