#ifndef PRIORITY_QUEUE_WITH_HASH_H_INCLUDED
#define PRIORITY_QUEUE_WITH_HASH_H_INCLUDED

#include <cassert>
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>

//updated version of DistancePixel_PriorityQueue using hash tables (unordered_map's) instead of maps to improve the running time to log(1)

// ASSIGNMENT: The class implementation is incomplete.  Finish the
//   implementation of this class, and add any functions you need.

// =========================================================================

class DistancePixel_PriorityQueue_with_hash {

 public:

  // --------------------------
  // CONSTRUCTORS
  // default constructor
  DistancePixel_PriorityQueue_with_hash() {}
  // construct a heap from a vector of data
  DistancePixel_PriorityQueue_with_hash(const std::vector<DistancePixel*> &values) {
    for(int x = 0; x < values.size(); x++)
        this->push(values[x]);
  }

  // ------------------------
  // ACCESSORS
  int size() { return m_heap.size(); }
  bool empty() { return m_heap.empty(); }
  int last_non_leaf() { return (size()-1) / 2; }
  int get_parent(int i) { assert (i > 0 && i < size()); return (i-1) / 2; }
  int has_left_child(int i) { return (2*i)+1 < size(); }
  int has_right_child(int i) { return (2*i)+2 < size(); }
  int get_left_child(int i) { assert (i >= 0 && has_left_child(i)); return 2*i + 1; }
  int get_right_child(int i) { assert (i >= 0 && has_right_child(i)); return 2*i + 2; }

  // read the top element
  const DistancePixel* top() const  {
    assert(!m_heap.empty());
    return m_heap[0];
  }

  // is this element in the heap?
  bool in_heap(DistancePixel* element) const {
    std::unordered_map<DistancePixel*,int>::const_iterator itr = backpointers.find(element);
    return (itr != backpointers.end());
  }

  // add an element to the heap
  void push(DistancePixel* element) {
    std::unordered_map<DistancePixel*,int>::iterator itr = backpointers.find(element);
    assert (itr == backpointers.end());
    m_heap.push_back(element);
    backpointers[element] = m_heap.size()-1;
    this->percolate_up(int(m_heap.size()-1));
  }

  // the value of this element has been edited, move the element up or down
  void update_position(DistancePixel* element) {
    std::unordered_map<DistancePixel*,int>::iterator itr = backpointers.find(element);
    assert (itr != backpointers.end());
    this->percolate_up(itr->second);
    this->percolate_down(itr->second);
  }

  // remove the top (minimum) element
  void pop() {
    assert(!m_heap.empty());
    int success = backpointers.erase(m_heap[0]);
    assert (success == 1);
    m_heap[0] = m_heap.back();
    m_heap.pop_back();
    this->percolate_down(0);
  }

 private:

  // REPRESENTATION
  // the heap is stored in a vector representation (the binary tree
  // structure "unrolled" one row at a time)
  std::vector<DistancePixel*> m_heap;
  // the map stores a correpondence between elements & indices in the heap
  std::unordered_map<DistancePixel*,int> backpointers;

  // private helper functions
  void percolate_up(int i) {  //order notation for this is log(n)
    if(i > 0){ //if the element is not at the top level, continue to search up (to the left in the vector)
        if(m_heap[(i-1)/2]->getValue() > m_heap[i]->getValue()){ //if the parent is larger than the current DistancePixel, switch them in both the heap and backpointers
            DistancePixel* prevParent = m_heap[(i-1)/2];  //temporary variables
            int prevParentIndex = backpointers[prevParent];
            backpointers[prevParent] == backpointers[m_heap[i]];  //switch the backpointers for the old parent and the to-be moved up rolling node
            backpointers[m_heap[i]] = prevParentIndex;
            m_heap[(i-1)/2] = m_heap[i]; //switch the values
            m_heap[i] = prevParent;
            percolate_up((i-1)/2); //continue to go up as far as possible (look up swap)
        }
    }
  }

  void percolate_down(int i) {
        if((2*i+2) < m_heap.size()){ //if both left and right children exist...
            int leftChildIndex = get_left_child(i);
            int rightChildIndex = get_right_child(i);
            if(m_heap[leftChildIndex]->getValue() > m_heap[rightChildIndex]->getValue() && m_heap[i]->getValue() > m_heap[rightChildIndex]->getValue()){ //if the right child is smaller than the left and parent, switch
                DistancePixel* rightChild = m_heap[rightChildIndex];  //temp variable
                backpointers[rightChild] = backpointers[m_heap[i]];
                backpointers[m_heap[i]] = rightChildIndex;
                m_heap[rightChildIndex] = m_heap[i];  //switch nodes and keep going down
                m_heap[i] = rightChild;
                percolate_down(rightChildIndex);
            }
            else if(m_heap[i]->getValue() > m_heap[leftChildIndex]->getValue()){ //else if the left child is smaller, switch
                DistancePixel* leftChild = m_heap[leftChildIndex];  //temp variable
                backpointers[leftChild] = backpointers[m_heap[i]];
                backpointers[m_heap[i]] = leftChildIndex;
                m_heap[leftChildIndex] = m_heap[i];  //switch nodes and keep going down
                m_heap[i] = leftChild;
                percolate_down(leftChildIndex);
            }
        }
        else if((2*i+1) < m_heap.size()){  //else if only the left child exists...
            int leftChildIndex = get_left_child(i);
            if(m_heap[i]->getValue() > m_heap[leftChildIndex]->getValue()){ //if the left child is smaller, switch
                DistancePixel* leftChild = m_heap[leftChildIndex];  //temp variable
                backpointers[leftChild] = backpointers[m_heap[i]];
                backpointers[m_heap[i]] = leftChildIndex;
                m_heap[leftChildIndex] = m_heap[i];  //switch nodes and keep going down
                m_heap[i] = leftChild;
                percolate_down(leftChildIndex);
            }
        }
  }
};

#endif // PRIORITY_QUEUE_WITH_HASH_H_INCLUDED
