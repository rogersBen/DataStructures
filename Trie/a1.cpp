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
	int prefixCount;	//How far does the tree go
	struct trieNode *child[26];
	bool isEnd;			//Is this node the last in the tree
	char letter;		//The character in the trieNode
}*head;



void parseLine(string);


string lower(string);
void initialize();
void addWord(string);
int countPreffixes();
int countWords();
bool search(string);
int wordsWithPrefix(string); //Can search for a whole string this way?

int wordCount = 0;


int main(int argc, char* argv[]) {

	fstream fin;
	//int wordCount = 0;
	initialize();
	//Set the filename from command line

	fin.open(argv[1]);


	if(fin.good()) {
		while(!fin.eof()) {
			char tempLine[100];
			fin.getline(tempLine, 256, '\n'); //Read in line or until newline character reached
			parseLine(tempLine); //


		}
		cout << "Unique Word count: " << wordCount << endl;

		cout << "If this says 1 we are laughing:  " << wordsWithPrefix("benjamin") << endl;

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
		lowerWord = lower(word);
		addWord(lowerWord);
	}
	return;
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
	return newWord;
}

//First have to initialize the vertexes
void initialize() {
	head = new trieNode();
	head->isEnd = false;
	head->prefixCount = 0;
}

//Two parameters, the vertex of the insertion and the word to be added
//When a string word is added to a vertex we will add word to the corresponding
//branch of vertex cutting the leftmost character of word
//If the needed branch does not exist we will have to create it
void addWord(string word) {
	
	trieNode *current = head;	//Create node and set to root
	current->prefixCount++;	//Increase prefix count as a new letter is added

	//Check if word is unique
	if(search(word)) {
		cout << "Word is not unique: " << word << endl;
	}else {
		for(int i = 0; i < word.length(); i++) {
			//Letter is 
			int letter = (int)word[i] - (int)'a'; //Extract first character of word
		
			//If child branch does not exist it is created
			if(current->child[letter] == NULL) {
					current->child[letter] = new trieNode();
			}
			current->child[letter]->prefixCount++;
			current = current->child[letter];
			}
		current->isEnd = true;
		wordCount++;
	}
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

//Use this to check a word, if you search the complete word and it returns 1
//Then that word is in the trie
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