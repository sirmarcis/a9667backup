#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cassert>
#include <iomanip>
#include <cmath>
#include <map>
#include <utility>

// ======================================================================================

// Helper function to read the provided font from a file.  The format
// of the font file is described in comments below.  The width,
// height, and bitmap_letters variables are set by this function.

std::vector<char>
ReadFont2(const std::string &font_file,
	      int &width,
	      int &height,
	      std::vector<std::vector<std::string> > &bitmap_letters
	      ) {
    //called by main, makes the bitmap_letters an array filled with all the chars respective bitmaps, also returning a corresponding array of chars for refrence
    // open the font file for reading
    std::ifstream istr(font_file.c_str());
    std::vector<char> char_arr;
    if (!istr) {
        std::cerr << "ERROR: cannot open font file " << font_file << std::endl;
        return char_arr;
    }

  // read in the width & height for every character in the file
  istr >> width >> height;
  width += 1;
  assert (width > 1);
  assert (height >= 1);

  // Create a vector to store all 256 ASCII characters of the
  // characters.  Each character is represented as a vector of
  // <height> strings that are each <width> wide.  Initially the
  // characters are unknown (represented with the '?' character).
  //bitmap_letters = std::vector<std::vector<std::string> >
  //  ( 256, std::vector<std::string> ( height, std::string(width, '?')));

  // read in all the characters
  // first is the ascii integer representation of the character
  int ascii;
  while (istr >> ascii) {
    assert (ascii >= 0 && ascii < 256);
    // next the character is printed in single quotes
    char c;
    istr >> c;
    assert (c == '\'');
    // use std::noskipws to make sure we can read the space character correctly
    istr >> std::noskipws >> c;
    // verify that the ascii code matches the character
    assert (c == (char)ascii);
    // switch back to std::skipws mode
    istr >> std::skipws >> c;
    assert (c == '\'');
    // read in the letter
    std::vector<std::string> bitmap;
    std::string tmp;
    for (int i = 0; i < height; i++) {
      istr >> tmp;
      tmp.push_back('.');
      assert ((int)tmp.size() == width);
      // make sure the letter uses only '#' and '.' characters
      for (unsigned int j = 0; j < tmp.size(); j++) {
        assert (tmp[j] == '.' || tmp[j] == '#');
      }
      bitmap.push_back(tmp);
    }
    c  = char(ascii);
    //std::cout << "a char added is: " << c << std::endl;
    char_arr.push_back(c);
    // overwrite the initially unknown letter in the vector
    bitmap_letters.push_back(bitmap);

  }
  return char_arr;
}

void ReadFont(const std::string &font_file,
	      int &width,
	      int &height,
	      int &numChars,
	      std::map<char, std::vector<std::string> > &bitmap_map) {
    //called by main, fills a hash table (or whatever the hell it is in c++) with the bitmap for each char, with the char being the key
    // open the font file for reading
    std::ifstream istr(font_file.c_str());
    if (!istr) {
        std::cerr << "ERROR: cannot open font file " << font_file << std::endl;
        return;
    }

  // read in the width & height for every character in the file
  istr >> width >> height;
  width += 1;
  assert (width >= 1);
  assert (height >= 1);

  // Create a vector to store all 256 ASCII characters of the
  // characters.  Each character is represented as a vector of
  // <height> strings that are each <width> wide.  Initially the
  // characters are unknown (represented with the '?' character).
  std::vector<std::vector<std::string> > bitmap_letters = std::vector<std::vector<std::string> >
    ( 256, std::vector<std::string> ( height, std::string(width, '?')));

  // read in all the characters
  // first is the ascii integer representation of the character
  int ascii;
  while (istr >> ascii) {
    assert (ascii >= 0 && ascii < 256);
    // next the character is printed in single quotes
    char c;
    istr >> c;
    assert (c == '\'');
    // use std::noskiwps to make sure we can read the space character correctly
    istr >> std::noskipws >> c;
    // verify that the ascii code matches the character
    assert (c == (char)ascii);
    // switch back to std::skipws mode
    istr >> std::skipws >> c;
    assert (c == '\'');
    // read in the letter
    std::vector<std::string> bitmap;
    std::string tmp;
    for (int i = 0; i < height; i++) {
      istr >> tmp;
      tmp.push_back('.');
      assert ((int)tmp.size() == width);
      // make sure the letter uses only '#' and '.' characters
      for (unsigned int j = 0; j < tmp.size(); j++) {
        assert (tmp[j] == '.' || tmp[j] == '#');
      }
      bitmap.push_back(tmp);
    }
    // overwrite the initially unknown letter in the vector
    bitmap_map[char(ascii)] = bitmap;
    bitmap_letters[ascii] = bitmap;
  }
  numChars = bitmap_letters.size();
}

// ======================================================================================

void
convertBitmap(std::vector<std::string> &bitmap,
					char foreground, char backround,
					int width, int height
					){
        //called by printOutput, converts the bitmap to the user inputted foreground and backround
		std::string curr_string;
		for(int h = 0; h < height; h++){
			curr_string = bitmap[h];
			for(int k = 0; k < curr_string.size(); k++){
				char tmp = curr_string.at(k);
				if(tmp == '#')
					curr_string.at(k) = foreground;
				else if(tmp == '.'){
					curr_string.at(k) = backround;
				}
			}
			bitmap[h] = curr_string;
		}
}

void
addCharToOutput (std::vector<std::string> input_bitmap, std::vector<std::string> &output){
    for(int h = 0; h < input_bitmap.size(); h++){
        std::string output_str = output[h];
        std::string input_str = input_bitmap[h];
        output_str = output_str + input_str;
    }
}

void
printOutput(std::vector<std::vector<std::string> > output, int height){
    //called by getOutput, prints the final array, nothing too fancy
    for(int curr_height = 0; curr_height < height; curr_height++){
        for(int h = 0; h < output.size(); h++){
            std::vector<std::string> curr_bitmap = output[h];
            std::string curr_line = curr_bitmap[curr_height];
            std::cout << curr_line;
        }
        std::cout << std::endl;
    }
}

void
getOutput(std::map<char, std::vector<std::string> > &bitmap_map,
		char foreground, char backround,
		int width, int height,
		std::string &input_string){
    //called by main, is in charge of finding all the bitmaps for each letter in the input, translating their foreground and backround, and finally printing it
	std::vector<std::string> curr_bitmap;
	char curr_char;
	std::vector<std::vector<std::string> > output;
	for(int i = 0; i < input_string.size(); i++){
		curr_char = input_string.at(i);
		curr_bitmap = bitmap_map[curr_char];
		convertBitmap(curr_bitmap, foreground, backround, width, height);
		output.push_back(curr_bitmap);
	}
    printOutput(output, height);
}

float
charCnt(std::string line1){
    char char_type1 = ' ', char_type2 = ' ';
        bool char_open1 = true, char_open2 = true;
        for(int x = 0; x < line1.size(); x++){
            char curr_char = line1.at(x);
            if( (!(curr_char == char_type1) || (char_type1 == ' ')) && char_open1){
                char_type1 = curr_char;
                char_open1 = false;
            }
            else if( (!(curr_char == char_type1) || (char_type2 == ' ')) && !char_open1 && char_open2){
                char_type2 = curr_char;
                char_open2 = false;
            }
        }
        float char_cnt1 = 0, char_cnt2 = 0;
        for(int x = 0; x < line1.size(); x++){
            char curr_char = line1.at(x);
            if(curr_char == char_type1){
                char_cnt1 += 1;
            }
            else if(curr_char == char_type2){
                char_cnt2 += 1;
            }
        }
        //std::cout << "began a line compare..." << std::endl;
    return (char_cnt1 / char_cnt2);
}

bool
lineCompare(std::string line1, std::string line2){
    if(line1.size() == line2.size()){
        char char_type1 = ' ', char_type2 = ' ';
        bool char_open1 = true, char_open2 = true;
        for(int x = 0; x < line1.size(); x++){
            char curr_char = line1.at(x);
            if( (!(curr_char == char_type1) || (char_type1 == ' ')) && char_open1){
                char_type1 = curr_char;
                char_open1 = false;
            }
            else if( (!(curr_char == char_type1) || (char_type2 == ' ')) && !char_open1 && char_open2){
                char_type2 = curr_char;
                char_open2 = false;
            }
        }
        float char_cnt1 = 0, char_cnt2 = 0;
        for(int x = 0; x < line1.size(); x++){
            char curr_char = line1.at(x);
            if(curr_char == char_type1){
                char_cnt1 += 1;
            }
            else if(curr_char == char_type2){
                char_cnt2 += 1;
            }
        }
        char char_type3 = ' ', char_type4 = ' ';
        bool char_open3 = true, char_open4 = true;
        for(int x = 0; x < line2.size(); x++){
            char curr_char = line2.at(x);
            if( (!(curr_char == char_type3) || (char_type3 == ' ')) && char_open3){
                char_type3 = curr_char;
                char_open3 = false;
            }
            else if( (!(curr_char == char_type3) || (char_type4 == ' ')) && !char_open3 && char_open4){
                char_type4 = curr_char;
                char_open4 = false;
            }
        }
        float char_cnt3 = 0, char_cnt4 = 0;
        for(int x = 0; x < line2.size(); x++){
            char curr_char = line2.at(x);
            if(curr_char == char_type3){
                char_cnt3 += 1;
            }
            else if(curr_char == char_type4){
                char_cnt4 += 1;
            }
        }
        if ((char_cnt1 == char_cnt3) && (char_cnt2 == char_cnt4)){
            return true;
        }
    }
    else{
        return false;
    }
}

char
findChar(std::vector<std::string> input_bitmap,
         std::vector<char> &char_arr,
         std::vector<std::vector<std::string> > bitmap_letters){
    //called by readInputFile, attempts to find the appropriate match between bitmaps of the input and whats in the bitmap_letters array
    char theChar;
    for(int x = 0; x < bitmap_letters.size(); x++){
        std::vector<std::string> curr_bitmap = bitmap_letters[x];
        bool corresponds = false;
        bool broken = false;
        for(int h = 0; h < curr_bitmap.size(); h++){
            if(!broken && lineCompare(input_bitmap[h], curr_bitmap[h]) && (char_arr.size() > x)){
                //std::cout << lineCompare(input_bitmap[h], curr_bitmap[h]) << std::endl;
                corresponds = true;
            }
            else {
                broken = true;
                corresponds = false;
            }
        }
        if (corresponds){
            //std::cout << "matched a char, and it is at : [" << x << "]" <<std::endl;
            theChar = char_arr[x];
            return theChar;
        }
    }
    return theChar;
}

std::string
readInputFile(std::string &file_path, int &width, int height, int &numChars, std::vector<char> &char_arr, std::vector<std::vector<std::string> > bitmap_letters){
    //called by main, reads in the input file, finds the appropriate letters, and prints the resulting message
    std::ifstream ifstr(file_path.c_str());
    std::vector<std::string> input_arr;
    std::vector<std::vector<std::string> > final_arr;
    std::vector<char> output_arr;
    std::string final_str;
    numChars = 0;
    int running_cnt = 0;
    for (int x = 0; x < height; x++){
        std::string input_line;
        ifstr >> input_line;
        input_arr.push_back(input_line);
        if (numChars == 0){
            //std::cout << "length input line: " << input_line.size() << " width: " << width << std::endl;
            numChars = input_line.size() / width;
        }
    }
    //std::cout << "loaded in input from file with " << numChars << " charcters discovered" << std::endl;
    int start_pnt = 0, end_pnt = 0;
    for (int y = 0; y < numChars; y++){
        start_pnt = end_pnt;
        end_pnt = width * (y + 1);
        std::vector<std::string> curr_bitmap;
        for(int h = 0; h < input_arr.size(); h++){
            std::string input_str = input_arr[h].substr(start_pnt, width);
            //input_str.push_back('.');
            //std::cout << input_str << std::endl;
            curr_bitmap.push_back(input_str);
        }
        char to_append = findChar(curr_bitmap, char_arr, bitmap_letters);
        final_str.push_back(to_append);
        //final_arr[y] = curr_bitmap;

    }
    return final_str;
}

int
main(int argc, char* argv[]){
	if(std::string(argv[1]) == std::string("display")){
        //handles the case of the user wanting a message printed (most of this is declarations, except for the last two lines)
		std::string font_map_txt = std::string(argv[2]), output_string = std::string(argv[3]);
		std::string foreground_str = std::string(argv[4]), backround_str = std::string(argv[5]);
		char foreground = foreground_str.at(0), backround = backround_str.at(0);
		std::map<char, std::vector<std::string> > bitmap_map;
		int width, height, numChars;
		ReadFont(font_map_txt, width, height, numChars, bitmap_map);
        getOutput(bitmap_map, foreground, backround, width, height, output_string);
	}
	else if (std::string(argv[1]) == std::string("read")){
	    //handles the case of the user wanting a message desiphered
        std::string font_map_txt = std::string(argv[2]), input_bitmap_txt = std::string(argv[3]);
        std::map<char, std::vector<std::string> > bitmap_map;
        std::vector<std::vector<std::string> > bitmap;
        std::vector<std::vector<std::string> > input_map;
        std::vector<char> char_arr;
        std::string final_str;
		int width, height, numChars;
		char_arr = ReadFont2(font_map_txt, width, height, bitmap);
		final_str = readInputFile(input_bitmap_txt, width, height, numChars, char_arr, bitmap);
		std::cout << final_str << std::endl;
	}
	else std::cout << "ERROR: invalid command argument" << std::endl;
	return 0;
}
