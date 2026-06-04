#pragma once 

#include <sqlite3.h>

#include "Config.hpp"
#include "FileExplorer.hpp"

class DataBase
{
public:
	DataBase() = default;
	//{

		//m_PathToDB = m_FileEplr.openExplorer(Config::savesDir.string());
		//m_Exit = sqlite3_open(m_PathToDB.c_str(), &DB);
		//if (m_Exit != SQLITE_OK)
		//{
		//	std::cout <<  "FATAL ERROR " << sqlite3_errmsg(DB) << "\n";
		//}
		//else
		//{
		//	std::cout << "Success opened the file at " << m_PathToDB << "\n";
		//}
	//}
	~DataBase()
	{
		sqlite3_close(m_DB);
	}


	void fillTable()
	{

	}


	void createTable()
	{
		sqlite3_stmt *SQLstmt;
		std::string SQLcmd("create table if not exists TILES("
			"id int primary key AUTOINCREMENT"
			"scRecX int,"
			"scRecY int,"
			"scRecW int,"
			"scRecH int,"
			"SpriteName text"
			");");

		m_Exit = sqlite3_prepare_v2(m_DB, SQLcmd.c_str(), static_cast<int>(SQLcmd.length()), &SQLstmt, nullptr);
		sqlite3_step(SQLstmt);

		if (m_Exit != SQLITE_OK)
		{
			std::cout << "\n" << sqlite3_errmsg(m_DB) << "\n";
		}
		else
		{
			std::cout << "\nTable created!\n";
		}

		sqlite3_finalize(SQLstmt);
	}

private:
	sqlite3* m_DB;

	std::string m_PathToDB;

	int m_Exit = 0;

	FileExplorer m_FileEplr;


};
