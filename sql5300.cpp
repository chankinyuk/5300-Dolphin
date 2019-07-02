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
 * Helper function to assist in parsing AST columns to string
 * @param col AST column to parse
 * @return a string of the SQL table column(s)
 */
string columnToString(const ColumnDefinition *col) {
  string columnList(col->name);
  if (col->type == ColumnDefinition::DOUBLE) {
    columnList += " DOUBLE";
  } else if (col->type == ColumnDefinition::TEXT) {
    columnList += " TEXT";
  } else if (col->type == ColumnDefinition::INT) {
    columnList += " INT";
  } else {
    columnList += "...";
  }
  return columnList;
}

/**
 * Helper function to interpret create SQL command from AST
 * @param stmt CreateStatement AST to be executed
 * @return a string of the create SQL query
 */
string executeCreate(const CreateStatement *stmt) {
  string createResult = "CREATE TABLE ";

  // If AST type is not a table, return empty
  if (stmt->type != CreateStatement::kTable) {
    return createResult += "...";
  }

  // Append IF NOT EXISTS flag if present in AST
  if (stmt->ifNotExists) {
    createResult += "IF NOT EXISTS ";
  }

  // Append column(s) to return
  createResult += string(stmt->tableName) + " (";
  bool comma = false;
  for(ColumnDefinition *col : *stmt->columns) {
    if (comma) {
      createResult += ", ";
    }
    createResult += columnToString(col);
    comma = true;
  }
  createResult += ")";
  return createResult;
}

/**
 * Interprets create and select SQL statements into string text
 * @param stmt SQLParserResult AST to be executed
 * @return a string of the SQL query
 */
string execute(const SQLStatement *stmt) {
  if (stmt->type() == kStmtSelect) {
    return "select test working";
  } else if (stmt->type() == kStmtCreate) {  
    return executeCreate((const CreateStatement*) stmt);
  } else {
    return "Command not yet implemented";
  }
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
    cerr << e.what() << endl;
    return 1;
  } catch (exception &e) {
    cerr << "Error: unable to open the " << dbenv << " database environment\n";
    cerr << e.what() << endl;
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
          //FIXME finish building execute functionality
          cout << execute(result->getStatement(i)) << endl;
        }
      } else {
        cout << "Error invalid SQL: " << query << endl;
      }
    }
    
  } while (promptUser);

  return 0;
}
