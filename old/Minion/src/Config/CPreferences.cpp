
#include <conio.h>
//#include <io.h>
#include <iostream>

#include "Config/CIniReader.h"
#include "Config/CPreferences.h"

#include "Util/Util.h"

CPreferences::CPreferences()
{
	std::string config_file_name = ".\\config.ini";
	bool config_file_exists = false;
	config_file_exists = checkFileExistence(config_file_name);
	if(!config_file_exists)
	{
		_cprintf("Error could not locate config.ini\nPress any key to exit.\n");
		_getch();
		std::exit(EXIT_FAILURE);
	}

	CIniReader reader(config_file_name.c_str());

	// 0, 1, 2
	general.set_currency(reader.ReadInteger("general", "currency", 0));
	general.set_rebuy_amt(reader.ReadInteger("general", "rebuy_amt", 200));
	general.set_rebuy_mode(reader.ReadInteger("general", "rebuy_mode", 0));

	log.set_console(reader.ReadBoolean("log", "console", true));
	log.set_file(reader.ReadBoolean("log", "file", false));
	log.set_debug_bip(reader.ReadBoolean("log", "debug_bip", false));
	log.set_error_bip(reader.ReadBoolean("log", "error_bip", false));

	db.set_debug(reader.ReadBoolean("database", "debug", true));
	db.set_name(reader.ReadString("database", "name", "kasper_knn"));
	db.set_user(reader.ReadString("database", "user", "postgres"));
	db.set_pass(reader.ReadString("database", "pass", "UNKNOWN"));
	db.set_ip(reader.ReadString("database", "ip", "127.0.0.1"));
	db.set_port(reader.ReadInteger("database", "port", 5432));

	knn.set_debug(reader.ReadBoolean("knn", "debug", true));
	knn.set_istraining(reader.ReadBoolean("knn", "istraining", false));

	knn.set_num_neighbours(reader.ReadInteger("knn", "num_neighbours_preflop", 200), ePreflopIndex);
	knn.set_num_neighbours(reader.ReadInteger("knn", "num_neighbours_flop", 100), eFlopIndex);
	knn.set_num_neighbours(reader.ReadInteger("knn", "num_neighbours_turn", 50), eTurnIndex);
	knn.set_num_neighbours(reader.ReadInteger("knn", "num_neighbours_river", 25), eRiverIndex);

	knn.set_max_pts(reader.ReadInteger("knn", "max_pts", 100000));
	knn.set_eps(reader.ReadInteger("knn", "eps", 100000));

	// 0, 1, 2
	net.set_debug(reader.ReadBoolean("net", "debug", false));
	net.set_mode(reader.ReadInteger("net", "mode", 0));
	net.set_ip(reader.ReadString("net", "ip", "192.168.56.103"));
	net.set_port(reader.ReadInteger("net", "port", 7777));
}
