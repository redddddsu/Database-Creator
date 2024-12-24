#ifndef SQLFUNC_HPP
#define SQLFUNC_HPP

#include <iostream>
#include <string>
#include "sqlite3.h"

using namespace std;

void createDatabase();
void insertDB(const string &, sqlite3_stmt *, sqlite3 *);
void deleteDB(const string &, sqlite3_stmt *, sqlite3 *);
void updateRow(const string &, sqlite3_stmt *, sqlite3 *);
int callback(void *, int , char **, char** );
void search(const string &, sqlite3_stmt *, sqlite3 *);
void editDatabase(const string &);

#endif
