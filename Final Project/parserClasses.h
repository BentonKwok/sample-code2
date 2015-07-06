#ifndef PARSERCLASSES_H_
#define PARSERCLASSES_H_

//Use only the string library DO NOT add any other libraries
#include <string>
#include "lexemesTypes.h"

using namespace std;
using namespace ensc251;
//Declare your variables for storing delimiters here:


//typedef for custom data type defined in header file "lexemesTypes.h"
typedef ensc251::LexemeType TokenClass;


//Token class for a doubly-linked list of string tokens
class Token {
private:
	Token *next; //Next pointer for doubly linked list
	Token *prev; //Previous pointer for doubly linked list
	string stringRep; //Token value
	TokenClass stringType; //Token type (to which class this token belongs to, for e.g. identifier, keywords...etc)
	 
	//Allow TokenList class to access Token member variables marked private
	friend class TokenList;

public:
	//Default Constructor, pointers initialized to NULL
	Token() : next(NULL), prev(NULL) { }
		
	//Constructor with string initialization, pointers initialized to NULL
	Token(const string& mystringRep) : next(NULL), prev(NULL), stringRep(mystringRep) { }

	//Returns the Token's *next member 
	Token* getNext() const { return next; }

	//Sets the Token's *next member
	void setNext(Token* mynext) { next = mynext; }

	//Returns the Token's *prev member 
	Token* getPrev() const { return prev; }

	//Sets the Token's *prev member
	void setPrev(Token* myprev){ prev = myprev; }

	//Returns a reference to the Token's stringRep member variable
	const string& getStringRep() const { return stringRep; }

	//Sets the token's stringRep variable
	void setStringRep(const string& mystringRep) { stringRep = mystringRep; }
	//===================================================================================~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//===================================================================================~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	//Returns a reference to the Token's stringType member variable
	const TokenClass getStringType() const { return stringType; }

	//Sets the token's stringType variable
	void setStringType(const TokenClass& myStringType) { stringType = myStringType; }

	//===================================================================================~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//===================================================================================~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
};

//A doubly-linked list class consisting of Token elements
class TokenList {
public:
	Token *head; //Points to the head of the token list (doubly linked)
	Token *tail; //Points to the tail of the function list (doubly linked)
	
public:
	//Default Constructor, Empty list with pointers initialized to NULL
	TokenList() : head(NULL), tail(NULL) { }
	
	//Returns a pointer to the head of the list
	Token* getFirst() const { return head; }

	//Returns a pointer to the tail of the list
	Token* getLast() const { return tail; }

	//Creates a new token for the string input, str
	//Appends this new token to the TokenList
	//On return from the function, it will be the last token in the list
	void append(const string &str); //example comment

	//Appends the token to the TokenList if not null
	//On return from the function, it will be the last token in the list
	void append(Token *token);

    //Removes the token from the linked list if it is not null
	//Deletes the token
	//On return from function, head, tail and the prev and next Tokens (in relation to the provided token) may be modified.
	void deleteToken(Token *token);
	//===================================================================================~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//===================================================================================~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void setTokenClass(Token *token);
	//===================================================================================~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//===================================================================================~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	int getSize();
};

//A class for tokenizing a string of C++  code into tokens
class Tokenizer {
private:
	/*State tracking variables for processing a single string*/
	bool processingInlineComment; //True if processing an In-line comment //
	bool processingBlockComment;  //True if processing a Block Comment /* */
	bool processingIncludeStatement; //True if processing an include statement <> ""
	bool complete; //True if finished processing the current string
	

	size_t offset; //Current position in string
	size_t tokenLength; //Current token length
	string *str; //A pointer to the current string being processed
	int delSize;

	//Include any helper functions here
	//e.g. trimming whitespace, comment processing
	
	//removes tabs ("\t") in the string
	string* removeTab(string* myStr);


	//returns the first delimeter charactor in the string
	string find_first_delimiter(int first);


	//Computes a new tokenLength for the next token
	//Modifies: size_t tokenLength, and bool complete
	//(Optionally): may modify offset
	//Does NOT modify any other member variable of Tokenizer
	void prepareNextToken();
	
public:
	//Default Constructor- YOU need to add the member variable initializers.
	Tokenizer()  
	{
		str = NULL;
		processingInlineComment = false; 
		processingBlockComment = false;  
		processingIncludeStatement = false; 
		complete = false; 
		offset = 0;
		tokenLength = 0;
		delSize = 0;
	}


	//Sets the current string to be tokenized
	//Resets all Tokenizer state variables
	//Calls Tokenizer::prepareNextToken() as the last statement before returning.
	void setString(string *str);


	//Returns true if all possible tokens have been extracted from the current string (string *str)
	bool isComplete() const { return complete; }


	//Returns the next token. Hint: consider the substr function
	//Updates the tokenizer state
	//Updates offset, resets tokenLength, updates processingABC member variables
	//Calls Tokenizer::prepareNextToken() as the last statement before returning.
	string getNextToken();
};




#endif /* PARSERCLASSES_H_ */
