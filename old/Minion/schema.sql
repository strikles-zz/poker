CREATE DATABASE kasper_knn WITH TEMPLATE = template0 ENCODING = 'UTF8' LC_COLLATE = 'C' LC_CTYPE = 'C';
\connect kasper_knn

SET statement_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;

CREATE TABLE br1_0 (
    hand_id serial,
    dim_action1 smallint,
    dim_hand_strength smallint,
    dim_players_in_hand smallint,
    dim_players_dealt smallint,
    dim_players_yet_to_act smallint,
    dim_balance integer,
    dim_initial_balance integer,
    dim_call integer,
    dim_pot integer,
    dim_facing_nraises smallint
);

CREATE TABLE br1_1 (
    id serial,
    hand_id bigint,
    dim_action1 smallint,
    dim_action2 smallint,
    dim_hand_strength smallint,
    dim_players_in_hand smallint,
    dim_players_dealt smallint,
    dim_players_yet_to_act smallint,
    dim_balance integer,
    dim_initial_balance integer,
    dim_call integer,
    dim_pot integer,
    dim_facing_nraises smallint
);

CREATE TABLE br1_2 (
    id serial,
    hand_id bigint,
    dim_action1 smallint,
    dim_action2 smallint,
    dim_action3 smallint,
    dim_hand_strength smallint,
    dim_players_in_hand smallint,
    dim_players_dealt smallint,
    dim_players_yet_to_act smallint,
    dim_balance integer,
    dim_initial_balance integer,
    dim_call integer,
    dim_pot integer,
    dim_facing_nraises smallint
);

CREATE TABLE br1_3 (
    id serial,
    hand_id bigint,
    dim_action1 smallint,
    dim_action2 smallint,
    dim_action3 smallint,
    dim_action4 smallint,
    dim_hand_strength smallint,
    dim_players_in_hand smallint,
    dim_players_dealt smallint,
    dim_players_yet_to_act smallint,
    dim_balance integer,
    dim_initial_balance integer,
    dim_call integer,
    dim_pot integer,
    dim_facing_nraises smallint
);

CREATE TABLE br2_0 (
    id serial,
    hand_id bigint,
    dim_action1 smallint,
    dim_hand_strength smallint,
    dim_players_in_hand smallint,
    dim_players_dealt smallint,
    dim_players_yet_to_act smallint,
    dim_balance integer,
    dim_initial_balance integer,
    dim_call integer,
    dim_pot integer,
    dim_facing_nraises smallint,
    dim_ppot smallint,
    dim_npot smallint
);

CREATE TABLE br2_1 (
    id serial,
    hand_id bigint,
    dim_action1 smallint,
    dim_action2 smallint,
    dim_hand_strength smallint,
    dim_players_in_hand smallint,
    dim_players_dealt smallint,
    dim_players_yet_to_act smallint,
    dim_balance integer,
    dim_initial_balance integer,
    dim_call integer,
    dim_pot integer,
    dim_facing_nraises smallint,
    dim_ppot smallint,
    dim_npot smallint
);

CREATE TABLE br2_2 (
    id serial,
    hand_id bigint,
    dim_action1 smallint,
    dim_action2 smallint,
    dim_action3 smallint,
    dim_hand_strength smallint,
    dim_players_in_hand smallint,
    dim_players_dealt smallint,
    dim_players_yet_to_act smallint,
    dim_balance integer,
    dim_initial_balance integer,
    dim_call integer,
    dim_pot integer,
    dim_facing_nraises smallint,
    dim_ppot smallint,
    dim_npot smallint
);

CREATE TABLE br2_3 (
    id serial,
    hand_id bigint,
    dim_action1 smallint,
    dim_action2 smallint,
    dim_action3 smallint,
    dim_action4 smallint,
    dim_hand_strength smallint,
    dim_players_in_hand smallint,
    dim_players_dealt smallint,
    dim_players_yet_to_act smallint,
    dim_balance integer,
    dim_initial_balance integer,
    dim_call integer,
    dim_pot integer,
    dim_facing_nraises smallint,
    dim_ppot smallint,
    dim_npot smallint
);

CREATE TABLE br3_0 (
    id serial,
    hand_id bigint,
    dim_action1 smallint,
    dim_hand_strength smallint,
    dim_players_in_hand smallint,
    dim_players_dealt smallint,
    dim_players_yet_to_act smallint,
    dim_balance integer,
    dim_initial_balance integer,
    dim_call integer,
    dim_pot integer,
    dim_facing_nraises smallint,
    dim_ppot smallint,
    dim_npot smallint
);

CREATE TABLE br3_1 (
    id serial,
    hand_id bigint,
    dim_action1 smallint,
    dim_action2 smallint,
    dim_hand_strength smallint,
    dim_players_in_hand smallint,
    dim_players_dealt smallint,
    dim_players_yet_to_act smallint,
    dim_balance integer,
    dim_initial_balance integer,
    dim_call integer,
    dim_pot integer,
    dim_facing_nraises smallint,
    dim_ppot smallint,
    dim_npot smallint
);

CREATE TABLE br3_2 (
    id serial,
    hand_id bigint,
    dim_action1 smallint,
    dim_action2 smallint,
    dim_action3 smallint,
    dim_hand_strength smallint,
    dim_players_in_hand smallint,
    dim_players_dealt smallint,
    dim_players_yet_to_act smallint,
    dim_balance integer,
    dim_initial_balance integer,
    dim_call integer,
    dim_pot integer,
    dim_facing_nraises smallint,
    dim_ppot smallint,
    dim_npot smallint
);

CREATE TABLE br3_3 (
    id serial,
    hand_id bigint,
    dim_action1 smallint,
    dim_action2 smallint,
    dim_action3 smallint,
    dim_action4 smallint,
    dim_hand_strength smallint,
    dim_players_in_hand smallint,
    dim_players_dealt smallint,
    dim_players_yet_to_act smallint,
    dim_balance integer,
    dim_initial_balance integer,
    dim_call integer,
    dim_pot integer,
    dim_facing_nraises smallint,
    dim_ppot smallint,
    dim_npot smallint
);

CREATE TABLE br4_0 (
    id serial,
    hand_id bigint,
    dim_action1 smallint,
    dim_hand_strength smallint,
    dim_players_in_hand smallint,
    dim_players_dealt smallint,
    dim_players_yet_to_act smallint,
    dim_balance integer,
    dim_initial_balance integer,
    dim_call integer,
    dim_pot integer,
    dim_facing_nraises smallint,
    dim_ppot smallint,
    dim_npot smallint
);

CREATE TABLE br4_1 (
    id serial,
    hand_id bigint,
    dim_action1 smallint,
    dim_action2 smallint,
    dim_hand_strength smallint,
    dim_players_in_hand smallint,
    dim_players_dealt smallint,
    dim_players_yet_to_act smallint,
    dim_balance integer,
    dim_initial_balance integer,
    dim_call integer,
    dim_pot integer,
    dim_facing_nraises smallint,
    dim_ppot smallint,
    dim_npot smallint
);

CREATE TABLE br4_2 (
    id serial,
    hand_id bigint,
    dim_action1 smallint,
    dim_action2 smallint,
    dim_action3 smallint,
    dim_hand_strength smallint,
    dim_players_in_hand smallint,
    dim_players_dealt smallint,
    dim_players_yet_to_act smallint,
    dim_balance integer,
    dim_initial_balance integer,
    dim_call integer,
    dim_pot integer,
    dim_facing_nraises smallint,
    dim_ppot smallint,
    dim_npot smallint
);

CREATE TABLE br4_3 (
    id serial,
    hand_id bigint,
    dim_action1 smallint,
    dim_action2 smallint,
    dim_action3 smallint,
    dim_action4 smallint,
    dim_hand_strength smallint,
    dim_players_in_hand smallint,
    dim_players_dealt smallint,
    dim_players_yet_to_act smallint,
    dim_balance integer,
    dim_initial_balance integer,
    dim_call integer,
    dim_pot integer,
    dim_facing_nraises smallint,
    dim_ppot smallint,
    dim_npot smallint
);

CREATE TABLE lookup_actions (
    action_id smallint NOT NULL,
    action_char character(1)
);

CREATE TABLE lookup_board_texture (
    texture_id smallint NOT NULL,
    description text
);

CREATE TABLE lookup_hand_type (
    handtype_id smallint NOT NULL,
    description text
);

CREATE TABLE profits (
    hand_id bigint,
    profit integer
);

ALTER TABLE ONLY br1_0
    ADD CONSTRAINT br1_0_pkey PRIMARY KEY (hand_id);

ALTER TABLE ONLY br1_1
    ADD CONSTRAINT br1_1_pkey PRIMARY KEY (id);

ALTER TABLE ONLY br1_2
    ADD CONSTRAINT br1_2_pkey PRIMARY KEY (id);

ALTER TABLE ONLY br1_3
    ADD CONSTRAINT br1_3_pkey PRIMARY KEY (id);

ALTER TABLE ONLY br2_0
    ADD CONSTRAINT br2_0_pkey PRIMARY KEY (id);

ALTER TABLE ONLY br2_1
    ADD CONSTRAINT br2_1_pkey PRIMARY KEY (id);

ALTER TABLE ONLY br2_2
    ADD CONSTRAINT br2_2_pkey PRIMARY KEY (id);

ALTER TABLE ONLY br2_3
    ADD CONSTRAINT br2_3_pkey PRIMARY KEY (id);

ALTER TABLE ONLY br3_0
    ADD CONSTRAINT br3_0_pkey PRIMARY KEY (id);

ALTER TABLE ONLY br3_1
    ADD CONSTRAINT br3_1_pkey PRIMARY KEY (id);

ALTER TABLE ONLY br3_2
    ADD CONSTRAINT br3_2_pkey PRIMARY KEY (id);

ALTER TABLE ONLY br3_3
    ADD CONSTRAINT br3_3_pkey PRIMARY KEY (id);

ALTER TABLE ONLY br4_0
    ADD CONSTRAINT br4_0_pkey PRIMARY KEY (id);

ALTER TABLE ONLY br4_1
    ADD CONSTRAINT br4_1_pkey PRIMARY KEY (id);

ALTER TABLE ONLY br4_2
    ADD CONSTRAINT br4_2_pkey PRIMARY KEY (id);

ALTER TABLE ONLY br4_3
    ADD CONSTRAINT br4_3_pkey PRIMARY KEY (id);

ALTER TABLE ONLY lookup_actions
    ADD CONSTRAINT lookup_actions_pkey PRIMARY KEY (action_id);

ALTER TABLE ONLY lookup_board_texture
    ADD CONSTRAINT lookup_board_texture_pkey PRIMARY KEY (texture_id);

ALTER TABLE ONLY lookup_hand_type
    ADD CONSTRAINT lookup_hand_type_pkey PRIMARY KEY (handtype_id);

ALTER TABLE ONLY profits
    ADD CONSTRAINT profits_pkey PRIMARY KEY (hand_id);

ALTER TABLE ONLY br1_1
    ADD CONSTRAINT unique_hand_id_1_1 UNIQUE (hand_id);

ALTER TABLE ONLY br1_2
    ADD CONSTRAINT unique_hand_id_1_2 UNIQUE (hand_id);

ALTER TABLE ONLY br1_3
    ADD CONSTRAINT unique_hand_id_1_3 UNIQUE (hand_id);

ALTER TABLE ONLY br2_0
    ADD CONSTRAINT unique_hand_id_2_0 UNIQUE (hand_id);

ALTER TABLE ONLY br2_1
    ADD CONSTRAINT unique_hand_id_2_1 UNIQUE (hand_id);

ALTER TABLE ONLY br2_2
    ADD CONSTRAINT unique_hand_id_2_2 UNIQUE (hand_id);

ALTER TABLE ONLY br2_3
    ADD CONSTRAINT unique_hand_id_2_3 UNIQUE (hand_id);

ALTER TABLE ONLY br3_0
    ADD CONSTRAINT unique_hand_id_3_0 UNIQUE (hand_id);

ALTER TABLE ONLY br3_1
    ADD CONSTRAINT unique_hand_id_3_1 UNIQUE (hand_id);

ALTER TABLE ONLY br3_2
    ADD CONSTRAINT unique_hand_id_3_2 UNIQUE (hand_id);

ALTER TABLE ONLY br3_3
    ADD CONSTRAINT unique_hand_id_3_3 UNIQUE (hand_id);

ALTER TABLE ONLY br4_0
    ADD CONSTRAINT unique_hand_id_4_0 UNIQUE (hand_id);

ALTER TABLE ONLY br4_1
    ADD CONSTRAINT unique_hand_id_4_1 UNIQUE (hand_id);

ALTER TABLE ONLY br4_2
    ADD CONSTRAINT unique_hand_id_4_2 UNIQUE (hand_id);

ALTER TABLE ONLY br4_3
    ADD CONSTRAINT unique_hand_id_4_3 UNIQUE (hand_id);
	

