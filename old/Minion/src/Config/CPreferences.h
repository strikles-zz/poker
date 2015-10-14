#pragma once

#include <string>

struct general_config
{

private:

	int rebuy_amt;
	int rebuy_mode;
	int currency;

public:

	void set_rebuy_amt(int i)		{ rebuy_amt = i; }
	void set_rebuy_mode(int i)		{ rebuy_mode = i; }                       
	void set_currency(int i)		{ currency = i; }

	int get_rebuy_amt()				{ return rebuy_amt; }
	int get_rebuy_mode()			{ return rebuy_mode; }                       
	int get_currency()				{ return currency; }
};                                              

struct log_config
{

private:

	bool console;
	bool file;
	bool debug_bip;
	bool error_bip;

public:

	void set_console(bool b)		{ console = b; }
	void set_file(bool b)			{ file = b; }
	void set_debug_bip(bool b)		{ debug_bip = b; }
	void set_error_bip(bool b)		{ error_bip = b; }

	bool get_console()				{ return console; }
	bool get_file()					{ return file; }
	bool get_debug_bip()			{ return debug_bip; }
	bool get_error_bip()			{ return error_bip; }
};

struct db_config
{

private:

	bool debug;
	int port;
	std::string ip;
	std::string name;
	std::string user;
	std::string pass;

public:

	void set_debug(bool b)			{ debug = b; }
	void set_port(int i)			{ port = i; }
	void set_ip(char* s)			{ ip = std::string(s); }
	void set_name(char* s)			{ name = std::string(s); }
	void set_user(char* s)			{ user = std::string(s); }
	void set_pass(char* s)			{ pass = std::string(s); }


	bool get_debug()				{ return debug; }
	int get_port()					{ return port; }
	std::string get_ip()			{ return ip; }
	std::string get_name()			{ return name; }
	std::string get_user()			{ return user; }
	std::string get_pass()			{ return pass; }

};



struct knn_config
{
public:

private:

	bool debug;
	bool istraining;
	int num_neighbours[4];
	int max_pts;
	int eps;

public:

	void set_debug(bool b)								{ debug = b; }
	void set_istraining(bool b)							{ istraining = b; }
	void set_num_neighbours(int i, int j)				{ num_neighbours[j] = i; }
	void set_max_pts(int i)								{ max_pts = i; }
	void set_eps(int i)									{ eps = i; }

	bool get_debug()									{ return debug; }
	bool get_istraining()								{ return istraining; }
	int get_num_neighbours(int round)					{ return num_neighbours[round]; }
	int get_max_pts()									{ return max_pts; }
	int get_eps()										{ return eps; }
};

struct srv_config
{

private:

	bool debug; 
	int mode;	
	int port;
	std::string ip;

public:

	void set_debug(bool b)			{ debug = b; }
	void set_mode(int i)			{ mode = i; }
	void set_port(int i)			{ port = i; }
	void set_ip(char* s)			{ ip = std::string(s); }

	bool get_debug()				{ return debug; }
	int get_mode()					{ return mode; }
	int get_port()					{ return port; }
	std::string get_ip()			{ return ip; }
};



class CPreferences
{

public:

	CPreferences();

public:

	general_config general;
	log_config log;
	db_config db;
	knn_config knn;
	srv_config net;
};

extern CPreferences gPrefs;
