/*
	Student Name:		Benjamin Rogers
	Student Number:		4503600
	Student Login:		bjr342
	File Description:	CSCI203 Spring 2017 Ass 1
*/


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


using namespace std;

//Nodes in the trie
struct trieNode {
	int words;
	int prefixCount;	//How many children the node has
	struct trieNode *child[26];
	bool isEnd;
	char letter;		//The character in the trieNode
}*head;



void parseLine(string);


string lower(string);
void initialize();
void addWord(string);
int countPreffixes();
int countWords();
bool search(string);
int wordsWithPrefix(string);
string checkWord(string);



//trieNode vertex;


int wordCount = 0;
string words[200];

int main(int argc, char* argv[]) {

	fstream fin;
	//int wordCount = 0;
	initialize();


	
	fin.open(argv[1]);


	if(fin.good()) {
		while(!fin.eof()) {
			char tempLine[100];

			//Read in line or until newline character reached
			fin.getline(tempLine, 256, '\n');
			parseLine(tempLine);

		}
		cout << "Word count: " << wordCount << endl;


			if(search("bite")) {
				cout << "Found bite" << endl;
			}
			if(search("mimsy")) {
				cout << "Found mimsy" << endl;
			}

			cout << "Number of words with prefix ban are " << wordsWithPrefix("ban") << endl;

	}else {
		cerr << "ERROR OPENING FILE..." << endl;
		cerr << "TERMINATING PROGRAM";
		return -1;
	}
	

	return 0;
}


void parseLine(string tempLine) {
	
	//Using stringstream example provided to break string into words
	istringstream iss(tempLine);
	for(;;) {
		string word, lowerWord;
		iss >> word;
		if(iss.fail())break;
		lowerWord = checkWord(word);
		addWord(lowerWord);
	}
	return;
}

string checkWord(string word) {
	//Fold all letters into lowercase and remove punctuation	
	char lowerChar;
	string newWord;
	for(int i = 0; i < word.length(); i++) {
		int charAscii = (int)word[i];
		if(charAscii >= 97 && charAscii <= 122 ) {
			//Char is a lowercase letter, add to word
			lowerChar = word[i];
			newWord = newWord+lowerChar; 

		}else if(charAscii >= 64 && charAscii <= 91) {
			//Char is uppercase letter, convert to lowercase then add to word
			lowerChar = word[i];
			lowerChar = tolower(lowerChar);
			newWord = newWord+lowerChar;
		}else {
			//Char is symbol and needs to be discarded
			//DO nothing
		}
	}
	wordCount++;
	return newWord;
}


string lower(string word) {
	//Converts string into chars and then makes lowercase
	//Then adds chars to a new string until whole string has been processed
	char lowerChar;
	string newWord;
	for(int i = 0; i < word.length(); i++) {
		lowerChar = word[i];
		lowerChar = tolower(lowerChar);
		newWord = newWord+lowerChar; 
	}
	//words[wordCount] = newWord;
	wordCount++;
	return newWord;
}

//First have to initialize the root element to 0
void initialize() {
	head = new trieNode();
	head->isEnd = false;
	head->prefixCount = 0;

}


//When a word is added to a node we will add word to the corresponding
//branch of node cutting the leftmost character of word
//If the needed branch does not exist we will have to create it
void addWord(string word) {
	
	trieNode *current = head;	//Create node and set to root
	current->prefixCount++;	//Increase prefix count as a new letter is added

	for(int i = 0; i < word.length(); i++) {
		int letter = (int)word[i] - (int)'a'; //Extract first character of word
		
		//If child branch does not exist it is created
		if(current->child[letter] == NULL) {
				current->child[letter] = new trieNode();
		}
		current->child[letter]->prefixCount++;
		current = current->child[letter];
	}
	current->isEnd = true;
}

bool search(string word) {
	trieNode *current = head;
	for(int i = 0; i < word.length(); i++) {

		int letter = (int)word[i] - (int)'a';

		if(current->child[letter] == NULL) {
			return false;
		}
		current = current->child[letter];
	}
	return current->isEnd;
}


int wordsWithPrefix(string prefix) {
	trieNode *current = head;
	for(int i = 0; i < prefix.length(); i++) {
		int letter = (int)prefix[i] - (int)'a';
		if(current->child[letter] == NULL) {
			return 0;
		}else {
			current = current->child[letter];
		}
	}
	return current->prefixCount;
}


