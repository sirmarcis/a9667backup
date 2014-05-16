#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <cstring>
#include <map>

using namespace std;

class Node {
public:
	Node* leftChild;
	Node* rightChild;
	string name;
	int frequency;
	string encodedVal;

	Node(const string& nName) : name(nName) {
		frequency = 0;
		encodedVal = "";
		leftChild = NULL;
		rightChild = NULL;
	}
	Node(const string& nName, const int nFreq) : name(nName), frequency(nFreq){
		leftChild = NULL;
		rightChild = NULL;
		encodedVal = "";
	}
};

class Word{
public:
	Word* leftChild;
	Word* rightChild;
	string name;
	int frequency;
	string encodedVal;

	Word(const string& nName, const int nFreq) : name(nName), frequency(nFreq){
		leftChild = NULL;
		rightChild = NULL;
		encodedVal = "";
	}
};

void printFreqTree(Node* currLink, int numSteps){
	for(int x = 0; x < numSteps; x++)
		cout << "  ";
	if(currLink->name == "@"){
		cout << "->" << currLink->frequency << endl;
		printFreqTree(currLink->rightChild, numSteps++);
		printFreqTree(currLink->leftChild, numSteps++);
	}
	else
		cout << currLink->name << ": " << currLink->encodedVal << endl;

}

void assignCodeVals(Node* currLink, string currCode){
	if(currLink->name == "@"){
		assignCodeVals(currLink->rightChild, currCode + "0"); //right is zero, left is one
		assignCodeVals(currLink->leftChild, currCode + "1");
	}
	else
		currLink->encodedVal = currCode;
}

void deleteFreqTree(Node* currLink){ // needs to be updated
	if(currLink->leftChild != NULL){
		deleteFreqTree(currLink->leftChild);
		deleteFreqTree(currLink->rightChild);
		delete currLink;
	}
}

void sortPQueue(vector<Node*>& pQueue, Node* newNode){
	vector<Node*>::iterator it;
	for(it = pQueue.begin(); it != pQueue.end(); it++)
		if(newNode->frequency > (*it)->frequency){
			pQueue.insert(it, newNode);
			return;
		}
	pQueue.push_back(newNode);
}

Node* huffmanWordEncode(vector<string>& inputStrArr){
	map<string, int> frequencyCounter;
	int totChars = 0;
	for(int x = 0; x < inputStrArr.size(); x++){
		frequencyCounter[inputStrArr[x]]++;
		totChars++;
		
	}
	map<string, int>::iterator it;
	Node* currLink = NULL;
	Node* newN1;
	Node* newN2;
	vector<Node*> pQueue;
	while(!frequencyCounter.empty()){
		string ch1;
		int largestFreq = 0;
		for(it = frequencyCounter.begin(); it != frequencyCounter.end(); it++){ //cuz im lazy
			if(it->second > largestFreq){
				ch1 = it->first;
				largestFreq = it->second;
			}
		}
		newN1 = new Node(ch1, largestFreq);
		pQueue.push_back(newN1);
		it = frequencyCounter.find(ch1);
		frequencyCounter.erase(it);
	}
	while(!pQueue.empty()){
		if(pQueue.size() > 1){
			newN1 = pQueue[pQueue.size()-1];
			pQueue.pop_back();
			newN2 = pQueue[pQueue.size()-1];
			pQueue.pop_back();
			currLink = new Node("@", newN1->frequency + newN2->frequency);
			currLink->rightChild = newN1; 
			currLink->leftChild = newN2;
			//cout << "currently on nodes: " << newN1->name << " " << newN2->name << endl;
			sortPQueue(pQueue, currLink);
		}
		else{
			//cout << "curr link: " << currLink->name << " " << currLink->frequency << endl;
			currLink = pQueue[pQueue.size()-1];
			pQueue.pop_back();
		}
	}
	
	//currLink->frequency = 0;
	assignCodeVals(currLink, "");
	//printFreqTree(currLink, 0);
	return currLink;
}

Node* huffmanEncode(vector<string>& inputStrArr){
	map<char, int> frequencyCounter;
	int totChars = 0;
	for(int x = 0; x < inputStrArr.size(); x++){
		for(int y = 0; y < inputStrArr[x].size(); y++){
			frequencyCounter[inputStrArr[x].at(y)]++;
			totChars++;
		}
	}
	map<char, int>::iterator it;
	Node* currLink = NULL;
	Node* newN1;
	Node* newN2;
	vector<Node*> pQueue;
	while(!frequencyCounter.empty()){
		char ch1;
		int largestFreq = 0;
		for(it = frequencyCounter.begin(); it != frequencyCounter.end(); it++){ //cuz im lazy
			if(it->second > largestFreq){
				ch1 = it->first;
				largestFreq = it->second;
			}
		}
		string str1;
		str1.push_back(ch1);
		newN1 = new Node(str1, largestFreq);
		pQueue.push_back(newN1);
		it = frequencyCounter.find(ch1);
		frequencyCounter.erase(it);
	}
	while(!pQueue.empty()){
		if(pQueue.size() > 1){
			newN1 = pQueue[pQueue.size()-1];
			pQueue.pop_back();
			newN2 = pQueue[pQueue.size()-1];
			pQueue.pop_back();
			currLink = new Node("@", newN1->frequency + newN2->frequency);
			currLink->rightChild = newN1; 
			currLink->leftChild = newN2;
			sortPQueue(pQueue, currLink);
		}
		else{
			currLink = pQueue[pQueue.size()-1];
			pQueue.pop_back();
		}
	}
	
	//currLink->frequency = 0;
	assignCodeVals(currLink, "");
	//printFreqTree(currLink, 0);
	return currLink;
}

void mapCharsToCodes(Node* huffmanTree, map<string, string>& huffmanMap){
	if(huffmanTree->name == "@"){
		mapCharsToCodes(huffmanTree->rightChild, huffmanMap);
		mapCharsToCodes(huffmanTree->leftChild, huffmanMap);
	}
	else{
		//cout << "mapped " << huffmanTree->name << " to " << huffmanTree->encodedVal << endl;
		huffmanMap[huffmanTree->name] = huffmanTree->encodedVal;
	}
}

void encodeInput(vector<string>& inputStrArr, map<string, string>& huffmanMap, ofstream& output){
	for(int x = 0; x < inputStrArr.size(); x++){
		string currEncoding = "";
		for(int y = 0; y < inputStrArr[x].size(); y++){
			char newCh = inputStrArr[x].at(y);
			string temp;
			temp.push_back(newCh);
			output << huffmanMap[temp];
		}
	}
}

void encodeWordInput(vector<string>& inputStrArr, map<string, string>& huffmanMap, ofstream& output){
	for(int x = 0; x < inputStrArr.size(); x++)
		output << huffmanMap[inputStrArr[x]];
	
}

string nextChar(Node* currNode, string& inputStr, int strPos){
	if(currNode->name == "@"){
		if(inputStr.at(strPos) == '0')
			return nextChar(currNode->rightChild, inputStr, strPos + 1);
		else
			return nextChar(currNode->leftChild, inputStr, strPos + 1);
	}
	inputStr = inputStr.substr(strPos++);
	return currNode->name;
}

void decodeInput(string& encodedInput, Node* huffmanTree){
	while(encodedInput.size() > 0){
		string decodedChar = nextChar(huffmanTree, encodedInput, 0);
		cout << decodedChar;
	}
	cout << endl;
}

int main(int argc, char* argv[]){
	string hufFilename;
	vector<string> inputStrArr;
	if(argc < 2 || argc > 4){
		cout << "Usage: " << argv[0] << " input.txt" << endl;
		return 1;
	}
	bool decode = false;
	bool words = false;
	if(argc == 3){ // --decode or --words flag
		if(string(argv[2]) == "--decode")
			decode = true;
		else if(string(argv[2]) == "--words")
			words = true;
		else{
			cout << "Usage: " << argv[0] << "  " << argv[1] << " --decode || --words" << endl;
			return 1;
		}
	}
	else if(argc == 4){
		if(string(argv[2]) == "--words" && string(argv[3]) == "--decode"){
			decode = true;
			words = true;
		}
		else{
			cout << "Usage: " << argv[0] << "  " << argv[1] << " --words --decode" << endl;
			return 1;
		}
	}
	ifstream inFile( argv[ 1 ] );
    if( !inFile ){
        cerr << "Cannot open " << argv[ 1 ] << endl;
        return 1;
    }
    
    hufFilename = string(argv[1]) + ".huf";
    ofstream output;
    string currWord;
    while(inFile >> currWord){
    	inputStrArr.push_back(currWord);
    	inputStrArr.push_back(" ");
    }
    Node* huffmanTree;
    if(!words)
    	huffmanTree = huffmanEncode(inputStrArr);
    
    else
    	huffmanTree = huffmanWordEncode(inputStrArr);
    map<string, string> huffmanMap;
    mapCharsToCodes(huffmanTree, huffmanMap);
    if(!decode){
    	output.open(hufFilename.c_str());
    	if(!words)
    		encodeInput(inputStrArr, huffmanMap, output);
    	else
    		encodeWordInput(inputStrArr, huffmanMap, output);
    	output.close();
    }
    else{
    	cout << "decoding..." <<endl;
    	output.close();
    	ifstream inputStream(hufFilename.c_str());
    	string encodedInput;
    	inputStream >> encodedInput;
    	decodeInput(encodedInput, huffmanTree);
    	//cout << encodedInput << endl;
    }
	
	return 0;
}