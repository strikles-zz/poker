--
-- PostgreSQL database dump
--

-- Dumped from database version 9.1.3
-- Dumped by pg_dump version 9.1.3
-- Started on 2013-06-10 21:24:38

SET statement_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;

--
-- TOC entry 2064 (class 1262 OID 16579)
-- Name: kasper_knn; Type: DATABASE; Schema: -; Owner: postgres
--

CREATE DATABASE kasper_knn WITH TEMPLATE = template0 ENCODING = 'UTF8' LC_COLLATE = 'English_United States.1252' LC_CTYPE = 'English_United States.1252';


ALTER DATABASE kasper_knn OWNER TO postgres;

\connect kasper_knn

SET statement_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;

--
-- TOC entry 197 (class 3079 OID 11639)
-- Name: plpgsql; Type: EXTENSION; Schema: -; Owner: 
--

CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;


--
-- TOC entry 2067 (class 0 OID 0)
-- Dependencies: 197
-- Name: EXTENSION plpgsql; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';


SET search_path = public, pg_catalog;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- TOC entry 161 (class 1259 OID 16580)
-- Dependencies: 6
-- Name: br1_0; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE br1_0 (
    hand_id integer NOT NULL,
    dim_action1 smallint,
    dim_hand_strength smallint,
    dim_players_in_hand smallint,
    dim_players_dealt smallint,
    dim_players_yet_to_act smallint,
    dim_balance smallint,
    dim_initial_balance smallint,
    dim_call integer,
    dim_pot integer,
    dim_facing_nraises smallint
);


ALTER TABLE public.br1_0 OWNER TO postgres;

--
-- TOC entry 162 (class 1259 OID 16583)
-- Dependencies: 161 6
-- Name: br1_0_hand_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE br1_0_hand_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.br1_0_hand_id_seq OWNER TO postgres;

--
-- TOC entry 2068 (class 0 OID 0)
-- Dependencies: 162
-- Name: br1_0_hand_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE br1_0_hand_id_seq OWNED BY br1_0.hand_id;


--
-- TOC entry 2069 (class 0 OID 0)
-- Dependencies: 162
-- Name: br1_0_hand_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('br1_0_hand_id_seq', 3, true);


--
-- TOC entry 163 (class 1259 OID 16585)
-- Dependencies: 6
-- Name: br1_1; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE br1_1 (
    id integer NOT NULL,
    hand_id integer NOT NULL,
    dim_action1 smallint,
    dim_action2 smallint,
    dim_hand_strength smallint,
    dim_players_in_hand smallint,
    dim_players_dealt smallint,
    dim_players_yet_to_act smallint,
    dim_balance smallint,
    dim_initial_balance smallint,
    dim_call integer,
    dim_pot integer,
    dim_facing_nraises smallint
);


ALTER TABLE public.br1_1 OWNER TO postgres;

--
-- TOC entry 164 (class 1259 OID 16588)
-- Dependencies: 163 6
-- Name: br1_1_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE br1_1_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.br1_1_id_seq OWNER TO postgres;

--
-- TOC entry 2070 (class 0 OID 0)
-- Dependencies: 164
-- Name: br1_1_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE br1_1_id_seq OWNED BY br1_1.id;


--
-- TOC entry 2071 (class 0 OID 0)
-- Dependencies: 164
-- Name: br1_1_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('br1_1_id_seq', 4, true);


--
-- TOC entry 165 (class 1259 OID 16590)
-- Dependencies: 6
-- Name: br1_2; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE br1_2 (
    id integer NOT NULL,
    hand_id integer NOT NULL,
    dim_action1 smallint,
    dim_action2 smallint,
    dim_action3 smallint,
    dim_hand_strength smallint,
    dim_players_in_hand smallint,
    dim_players_dealt smallint,
    dim_players_yet_to_act smallint,
    dim_balance smallint,
    dim_initial_balance smallint,
    dim_call integer,
    dim_pot integer,
    dim_facing_nraises smallint
);


ALTER TABLE public.br1_2 OWNER TO postgres;

--
-- TOC entry 166 (class 1259 OID 16593)
-- Dependencies: 6 165
-- Name: br1_2_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE br1_2_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.br1_2_id_seq OWNER TO postgres;

--
-- TOC entry 2072 (class 0 OID 0)
-- Dependencies: 166
-- Name: br1_2_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE br1_2_id_seq OWNED BY br1_2.id;


--
-- TOC entry 2073 (class 0 OID 0)
-- Dependencies: 166
-- Name: br1_2_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('br1_2_id_seq', 4, true);


--
-- TOC entry 167 (class 1259 OID 16595)
-- Dependencies: 6
-- Name: br1_3; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE br1_3 (
    id integer NOT NULL,
    hand_id integer NOT NULL,
    dim_action1 smallint,
    dim_action2 smallint,
    dim_action3 smallint,
    dim_action4 smallint,
    dim_hand_strength smallint,
    dim_players_in_hand smallint,
    dim_players_dealt smallint,
    dim_players_yet_to_act smallint,
    dim_balance smallint,
    dim_initial_balance smallint,
    dim_call integer,
    dim_pot integer,
    dim_facing_nraises smallint
);


ALTER TABLE public.br1_3 OWNER TO postgres;

--
-- TOC entry 168 (class 1259 OID 16598)
-- Dependencies: 167 6
-- Name: br1_3_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE br1_3_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.br1_3_id_seq OWNER TO postgres;

--
-- TOC entry 2074 (class 0 OID 0)
-- Dependencies: 168
-- Name: br1_3_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE br1_3_id_seq OWNED BY br1_3.id;


--
-- TOC entry 2075 (class 0 OID 0)
-- Dependencies: 168
-- Name: br1_3_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('br1_3_id_seq', 4, true);


--
-- TOC entry 169 (class 1259 OID 16600)
-- Dependencies: 6
-- Name: br2_0; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE br2_0 (
    id integer NOT NULL,
    hand_id integer NOT NULL,
    dim_action1 smallint,
    dim_hand_strength smallint,
    dim_players_in_hand smallint,
    dim_players_dealt smallint,
    dim_players_yet_to_act smallint,
    dim_balance smallint,
    dim_initial_balance smallint,
    dim_call integer,
    dim_pot integer,
    dim_facing_nraises smallint,
    dim_ppot smallint,
    dim_npot smallint
);


ALTER TABLE public.br2_0 OWNER TO postgres;

--
-- TOC entry 170 (class 1259 OID 16603)
-- Dependencies: 169 6
-- Name: br2_0_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE br2_0_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.br2_0_id_seq OWNER TO postgres;

--
-- TOC entry 2076 (class 0 OID 0)
-- Dependencies: 170
-- Name: br2_0_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE br2_0_id_seq OWNED BY br2_0.id;


--
-- TOC entry 2077 (class 0 OID 0)
-- Dependencies: 170
-- Name: br2_0_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('br2_0_id_seq', 1, true);


--
-- TOC entry 171 (class 1259 OID 16605)
-- Dependencies: 6
-- Name: br2_1; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE br2_1 (
    id integer NOT NULL,
    hand_id integer NOT NULL,
    dim_action1 smallint,
    dim_action2 smallint,
    dim_hand_strength smallint,
    dim_players_in_hand smallint,
    dim_players_dealt smallint,
    dim_players_yet_to_act smallint,
    dim_balance smallint,
    dim_initial_balance smallint,
    dim_call integer,
    dim_pot integer,
    dim_facing_nraises smallint,
    dim_ppot smallint,
    dim_npot smallint
);


ALTER TABLE public.br2_1 OWNER TO postgres;

--
-- TOC entry 172 (class 1259 OID 16608)
-- Dependencies: 171 6
-- Name: br2_1_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE br2_1_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.br2_1_id_seq OWNER TO postgres;

--
-- TOC entry 2078 (class 0 OID 0)
-- Dependencies: 172
-- Name: br2_1_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE br2_1_id_seq OWNED BY br2_1.id;


--
-- TOC entry 2079 (class 0 OID 0)
-- Dependencies: 172
-- Name: br2_1_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('br2_1_id_seq', 1, true);


--
-- TOC entry 173 (class 1259 OID 16610)
-- Dependencies: 6
-- Name: br2_2; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE br2_2 (
    id integer NOT NULL,
    hand_id integer NOT NULL,
    dim_action1 smallint,
    dim_action2 smallint,
    dim_action3 smallint,
    dim_hand_strength smallint,
    dim_players_in_hand smallint,
    dim_players_dealt smallint,
    dim_players_yet_to_act smallint,
    dim_balance smallint,
    dim_initial_balance smallint,
    dim_call integer,
    dim_pot integer,
    dim_facing_nraises smallint,
    dim_ppot smallint,
    dim_npot smallint
);


ALTER TABLE public.br2_2 OWNER TO postgres;

--
-- TOC entry 174 (class 1259 OID 16613)
-- Dependencies: 6 173
-- Name: br2_2_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE br2_2_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.br2_2_id_seq OWNER TO postgres;

--
-- TOC entry 2080 (class 0 OID 0)
-- Dependencies: 174
-- Name: br2_2_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE br2_2_id_seq OWNED BY br2_2.id;


--
-- TOC entry 2081 (class 0 OID 0)
-- Dependencies: 174
-- Name: br2_2_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('br2_2_id_seq', 1, true);


--
-- TOC entry 175 (class 1259 OID 16615)
-- Dependencies: 6
-- Name: br2_3; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE br2_3 (
    id integer NOT NULL,
    hand_id integer NOT NULL,
    dim_action1 smallint,
    dim_action2 smallint,
    dim_action3 smallint,
    dim_action4 smallint,
    dim_hand_strength smallint,
    dim_players_in_hand smallint,
    dim_players_dealt smallint,
    dim_players_yet_to_act smallint,
    dim_balance smallint,
    dim_initial_balance smallint,
    dim_call integer,
    dim_pot integer,
    dim_facing_nraises smallint,
    dim_ppot smallint,
    dim_npot smallint
);


ALTER TABLE public.br2_3 OWNER TO postgres;

--
-- TOC entry 176 (class 1259 OID 16618)
-- Dependencies: 175 6
-- Name: br2_3_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE br2_3_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.br2_3_id_seq OWNER TO postgres;

--
-- TOC entry 2082 (class 0 OID 0)
-- Dependencies: 176
-- Name: br2_3_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE br2_3_id_seq OWNED BY br2_3.id;


--
-- TOC entry 2083 (class 0 OID 0)
-- Dependencies: 176
-- Name: br2_3_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('br2_3_id_seq', 1, true);


--
-- TOC entry 177 (class 1259 OID 16620)
-- Dependencies: 6
-- Name: br3_0; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE br3_0 (
    id integer NOT NULL,
    hand_id integer NOT NULL,
    dim_action1 smallint,
    dim_hand_strength smallint,
    dim_players_in_hand smallint,
    dim_players_dealt smallint,
    dim_players_yet_to_act smallint,
    dim_balance smallint,
    dim_initial_balance smallint,
    dim_call integer,
    dim_pot integer,
    dim_facing_nraises smallint,
    dim_ppot smallint,
    dim_npot smallint
);


ALTER TABLE public.br3_0 OWNER TO postgres;

--
-- TOC entry 178 (class 1259 OID 16623)
-- Dependencies: 177 6
-- Name: br3_0_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE br3_0_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.br3_0_id_seq OWNER TO postgres;

--
-- TOC entry 2084 (class 0 OID 0)
-- Dependencies: 178
-- Name: br3_0_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE br3_0_id_seq OWNED BY br3_0.id;


--
-- TOC entry 2085 (class 0 OID 0)
-- Dependencies: 178
-- Name: br3_0_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('br3_0_id_seq', 1, true);


--
-- TOC entry 179 (class 1259 OID 16625)
-- Dependencies: 6
-- Name: br3_1; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE br3_1 (
    id integer NOT NULL,
    hand_id integer NOT NULL,
    dim_action1 smallint,
    dim_action2 smallint,
    dim_hand_strength smallint,
    dim_players_in_hand smallint,
    dim_players_dealt smallint,
    dim_players_yet_to_act smallint,
    dim_balance smallint,
    dim_initial_balance smallint,
    dim_call integer,
    dim_pot integer,
    dim_facing_nraises smallint,
    dim_ppot smallint,
    dim_npot smallint
);


ALTER TABLE public.br3_1 OWNER TO postgres;

--
-- TOC entry 180 (class 1259 OID 16628)
-- Dependencies: 179 6
-- Name: br3_1_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE br3_1_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.br3_1_id_seq OWNER TO postgres;

--
-- TOC entry 2086 (class 0 OID 0)
-- Dependencies: 180
-- Name: br3_1_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE br3_1_id_seq OWNED BY br3_1.id;


--
-- TOC entry 2087 (class 0 OID 0)
-- Dependencies: 180
-- Name: br3_1_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('br3_1_id_seq', 1, true);


--
-- TOC entry 181 (class 1259 OID 16630)
-- Dependencies: 6
-- Name: br3_2; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE br3_2 (
    id integer NOT NULL,
    hand_id integer NOT NULL,
    dim_action1 smallint,
    dim_action2 smallint,
    dim_action3 smallint,
    dim_hand_strength smallint,
    dim_players_in_hand smallint,
    dim_players_dealt smallint,
    dim_players_yet_to_act smallint,
    dim_balance smallint,
    dim_initial_balance smallint,
    dim_call integer,
    dim_pot integer,
    dim_facing_nraises smallint,
    dim_ppot smallint,
    dim_npot smallint
);


ALTER TABLE public.br3_2 OWNER TO postgres;

--
-- TOC entry 182 (class 1259 OID 16633)
-- Dependencies: 181 6
-- Name: br3_2_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE br3_2_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.br3_2_id_seq OWNER TO postgres;

--
-- TOC entry 2088 (class 0 OID 0)
-- Dependencies: 182
-- Name: br3_2_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE br3_2_id_seq OWNED BY br3_2.id;


--
-- TOC entry 2089 (class 0 OID 0)
-- Dependencies: 182
-- Name: br3_2_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('br3_2_id_seq', 1092, true);


--
-- TOC entry 183 (class 1259 OID 16635)
-- Dependencies: 6
-- Name: br3_3; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE br3_3 (
    id integer NOT NULL,
    hand_id integer NOT NULL,
    dim_action1 smallint,
    dim_action2 smallint,
    dim_action3 smallint,
    dim_action4 smallint,
    dim_hand_strength smallint,
    dim_players_in_hand smallint,
    dim_players_dealt smallint,
    dim_players_yet_to_act smallint,
    dim_balance smallint,
    dim_initial_balance smallint,
    dim_call integer,
    dim_pot integer,
    dim_facing_nraises smallint,
    dim_ppot smallint,
    dim_npot smallint
);


ALTER TABLE public.br3_3 OWNER TO postgres;

--
-- TOC entry 184 (class 1259 OID 16638)
-- Dependencies: 6 183
-- Name: br3_3_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE br3_3_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.br3_3_id_seq OWNER TO postgres;

--
-- TOC entry 2090 (class 0 OID 0)
-- Dependencies: 184
-- Name: br3_3_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE br3_3_id_seq OWNED BY br3_3.id;


--
-- TOC entry 2091 (class 0 OID 0)
-- Dependencies: 184
-- Name: br3_3_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('br3_3_id_seq', 1, true);


--
-- TOC entry 185 (class 1259 OID 16640)
-- Dependencies: 6
-- Name: br4_0; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE br4_0 (
    id integer NOT NULL,
    hand_id integer NOT NULL,
    dim_action1 smallint,
    dim_hand_strength smallint,
    dim_players_in_hand smallint,
    dim_players_dealt smallint,
    dim_players_yet_to_act smallint,
    dim_balance smallint,
    dim_initial_balance smallint,
    dim_call integer,
    dim_pot integer,
    dim_facing_nraises smallint,
    dim_ppot smallint,
    dim_npot smallint
);


ALTER TABLE public.br4_0 OWNER TO postgres;

--
-- TOC entry 186 (class 1259 OID 16643)
-- Dependencies: 185 6
-- Name: br4_0_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE br4_0_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.br4_0_id_seq OWNER TO postgres;

--
-- TOC entry 2092 (class 0 OID 0)
-- Dependencies: 186
-- Name: br4_0_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE br4_0_id_seq OWNED BY br4_0.id;


--
-- TOC entry 2093 (class 0 OID 0)
-- Dependencies: 186
-- Name: br4_0_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('br4_0_id_seq', 1, true);


--
-- TOC entry 187 (class 1259 OID 16645)
-- Dependencies: 6
-- Name: br4_1; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE br4_1 (
    id integer NOT NULL,
    hand_id integer NOT NULL,
    dim_action1 smallint,
    dim_action2 smallint,
    dim_hand_strength smallint,
    dim_players_in_hand smallint,
    dim_players_dealt smallint,
    dim_players_yet_to_act smallint,
    dim_balance smallint,
    dim_initial_balance smallint,
    dim_call integer,
    dim_pot integer,
    dim_facing_nraises smallint,
    dim_ppot smallint,
    dim_npot smallint
);


ALTER TABLE public.br4_1 OWNER TO postgres;

--
-- TOC entry 188 (class 1259 OID 16648)
-- Dependencies: 187 6
-- Name: br4_1_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE br4_1_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.br4_1_id_seq OWNER TO postgres;

--
-- TOC entry 2094 (class 0 OID 0)
-- Dependencies: 188
-- Name: br4_1_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE br4_1_id_seq OWNED BY br4_1.id;


--
-- TOC entry 2095 (class 0 OID 0)
-- Dependencies: 188
-- Name: br4_1_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('br4_1_id_seq', 1, true);


--
-- TOC entry 189 (class 1259 OID 16650)
-- Dependencies: 6
-- Name: br4_2; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE br4_2 (
    id integer NOT NULL,
    hand_id integer NOT NULL,
    dim_action1 smallint,
    dim_action2 smallint,
    dim_action3 smallint,
    dim_hand_strength smallint,
    dim_players_in_hand smallint,
    dim_players_dealt smallint,
    dim_players_yet_to_act smallint,
    dim_balance smallint,
    dim_initial_balance smallint,
    dim_call integer,
    dim_pot integer,
    dim_facing_nraises smallint,
    dim_ppot smallint,
    dim_npot smallint
);


ALTER TABLE public.br4_2 OWNER TO postgres;

--
-- TOC entry 190 (class 1259 OID 16653)
-- Dependencies: 189 6
-- Name: br4_2_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE br4_2_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.br4_2_id_seq OWNER TO postgres;

--
-- TOC entry 2096 (class 0 OID 0)
-- Dependencies: 190
-- Name: br4_2_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE br4_2_id_seq OWNED BY br4_2.id;


--
-- TOC entry 2097 (class 0 OID 0)
-- Dependencies: 190
-- Name: br4_2_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('br4_2_id_seq', 1, true);


--
-- TOC entry 191 (class 1259 OID 16655)
-- Dependencies: 6
-- Name: br4_3; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE br4_3 (
    id integer NOT NULL,
    hand_id integer NOT NULL,
    dim_action1 smallint,
    dim_action2 smallint,
    dim_action3 smallint,
    dim_action4 smallint,
    dim_hand_strength smallint,
    dim_players_in_hand smallint,
    dim_players_dealt smallint,
    dim_players_yet_to_act smallint,
    dim_balance smallint,
    dim_initial_balance smallint,
    dim_call integer,
    dim_pot integer,
    dim_facing_nraises smallint,
    dim_ppot smallint,
    dim_npot smallint
);


ALTER TABLE public.br4_3 OWNER TO postgres;

--
-- TOC entry 192 (class 1259 OID 16658)
-- Dependencies: 6 191
-- Name: br4_3_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE br4_3_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.br4_3_id_seq OWNER TO postgres;

--
-- TOC entry 2098 (class 0 OID 0)
-- Dependencies: 192
-- Name: br4_3_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE br4_3_id_seq OWNED BY br4_3.id;


--
-- TOC entry 2099 (class 0 OID 0)
-- Dependencies: 192
-- Name: br4_3_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('br4_3_id_seq', 1, true);


--
-- TOC entry 193 (class 1259 OID 16660)
-- Dependencies: 6
-- Name: lookup_actions; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE lookup_actions (
    action_id smallint NOT NULL,
    action_char character(1)
);


ALTER TABLE public.lookup_actions OWNER TO postgres;

--
-- TOC entry 194 (class 1259 OID 16663)
-- Dependencies: 6
-- Name: lookup_board_texture; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE lookup_board_texture (
    texture_id smallint NOT NULL,
    description text
);


ALTER TABLE public.lookup_board_texture OWNER TO postgres;

--
-- TOC entry 195 (class 1259 OID 16669)
-- Dependencies: 6
-- Name: lookup_hand_type; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE lookup_hand_type (
    handtype_id smallint NOT NULL,
    description text
);


ALTER TABLE public.lookup_hand_type OWNER TO postgres;

--
-- TOC entry 196 (class 1259 OID 16675)
-- Dependencies: 6
-- Name: profits; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE profits (
    hand_id integer NOT NULL,
    profit integer
);


ALTER TABLE public.profits OWNER TO postgres;

--
-- TOC entry 1956 (class 2604 OID 25459)
-- Dependencies: 162 161
-- Name: hand_id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY br1_0 ALTER COLUMN hand_id SET DEFAULT nextval('br1_0_hand_id_seq'::regclass);


--
-- TOC entry 1957 (class 2604 OID 25460)
-- Dependencies: 164 163
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY br1_1 ALTER COLUMN id SET DEFAULT nextval('br1_1_id_seq'::regclass);


--
-- TOC entry 1958 (class 2604 OID 25461)
-- Dependencies: 166 165
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY br1_2 ALTER COLUMN id SET DEFAULT nextval('br1_2_id_seq'::regclass);


--
-- TOC entry 1959 (class 2604 OID 25462)
-- Dependencies: 168 167
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY br1_3 ALTER COLUMN id SET DEFAULT nextval('br1_3_id_seq'::regclass);


--
-- TOC entry 1960 (class 2604 OID 25463)
-- Dependencies: 170 169
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY br2_0 ALTER COLUMN id SET DEFAULT nextval('br2_0_id_seq'::regclass);


--
-- TOC entry 1961 (class 2604 OID 25464)
-- Dependencies: 172 171
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY br2_1 ALTER COLUMN id SET DEFAULT nextval('br2_1_id_seq'::regclass);


--
-- TOC entry 1962 (class 2604 OID 25465)
-- Dependencies: 174 173
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY br2_2 ALTER COLUMN id SET DEFAULT nextval('br2_2_id_seq'::regclass);


--
-- TOC entry 1963 (class 2604 OID 25466)
-- Dependencies: 176 175
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY br2_3 ALTER COLUMN id SET DEFAULT nextval('br2_3_id_seq'::regclass);


--
-- TOC entry 1964 (class 2604 OID 25467)
-- Dependencies: 178 177
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY br3_0 ALTER COLUMN id SET DEFAULT nextval('br3_0_id_seq'::regclass);


--
-- TOC entry 1965 (class 2604 OID 25468)
-- Dependencies: 180 179
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY br3_1 ALTER COLUMN id SET DEFAULT nextval('br3_1_id_seq'::regclass);


--
-- TOC entry 1966 (class 2604 OID 25469)
-- Dependencies: 182 181
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY br3_2 ALTER COLUMN id SET DEFAULT nextval('br3_2_id_seq'::regclass);


--
-- TOC entry 1967 (class 2604 OID 25470)
-- Dependencies: 184 183
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY br3_3 ALTER COLUMN id SET DEFAULT nextval('br3_3_id_seq'::regclass);


--
-- TOC entry 1968 (class 2604 OID 25471)
-- Dependencies: 186 185
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY br4_0 ALTER COLUMN id SET DEFAULT nextval('br4_0_id_seq'::regclass);


--
-- TOC entry 1969 (class 2604 OID 25472)
-- Dependencies: 188 187
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY br4_1 ALTER COLUMN id SET DEFAULT nextval('br4_1_id_seq'::regclass);


--
-- TOC entry 1970 (class 2604 OID 25473)
-- Dependencies: 190 189
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY br4_2 ALTER COLUMN id SET DEFAULT nextval('br4_2_id_seq'::regclass);


--
-- TOC entry 1971 (class 2604 OID 25474)
-- Dependencies: 192 191
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY br4_3 ALTER COLUMN id SET DEFAULT nextval('br4_3_id_seq'::regclass);


--
-- TOC entry 2042 (class 0 OID 16580)
-- Dependencies: 161
-- Data for Name: br1_0; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY br1_0 (hand_id, dim_action1, dim_hand_strength, dim_players_in_hand, dim_players_dealt, dim_players_yet_to_act, dim_balance, dim_initial_balance, dim_call, dim_pot, dim_facing_nraises) FROM stdin;
0	3	100	0	\N	0	\N	\N	0	0	\N
1	1	50	0	\N	0	\N	\N	0	0	\N
2	0	0	0	\N	0	\N	\N	0	0	\N
3	6	117	3	6	0	131	131	0	2	1
\.


--
-- TOC entry 2043 (class 0 OID 16585)
-- Dependencies: 163
-- Data for Name: br1_1; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY br1_1 (id, hand_id, dim_action1, dim_action2, dim_hand_strength, dim_players_in_hand, dim_players_dealt, dim_players_yet_to_act, dim_balance, dim_initial_balance, dim_call, dim_pot, dim_facing_nraises) FROM stdin;
2	0	3	3	100	0	\N	0	\N	\N	0	0	\N
3	1	1	1	50	0	\N	0	\N	\N	0	0	\N
4	2	0	0	0	0	\N	0	\N	\N	0	0	\N
\.


--
-- TOC entry 2044 (class 0 OID 16590)
-- Dependencies: 165
-- Data for Name: br1_2; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY br1_2 (id, hand_id, dim_action1, dim_action2, dim_action3, dim_hand_strength, dim_players_in_hand, dim_players_dealt, dim_players_yet_to_act, dim_balance, dim_initial_balance, dim_call, dim_pot, dim_facing_nraises) FROM stdin;
2	0	3	3	3	100	0	\N	0	\N	\N	0	0	\N
3	1	1	1	1	50	0	\N	0	\N	\N	0	0	\N
4	2	0	0	0	0	0	\N	0	\N	\N	0	0	\N
\.


--
-- TOC entry 2045 (class 0 OID 16595)
-- Dependencies: 167
-- Data for Name: br1_3; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY br1_3 (id, hand_id, dim_action1, dim_action2, dim_action3, dim_action4, dim_hand_strength, dim_players_in_hand, dim_players_dealt, dim_players_yet_to_act, dim_balance, dim_initial_balance, dim_call, dim_pot, dim_facing_nraises) FROM stdin;
2	0	3	3	3	3	100	0	\N	0	\N	\N	0	0	\N
3	1	1	1	1	1	50	0	\N	0	\N	\N	0	0	\N
4	2	0	0	0	0	0	0	\N	0	\N	\N	0	0	\N
\.


--
-- TOC entry 2046 (class 0 OID 16600)
-- Dependencies: 169
-- Data for Name: br2_0; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY br2_0 (id, hand_id, dim_action1, dim_hand_strength, dim_players_in_hand, dim_players_dealt, dim_players_yet_to_act, dim_balance, dim_initial_balance, dim_call, dim_pot, dim_facing_nraises, dim_ppot, dim_npot) FROM stdin;
0	0	3	100	0	\N	0	\N	\N	0	0	\N	0	0
1	1	2	50	0	\N	0	\N	\N	0	0	\N	0	0
2	2	0	0	0	\N	0	\N	\N	0	0	\N	0	0
\.


--
-- TOC entry 2047 (class 0 OID 16605)
-- Dependencies: 171
-- Data for Name: br2_1; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY br2_1 (id, hand_id, dim_action1, dim_action2, dim_hand_strength, dim_players_in_hand, dim_players_dealt, dim_players_yet_to_act, dim_balance, dim_initial_balance, dim_call, dim_pot, dim_facing_nraises, dim_ppot, dim_npot) FROM stdin;
0	0	3	3	100	0	\N	0	\N	\N	0	0	\N	0	0
1	1	2	2	50	0	\N	0	\N	\N	0	0	\N	0	0
2	2	0	0	0	0	\N	0	\N	\N	0	0	\N	0	0
\.


--
-- TOC entry 2048 (class 0 OID 16610)
-- Dependencies: 173
-- Data for Name: br2_2; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY br2_2 (id, hand_id, dim_action1, dim_action2, dim_action3, dim_hand_strength, dim_players_in_hand, dim_players_dealt, dim_players_yet_to_act, dim_balance, dim_initial_balance, dim_call, dim_pot, dim_facing_nraises, dim_ppot, dim_npot) FROM stdin;
0	0	3	3	3	100	0	\N	0	\N	\N	0	0	\N	0	0
1	1	2	2	2	50	0	\N	0	\N	\N	0	0	\N	0	0
2	2	0	0	0	0	0	\N	0	\N	\N	0	0	\N	0	0
\.


--
-- TOC entry 2049 (class 0 OID 16615)
-- Dependencies: 175
-- Data for Name: br2_3; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY br2_3 (id, hand_id, dim_action1, dim_action2, dim_action3, dim_action4, dim_hand_strength, dim_players_in_hand, dim_players_dealt, dim_players_yet_to_act, dim_balance, dim_initial_balance, dim_call, dim_pot, dim_facing_nraises, dim_ppot, dim_npot) FROM stdin;
0	0	3	3	3	3	100	0	\N	0	\N	\N	0	0	\N	0	0
1	1	2	2	2	2	50	0	\N	0	\N	\N	0	0	\N	0	0
2	2	0	0	0	0	0	0	\N	0	\N	\N	0	0	\N	0	0
\.


--
-- TOC entry 2050 (class 0 OID 16620)
-- Dependencies: 177
-- Data for Name: br3_0; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY br3_0 (id, hand_id, dim_action1, dim_hand_strength, dim_players_in_hand, dim_players_dealt, dim_players_yet_to_act, dim_balance, dim_initial_balance, dim_call, dim_pot, dim_facing_nraises, dim_ppot, dim_npot) FROM stdin;
0	0	3	100	0	\N	0	\N	\N	0	0	\N	0	0
1	1	2	50	0	\N	0	\N	\N	0	0	\N	0	0
2	2	0	0	0	\N	0	\N	\N	0	0	\N	0	0
\.


--
-- TOC entry 2051 (class 0 OID 16625)
-- Dependencies: 179
-- Data for Name: br3_1; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY br3_1 (id, hand_id, dim_action1, dim_action2, dim_hand_strength, dim_players_in_hand, dim_players_dealt, dim_players_yet_to_act, dim_balance, dim_initial_balance, dim_call, dim_pot, dim_facing_nraises, dim_ppot, dim_npot) FROM stdin;
0	0	3	3	100	0	\N	0	\N	\N	0	0	\N	0	0
1	1	2	2	50	0	\N	0	\N	\N	0	0	\N	0	0
2	2	0	0	0	0	\N	0	\N	\N	0	0	\N	0	0
\.


--
-- TOC entry 2052 (class 0 OID 16630)
-- Dependencies: 181
-- Data for Name: br3_2; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY br3_2 (id, hand_id, dim_action1, dim_action2, dim_action3, dim_hand_strength, dim_players_in_hand, dim_players_dealt, dim_players_yet_to_act, dim_balance, dim_initial_balance, dim_call, dim_pot, dim_facing_nraises, dim_ppot, dim_npot) FROM stdin;
0	0	3	3	3	100	0	\N	0	\N	\N	0	0	\N	0	0
1	1	2	2	2	50	0	\N	0	\N	\N	0	0	\N	0	0
2	2	0	0	0	0	0	\N	0	\N	\N	0	0	\N	0	0
\.


--
-- TOC entry 2053 (class 0 OID 16635)
-- Dependencies: 183
-- Data for Name: br3_3; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY br3_3 (id, hand_id, dim_action1, dim_action2, dim_action3, dim_action4, dim_hand_strength, dim_players_in_hand, dim_players_dealt, dim_players_yet_to_act, dim_balance, dim_initial_balance, dim_call, dim_pot, dim_facing_nraises, dim_ppot, dim_npot) FROM stdin;
0	0	3	3	3	3	100	0	\N	0	\N	\N	0	0	\N	0	0
1	1	2	2	2	2	50	0	\N	0	\N	\N	0	0	\N	0	0
2	2	0	0	0	0	0	0	\N	0	\N	\N	0	0	\N	0	0
\.


--
-- TOC entry 2054 (class 0 OID 16640)
-- Dependencies: 185
-- Data for Name: br4_0; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY br4_0 (id, hand_id, dim_action1, dim_hand_strength, dim_players_in_hand, dim_players_dealt, dim_players_yet_to_act, dim_balance, dim_initial_balance, dim_call, dim_pot, dim_facing_nraises, dim_ppot, dim_npot) FROM stdin;
0	0	3	100	0	\N	0	\N	\N	0	0	\N	0	0
1	1	2	50	0	\N	0	\N	\N	0	0	\N	0	0
2	2	0	0	0	\N	0	\N	\N	0	0	\N	0	0
\.


--
-- TOC entry 2055 (class 0 OID 16645)
-- Dependencies: 187
-- Data for Name: br4_1; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY br4_1 (id, hand_id, dim_action1, dim_action2, dim_hand_strength, dim_players_in_hand, dim_players_dealt, dim_players_yet_to_act, dim_balance, dim_initial_balance, dim_call, dim_pot, dim_facing_nraises, dim_ppot, dim_npot) FROM stdin;
0	0	3	3	100	0	\N	0	\N	\N	0	0	\N	0	0
1	1	2	2	50	0	\N	0	\N	\N	0	0	\N	0	0
2	2	0	0	0	0	\N	0	\N	\N	0	0	\N	0	0
\.


--
-- TOC entry 2056 (class 0 OID 16650)
-- Dependencies: 189
-- Data for Name: br4_2; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY br4_2 (id, hand_id, dim_action1, dim_action2, dim_action3, dim_hand_strength, dim_players_in_hand, dim_players_dealt, dim_players_yet_to_act, dim_balance, dim_initial_balance, dim_call, dim_pot, dim_facing_nraises, dim_ppot, dim_npot) FROM stdin;
0	0	3	3	3	100	0	\N	0	\N	\N	0	0	\N	0	0
1	1	2	2	2	50	0	\N	0	\N	\N	0	0	\N	0	0
2	2	0	0	0	0	0	\N	0	\N	\N	0	0	\N	0	0
\.


--
-- TOC entry 2057 (class 0 OID 16655)
-- Dependencies: 191
-- Data for Name: br4_3; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY br4_3 (id, hand_id, dim_action1, dim_action2, dim_action3, dim_action4, dim_hand_strength, dim_players_in_hand, dim_players_dealt, dim_players_yet_to_act, dim_balance, dim_initial_balance, dim_call, dim_pot, dim_facing_nraises, dim_ppot, dim_npot) FROM stdin;
0	0	3	3	3	3	100	0	\N	0	\N	\N	0	0	\N	0	0
1	1	2	2	2	2	50	0	\N	0	\N	\N	0	0	\N	0	0
2	2	0	0	0	0	0	0	\N	0	\N	\N	0	0	\N	0	0
\.


--
-- TOC entry 2058 (class 0 OID 16660)
-- Dependencies: 193
-- Data for Name: lookup_actions; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY lookup_actions (action_id, action_char) FROM stdin;
\.


--
-- TOC entry 2059 (class 0 OID 16663)
-- Dependencies: 194
-- Data for Name: lookup_board_texture; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY lookup_board_texture (texture_id, description) FROM stdin;
\.


--
-- TOC entry 2060 (class 0 OID 16669)
-- Dependencies: 195
-- Data for Name: lookup_hand_type; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY lookup_hand_type (handtype_id, description) FROM stdin;
\.


--
-- TOC entry 2061 (class 0 OID 16675)
-- Dependencies: 196
-- Data for Name: profits; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY profits (hand_id, profit) FROM stdin;
0	100
1	10
2	-100
\.


--
-- TOC entry 1973 (class 2606 OID 25454)
-- Dependencies: 161 161
-- Name: br1_0_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY br1_0
    ADD CONSTRAINT br1_0_pkey PRIMARY KEY (hand_id);


--
-- TOC entry 1975 (class 2606 OID 16697)
-- Dependencies: 163 163
-- Name: br1_1_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY br1_1
    ADD CONSTRAINT br1_1_pkey PRIMARY KEY (id);


--
-- TOC entry 1979 (class 2606 OID 16699)
-- Dependencies: 165 165
-- Name: br1_2_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY br1_2
    ADD CONSTRAINT br1_2_pkey PRIMARY KEY (id);


--
-- TOC entry 1983 (class 2606 OID 16701)
-- Dependencies: 167 167
-- Name: br1_3_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY br1_3
    ADD CONSTRAINT br1_3_pkey PRIMARY KEY (id);


--
-- TOC entry 1987 (class 2606 OID 16703)
-- Dependencies: 169 169
-- Name: br2_0_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY br2_0
    ADD CONSTRAINT br2_0_pkey PRIMARY KEY (id);


--
-- TOC entry 1991 (class 2606 OID 16705)
-- Dependencies: 171 171
-- Name: br2_1_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY br2_1
    ADD CONSTRAINT br2_1_pkey PRIMARY KEY (id);


--
-- TOC entry 1995 (class 2606 OID 16707)
-- Dependencies: 173 173
-- Name: br2_2_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY br2_2
    ADD CONSTRAINT br2_2_pkey PRIMARY KEY (id);


--
-- TOC entry 1999 (class 2606 OID 16709)
-- Dependencies: 175 175
-- Name: br2_3_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY br2_3
    ADD CONSTRAINT br2_3_pkey PRIMARY KEY (id);


--
-- TOC entry 2003 (class 2606 OID 16711)
-- Dependencies: 177 177
-- Name: br3_0_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY br3_0
    ADD CONSTRAINT br3_0_pkey PRIMARY KEY (id);


--
-- TOC entry 2007 (class 2606 OID 16713)
-- Dependencies: 179 179
-- Name: br3_1_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY br3_1
    ADD CONSTRAINT br3_1_pkey PRIMARY KEY (id);


--
-- TOC entry 2011 (class 2606 OID 16715)
-- Dependencies: 181 181
-- Name: br3_2_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY br3_2
    ADD CONSTRAINT br3_2_pkey PRIMARY KEY (id);


--
-- TOC entry 2015 (class 2606 OID 16717)
-- Dependencies: 183 183
-- Name: br3_3_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY br3_3
    ADD CONSTRAINT br3_3_pkey PRIMARY KEY (id);


--
-- TOC entry 2019 (class 2606 OID 16719)
-- Dependencies: 185 185
-- Name: br4_0_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY br4_0
    ADD CONSTRAINT br4_0_pkey PRIMARY KEY (id);


--
-- TOC entry 2023 (class 2606 OID 16721)
-- Dependencies: 187 187
-- Name: br4_1_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY br4_1
    ADD CONSTRAINT br4_1_pkey PRIMARY KEY (id);


--
-- TOC entry 2027 (class 2606 OID 16723)
-- Dependencies: 189 189
-- Name: br4_2_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY br4_2
    ADD CONSTRAINT br4_2_pkey PRIMARY KEY (id);


--
-- TOC entry 2031 (class 2606 OID 16725)
-- Dependencies: 191 191
-- Name: br4_3_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY br4_3
    ADD CONSTRAINT br4_3_pkey PRIMARY KEY (id);


--
-- TOC entry 2035 (class 2606 OID 16727)
-- Dependencies: 193 193
-- Name: lookup_actions_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY lookup_actions
    ADD CONSTRAINT lookup_actions_pkey PRIMARY KEY (action_id);


--
-- TOC entry 2037 (class 2606 OID 16729)
-- Dependencies: 194 194
-- Name: lookup_board_texture_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY lookup_board_texture
    ADD CONSTRAINT lookup_board_texture_pkey PRIMARY KEY (texture_id);


--
-- TOC entry 2039 (class 2606 OID 16731)
-- Dependencies: 195 195
-- Name: lookup_hand_type_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY lookup_hand_type
    ADD CONSTRAINT lookup_hand_type_pkey PRIMARY KEY (handtype_id);


--
-- TOC entry 2041 (class 2606 OID 25482)
-- Dependencies: 196 196
-- Name: profits_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY profits
    ADD CONSTRAINT profits_pkey PRIMARY KEY (hand_id);


--
-- TOC entry 1977 (class 2606 OID 16735)
-- Dependencies: 163 163
-- Name: unique_hand_id_1_1; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY br1_1
    ADD CONSTRAINT unique_hand_id_1_1 UNIQUE (hand_id);


--
-- TOC entry 1981 (class 2606 OID 16737)
-- Dependencies: 165 165
-- Name: unique_hand_id_1_2; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY br1_2
    ADD CONSTRAINT unique_hand_id_1_2 UNIQUE (hand_id);


--
-- TOC entry 1985 (class 2606 OID 16739)
-- Dependencies: 167 167
-- Name: unique_hand_id_1_3; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY br1_3
    ADD CONSTRAINT unique_hand_id_1_3 UNIQUE (hand_id);


--
-- TOC entry 1989 (class 2606 OID 16741)
-- Dependencies: 169 169
-- Name: unique_hand_id_2_0; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY br2_0
    ADD CONSTRAINT unique_hand_id_2_0 UNIQUE (hand_id);


--
-- TOC entry 1993 (class 2606 OID 16743)
-- Dependencies: 171 171
-- Name: unique_hand_id_2_1; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY br2_1
    ADD CONSTRAINT unique_hand_id_2_1 UNIQUE (hand_id);


--
-- TOC entry 1997 (class 2606 OID 16745)
-- Dependencies: 173 173
-- Name: unique_hand_id_2_2; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY br2_2
    ADD CONSTRAINT unique_hand_id_2_2 UNIQUE (hand_id);


--
-- TOC entry 2001 (class 2606 OID 16747)
-- Dependencies: 175 175
-- Name: unique_hand_id_2_3; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY br2_3
    ADD CONSTRAINT unique_hand_id_2_3 UNIQUE (hand_id);


--
-- TOC entry 2005 (class 2606 OID 16749)
-- Dependencies: 177 177
-- Name: unique_hand_id_3_0; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY br3_0
    ADD CONSTRAINT unique_hand_id_3_0 UNIQUE (hand_id);


--
-- TOC entry 2009 (class 2606 OID 16751)
-- Dependencies: 179 179
-- Name: unique_hand_id_3_1; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY br3_1
    ADD CONSTRAINT unique_hand_id_3_1 UNIQUE (hand_id);


--
-- TOC entry 2013 (class 2606 OID 16753)
-- Dependencies: 181 181
-- Name: unique_hand_id_3_2; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY br3_2
    ADD CONSTRAINT unique_hand_id_3_2 UNIQUE (hand_id);


--
-- TOC entry 2017 (class 2606 OID 16755)
-- Dependencies: 183 183
-- Name: unique_hand_id_3_3; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY br3_3
    ADD CONSTRAINT unique_hand_id_3_3 UNIQUE (hand_id);


--
-- TOC entry 2021 (class 2606 OID 16757)
-- Dependencies: 185 185
-- Name: unique_hand_id_4_0; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY br4_0
    ADD CONSTRAINT unique_hand_id_4_0 UNIQUE (hand_id);


--
-- TOC entry 2025 (class 2606 OID 16759)
-- Dependencies: 187 187
-- Name: unique_hand_id_4_1; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY br4_1
    ADD CONSTRAINT unique_hand_id_4_1 UNIQUE (hand_id);


--
-- TOC entry 2029 (class 2606 OID 16761)
-- Dependencies: 189 189
-- Name: unique_hand_id_4_2; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY br4_2
    ADD CONSTRAINT unique_hand_id_4_2 UNIQUE (hand_id);


--
-- TOC entry 2033 (class 2606 OID 16763)
-- Dependencies: 191 191
-- Name: unique_hand_id_4_3; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY br4_3
    ADD CONSTRAINT unique_hand_id_4_3 UNIQUE (hand_id);


--
-- TOC entry 2066 (class 0 OID 0)
-- Dependencies: 6
-- Name: public; Type: ACL; Schema: -; Owner: postgres
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;


-- Completed on 2013-06-10 21:24:38

--
-- PostgreSQL database dump complete
--

