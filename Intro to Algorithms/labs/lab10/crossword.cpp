#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

bool valSort(std::pair<std::string, int> a, std::pair<std::string, int> b)
{
	return a.second > b.second;
}

void printSol(std::vector<std::vector<char> > puzzle)
{
	for (int i = 0; i < puzzle.size(); ++i)
	{
		for (int j = 0; j < puzzle[i].size(); ++j)
		{
			std::cout << ' ' << puzzle[i][j];
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

bool verifySub(std::vector<std::vector<char> > puzzle, int pos, std::map<char, std::vector<std::string> > wordMap)
{
	int count = 1;
	for (int i = 1; i < puzzle.size(); ++i)
	{
		std::string tmp_str;
		std::stringstream ss;
		for (int j = 0; j < pos; ++j)
		{
			ss << puzzle[i][j];
		}
		ss >> tmp_str;
		std::vector<std::string> candidates = wordMap[puzzle[i][0]];
		for (int j = 0; j < candidates.size(); ++j)
		{
			if (candidates[j].compare(0, tmp_str.length(), tmp_str) == 0)
			{
				count++;
				break;
			}
		}
	}
	
	if(count == 5) return true;
	return false;
}

bool verifyPuzzle(std::vector<std::vector<char> > puzzle, std::vector<std::string> words)
{
	for (int i = 1; i < puzzle.size(); ++i)
	{
		std::string tmp_str;

		std::stringstream ss;

		for (int j = 0; j < puzzle[i].size(); ++j)
		{
			ss << puzzle[i][j];
		}
		ss >> tmp_str;

		if(std::find(words.begin(), words.end(), tmp_str) == words.end()) return false;
	}
	return true;
}

void vertical(std::string str, int line, std::vector<std::vector<char> > &tmp)
{
	for (int i = 0; i < str.size(); ++i)
	{
		tmp[i][line] = str[i];
	}
}

void solve(std::vector<std::string> words, std::map<char, std::vector<std::string> > wordMap)
{
	std::cout << "Solving!" << std::endl;
	
	for (int i = 0; i < words.size(); ++i)
	{
		std::vector<std::vector<char> > sol;
		std::vector<char> first;

		for (int j = 0; j < words[i].size(); ++j)
		{
			first.push_back(words[i][j]);
		}

		for (int j = 0; j < words[i].size(); ++j)
		{
			sol.push_back(first);
		}

		char c1 = sol[0][0];
		char c2 = sol[0][1];
		char c3 = sol[0][2];
		char c4 = sol[0][3];
		char c5 = sol[0][4];

		for (int j = 0; j < wordMap[c1].size(); ++j)
		{
			std::vector<std::vector<char> > tmp = sol;
			vertical(wordMap[c1][j], 0, tmp);
			for (int k = 0; k < wordMap[c2].size(); ++k)
			{
				vertical(wordMap[c2][k], 1, tmp);
				if(!verifySub(tmp, 2, wordMap)) continue;
				for (int l = 0; l < wordMap[c3].size(); ++l)
				{
					vertical(wordMap[c3][l], 2, tmp);
					if(!verifySub(tmp, 3, wordMap)) continue;
					for (int m = 0; m < wordMap[c4].size(); ++m)
					{
						vertical(wordMap[c4][m], 3, tmp);
						if(!verifySub(tmp, 4, wordMap)) continue;
						for (int n = 0; n < wordMap[c5].size(); ++n)
						{
							vertical(wordMap[c5][n], 4, tmp);
							if(verifyPuzzle(tmp, words)) printSol(tmp);
						}
					}
				}
			}
		}
	}

	return;
}

int main(int argc, char const *argv[])
{
	if(argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " dictionary" << std::endl;
		return 1;
	}

	std::ifstream inFile(argv[1]);
	if(!inFile)
	{
		std::cerr << "Cannot open " << argv[1] << std::endl;
		return 1;
	}

	std::vector<std::pair<std::string, int> > wordPairs;
	std::map<char, int> letterWeight;

	std::string str;

	while(inFile >> str)
	{
		wordPairs.push_back(std::make_pair(str, 0));
		for (int i = 0; i < str.size(); ++i)
		{
			letterWeight[str[i]]++;
		}
	}

	for (int i = 0; i < wordPairs.size(); ++i)
	{
		std::string tw = wordPairs[i].first;
		for (int j = 0; j < tw.size(); ++j)
		{
			wordPairs[i].second += letterWeight[tw[j]];
		}
	}

	std::sort(wordPairs.begin(), wordPairs.end(), valSort);

	std::vector<std::string> words;
	std::map<char, std::vector<std::string> > wordMap;
	for (int i = 0; i < wordPairs.size(); ++i)
	{
		words.push_back(wordPairs[i].first);
		wordMap[words[i][0]].push_back(words[i]);
	}

	solve(words, wordMap);
	return 0;
}