#include <iostream>
#include <list>

// ===========================================================================

// A SIMPLE LINKED LIST CLASS
// (note: it's ok that all the member variables are private for this tiny class)

class Node {
public:
  int value;
  Node* ptr;
};


// ===========================================================================

// a helper function to print the contents of a linked list of Nodes
void print_linked_list(const std::string &name, Node *my_linked_list) {
  // print the name of this list
  std::cout << name;
  // use a temporary pointer to walk down the list
  Node *tmp = my_linked_list;
  while (tmp != NULL) {
    // print out each element
    std::cout << " [" << tmp->value << "]";
    tmp = tmp->ptr;
  }
  std::cout << std::endl;
}


// YOU NEED TO WRITE THIS FUNCTION
Node* make_linked_list_from_STL_list( std::list<int> &lst) {

  // create a linked list chain of Nodes, populating the structure
  // with the data in the STL list

  // NOTE: this code should work for input STL lists of arbitrary
  // length
  Node *head;
  Node *prev_pntr;
  bool firstp = true;
  std::list<int>::iterator it;
  for(it = lst.begin(); it != lst.end(); it++){
    Node *curr_node = new Node;
    curr_node->value = *it;
    curr_node->ptr = NULL;
    prev_pntr->ptr = curr_node;
    prev_pntr = curr_node;
    if(firstp){
        head = curr_node;
        firstp = false;
    }
  }
  return head;
}

Node* fetch_new_head(Node *input){
    if(input->ptr == NULL){
        return input;
    }
    else
        fetch_new_head(input->ptr);
}

Node* reverse_nodes(Node *input){
    if(input->ptr == NULL){
    return input;
  }
  else{
    reverse_nodes(input->ptr)->ptr = input;
    input->ptr = NULL;
    return input;
  }
}

// YOU NEED TO WRITE THIS FUNCTION
Node* reverse_nodes_in_linked_list(Node *input) {

  // rearrange the pointers in the linked list chain of Nodes,
  // so that the structure is reversed.

  // NOTE: Do not edit the values of the Nodes.  Do not create any new
  // Node objects (don't call 'new').  Instead, change the ptrs of the
  // existing Nodes to shuffle the order.

  Node* new_head = fetch_new_head(input);
  reverse_nodes(input);
  return new_head;


}


// ===========================================================================

int main() {

  // manually create a linked list of notes with 4 elements
  Node* a = new Node;
  a->value = 6;
  a->ptr = new Node;
  a->ptr->value = 7;
  a->ptr->ptr = new Node;
  a->ptr->ptr->value = 8;
  a->ptr->ptr->ptr = new Node;
  a->ptr->ptr->ptr->value = 9;
  a->ptr->ptr->ptr->ptr = NULL;
  // print out this list
  print_linked_list("a",a);

  // create an STL list with 4 elements
  std::list<int> b;
  b.push_back(10);
  b.push_back(11);
  b.push_back(12);
  b.push_back(13);

  // use the STL list as input to a creator function that creates
  // linked lists with the same data
  Node* c = make_linked_list_from_STL_list(b);
  // print that data
  print_linked_list("c",c);

  //
  // WRITE A FEW MORE TEST CASES OF make_linked_lists_from_STL_list
  //

  // reverse a linked list of nodes

  Node* d = reverse_nodes_in_linked_list(c);
  // print this data
  print_linked_list("d",d);

  //
  // WRITE A FEW MORE TEST CASES OF reverse_nodes_in_linked_list
  //


  // NOTE: We are not deleting any of the Nodes we created...  so this
  //       program has some memory leaks!  More on this in future
  //       lectures & labs :)

}

// ===========================================================================
