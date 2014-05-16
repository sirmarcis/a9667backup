#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int diff(string str1, string str2, int str1Pos, int str2Pos){
	if(str1.at(str1Pos) == str2.at(str2Pos))
		return 0;
	else
		return 1;	
}

int firstCharCheck(const string& str1, const string& str2){
	if(str1.at(0) == str2.at(0))
		return 0;
	return 1;
}

vector<vector<int> > fillEVector(string str1, string str2){
	vector<vector<int> > eVector;
	for(int x = 0; x < str1.size(); x++){
		vector<int> currRow;
		for(int y = 0; y < str2.size(); y++){
			if(x==0 && y==0)
				currRow.push_back(firstCharCheck(str1, str2));
			else if(x==0)
				currRow.push_back(y);
			else if(y == 0)
				currRow.push_back(x);
			else
				currRow.push_back(100000);
		}
		eVector.push_back(currRow);
	}
	return eVector;
}

//vertical axis of eVector corresponds to str1, while the horizontal axis corresponds to str2
void printEVector(const vector<vector<int> >& eVector, const string& str1, const string& str2){
	for(int x = 0; x <= eVector.size(); x++){
		for(int y = 0; y <= eVector[0].size(); y++){
			if(x != 0 && y != 0)
				cout << eVector[x-1][y-1];
			else if(x == 0 && y == 0)
				cout << " ";
			else if(y == 0 && x != 0)
				cout << str1.at(x-1);
			else if(x == 0 && y != 0)
				cout << str2.at(y-1);
		}
		cout << endl;
	}
}



//edits include: insertion, deletion, or substitution 
int E(string str1, string str2){ //OBSOLETE
	int str1Length = str1.size()-1;
	int str2Length = str2.size()-1;
	if(str1Length >= 0 && str2Length >= 0){
		if(str1Length > str2Length) // case 1: str1 is longer than str2 and thus the last bytes do not match (deletion)
			return 1 + E(str1.substr(0, str1Length), str2);	
		else if(str1Length < str2Length) // case 2 : str2 is longer than str1 and again the last bytes do not match (insertion)
			return 1 + E(str1, str2.substr(0, str2Length));	
		else
			return diff(str1, str2, str1Length, str2Length) + E(str1.substr(0, str1Length), str2.substr(0, str2Length));
	}
	else
		return 0;
}

int min(int x, int y, int z){
	if(x <= y && x <= z)
		return x;
	else if(y <= x && y <= z)
		return y;
	else if(z <= x && z <= y)
		return z;
}

int max(int x, int y, int z){
	if(x >= y && x >= z)
		return x;
	else if(y >= x && y >= z)
		return y;
	else if(z >= x && z >= y)
		return z;
}

int findMinimumEditDistance(string str1, string str2){
	vector<vector<int> > eVector = fillEVector(str1, str2);
	for(int x = 1; x < str1.size(); x++){
		for(int y = 1; y < str2.size(); y++)
			eVector[x][y] = min(1+eVector[x-1][y], 1+eVector[x][y-1], diff(str1, str2, x, y) + eVector[x-1][y-1]);	
	}
	//printEVector(eVector, str1, str2);
	return eVector[eVector.size()-1][eVector[0].size()-1];
}

bool alreadyPresesnt(const string& possSubstr, const vector<string>& allPossibleSubstrings){
	for(int x = 0; x < allPossibleSubstrings.size(); x++){
		if(allPossibleSubstrings[x] == possSubstr)
			return false;
	}
	return true;
}

void getAllPossibleSubstrings(vector<string>& allPossibleSubstrings, string& str, int startPos, int endPos){
	string possSubstr;
	if(startPos+1 < str.size()-1){
		possSubstr = str.substr(startPos+1, endPos+1);
		if(alreadyPresesnt(possSubstr, allPossibleSubstrings)){
			allPossibleSubstrings.push_back(possSubstr);
			getAllPossibleSubstrings(allPossibleSubstrings, str, startPos+1, endPos);
		}
	}
	if(endPos-1 > 0){
		possSubstr = str.substr(startPos, endPos);
		if(alreadyPresesnt(possSubstr, allPossibleSubstrings)){
			allPossibleSubstrings.push_back(possSubstr);
			getAllPossibleSubstrings(allPossibleSubstrings, str, startPos, endPos-1);
		}
	}
	if(startPos+1 < str.size()-1 && endPos-1 > 0){
		possSubstr = str.substr(startPos+1, endPos);
		if(alreadyPresesnt(possSubstr, allPossibleSubstrings)){
			allPossibleSubstrings.push_back(possSubstr);
			getAllPossibleSubstrings(allPossibleSubstrings, str, startPos+1, endPos-1);
		}
	}
}

bool ispalendrome(string& str, int startPos, int endPos){
	string prefix = str.substr(startPos, startPos + ((endPos-startPos)/2)+1);
	string suffix;
	if((endPos-startPos)%2 == 1)
		suffix = str.substr(startPos + ((endPos-startPos)/2), endPos);
	else
		suffix = str.substr(startPos + ((endPos-startPos)/2)-1, endPos);
	reverse(suffix.begin(), suffix.end());
	if(findMinimumEditDistance(prefix, suffix) == 0){
		//cout << prefix << " " << suffix << endl;
		return true;
	}
	else
		return false;
}

int longestPalendrome(string& str, int startPos, int endPos){
	vector<string> allPossibleSubstrings; 
	allPossibleSubstrings.push_back(str);
	getAllPossibleSubstrings(allPossibleSubstrings, str, startPos, endPos);
	int longestSubstr = 0;
	int currLength = 0;
	string currPalendrome;
	for(int x = 0; x < allPossibleSubstrings.size(); x++){
		currLength = allPossibleSubstrings[x].size();
		if(ispalendrome(allPossibleSubstrings[x], 0, currLength) && currLength > longestSubstr){
			longestSubstr = currLength;
			currPalendrome = allPossibleSubstrings[x];
			//cout << "curr longest palendrome is now: " << currPalendrome << endl;
		}
	}
	cout << "longest consecutive palendrome is: " << currPalendrome << endl;
	return longestSubstr;
}

int main(int argc, char *argv[]){
	if( argc != 3 ){
        cerr << "Usage: " << argv[ 0 ] << " string1 string2" << endl;
        return 1;
    }
    char* string1 = argv[1];
    char* string2 = argv[2];
    string str1(string1);
    string str2(string2);
    cout << "longest palendrome length: " << longestPalendrome(str1, 0, str1.size()-1) << endl;
    //cout << "min edit distance between [" << str1 << "] and [" << str2 << "] is [" << findMinimumEditDistance(str1, str2) << "]" << endl;
	return 0;
}