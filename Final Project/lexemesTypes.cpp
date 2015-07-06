/*
 * lexemesTypes.cpp
 *
 * Created on: Oct 27, 2014
 *
 */

#include "lexemesTypes.h"
#include <iostream>
//tables


namespace ensc251{

  //***Complete the implementation of the following functions: ***

  //This function returns TRUE if the input is identifier else FALSE
  bool isIdentifier(const string &lexeme) 
  {
	  for (int i = 0; i < sizeof(tableOfKeywords) / sizeof(tableOfKeywords[0]); i++)
	  {
		  if (tableOfKeywords[i].compare(lexeme) == 0)
			  return false;
	  }
	  for (int i = 0; i < sizeof(booleanValues) / sizeof(booleanValues[0]); i++)
	  {
		  if (booleanValues[i].compare(lexeme) == 0)
			  return false;
	  }
	  if (lexeme[0] <= '9' && lexeme[0] >= '0') // starts with a number
		  return false;
	  else
	  {
		  for (int i = 0; i < lexeme.length(); i++)
		  if (!(lexeme[i] <= 'z' && lexeme[i] >= 'a') && !(lexeme[i] <= 'Z' && lexeme[i] >= 'A') && !(lexeme[i] <= '9' && lexeme[i] >= '0'))// not a letter or number
		  {
			  if (lexeme[i] != '_')// not _
				  return false;
		  }
	  }
	  return true;
  }

  //This function returns TRUE if the input is one of the operators defined in the variable
  //"tableOfOperators" in "lexemesTypes.h" else it returns FALSE
  bool isOperator(const string &lexeme)
  {
	  for (int i = 0; i < sizeof(tableOfOperators) / sizeof(tableOfOperators[0]); i++)
	  {
		  if (tableOfOperators[i].compare(lexeme) == 0)
			  return true;
	  }
	  return false;
  }

  //This function returns TRUE if the input is one of the punctuators defined in the variable
  //"tableOfPunctuators" in "lexemesTypes.h" else it returns FALSE
  bool isPunctuator(char lexeme)
  {
	  for (int i = 0; i < sizeof(tableOfPunctuators); i++)
		{
		  if (tableOfPunctuators[i] == lexeme)
				return true;
		}
		return false;
  }

  //This function returns TRUE if the input is one of the keywords defined in the variable
  //"tableOfKeywords" in "lexemesTypes.h" else it returns FALSE
  bool isKeyword(const string &lexeme) 
  {
	  for (int i = 0; i < sizeof(tableOfKeywords) / sizeof(tableOfKeywords[0]); i++)
	  {
		  if (tableOfKeywords[i].compare(lexeme) == 0)
			  return true;
	  }
	  return false;
  }

  //This function returns TRUE if the input is one of the boolean defined in the variable
  //"booleanValues" in "lexemesTypes.h" else it returns FALSE
  bool isBooleanValue(const string &lexeme)
  {
	  for (int i = 0; i < sizeof(booleanValues) / sizeof(booleanValues[0]); i++)
	  {
		  if (booleanValues[i].compare(lexeme) == 0)
			  return true;
	  }
	  return false;
  }

  //This function returns TRUE if the input is integer literal else FALSE
  bool isIntegerLiteral(const string &lexeme) 
  {
	  if (lexeme[0] <= '9' && lexeme[0] >= '0')
	  {
		  if (lexeme.find('.', 0) == -1)
			  return true;
	  }
	  return false;
  }

  //This function returns TRUE if the input is float literal else FALSE
  bool isFloatLiteral(const string &lexeme) 
  {
	  if (lexeme[0] <= '9' && lexeme[0] >= '0')
	  {
		  if (lexeme.find('.', 0) != -1)
			  return true;
	  }
	  return false;
  }

  //This function returns TRUE if the input is string literal else FALSE
  bool isStringLiteral(const string &lexeme)
  {
	  if (lexeme.substr(0, 1) == "\"")
		  return true;
	  return false;
  }
}


