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
	int isEnd;	//-1 means not end, 0 or greater means yes
	char letter;		//The character in the trieNode
}*head;



void parseLine(string);
string lower(string);
void initialize();
void addWord(string);
bool search(string);
int wordsWithPrefix(string);
string checkWord(string);
void sortList(int[], int, int);
void printArray(int[], int);
int partition(int[], int, int);
void quickSort(int[], int, int);
void swap(int*, int*);
void insertionSort(int[], int);


int wordCount = 0;	//Count of total words
int uniqueCount = 0; //Count of unique words
string words[MAXWORDS]; //Stores unique words

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
		int wordsCount[uniqueCount];
		cout << "Unique word count: " << uniqueCount << endl;
		
		for(int i = 0; i < uniqueCount; i++) {
			//Creates new array of word counts corresponding with indexes in words
			//wordsCount[i] is the count for words[i]
			wordsCount[i] = wordsWithPrefix(words[i]);

			//Output list for testing
			//cout << words[i] << " " << wordsCount[i] << endl;
		}
		
		//Sort by decreasing count
		//sortList(wordsCount, 0, uniqueCount);
		//quickSort(wordsCount, 0, uniqueCount);
		insertionSort(wordsCount, uniqueCount);
		printArray(wordsCount, uniqueCount);




		//Output first ten words in sorted list with count
		
		cout << endl << "****Top 10****" << endl;
		for(int i = 0; i <= 10; i ++) {
			cout << words[i] << " " << wordsCount[i] << endl;
		}


		//Output last ten words in list along with counts
		cout << endl << "****Bottom 10****" << endl;
		for(int j = uniqueCount-1; j > (uniqueCount-11); j--) {
			cout << words[j] << " " << wordsCount[j] << endl;
		}
			
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
	//wordCount++;
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
	
	return newWord;
}

//First have to initialize the root element to 0
void initialize() {
	head = new trieNode();
	head->isEnd = -1;
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
	current->isEnd = 0;
	wordCount++;

	if(wordsWithPrefix(word) > 1) {

	}else {
		//Word is unique so store it and increase unique word count
		words[uniqueCount] = word;
		uniqueCount++;
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
	return current->isEnd = 0;
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

/*
void sortList(int wordsCount[], int left, int right) {

	//Perform quicksort on list
	int i = left;
	int j = right-1;
	int tmp;
	
	//Choose a pivot value

	int pivotIndex = (j-i)/2;
	cout << i << " " << j << " " << pivotIndex << endl;
	int pivot = wordsCount[(j-i)/2]; //Correct
	cout << "Pivot: " << pivot << endl;

	while(i <= j) {

		cout << "Before i: " << i << " wordsCount[i]: " << wordsCount[i] << " j: " << j << " wordsCount[j]: " << wordsCount[j] << endl;
		
		//Move i forward until an element with value greater or equal to pivot is found
		while(wordsCount[i] < pivot) {
			i++; //should skip this first turn
		}
		
		//Move j back until an element with value lesser or equal to pivot
		while(wordsCount[j] > pivot) {
			j--;
		}
		
		//If i <= j then they are swapped and i steps to the next position
		if(wordsCount[i] == wordsCount[j]) {
			i++;
		}else if (i < j) {
			tmp = wordsCount[i];
			wordsCount[i] = wordsCount[j];
			wordsCount[j] = tmp;
			i++;
			j--;
		}
		cout << "After i: " << i << " wordsCount[i]: " << wordsCount[i] << " j: " << j << " wordsCount[j]: " << wordsCount[j] << endl;
		
		int temp;
		cin >> temp;
	}

	//Recursion
	if(left < j) {
		//cout << "hello";
		sortList(wordsCount, left, j);
	}
	if(i < right) {
		//cout << "bfbf";
		sortList(wordsCount, i, right);
	}		
}
*/

void swap(int *a, int *b) {
	int t = *a;
	*a = *b;
	*b = t;
}


void printArray(int wordsCount[], int size) {
	cout << "Sorted list" << endl;
	for(int i =0; i < size; i++) {
		cout << words[i] << ": " << wordsCount[i] << endl;
	}
}

void insertionSort(int wordsCount[], int length) {
	int j, temp;

	for(int i=0; i < length; i++) {
		j = i;
		//While there are still elements in the list
		while(j > 0 && wordsCount[j] > wordsCount[j-1]) {
			if(wordsCount[j] == wordsCount[j-1]) {
				//Same count, sort alphabetically
				if(words[j].compare(words[j-1]) > 1) {
					cout << words[j] << " " << words[j-1] << endl;
				}
			}else {
				swap(wordsCount[j], wordsCount[j-1]);
				swap(words[j], words[j-1]); //To maintain relationship of indexes
				j--;
			}

		}
			
	}
}

