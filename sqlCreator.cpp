#include <iostream>
#include "sqlite3.h"
#include <string>

using namespace std;

#include "sqlFunc.hpp"


int main() {
    string databaseName;
    string userInput;
    while (userInput != "q") {
      cout << "Press 1 to create a new database" << endl;
      cout << "Press 2 to edit/search a database" << endl;
      cout << "Press q to quit" << endl;
      cin >> userInput;
      
      if (userInput == "1") {
        createDatabase();
      }
      else if (userInput == "2") {
        cout << "Enter database name" << endl;
        cin >> databaseName;
        cout << endl;     
        editDatabase(databaseName);   
      }
    }
  
    return 0;
}