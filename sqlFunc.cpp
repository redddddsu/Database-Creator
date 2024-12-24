#include "sqlFunc.hpp"

void createDatabase() {
  string databaseNameInput;
  string directoryName;
  int numberCol;
  sqlite3 *db;
  string columnName;
  char *messageError;
  char intOrStr;

  cout << "Enter database name. (Please note that you cannot have whitespace)" << endl;
  cin >> databaseNameInput;
  cout << endl;
  directoryName = databaseNameInput + ".db";

  cout << "How many columns?" << endl;
  while (!(cin >> numberCol)) {
    cin.clear();
    cin.ignore();
    cout << "Invalid input. Please enter a number." << endl;
  }
  cout << endl;


  string createTable = "CREATE TABLE IF NOT EXISTS " + databaseNameInput + " (ID INTEGER PRIMARY KEY AUTOINCREMENT, ";

  for (int i = 0; i < numberCol; i++) {
    cout << "Enter the column's name" << endl;
    cin >> columnName;
    cout << endl;
    while (intOrStr != '1' && intOrStr != '2') {
      cout << "Press 1 for integar, press 2 for string." << endl;
      while (!(cin >> intOrStr)) {
        cin.clear();
        cin.ignore();
        cout << "Invalid input. Please enter 1 or 2." << endl;
      }
      cout << endl;
    }
    
    if (intOrStr == '1') {
      createTable += columnName + " INT NOT NULL";
    }
    else {
      createTable += columnName + " TEXT NOT NULL";
    }
    
    if (!(i == numberCol - 1)) {
      createTable += ", ";
    }
    intOrStr = 0;
  }
  createTable += ")";

  int createDB = sqlite3_open(directoryName.c_str(), &db);
  createDB = sqlite3_exec(db, createTable.c_str(), NULL, 0, &messageError);
  
  sqlite3_close(db);

}

void insertDB(const string &dbName, sqlite3_stmt *stmt, sqlite3 *db) {

  string sqlInsert = "INSERT INTO " + dbName + "(";
  char *messageError;
  
  string columnNames[sqlite3_column_count(stmt)];
  cout << endl << "(Please note that you cannot have whitespaces)" << endl;
  for (int i = 1; i < sqlite3_column_count(stmt); i++) {
    sqlInsert += sqlite3_column_name(stmt, i);
    cout << "Enter " << sqlite3_column_name(stmt, i) << "." << endl;
    getline(cin, columnNames[i]);
    cout << endl;
    if (!(i == sqlite3_column_count(stmt) -1)) {
      sqlInsert += ", ";
    }
  }  
  sqlInsert += ") VALUES (";

  for (int i = 1; i < sqlite3_column_count(stmt); i++) {
    sqlInsert += "'"; 
    sqlInsert += columnNames[i];
    sqlInsert += "'";
    if (!(i == sqlite3_column_count(stmt) -1)) {
      sqlInsert += ", ";
    }
  }
  sqlInsert += ")";

  sqlite3_exec(db, sqlInsert.c_str(), NULL, 0, &messageError);
}

void deleteRow(const string &dbName, sqlite3_stmt *stmt, sqlite3 *db) {
  char *messageError;
  string deleteID;
  cout << "Enter the ID" << endl;
  cin >> deleteID;


  string deleteSQL = "DELETE FROM " + dbName + " WHERE ID = " + deleteID;
  sqlite3_exec(db, deleteSQL.c_str(), NULL, 0, &messageError);
  
}

void updateRow(const string &dbName, sqlite3_stmt *stmt, sqlite3 *db) {
  char *messageError;
  string updateID;
  string whichColumn;
  string whatUpdate;

  cout << "Enter the ID you want to update" << endl;
  cin >> updateID;
  cout << endl;

  cout << "What column do you want to update?" << endl;
  cout << "(Enter the column name not the number)" << endl;
  for (int i = 1; i < sqlite3_column_count(stmt); i++) {
    cout << i << ". " << sqlite3_column_name(stmt, i) << endl;

  }
  cin >> whichColumn;
  cout << endl;

  cout << "What do you want to change it to?" << endl;
  cin >> whatUpdate;
  cout << endl;

  cout << updateID << " " << whichColumn << " " << whatUpdate << endl;

  string updateSQL = "UPDATE " + dbName + " SET " + whichColumn + "='" + whatUpdate + "' WHERE ID = '" + updateID + "'";
  sqlite3_exec(db, updateSQL.c_str(), NULL, 0 , &messageError);
}

int callback(void *NotUsed, int argc, char **argv, char** azColName) {
  for (int i = 0; i < argc; i++) {
    cout << azColName[i] << ": " << argv[i] << endl;
  }

  cout << endl;
  return 0;
}

void search(const string &dbName, sqlite3_stmt *stmt, sqlite3 *db) {
  char *messageError;
  string whichColumn;
  string searchWhat;

  cout << "What column do you want to update?" << endl;
  cout << "Which column do you want to search by?" << endl;
  for (int i = 1; i < sqlite3_column_count(stmt); i++) {
    cout << i << ". " << sqlite3_column_name(stmt, i) << endl;

  }
  cout << endl;

  cin >> whichColumn;
  cout << "What do you want search?" << endl;
  cin >> searchWhat;
  cout << endl;

  string searchSQL = "SELECT * FROM " + dbName + " WHERE " + whichColumn + " = '" + searchWhat + "'";
  sqlite3_exec(db, searchSQL.c_str(), callback , NULL, NULL);  
}

void editDatabase(const string &dbName) {
  sqlite3 *db;
  sqlite3_stmt *stmt;
  char *messageError;
  string dbDir = dbName + ".db";
  string userInput;

  string sqlSelect = "SELECT * FROM " + dbName;

  int openDB = sqlite3_open_v2(dbDir.c_str(), &db, SQLITE_OPEN_READWRITE, NULL);
  if (openDB) {
    throw runtime_error("This database does not exist");
  }

  sqlite3_prepare_v2(db, sqlSelect.c_str(), sqlSelect.length(), &stmt, nullptr);

  while (userInput != "q") {
    cout << "Press 1 to insert" << endl;
    cout << "Press 2 to delete" << endl;
    cout << "Press 3 to update" << endl;
    cout << "Press 4 to search" << endl;
    cout << "Press q to quit" << endl;
    cin >> userInput;

    if (userInput == "1") {
      cin.ignore();
      insertDB(dbName, stmt, db);
    }
    else if (userInput == "2") {
      deleteRow(dbName, stmt, db);
    }
    else if (userInput == "3") {
      updateRow(dbName, stmt, db);
    }
    else if (userInput == "4") {
      search(dbName, stmt, db);
    }
  } 

  sqlite3_finalize(stmt);
  sqlite3_close(db);

}