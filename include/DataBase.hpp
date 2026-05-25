#pragma once 


#include <sqlite3.h>

#include <Config.hpp>

class DataBase
{
public:
	DataBase()
	{
		exit = sqlite3_open(std::filesystem::path(Config::savesDir).append("test.db").string().c_str(), &DB);
		


	}

private:


	static sqlite3* DB;
	int exit = 0;
};
