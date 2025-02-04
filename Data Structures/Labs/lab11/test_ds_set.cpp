#include <iostream>
#include <string>
#include <utility>
#include <cassert>

#include "ds_set.h"

int main() {

  ds_set<std::string> set1;
  set1.insert("hello");
  set1.insert("good-bye");
  set1.insert("friend");
  set1.insert("abc");
  set1.insert("puppy");
  set1.insert("zebra");
  set1.insert("daddy");
  set1.insert("puppy");  // should recognize that this is a duplicate!

  assert(set1.sanity_check());
  assert (set1.size() == 7);

  ds_set<std::string>::iterator p = set1.begin();
  assert(p != set1.end() && *p == std::string("abc"));

  p = set1.find( "foo" );
  assert (p == set1.end());

  p = set1.find("puppy");
  assert (p != set1.end());
  assert (*p == "puppy");

  std::cout << "Here is the tree, printed sideways.\n"
	    << "The indentation is proportional to the depth of the node\n"
	    << "so that the value stored at the root is the only value printed\n"
	    << "without indentation.  Also, for each node, the right subtree\n"
	    << "can be found above where the node is printed and indented\n"
	    << "relative to it\n";
  set1.print_as_sideways_tree( std::cout );
  std::cout << std::endl;


  // copy the set
  ds_set<std::string> set2( set1 );
  assert(set1.sanity_check());
  assert(set2.sanity_check());
  assert(set1.size() == set2.size());


  //  Now add stuff to set2
  set2.insert( std::string("a") );
  set2.insert( std::string("b") );
  std::cout << "After inserting stuff:\n";
  set2.print_as_sideways_tree( std::cout );
  std::cout << std::endl;
  assert(set1.sanity_check());
  assert(set2.sanity_check());
  assert(set1.size() == set2.size() - 2);


  //  Now erase stuff from set2
  set2.erase("hello");
  set2.erase("a");
  set2.erase("hello"); // should recognize that it's not there anymore!
  set2.erase("abc");
  set2.erase("friend");
  std::cout << "After erasing stuff:\n";
  set2.print_as_sideways_tree( std::cout );
  std::cout << std::endl;
  assert(set1.sanity_check());
  assert(set2.sanity_check());
  assert(set1.size() == set2.size() + 2);

    ds_set<std::string>::iterator it;
    for(it = set1.begin(); it != set1.end(); it++)
        std::cout << "current word going forward is " << *it << std::endl;
    it = set1.end();
    while(it != set1.begin()){
        it--;
        std::cout << "current word going backward is " << *it << std::endl;
    }
    ds_set<int> set3;
    for(int x = 8; x > 0; x--)
        set3.insert(x);
    set3.end();
    for(int x = 9; x <16; x++)
        set3.insert(x);
    set3.print_as_sideways_tree(std::cout);
    std::string overallVal;
    set1.accumulate(overallVal);
    std::cout << std::endl << "accumulated value for set1 is: " << overallVal <<std::endl;
    int set3Accum = 0;
    set3.accumulate(set3Accum);
    std::cout << "accumulated value for set3 is: " << set3Accum <<std::endl;


  return 0;
}
