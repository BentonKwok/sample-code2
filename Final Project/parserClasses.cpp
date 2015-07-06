//Use only the following libraries:
#include "parserClasses.h"
#include <string>
#include <iostream>

//Complete the implementation of the following member functions:
bool problem = false;
string delimiters = " .;<>#&()+-/:,%$[]^?~{}=*!|";//=*!|
string twoBitDel[] = { "&&", "||", "->", "++", "--", "<=", "::", ">=", "\"", "//", "!=", "+=", "%=", ".*", "-=", "/=", "*=", "&=", "|=", "^=", "/*", "*/", "\t", "<<", ">>", "=="}; //, "=="
string threeBitDel[] = { "<<=", ">>=", "=>*"};

//****TokenList class function definitions******

//Creates a new token for the string input, str
//Appends this new token to the TokenList
//On return from the function, it will be the last token in the list
void TokenList::append(const string &str) 
{
	Token * t = new Token(str);
	setTokenClass(t);
	if (tail == NULL && head == NULL)
	{
		head = t;
	}
	else if (head != NULL)
	{
		Token* temp = head;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = t;
		t->prev = temp;
	}
	tail = t;
}

//Appends the token to the TokenList if not null
//On return from the function, it will be the last token in the list
void TokenList::append(Token *token) 
{
	setTokenClass(token);
	if (token != NULL)
	{
		if (tail == NULL && head == NULL)
		{
			head = token;
		}
		else if (head != NULL)
		{
			Token* temp = head;
			while (temp->next != NULL)
				temp = temp->next;
			temp->next = token;
			token->prev = temp;
		}
		tail = token;
	}//if not null
	else// null
		cout << "You are appending no Token" << endl;
}

//Removes the token from the linked list if it is not null
//Deletes the token
//On return from function, head, tail and the prev and next Tokens (in relation to the provided token) may be modified.
void TokenList::deleteToken(Token *token)
{
	if (token != NULL)
	{
		Token* temp = token;
		if (temp->stringRep == token->stringRep) //found it!
		{
			if (temp == head)
			{
				head = head->next;
				head->prev = NULL;
				delete temp;
			}
			else if (temp == tail)
			{
				tail = tail->prev;
				tail->next = NULL;
				delete temp;
			}
			else
			{
				Token* before = temp->prev;
				Token* after = temp->next;
				before->next = after;
				after->prev = before;
				delete temp;
			}
		}//found it!
	}// token not null
}
//===================================================================================~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//===================================================================================~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Input: a pointer to a token
//Output: it won't return anything, but within function, it should set the token class (i.e. token->stringType)
//Note: one can invoke this function before adding token to the token list
void TokenList::setTokenClass(Token *token)
{
	string lexeme = token->stringRep;
	if (isIdentifier(lexeme))
		token->stringType = T_Identifier;				// T_Identifier
	else if (isOperator(lexeme))
		token->stringType = T_Operator;				//T_Operator
	 
	else if (isKeyword(lexeme))
		token->stringType = T_Keyword;				//T_Keyword
	else if (isBooleanValue(lexeme))
		token->stringType = T_Boolean;				//T_Boolean
	else if (isIntegerLiteral(lexeme))
		token->stringType = T_IntegerLiteral;				//T_IntegerLiteral
	else if (isFloatLiteral(lexeme))
		token->stringType = T_FloatLiteral;				//T_FloatLiteral
	else if (isStringLiteral(lexeme))
		token->stringType = T_StringLiteral;				//T_StringLiteral
	else
		token->stringType = T_Unknown;				//T_Unknown
	if (lexeme.length() == 1)
	{
		if (isPunctuator(lexeme[0]))
			token->stringType = T_Punctuator;			// T_Punctuator
	}

}

//===================================================================================~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//===================================================================================~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int TokenList::getSize()
{
	int num = 0;
	Token* temp = head;
	while (temp != NULL)
	{
		temp = temp->next;
		num++;
	}
	return num;
}

//****Tokenizer class function definitions******
//Sets the current string to be tokenized
//Resets all Tokenizer state variables
//Calls Tokenizer::prepareNextToken() as the last statement before returning.
void Tokenizer::setString(string *mystr)
{
	str = mystr;// the current String
	if (str->find("\t") != -1)
	{
		removeTab(mystr);
	}
	if (str->length() == 0)
		complete = true;
	else
		complete = false; //True if finished processing the current string
	
	processingInlineComment = false;
	processingIncludeStatement = false;
	offset = 0;
	tokenLength = 0;
	delSize = 0;
	prepareNextToken();
}
//****************************************************************************************************
//****************************************************************************************************
//****************************************************************************************************
//****************************************************************************************************
//Computes a new tokenLength for the next token
//Modifies: size_t tokenLength, and bool complete
//(Optionally): may modify offset
//Does NOT modify any other member variable of Tokenizer
void Tokenizer::prepareNextToken()
{
	if (offset > str->length() && complete != true)
	{
		complete = true;
	}
	if (offset == str->length())
		complete = true;
	if (!complete)
	{
		string fDel = find_first_delimiter(offset);
		int del = str->find(fDel, offset);
		tokenLength = del - offset;
	}
}
//****************************************************************************************************
//****************************************************************************************************

//Returns the next token. Hint: consider the substr function
//Updates the tokenizer state
//Updates offset, resets tokenLength, updates processingABC member variables
//Calls Tokenizer::prepareNextToken() as the last statement before returning.
string Tokenizer::getNextToken() 
{
	string newToken;
	if (tokenLength == 0)//delimeters
	{
		newToken = str->substr(offset, delSize);
		offset = offset + delSize;
	}
	else
	{
		newToken = str->substr(offset, tokenLength);
		offset = offset + tokenLength;
	}
	
	prepareNextToken();
	//Trim white space ~~~~~~##############################################################################
	if (newToken == " ")
	{
		return getNextToken();
	}// :D ~~~~~~~~~#######################################################################################
	return newToken;
}
//****************************************************************************************************
//****************************************************************************************************
//returns the first delimeter charactor in the string
string Tokenizer :: find_first_delimiter(int first)
{
	string firstDelimiter = "\n";
	if (processingBlockComment != true)
	{
		delSize = 1;
		for (int i = 0; i < delimiters.length(); i++) //one-bit delimiters
		{
			if ((str->find(delimiters.at(i), first) != -1) && str->find(delimiters.at(i), first) < str->find(firstDelimiter, first))
			{
				firstDelimiter = delimiters.at(i);
			}
		}//for -- one-bit delimiters
		//-----------------------------------------------------------------------------------------------
		for (int i = 0; i < sizeof(twoBitDel) / sizeof(twoBitDel[0]); i++) //two-bit delimiters
		{
			if ((str->find(twoBitDel[i], first) != -1) && str->find(twoBitDel[i], first) <= str->find(firstDelimiter, first))
			{
				firstDelimiter = twoBitDel[i];
				delSize = 2;
			}
		}//for -- two-bit delimiters
		//-----------------------------------------------------------------------------------------------
		for (int i = 0; i < sizeof(threeBitDel) / sizeof(threeBitDel[0]); i++) //three-bit delimiters
		{
			if ((str->find(threeBitDel[i], first) != -1) && str->find(threeBitDel[i], first) <= str->find(firstDelimiter, first))
			{
				firstDelimiter = threeBitDel[i];
				delSize = 3;
			}
		}//for -- three-bit delimiters
		if (processingIncludeStatement == true && firstDelimiter == "<")// include<>
		{
				delSize = str->find(">", first) - first + 1;
				processingIncludeStatement = false;
				return str->substr(first, delSize);
		}//if -- include<>
		if (processingInlineComment == true) // In line comments
		{
			delSize = str->length() - first;
			processingInlineComment = false;
			return str->substr(first, delSize);
		}//if -- inLineComments
	}
	else
	{
		if (str->find("*/", first) == -1) // not end of block comment
		{
			delSize = str->length() - first;
			return str->substr(first, delSize);
		}
		else//end of block comment
		{
			delSize = str->find("*/", first) - first;
			processingBlockComment = false;
			return str->substr(first, delSize);
		}
	}//else -- blockcomment true
	if (firstDelimiter == "#")// include statement
		processingIncludeStatement = true;
	//-----------------------------------------------------------------------------------------
	if (firstDelimiter == "/*")//block comment
		processingBlockComment = true;
	//-----------------------------------------------------------------------------------------
	if (firstDelimiter == "//")//inline comment
		processingInlineComment = true;
	//-----------------------------------------------------------------------------------------
	if (firstDelimiter == "\"")// Strings
	{
		int end = str->find("\"", first + 1);
		while (str->substr(end - 1, 1) == "\\")
			end = str->find("\"", end + 1);
		delSize = str->find("\"", end) - first + 1;
		return str->substr(first, delSize);
		
	}// if -- String
	//-----------------------------------------------------------------------------------------
	if (firstDelimiter == ".")//number?
	{
		int pos = str->find(firstDelimiter, first) + 1;
		if (pos < str->length()) // ! 1234.
		{
			char dotNext = str->at(pos);
			if (dotNext <= '9' && dotNext >= '0')
			{
				return find_first_delimiter(pos);
			}
			else if (!((dotNext <= 'z' && dotNext >= 'A') || string(1, dotNext) == "_"))
				return find_first_delimiter(pos);
		}
		else //1234.
		{
			delSize = str->length() - first + 1;
			return str->substr(first, delSize);
		}
	}// dot operator
	//----------------------------------------------------------------------------------------
	return firstDelimiter;
}

//removes tabs ("\t") in the string
string* Tokenizer:: removeTab(string* myStr)
{
	for (int i = 0; i < myStr->length(); i++)
	{
		if (myStr->at(i) == '\t')
		{
			myStr->replace(i, 1, " ");
		}
	}
	return myStr;
}