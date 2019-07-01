/***
 * @file sql5300.cpp
 * @authors Alicia Mauldin & Kin Chan
 * @see "Seattle University, cpsc4300/5300, Summer 2019"
 */

#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <cerrno>
#include "SQLParser.h"
#include "sqlhelper.h"
#include "db_cxx.h"

using namespace std;
using namespace hsql;

/**
 * Converts all characters in a given string to lowercase.
 * @param word A string of characters to be converted to lowercase
 * @return string of all lowercase characters
 */
string toLowercase(string word) {
  string result;
  for(unsigned i = 0; i < word.length(); i++) {
    result += tolower(word[i]);
  }
  return result;
}

/**
 * Main function of CPSC-5300 SQL string parser program
 */
int main(int argc, char* argv[]) {

  bool promptUser = true; // Flags if user wants to quit
  string query;           // Stores user query input


  // Check proper number of command arguments are provided
  if (argc != 2) {
    cerr << "Error: invalid command input.\nFormat is [sql5300: directory]\n";
    return 1;
  }

  // Print out confirmation of database environment
  char *dbenv = argv[1];
  DbEnv *myEnv = new DbEnv(0U);
  try {
    myEnv->open(dbenv, DB_CREATE | DB_INIT_MPOOL, 0);
  } catch (DbException &e) {
    cerr << "Error: unable to open the " << dbenv << " database environment\n";
    return 1;
  }
  
  cout << "(" << argv[0] << ": running with database environment at "
       << dbenv <<")" << endl;


  // Prompt user for SQL query input
  do {
    cout << "SQL>";
    getline(cin, query);

    // Continue prompting user if blank command given
    while (query.length() == 0) {
      cout << "SQL>";
      getline(cin, query);
    }

    // Exit if user enters quit command
    if (toLowercase(query) == "quit") {
      promptUser = false;
    } else {
      // Parse the query command
      SQLParserResult *result = SQLParser::parseSQLString(query);

      // Execute the query command if it is valid
      if (result->isValid()) {
        for (unsigned i = 0; i < result->size(); i++) {
          //FIXME build execute functionality
          cout << result->getStatement(i) << endl;
        }
      } else {
        cout << "Error invalid SQL: " << query << endl;
      }
    }
    
  } while (promptUser);

  return 0;
}
