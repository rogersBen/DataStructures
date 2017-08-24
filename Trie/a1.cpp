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

const int MAXWORDS = 50000;

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




int wordCount = 0;
string words[MAXWORDS];

int main(int argc, char* argv[]) {

	fstream fin;
	initialize();
	fin.open(argv[1]);


	if(fin.good()) {
		while(!fin.eof()) {
			char tempLine[100];

			//Read in line or until newline character reached
			fin.getline(tempLine, 256, '\n');
			parseLine(tempLine); //parseLine calls checkWord and addWord

		}

		//Document has been read 
		cout << "Word count: " << wordCount << endl;


			if(search("bite")) {
				cout << "Found bite" << endl;
			}
			if(search("mimsy")) {
				cout << "Found mimsy" << endl;
			}

			cout << "Number of words with prefix ban are " << wordsWithPrefix("ban") << endl;

				cout << "brillig count: " << wordsWithPrefix("brillig") << endl;
			
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
		lowerWord = checkWord(word); //Discard punctuation and uppercase
		addWord(lowerWord); //
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
	
	//Create node and set to root
	trieNode *current = head;

	//Increase prefix count as a new letter is added	
	current->prefixCount++;	

	for(int i = 0; i < word.length(); i++) {

		//Extract character from word
		int letter = (int)word[i] - (int)'a'; 
		
		//If child branch does not exist for letter, create it
		if(current->child[letter] == NULL) {
				current->child[letter] = new trieNode();
		}
		//Increase prefixCount as that path is traversed
		current->child[letter]->prefixCount++;

		//Change to the next level of the tree
		current = current->child[letter];
	}
	//End of word
	current->isEnd = true;
	
	if(wordsWithPrefix(word) > 1) {

	}else {
		cout << "Unique: " << word << endl;
	}
}

bool search(string word) {

	//Create node and set to root
	trieNode *current = head;

	for(int i = 0; i < word.length(); i++) {

		//Extract character from word
		int letter = (int)word[i] - (int)'a';

		//If child branch does not exist the word does not exist
		if(current->child[letter] == NULL) {
			return false;
		}

		//Move down the tree
		current = current->child[letter];
	}
	//If word is in trie this will return true
	return current->isEnd;
}


int wordsWithPrefix(string prefix) {

	//Create node and set it to root
	trieNode *current = head;

	for(int i = 0; i < prefix.length(); i++) {

		//Extract character from word
		int letter = (int)prefix[i] - (int)'a';

		//If child branch does not exist the word does not exist
		if(current->child[letter] == NULL) {
			return 0;
		}else {
			//Otherwise it must exist, traverse this branch
			current = current->child[letter];
		}
	}
	//Returns number of times word is in trie
	return current->prefixCount;
}


