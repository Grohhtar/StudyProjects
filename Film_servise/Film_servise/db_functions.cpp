#include <stdio.h>
#include <iostream>
#include "sqlite3.h"
#include <string>
#include "db_functions.h"

sqlite3* db_open()
{
	sqlite3* db = 0;
	char* err = 0;
	sqlite3_open("FilmServiseData1.db", &db);
	sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS users(username TEXT PRIMARY KEY, password TEXT, userscore INT, if_admin INT); CREATE TABLE IF NOT EXISTS films(id INTEGER NOT NULL PRIMARY KEY , name TEXT UNIQUE, description TEXT, urating INT, genres TEXT);CREATE TABLE IF NOT EXISTS reviews(film_id INTEGER NOT NULL, text TEXT, grade INT, username TEXT, FOREIGN KEY (film_id) REFERENCES films(id));", 0, 0, &err);
	return db;
}



void dbadd_user(sqlite3* db, std::string uname, std::string password, int userscore, int if_admin)
{
	sqlite3_stmt* stmt;
	sqlite3_prepare(db, "INSERT INTO users (username, password, userscore, if_admin) VALUES (?, ?, ?, ?);", -1, &stmt, 0);
	sqlite3_bind_text(stmt, 1, uname.c_str(), -1, 0);
	sqlite3_bind_text(stmt, 2, password.c_str(), -1, 0);
	sqlite3_bind_int(stmt, 3, userscore);
	sqlite3_bind_int(stmt, 4, if_admin);
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);
}

sqlite3_stmt* dbget_user(sqlite3* db, std::string uname, std::string password)
{
	sqlite3_stmt* stmt;
	int err;
	sqlite3_prepare_v2(db, "SELECT * FROM users WHERE username=? AND password=?", -1, &stmt, 0);
	sqlite3_bind_text(stmt, 1, uname.c_str(), -1, 0);
	sqlite3_bind_text(stmt, 2, password.c_str(), -1, 0);
	err=sqlite3_step(stmt);

	
	return stmt;
}
	
sqlite3_stmt* dbget_user_name(sqlite3* db, std::string uname)
{
	sqlite3_stmt* stmt;
	int err;
	sqlite3_prepare_v2(db, "SELECT * FROM users WHERE username=?", -1, &stmt, 0);
	sqlite3_bind_text(stmt, 1, uname.c_str(), -1, 0);
	err = sqlite3_step(stmt);
	if (err == 101)
	{
		throw(err);
	}


	return stmt;
}

void dbchange_user_rating(sqlite3* db, std::string uname, int score)
{
	sqlite3_stmt* stmt;
	int err;
	err = sqlite3_prepare_v2(db, "UPDATE users SET userscore=? WHERE username=?", -1, &stmt, 0);
	sqlite3_bind_int(stmt, 1, score);
	sqlite3_bind_text(stmt, 2, uname.c_str(), -1, 0);
	sqlite3_step(stmt);	
	sqlite3_finalize(stmt);
}

void dbchange_film_rating(sqlite3* db, int id, int score)
{
	sqlite3_stmt* stmt;
	int err;
	err = sqlite3_prepare_v2(db, "UPDATE films SET urating=? WHERE id=?", -1, &stmt, 0);
	sqlite3_bind_int(stmt, 1, score);
	sqlite3_bind_int(stmt, 2, id);
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);
}

void dbadd_film(sqlite3* db, std::string name, std::string description, int urating, std::string genres)
{

	sqlite3_stmt* stmt;
	int err;
	sqlite3_prepare(db, "INSERT INTO films (name, description, urating, genres) VALUES (?, ?, ?, ?);", -1, &stmt, 0);
	
	sqlite3_bind_text(stmt, 1, name.c_str(), -1, 0);
	sqlite3_bind_text(stmt, 2, description.c_str(), -1, 0);
	sqlite3_bind_int(stmt, 3, urating);	
	sqlite3_bind_text(stmt, 4, genres.c_str(), -1, 0);	
	err = sqlite3_step(stmt);

	sqlite3_finalize(stmt);

}

sqlite3_stmt* dbget_film_name(sqlite3* db, std::string name)
{
	int err;
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(db, "SELECT * FROM films WHERE name=?", -1, &stmt, 0);
	
	sqlite3_bind_text(stmt, 1, name.c_str(), -1, 0);
	
	err=sqlite3_step(stmt);
	if (err == 101)
	{
		throw(err);
	}
	
	return stmt;
}

sqlite3_stmt* dbget_allfilms(sqlite3* db)
{
	sqlite3_stmt* stmt;
	int err;
	sqlite3_prepare_v2(db, "SELECT * FROM films", -1, &stmt, 0);
	err=sqlite3_step(stmt);
	if (err == 101)
	{
		throw(err);
	}
	return stmt;
}

sqlite3_stmt* dbget_film_number(sqlite3* db, int num)
{
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(db, "SELECT * FROM films WHERE id=?", -1, &stmt, 0);
	sqlite3_bind_int(stmt, 1, num);
	sqlite3_step(stmt);
	return stmt;
}

sqlite3_stmt* dbget_film_genre(sqlite3* db, std::string genre)
{
	sqlite3_stmt* stmt;
	int err;
	err=sqlite3_prepare_v2(db, "SELECT * FROM films WHERE genres=?", -1, &stmt, 0);

	sqlite3_bind_text(stmt, 1, genre.c_str(), -1, 0);
	err=sqlite3_step(stmt);
	if (err == 101)
	{
		throw(err);
	}

	return stmt;
}

int dbfilm_get_id(sqlite3* db, std::string name)
{
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(db, "SELECT id FROM films WHERE name=?", -1, &stmt, 0);
	sqlite3_bind_text(stmt, 1, name.c_str(), -1, 0);
	sqlite3_step(stmt);
	int id = sqlite3_column_int(stmt, 0);
	sqlite3_finalize(stmt);
	return id;
}

void db_delete_film(sqlite3* db, std::string name)
{
	sqlite3_stmt* stmt;
	int err;
	sqlite3_prepare_v2(db, "DELETE FROM films WHERE name=?", -1, &stmt, 0);
	sqlite3_bind_text(stmt, 1, name.c_str(), -1, 0);
	
	err = sqlite3_step(stmt);
}

void dbadd_review(sqlite3* db, int filmid, std::string rtext, int grade, std::string uname)
{
	sqlite3_stmt* stmt;
	int err;

	sqlite3_prepare(db, "INSERT INTO reviews (film_id, text, grade, username) VALUES (?, ?, ?, ?);", -1, &stmt, 0);


	sqlite3_bind_int(stmt, 1, filmid);
	sqlite3_bind_text(stmt, 2, rtext.c_str(), -1, 0);
	sqlite3_bind_int(stmt, 3, grade);

	sqlite3_bind_text(stmt, 4, uname.c_str(), -1, 0);
	err=sqlite3_step(stmt);

	sqlite3_finalize(stmt);

}

sqlite3_stmt* dbget_review_id(sqlite3* db, int id)
{
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(db, "SELECT * FROM reviews WHERE film_id=?", -1, &stmt, 0);
	sqlite3_bind_int(stmt, 1, id);
	sqlite3_step(stmt);
	return stmt;
}