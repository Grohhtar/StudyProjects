#ifndef DB_FUNCTIONS_H
#define DB_FUNCTIONS_H

sqlite3* db_open();

void dbadd_user(sqlite3* db, std::string uname, std::string password, int userscore, int if_admin);

sqlite3_stmt* dbget_user(sqlite3* db, std::string uname, std::string password);

void dbadd_film(sqlite3* db, std::string name, std::string description, int urating, std::string genres);


sqlite3_stmt* dbget_film_name(sqlite3* db, std::string name);

sqlite3_stmt* dbget_allfilms(sqlite3* db);

sqlite3_stmt* dbget_film_number(sqlite3* db, int num);

void db_delete_film(sqlite3* db, std::string name);

sqlite3_stmt* dbget_film_genre(sqlite3* db, std::string genre);

void dbadd_review(sqlite3* db, int filmid, std::string rtext, int grade, std::string uname);

int dbfilm_get_id(sqlite3* db, std::string name);

sqlite3_stmt* dbget_review_id(sqlite3* db, int id);

sqlite3_stmt* dbget_user_name(sqlite3* db, std::string uname);

void dbchange_user_rating(sqlite3* db, std::string uname, int score);

void dbchange_film_rating(sqlite3* db, int id, int score);
#endif