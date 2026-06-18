#pragma once

#include <iostream>
#include <string>
#include <format>
#include <print>
#include <ranges>

#include "sqlite3.h"

#include "ConfigCore.hpp"
#include "SaveData.hpp"

class DataBaseManager
{
public:
	DataBaseManager() = default;

	//I dont want this being copied around
	DataBaseManager(const DataBaseManager&) = delete;
	DataBaseManager& operator=(const DataBaseManager&) = delete;

	~DataBaseManager()
	{
		if (m_DB != nullptr)
		{
			sqlite3_close(m_DB);
		}
	}

	void ConnectToDB(const std::string& PathToSave)
	{
		if (m_DB != nullptr)
		{
			sqlite3_close(m_DB);
		}

		int exit = sqlite3_open(PathToSave.c_str(), &m_DB);
		if (exit != SQLITE_OK)
		{
			std::println("Error At connection {}", sqlite3_errmsg(m_DB));
		}
		else
		{
			std::println("everything OK! at ConnectToDB {}",PathToSave);
			m_PathToSave = PathToSave;
		}
	}

	void SetupNewDB(int gridW, int gridH)
	{
		m_TableSize = gridW * gridH;
		
		createTable();

		saveMetadata(gridW, gridH);

		fillTable();
	}


	void SaveGameState(std::span<Core::TileSaveData> tiles)
	{
		std::cout << "Saving game state...." << "\n";
		sqlite3_exec(m_DB, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

		const char* sql = "UPDATE TILES SET spriteName = ?, scRecXpos = ?, scRecYpos = ? WHERE id = ?;";
		sqlite3_stmt* stmt = nullptr;

		int rc = sqlite3_prepare_v2(m_DB, sql, -1, &stmt, nullptr);
		if (rc != SQLITE_OK)
		{
			std::println("Error preparing statement at SaveTiles: {}", sqlite3_errmsg(m_DB));
			sqlite3_exec(m_DB, "ROLLBACK;", nullptr, nullptr, nullptr);
			return;
		}

		for (auto const [index, tile] : std::views::enumerate(tiles))
		{
			
			sqlite3_bind_text(stmt, 1, tile.spriteName.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(stmt, 2, tile.srcX);
			sqlite3_bind_int(stmt, 3, tile.srcY);
			sqlite3_bind_int(stmt, 4, static_cast<int>(index));

			if (sqlite3_step(stmt) != SQLITE_DONE)
			{
				std::cout << "Error saving tile:" << sqlite3_errmsg(m_DB) << "\n";
			}

			sqlite3_reset(stmt);
		}

		sqlite3_exec(m_DB, "COMMIT;", nullptr, nullptr, nullptr);
		sqlite3_finalize(stmt);
		std::cout << "Game state saved" << "\n";

	}

	Core::SaveData LoadGameSave()
	{
		int gridW = 0;
		int gridH = 0;
		LoadMetaData(gridW, gridH);

		std::vector<Core::TileSaveData> tileDataVec;
		tileDataVec.reserve(gridW * gridH);
		LoadTileData(tileDataVec);


		return Core::SaveData{
			{gridW,gridH},
			tileDataVec
		};
	}

private:
	void createTable()
	{
		std::string SQLcmd(
			"CREATE TABLE IF NOT EXISTS TILES("
			"id int primary key,               "
			"scRecXpos int NOT NULL,                "
			"scRecYpos int NOT NULL,"
			"spriteName text NOT NULL);       "

			"CREATE TABLE IF NOT EXISTS METADATA("
			"id int primary key,               "
			"gridW int NOT NULL,          "
			"gridH int NOT NULL);        "
		);

		int exit = sqlite3_exec(m_DB, SQLcmd.c_str(), nullptr, nullptr, nullptr);

		if (exit != SQLITE_OK)
		{
			std::println("Error At createTable {}", sqlite3_errmsg(m_DB));
		}
		else
		{
			std::println("Func create table successfull");
		}

	}

	void fillTable()
	{
		sqlite3_exec(m_DB, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

		const char* sql = "INSERT INTO TILES (id,scRecXpos,scRecYpos, spriteName) VALUES (?,?,?,?);";
		sqlite3_stmt* stmt = nullptr;

		int exit = sqlite3_prepare_v2(m_DB, sql, -1, &stmt, nullptr);
		if (exit != SQLITE_OK)
		{
			std::println("Error preparing statement: {}", sqlite3_errmsg(m_DB));
			return;
		}

		const std::string defaultSprite = "GrassComplete";

		for (int i = 0; i < m_TableSize; ++i)
		{
			sqlite3_bind_int(stmt, 1, i);
			sqlite3_bind_int(stmt, 2, 0);
			sqlite3_bind_int(stmt, 3, 0);
			sqlite3_bind_text(stmt, 4, defaultSprite.c_str(), -1, SQLITE_TRANSIENT);

			int exit = sqlite3_step(stmt);
			if (exit != SQLITE_DONE)
			{
				std::println("Error inserting tile ID {}: {}", i, sqlite3_errmsg(m_DB));
			}

			sqlite3_reset(stmt);
			sqlite3_clear_bindings(stmt);
		}

		sqlite3_finalize(stmt);

		sqlite3_exec(m_DB, "COMMIT;", nullptr, nullptr, nullptr);

		std::println("filled database with {} empty tiles.", m_TableSize);
	}

	void saveMetadata(int gridW, int gridH)
	{
		std::string sql = std::format("INSERT INTO METADATA (id, gridW, gridH) VALUES (1, {}, {});", gridW, gridH);

		int rc = sqlite3_exec(m_DB, sql.c_str(), nullptr, nullptr, nullptr);

		if (rc != SQLITE_OK)
		{
			std::println("Error saving metadata: {}", sqlite3_errmsg(m_DB));
		}
	}


	void SaveTiles(std::span<Core::TileSaveData> tiles)
	{
		sqlite3_exec(m_DB, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

		const char* sql = "UPDATE TILES SET spriteName = ?, scRecXpos = ?, scRecYpos = ?, WHERE id = ?;";
		sqlite3_stmt* stmt = nullptr;

		int rc = sqlite3_prepare_v2(m_DB, sql, -1, &stmt, nullptr);
		if (rc != SQLITE_OK)
		{
			std::println("Error preparing statement at SaveTiles: {}", sqlite3_errmsg(m_DB));
			sqlite3_exec(m_DB, "ROLLBACK;", nullptr, nullptr, nullptr);
			return; 
		}

		for (auto const [index, tile] : std::views::enumerate(tiles))
		{
			std::cout << tile.spriteName << " : " << tile.srcX << " : " << tile.srcY << "\n";

			sqlite3_bind_text(stmt, 1, tile.spriteName.c_str(), -1, SQLITE_TRANSIENT);
			sqlite3_bind_int(stmt, 2, tile.srcX);
			sqlite3_bind_int(stmt, 3, tile.srcY);
			sqlite3_bind_int(stmt, 4, static_cast<int>(index));

			if (sqlite3_step(stmt) != SQLITE_DONE)
			{
				std::cout << "Error saving tile:" << sqlite3_errmsg(m_DB) << "\n";
			}

			sqlite3_reset(stmt);
		}

		sqlite3_exec(m_DB, "COMMIT;", nullptr, nullptr, nullptr);
		sqlite3_finalize(stmt);


	}

	void LoadMetaData(int& gridW, int& gridH)
	{
		sqlite3_exec(m_DB, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

		std::print("Loading MetaData");

		const char* sql = "SELECT * FROM METADATA;";
		sqlite3_stmt* stmt = nullptr;

		int rc = sqlite3_prepare_v2(m_DB, sql, -1, &stmt, nullptr);
		if (rc != SQLITE_OK)
		{
			std::println("Error preparing statement at LoadGameSave: {}", sqlite3_errmsg(m_DB));
			sqlite3_exec(m_DB, "ROLLBACK;", nullptr, nullptr, nullptr);
			return;
		}

		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			gridW = sqlite3_column_int(stmt, 1);
			gridH = sqlite3_column_int(stmt, 2);
		}

		std::println("\nThe metadata vals are {} : {} ", gridW, gridH);

		sqlite3_exec(m_DB, "COMMIT;", nullptr, nullptr, nullptr);
		sqlite3_finalize(stmt);
	}


	void LoadTileData(std::vector<Core::TileSaveData>& tileDataVec)
	{
		sqlite3_exec(m_DB, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

		const char* sql = "SELECT spriteName,scRecXpos,scRecYpos FROM TILES;";
		sqlite3_stmt* stmt = nullptr;

		std::print("Loading Tiles");

		int rc = sqlite3_prepare_v2(m_DB, sql, -1, &stmt, nullptr);
		if (rc != SQLITE_OK)
		{
			std::println("Error preparing statement at LoadTileData: {}", sqlite3_errmsg(m_DB));
			sqlite3_exec(m_DB, "ROLLBACK;", nullptr, nullptr, nullptr);
			return;
		}

		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			tileDataVec.emplace_back(Core::TileSaveData
				{
					reinterpret_cast<const char*>(sqlite3_column_text(stmt,0)), //column text gives back const unsigned char * :(, which std::string does not accept
					sqlite3_column_int(stmt,1),
					sqlite3_column_int(stmt,2)
				});
		}

		sqlite3_exec(m_DB, "COMMIT;", nullptr, nullptr, nullptr);
		sqlite3_finalize(stmt);

		std::print("Tiles loaded!");
	}

private:
	int m_TableSize = 0;

	int m_GridW = 0;
	int m_GridH = 0;
	
	sqlite3* m_DB = nullptr;
	std::string m_PathToSave;


};
