//This is a project of Simon University class ENSC 251 2014 written by Fatemeh Darbehani & Benton Kwok
//This code is a basic parser that takes a cpp/txt file and tokenize it.
//This code outputs all the function declarations on screen
//And return number of tokens (smallest number of consecutive
//characters that can be separated by whitespace without changing the functionality of the code) of different types



#include "parserClasses.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lexemesTypes.h"
#include <stack>

using namespace std;

//Removes all inline comments from the tokenList including the // marker
//Returns the number of comments removed
int removeInlineComments(TokenList &tokenList)
{
	int deletNum = 0;
	Token* temp = tokenList.getFirst();
	while (temp != NULL && temp->getNext() != NULL)
	{
		if (temp->getStringRep() == "//")//start deleting of a comment
		{
			Token* del = temp;
			temp = temp->getNext();
			tokenList.deleteToken(del);
			if (temp->getStringRep() != "\n")
			{
				Token* del = temp;
				temp = temp->getNext();
				tokenList.deleteToken(del);
			}// one ine comment was completely deleted
			if (temp->getStringRep() == "\n")//end of the line
			{
				Token* del = temp;
				temp = temp->getNext();
				tokenList.deleteToken(del);
				deletNum++;
			}
		}//if -- Whole comment
		else
			temp = temp->getNext();
	}//while -- travse the list
	return deletNum;
}

//Remove all block comments from the tokenList including /* and */ markers
//Returns the number of block comments removed (where each pair of /* and */ count as one comment)
int removeBlockComments(TokenList &tokenList)
{
	int deletNum = 0;
	Token* temp = tokenList.getFirst();
	while (temp != NULL && temp->getNext() != NULL)
	{
		if (temp->getStringRep() == "/*")//start of a blobk comment
		{
			Token* del = temp;
			temp = temp->getNext();
			//cout << "removing   " << del->getStringRep() << endl;
			tokenList.deleteToken(del);
			while (temp->getStringRep() != "*/")
			{
				Token* del = temp;
				temp = temp->getNext();
				//cout << "removing   " << del->getStringRep() << endl;
				tokenList.deleteToken(del);
			}
			if (temp->getStringRep() == "*/")
			{
				Token* del = temp;
				temp = temp->getNext();
				//cout << "removing   " << del->getStringRep() << endl;
				tokenList.deleteToken(del);
			}// one block comment was deleted
			if (temp->getStringRep() == "\n")//end of the line
			{
				Token* del = temp;
				temp = temp->getNext();
				//cout << "removing   " << del->getStringRep() << endl;
				tokenList.deleteToken(del);
				deletNum++;
			}
		}//if -- Whole block comment
		else // if (temp->getStringRep() != "/*")
			temp = temp->getNext();
	}//while -- travse the list
	return deletNum;
}

//Input: a list of tokens
//Output: head pointer to the list of assignment statements
//NOTE: Assignment statement must end with a semi-colon
//@ description: extract all the assignment statements from input token list, prepare a new token list (assignment list)
//using extracted statements and return the head pointer to it
bool isUserDefinedDataType(TokenList &userType, Token* token)
{
	Token* type = userType.getFirst();
	while (type)
	{
		if (token->getStringRep() == type->getStringRep())
			return true;
		else
			type = type->getNext();
	}
	return false;
}
Token* getAssignmentStatements(TokenList &tokenList, TokenList &assignmentList, TokenList &assignmentTokenList)
{
	Token* token = tokenList.getFirst();
	string assignment = "";
	while (token)
	{
		if (token->getStringRep() == "=" || token->getStringRep() == "+=" || token->getStringRep() == "-=" || token->getStringRep() == "*=" || token->getStringRep() == "/=" || token->getStringRep() == "%=") // assignment operator
		{
			Token* temp = token;
			while ((temp != tokenList.getFirst()) && (temp->getPrev()->getStringRep() != "{") && (temp->getPrev()->getStringRep() != "}") && (temp->getPrev()->getStringRep() != ";") && (temp->getPrev()->getStringType() != 3) && (temp->getPrev()->getStringRep() != "\n")) //find the beginong of the assignment expression
				temp = temp->getPrev();
			while (temp->getStringRep() != ";")
			{
				assignment += temp->getStringRep() + " "; // appned the expresion except  for ";"
				assignmentTokenList.append(temp->getStringRep());
				temp = temp->getNext();
			}
			assignment += temp->getStringRep();// adding ";"
			assignmentList.append(assignment);
			assignmentTokenList.append(temp->getStringRep());
			token = temp->getNext();
			assignment = "";
		}
		else
			token = token->getNext();
	}
	return assignmentList.getFirst();
}

Token* getFunctionDeclaration(TokenList &tokenList, TokenList userDefinedNames, TokenList &functionList, TokenList &functionTokenList)
{
	Token* token = tokenList.getFirst();
	string function = "";	//an empty string to store the line of the function declaration
	while (token)
	{

		if ((token != tokenList.getFirst())&& token->getStringRep() == "(" && (token->getNext()->getStringType() == 3 || isUserDefinedDataType(userDefinedNames, token->getNext())))
		{
			Token* temp = token;	//token that is currently pointing at the "("
			while (temp->getPrev()->getStringRep() != ";" && temp->getPrev()->getStringRep() != "/n" && temp->getPrev()->getStringRep() != ":" && temp->getPrev()->getStringRep() != "{" && temp->getPrev()->getStringRep() != "}")
				temp = temp->getPrev();
			while (temp->getStringRep() != ";")	//traversing to the end
			{
				function += temp->getStringRep() + " ";	//adding the whole line together as a string
				functionTokenList.append(temp->getStringRep());
				temp = temp->getNext();
			}
			function += temp->getStringRep();// adding ";"
			functionList.append(function);
			functionTokenList.append(temp->getStringRep());
			token = temp->getNext();
			function = "";
		}
		else if (token->getStringRep() == "(" && token->getNext()->getStringRep() == ")") // Functions with no parameters eg: void foo();
		{
			Token* nameOfFunction = token->getPrev();
			if (nameOfFunction->getStringType() == 0)// valid identifier
			if (nameOfFunction->getPrev()->getStringType() == 3 || isUserDefinedDataType(userDefinedNames, nameOfFunction->getPrev()))// valid return type
			{
				Token* start = token;
				while ((start != tokenList.getFirst()) && start->getPrev()->getStringRep() != ";" && start->getPrev()->getStringRep() != "/n" && start->getPrev()->getStringRep() != ":" && start->getPrev()->getStringRep() != "{" && start->getPrev()->getStringRep() != "}")// head of the list?!
					start = start->getPrev();
				while (start->getStringRep() != ";")  //traversing to the end
				{
					function += start->getStringRep() + " ";    //adding the whole line together as a string
					functionTokenList.append(start->getStringRep());
					start = start->getNext();
				}
				function += start->getStringRep();// adding ";"
				functionList.append(function);
				functionTokenList.append(start->getStringRep());
				token = start->getNext();
				function = "";
			}
		}
		token = token->getNext();
	}
	return functionList.getFirst();
}


//Input: a list of tokens
//Output: head pointer to the list of user defined data types (struct, class, typedef)
//@ description: extract all user defined data types from input token list and prepare a new token list
//Return the head pointer to the user defined type (usertDefT)
TokenList getUserDefinedTypes(TokenList &tokenList)
{
	Token* current = tokenList.getFirst();
	TokenList userDefinedTypes;
	while (current != tokenList.getLast()) //traversing the input list
	{
		if (current->getStringRep() == "struct" || current->getStringRep() == "class" || current->getStringRep() == "typedef")
			userDefinedTypes.append(current->getNext()->getStringRep());
		current = current->getNext();
	}

	return userDefinedTypes;
}
//Part 3, Error Detection & Analysis
//Compound data type Variable to store each variable's type and name
struct Variable
{
	string name;
	string type;
};
//input: a list of assignment statements and a head pointer to the list of tokens
//It finds the type of each variable from the list of tokens and stores it into a 2D array of string
//returns a pointer to a 2D array of string that keeps all the variables and their types
vector<Variable> getTableOfVariables(const TokenList& assignmentList, Token* tokens, TokenList userDefinedNames)
{
	vector <Variable> tableOfVariables;
	Token* current = tokens;
	string name;
	string type;
	while (current)
	{
		if (current->getStringType() == 3)//keyword
		{
			if (current->getStringRep() != "int" && current->getStringRep() != "float" && current->getStringRep() != "char" && current->getStringRep() != "bool" && current->getStringRep() != "double" && current->getStringRep() != "long" && current->getStringRep() != "short" && current->getStringRep() != "signed" && current->getStringRep() != "unsigned")
			{
				current = current->getNext();
				continue;
			}
			else
			{
				if (current->getNext()->getStringRep() == "*")// int * a;
				{
					name = current->getNext()->getNext()->getStringRep();
					type = current->getStringRep() + "*";
				}
				else if (current->getNext()->getNext()->getStringRep() == "[")//int a[5];
				{
					name = current->getNext()->getStringRep();
					type = current->getStringRep() + "*";
				}
				else // int a;
				{
					name = current->getNext()->getStringRep();
					type = current->getStringRep();
				}
				//-------------------------------------------------
				// Creating a new Variable to add it to the vector
				//-------------------------------------------------
				Variable vari;
				vari.name = name;
				vari.type = type;
				tableOfVariables.push_back(vari);
				//-------------------------------------------------

			}
		}
		else if (isUserDefinedDataType(userDefinedNames, current))// User defined data types
		{
			if (current->getPrev()->getStringRep() == "class" || current->getPrev()->getStringRep() == "struct" || current->getPrev()->getStringRep() == "enum")
			{
				current = current->getNext();
				continue;
			}
			else
			{
				if (current->getNext()->getStringRep() == "*")// int * a;
				{
					name = current->getNext()->getNext()->getStringRep();
					type = current->getStringRep() + "*";
				}
				else if (current->getNext()->getNext()->getStringRep() == "[")//int a[5];
				{
					name = current->getNext()->getStringRep();
					type = current->getStringRep() + "*";
				}
				else // int a;
				{
					name = current->getNext()->getStringRep();
					type = current->getStringRep();
				}
				//-------------------------------------------------
				// Creating a new Variable to add it to the vector
				//-------------------------------------------------
				Variable vari;
				vari.name = name;
				vari.type = type;
				tableOfVariables.push_back(vari);
				//-------------------------------------------------
			}
		}// else // User defined data types
		current = current->getNext();
	}
	//removing any duplication of a variable. i.e. multiple occurence of one variable
	if (!tableOfVariables.empty())
	{
		for (int i = 0; i < tableOfVariables.size(); i++)
		{
			for (int j = i + 1; j < tableOfVariables.size() ; j++)
			if (tableOfVariables[j].name == tableOfVariables[i].name)
			{
				tableOfVariables.erase(tableOfVariables.begin() + j);
				i = 0;// the indecis are changing
			}
		}
	}
	return tableOfVariables;
}


//=====================================================================================================
// This function unputs a string and a table (vector) of user defined data types
// Splits the string into tokens and keeps the tokens of types KEYWORD, IDENTIFIER, FLOAT, and INT
// Finds each token's type and stroes them in a vector
// In the end, if there is more than one item in the vector, it compares the items 
// These items should all be the same otherwise there is an umatch type
// The function prints out error message in that case and outputs 0
// if there is one item in the vector, i.e. expression only has one token
// it outputs type of the token
//======================================================================================================
string type(TokenList tokens, vector<Variable> table)
{
	//--------------------------------------------------------------------------------
	// Removing any token that shows scope [since there is no multiple use of names]
	//--------------------------------------------------------------------------------
	Token* del = tokens.getFirst();
	while (del)
	{
		if (del->getStringRep() == ".") // a.b + c
		{
			tokens.deleteToken(del->getPrev()); //.b + c
			del = del->getNext();
			tokens.deleteToken(del->getPrev()); // b + c
		}
		del = del->getNext();
	}
	//--------------------------------------------------------------------------------
	Token* temp = tokens.getFirst();
	vector<string> setOfTypes;
	while (temp)
	{
		if (temp->getStringType() == 0)//identifiers only
		{
			for (int i = 0; i < table.size(); i++)
			{
				if (temp->getStringRep() == table[i].name)
					setOfTypes.push_back(table[i].type);
			}
		}
		if (temp->getStringType() == 5) //integer
			setOfTypes.push_back("int");
		if (temp->getStringType() == 6)//float
			setOfTypes.push_back("float");
		if (temp->getStringType() == 8)//char
		{
			if (temp->getStringRep().substr(0, 1) == "\'")
				setOfTypes.push_back("char");
		}
		temp = temp->getNext();
	}
	if (setOfTypes.size() > 1) // If there is more than one variable or number
	{
		for (int i = 0; i < setOfTypes.size() - 1; i++)
		{
			if (setOfTypes[i] != setOfTypes[i + 1])
			{
				//------------------------------------------------------------
				//Printing the Error 
				//------------------------------------------------------------
				cout << "---> unmatched data type detected in :" << endl;
				cout << "---> ";
				Token* exp = tokens.getFirst();
				while (exp)
				{
					cout << exp->getStringRep() + " ";
					exp = exp->getNext();
				}
				cout << "\n";
				//------------------------------------------------------------
				//End of Printing the Error 
				//------------------------------------------------------------
				return "0";
			}
		}
		return setOfTypes[0];
	}
	else if (setOfTypes.size() == 1)// If there is only one variable or number in expression
	{
		return setOfTypes[0];
	}
	return "";
}
//Input: a TokenList of assignment statements
//Output: returns FALSE if there is an unmatched type in the assignment list
//@ description: looks at one assignment statement at a time and checks for type mismatches
//Prints out all the unmatch assignments
bool unmatchedTypes(TokenList assignmentTokenList, vector<Variable> tableOfTypes)
{
	bool noError = true;
	Token* ptr = assignmentTokenList.getFirst();
	while (ptr)
	{
		TokenList RHS, LHS;

		while (ptr->getStringRep() != "=")// Right Hand Side
		{
			LHS.append(ptr->getStringRep());
			ptr = ptr->getNext();
		}
		ptr = ptr->getNext();// =
		while (ptr->getStringRep() != ";")// Left Hand Side
		{
			RHS.append(ptr->getStringRep());
			ptr = ptr->getNext();
		}
		//ptr = ptr->getNext();// ;
		string typeR = type(RHS, tableOfTypes);
		string typeL = type(LHS, tableOfTypes);

		if (typeR != typeL)
		{
			//------------------------------------------------------------
			//Printing the Error 
			//------------------------------------------------------------
			cout << "===================================" << endl;
			cout << "--->  unmatched type detected : " << endl;
			Token* temp = LHS.getFirst();
			cout << "--->  ";
			while (temp)
			{
				cout << temp->getStringRep() + " ";
				temp = temp->getNext();
			}
			cout << "= ";
			temp = RHS.getFirst();
			while (temp)
			{
				cout << temp->getStringRep() + " ";
				temp = temp->getNext();
			}
			cout << ";" << endl;
			cout << "--->  LHS	:	" + typeL << endl;
			cout << "--->  RHS	:	" + typeR << endl;
			cout << "===================================" << endl;
			//------------------------------------------------------------
			//End of Printing the Error 
			//------------------------------------------------------------
			noError = false;
		}
		if (ptr)
			ptr = ptr->getNext();
	}
	return noError;
}
/*void errorCheckingAssignmentStatements(TokenList &assignmentTokenList)
{
	
}*/
/*bool areBalanced(TokenList &assignmentTokenList)
{
	Token* token = assignmentTokenList.getFirst();
	stack<Token> S;
	while (token)
	{
		if (token->getStringRep == '(' || token->getStringRep == '[' || token->getStringRep == '{')
		{
			S.push(token);
			token = token->getNext();
		}

		else if (token->getStringRep == ')' || token->getStringRep == ']' || token->getStringRep == '}')
		{

		}
	}
}*/
/*bool arePair()
{}*/
/*void errorCheckingFunctionDeclaration(TokenList &functionTokenList)
{
	int cntCurlyOpen = 0, cntSquareOpen = 0, cntBracOpen = 0, cntCurlyClose = 0, cntSquareClose = 0, cntBracClose = 0;    //counts for braces
	Token* testFunc = functionTokenList.getFirst();
	string function = "";
	Token* testAss = functionTokenList.getFirst();	//pointer to check for number of braces
	Token* testAss2 = functionTokenList.getFirst();	//pointer to check for unmatched braces
	Token* testAss3 = functionTokenList.getFirst();
	Token* testAss4 = functionTokenList.getFirst();
	Token* headAssingment = functionTokenList.getFirst();
	while (testFunc)
	{
		while (testFunc->getStringRep() != ";")
		{
			if (testFunc->getStringRep() == "(")	//counting number of (, { , [ ,) }, ]
				cntBracOpen++;
			else if (testFunc->getStringRep() == ")")
				cntBracClose++;
			else if (testFunc->getStringRep() == "{")
				cntCurlyOpen++;
			else if (testFunc->getStringRep() == "}")
				cntCurlyClose++;
			else if (testFunc->getStringRep() == "[")
				cntSquareOpen++;
			else if (testFunc->getStringRep() == "]")
				cntSquareClose++;
			function += testFunc->getStringRep();	//storing the current token into a string variable
			testFunc = testFunc->getNext();
		}
		if (cntBracOpen != cntBracClose || cntCurlyOpen != cntCurlyClose || cntSquareOpen != cntSquareClose)	//checking if the numbers of brackets are matching
		{
			cout << "Function Declaration: " << function << " is Missing ";
			if (cntBracOpen != cntBracClose)
			{
				int difference = cntBracOpen - cntBracClose;
				if (cntBracOpen > cntBracClose)
					cout << difference << " ')' bracket(s)\n";	//if they do not match, print put the number of difference
				else
					cout << difference*(-1) << " '(' bracket(s)\n\n";
			}
			if (cntCurlyOpen != cntCurlyClose)
			{
				int difference = cntCurlyOpen - cntCurlyClose;
				if (cntCurlyOpen > cntCurlyClose)
					cout << difference << " '}' bracket(s)\n";
				else
					cout << difference*(-1) << " '{' bracket(s)\n\n";
			}
			if (cntSquareOpen != cntSquareClose)
			{
				int difference = cntSquareOpen - cntSquareClose;
				if (cntSquareOpen > cntSquareClose)
					cout << difference << " ']' bracket(s)\n";
				else
					cout << difference*(-1) << " '[' bracket(s)\n\n";
			}
		}
		cntBracOpen = 0;	//resets the counts to zero, and string variable to empty
		cntBracClose = 0;
		cntCurlyOpen = 0;
		cntCurlyClose = 0;
		cntSquareOpen = 0;
		cntSquareClose = 0;
		testFunc = testFunc->getNext();	//to get next token (function declaration)
		function = "";
	}
}*/
//Total number of each type
void totalNumberOfEachType(TokenList tokens, bool list)
{
	int Identifier = 0;			//0
	int Operater = 0;			//1
	int Punctuator = 0;			//2
	int Keyword = 0;			//3
	int Boolean = 0;			//4
	int IntegerLiteral = 0;		//5
	int FloatLiteral = 0;		//6
	int StringLiteral = 0;		//7
	int Unknown = 0;			//8

	Token* temp = tokens.getFirst();
	while (temp)
	{
		switch (temp->getStringType())
		{
		case 0:
			Identifier++;
			break;
		case 1:
			Operater++;
			break;
		case 2:
			Punctuator++;
			break;
		case 3:
			Keyword++;
			break;
		case 4:
			Boolean++;
			break;
		case 5:
			IntegerLiteral++;
			break;
		case 6: 
			FloatLiteral++;
			break;
		case 7:
			StringLiteral++;
			break;
		case 8:
			Unknown++;
			break;
		}
		temp = temp->getNext();
	}// While (temp)
	//-------------------------------------------------------------------------------------
	//Print Type (0), i.e. Identifiers
	//-------------------------------------------------------------------------------------
	cout << "Number Of Identifers : " << Identifier << endl;
	cout << "---------------------------------------------" << endl;
	if (list)
	{
		cout << "List Of Identifiers: " << endl;
		temp = tokens.getFirst();
		while (temp)
		{
			if (temp->getStringType() == 0)
				cout << "--->  " + temp->getStringRep() << endl;
			temp = temp->getNext();
		}
		cout << "---------------------------------------------" << endl;
	}
	//-------------------------------------------------------------------------------------
	//Print Type (1), i.e. Operator
	//-------------------------------------------------------------------------------------
	cout << "Number Of Operator : " << Operater << endl;
	cout << "---------------------------------------------" << endl;
	if (list)
	{
		cout << "List Of Operator: " << endl;
		temp = tokens.getFirst();
		while (temp)
		{
			if (temp->getStringType() == 1)
				cout << "--->  " + temp->getStringRep() << endl;
			temp = temp->getNext();
		}
		cout << "---------------------------------------------" << endl;
	}
	//-------------------------------------------------------------------------------------
	//Print Type (2), i.e. Punctuator
	//-------------------------------------------------------------------------------------
	cout << "Number Of Punctuator : " << Punctuator << endl;
	cout << "---------------------------------------------" << endl;
	if (list)
	{
		cout << "List Of Punctuator: " << endl;
		temp = tokens.getFirst();
		while (temp)
		{
			if (temp->getStringType() == 2)
				cout << "--->  " + temp->getStringRep() << endl;
			temp = temp->getNext();
		}
		cout << "---------------------------------------------" << endl;
	}
	//-------------------------------------------------------------------------------------
	//Print Type (3), i.e. Keyword
	//-------------------------------------------------------------------------------------
	cout << "Number Of Keyword : " << Keyword << endl;
	cout << "---------------------------------------------" << endl;
	if (list)
	{
		cout << "List Of Keyword: " << endl;
		temp = tokens.getFirst();
		while (temp)
		{
			if (temp->getStringType() == 3)
				cout << "--->  " + temp->getStringRep() << endl;
			temp = temp->getNext();
		}
		cout << "---------------------------------------------" << endl;
	}
	//-------------------------------------------------------------------------------------
	//Print Type (4), i.e. Boolean
	//-------------------------------------------------------------------------------------
	cout << "Number Of Boolean : " << Boolean << endl;
	cout << "---------------------------------------------" << endl;
	if (list)
	{
		cout << "List Of Boolean: " << endl;
		temp = tokens.getFirst();
		while (temp)
		{
			if (temp->getStringType() == 4)
				cout << "--->  " + temp->getStringRep() << endl;
			temp = temp->getNext();
		}
		cout << "---------------------------------------------" << endl;
	}
	//-------------------------------------------------------------------------------------
	//Print Type (5), i.e. IntegerLiteral
	//-------------------------------------------------------------------------------------
	cout << "Number Of IntegerLiteral : " << IntegerLiteral << endl;
	cout << "---------------------------------------------" << endl;
	if (list)
	{
		cout << "List Of IntegerLiteral: " << endl;
		temp = tokens.getFirst();
		while (temp)
		{
			if (temp->getStringType() == 5)
				cout << "--->  " + temp->getStringRep() << endl;
			temp = temp->getNext();
		}
		cout << "---------------------------------------------" << endl;
	}
	//-------------------------------------------------------------------------------------
	//Print Type (6), i.e. FloatLiteral
	//-------------------------------------------------------------------------------------
	cout << "Number Of FloatLiteral : " << FloatLiteral << endl;
	cout << "---------------------------------------------" << endl;
	if (list)
	{
		cout << "List Of FloatLiteral: " << endl;
		temp = tokens.getFirst();
		while (temp)
		{
			if (temp->getStringType() == 6)
				cout << "--->  " + temp->getStringRep() << endl;
			temp = temp->getNext();
		}
		cout << "---------------------------------------------" << endl;
	}
	//-------------------------------------------------------------------------------------
	//Print Type (7), i.e. StringLiteral
	//-------------------------------------------------------------------------------------
	cout << "Number Of StringLiteral : " << StringLiteral << endl;
	cout << "---------------------------------------------" << endl;
	if (list)
	{
		cout << "List Of StringLiteral: " << endl;
		temp = tokens.getFirst();
		while (temp)
		{
			if (temp->getStringType() == 7)
				cout << "--->  " + temp->getStringRep() << endl;
			temp = temp->getNext();
		}
		cout << "---------------------------------------------" << endl;
	}
	//-------------------------------------------------------------------------------------
	//Print Type (8), i.e. Unknown
	//-------------------------------------------------------------------------------------
	cout << "Number Of Unknown : " << Unknown << endl;
	cout << "---------------------------------------------" << endl;
	if (list)
	{
		cout << "List Of Unknown: " << endl;
		temp = tokens.getFirst();
		while (temp)
		{
			if (temp->getStringType() == 8)
			{
				if (temp->getStringRep() == "\n")
					cout << "--->  " << "\\n" << endl;
				else
					cout << "--->  " + temp->getStringRep() << endl;
			}
			temp = temp->getNext();
		}
		cout << "---------------------------------------------" << endl;
	}
}

//Example Test code for interacting with your Token, TokenList, and Tokenizer classes
//Add your own code to further test the operation of your Token, TokenList, and Tokenizer classes
int main() {
	ifstream sourceFile;
	TokenList tokens, assignmentList, functionList, assignmentTokenList, functionTokenList;
	Tokenizer tokenizer;

	//string lineA = "abcd void && * + / afhsahd;ashfljkh ++ ^5 %";
	//tokens.append("alo salam");

	//Read in a file line-by-line and tokenize each line
	char inputname[80] = { '\0' };

	
	cout << "Enter file name: ";
	cin >> inputname;

	sourceFile.open(inputname);
	//sourceFile.open("test.cpp");
	if (!sourceFile.is_open()) {
		cout << "Failed to open file" << endl;
		system("pause");
		return 1;
	}//if

	while (!sourceFile.eof()) {
		string lineA, lineB;

		getline(sourceFile, lineA);

		//while the current line ends with a line-continuation \ append the next line to the current line
		if (lineA.length() != 0)
		{
			while (lineA[lineA.length() - 1] == '\\')
			{
				lineA.erase(lineA.length() - 1, 1);
				getline(sourceFile, lineB);
				lineA += lineB;
			}
			//****************************************************************************************************
			//****************************************************************************************************
			tokenizer.setString(&lineA);
			while (!tokenizer.isComplete()) {
				tokens.append(tokenizer.getNextToken());
			}
			//****************************************************************************************************
			//****************************************************************************************************
			//Re-insert newline that was removed by the getline function
			tokens.append("\n");
		}
		else
			continue;
	}
	removeInlineComments(tokens);
	removeBlockComments(tokens);
	//--------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------
	/*Test your tokenization of the file by traversing the tokens list and printing out the tokens and the tokens type*/
	//--------------------------------------------------------------------------------------------------------------------
	//Token *t = tokens.getFirst();
	/*while (t) {
		if (t->getStringRep() == "\n")
		{
			cout << "\\n" << " : ";
		}
		else
		{
			cout << t->getStringRep() << " : ";
		}
		switch (t->getStringType()) {
		case 0: cout << "T_Identifier\n"; break;
		case 1: cout << "T_Operator\n"; break;
		case 2: cout << "T_Punctuator\n"; break;
		case 3: cout << "T_Keyword\n"; break;
		case 4: cout << "T_Boolean\n"; break;
		case 5: cout << "T_IntegerLiteral\n"; break;
		case 6: cout << "T_FloatLiteral\n"; break;
		case 7: cout << "T_StringLiteral\n"; break;
		case 8: cout << "T_Unknown\n"; break;
		}
		t = t->getNext();
	}*/
	//------------------------------------------------------------------------------
	//Test your assignment statements
	//------------------------------------------------------------------------------
	getAssignmentStatements(tokens, assignmentList, assignmentTokenList);
	/*Token* aListPtr = assignmentTokenList.getFirst();
	while (aListPtr) 
	{
		cout << aListPtr->getStringRep() << endl;
		aListPtr = aListPtr->getNext();
	}*/
	//------------------------------------------------------------------------------
	//Table of User Defined Data Types
	//------------------------------------------------------------------------------
	TokenList userDefinedNames = getUserDefinedTypes(tokens);

	/*while (userDefinedNames) {
	cout << userDefinedNames->getStringRep() << endl;
	userDefinedNames = userDefinedNames->getNext();
	}*/
	//-----------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------
	//Test your fcuntion delcaration
	//-----------------------------------------------------------------------------------------
	Token *aListPtr2 = getFunctionDeclaration(tokens, userDefinedNames, functionList, functionTokenList);
	while (aListPtr2)
	{
		cout << "--> " + aListPtr2->getStringRep() << endl;
		aListPtr2 = aListPtr2->getNext();
	}
	//-----------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------------
	//Table of variables and their types
	//------------------------------------------------------------------------------------------------------
	vector<Variable> variables = getTableOfVariables(assignmentList, tokens.getFirst(), userDefinedNames);

	/*for (int i = 0; i < variables.size(); i++)
	{
		cout << "name : " << variables[i].name << "		";
		cout << "type : " << variables[i].type << endl;
	}*/
	//------------------------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------
	//Test Unmatched types
	//-----------------------------------------------------------------------------------------
	//unmatchedTypes(assignmentTokenList, variables);
	//-----------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------
	//Test Unmatched braces
	//-----------------------------------------------------------------------------------------
	//errorCheckingFunctionDeclaration(functionTokenList);
	//--------------------------------------------------------------------------------------------------------------------
	/*TStatistics*/
	//--------------------------------------------------------------------------------------------------------------------
	cout << endl << "Hello dear user! please input v/V for verbose mode or n/N for non-verbose mode" << endl;
	string mode;
	int modeNum = 0;
	cin >> mode;
	if (mode == "v" || mode == "V")
		modeNum = 0;
	else if (mode == "n" || mode == "N")
		modeNum = 1;
	switch (modeNum){
	case 0:
	{
			  cout << "---------------------------------------------" << endl;
			  cout << "Verbose mode selected :" << endl;
			  cout << "---------------------------------------------" << endl;
			  cout << "Number of Assignment Statement : ";
			  cout << assignmentList.getSize() << endl;
			  cout << "---------------------------------------------" << endl;
			  cout << "Number of Function Declarations : ";
			  cout << functionList.getSize() << endl;
			  cout << "---------------------------------------------" << endl;
			  cout << "Total Number of Tokens in The Program : ";
			  cout << tokens.getSize() << endl;
			  cout << "---------------------------------------------" << endl;
			  totalNumberOfEachType(tokens, true);
			  break;
	}
	case 1:
	{
			  cout << "---------------------------------------------" << endl;
			  cout << "Non-Verbose mode selected " << endl;
			  cout << "---------------------------------------------" << endl;
			  cout << "Number of Assignment Statement : ";
			  cout << assignmentList.getSize() << endl;
			  cout << "---------------------------------------------" << endl;
			  cout << "Number of Function Declarations : ";
			  cout << functionList.getSize() << endl;
			  cout << "---------------------------------------------" << endl;
			  cout << "Total Number of Tokens in The Program : ";
			  cout << tokens.getSize() << endl;
			  cout << "---------------------------------------------" << endl;
			  totalNumberOfEachType(tokens, false);
			  break;
	}

	}
	//--------------------------------------------------------------------------------------------------------------------
	system("pause");
	return 0;
}



	