#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

//#include "lamp.h"

using namespace std;

vector<int> make_change(vector<int> coins, unsigned int n){
    if(n == 1)
        return coins;
    else{
        vector<int> final_vec;
        vector<int> sec_vec = make_change(coins, n-1);
        for(int h = 0; h < coins.size(); h++){
            for(int k = 0; k < sec_vec.size(); k++){
                int curr_num = coins[h] + sec_vec[k];\
                bool add_to_vec = true;
                for(int i = 0; i < final_vec.size(); i++)
                    if(final_vec[i] == curr_num)
                        add_to_vec = false;
                if(add_to_vec)
                    final_vec.push_back(curr_num);
            }
        }
        sort(final_vec.begin(), final_vec.end());
        return final_vec;
    }
}

int main(){

    vector<int> test_vec;
    test_vec.push_back(1);
    test_vec.push_back(5);
    test_vec.push_back(10);
    test_vec = make_change(test_vec, 2);
    for (int i = 0; i < test_vec.size(); ++i)
    {
        cout<< test_vec[i] << ", ";
    }
    cout << endl;

    /*
    //input
    vector<vector<string> >input;
    vector<string> newLine;
    newLine.push_back("the"); newLine.push_back("hungry"); newLine.push_back("cat");
    input.push_back(newLine);
    newLine.clear();
    newLine.push_back("the"); newLine.push_back("mouse");
    input.push_back(newLine);
    newLine.clear();
    newLine.push_back("the"); newLine.push_back("fat"); newLine.push_back("cat");
    input.push_back(newLine);
    newLine.clear();
    newLine.push_back("goodbye");
    input.push_back(newLine);

    //given code
    map<string, vector<int> > words_to_lines;
    int line_number = 0;
    for(int x = 0; x < input.size(); x++){
        line_number++;
        for(vector<string>::iterator p = input[x].begin(); p != input[x].end(); p++){
            map<string, vector<int> >::iterator map_itr = words_to_lines.find(*p);
            if(map_itr == words_to_lines.end())
                words_to_lines[*p].push_back(line_number);
            else if(map_itr->second.back() != line_number)
                map_itr->second.push_back(line_number);
        }
    }

    //test code
    map<string, vector<int> >::iterator it;
    for(it = words_to_lines.begin(); it != words_to_lines.end(); it++){
        vector<int> word_to_line_locs = it->second;
        map<string, vector<int> >::iterator sec_it;
        cout << it->first << ": ";

        //for(int x = 0; x < word_to_line_locs.size(); x++)
          //  cout << word_to_line_locs[x] << ", ";

        map<string, int> used_words;
        for(sec_it = words_to_lines.begin(); sec_it != words_to_lines.end(); sec_it++){
            if(it != sec_it){
                vector<int> roaming_locs = sec_it->second;
                for(int h = 0; h < word_to_line_locs.size(); h++){
                    for(int k = 0; k < roaming_locs.size(); k++){
                        if(word_to_line_locs[h] == roaming_locs[k] && used_words.find(sec_it->first) == used_words.end()){
                            cout << sec_it->first << " ";
                            used_words[sec_it->first] = 1;
                        }
                    }
                }
            }
        }
        cout <<endl;
    }
    */



    return 0;
}
