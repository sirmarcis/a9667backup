// ========================================================


void print_breadth_first(TreeNode<int>* root) {
  if (root == NULL) return;
  // current will store current tier
  std::list<TreeNode<int>*> current;
  current.push_back(root);
  // next will store next tier
  std::list<TreeNode<int>*> next;
  int count = 0;
  while (current.size() != 0) {
     // print one row & prepare the next row
    cout << "TIER " << count << " ";
    std::list<TreeNode<int>*>::iterator itr = current.begin();
    while (itr != current.end()) {
      // print node
      cout << (*itr)->value << " ";
      // add nodes children (if they exist)
      if ((*itr)->left != NULL) {
        next.push_back((*itr)->left);  }
      if ((*itr)->right != NULL) {
        next.push_back((*itr)->right);  }
      itr++;
    }
    count++;    
    current = next;
    next.clear();
  }
}


// ========================================================
// DS SET MEMBER FUNCTIONS


  void destroy_tree(TreeNode<T>* p) { 
    /* Implemented in Lecture 17 */ 
    if (p != NULL) {
      destroy_tree(p->left);   
      destroy_tree(p->right);
      delete p;
    }
  }


  int erase(T const& key_value, TreeNode<T>* &p) { 
    /* Implemented in Lecture 17 */ 
    if (p == NULL) {
      // value not found
      return 0;
    }
    if (p->value < key_value) {
      erase(key_value, p->right);
    } else if (p->value > key_value) {
      erase(key_value, p->left);
    } else {
      assert (p->value == key_value);
      // no children
      if (p->left == NULL && p->right == NULL) {
        delete p;
        p = NULL;
        size_--;
        return 1;
      }
      // only one child  
      else if (p->left == NULL) {
        TreeNode<T> *tmp = p->right;
        delete p;
        p = tmp;
        size_--;
        return 1;
      } else if (p->right == NULL) {
        TreeNode<T> *tmp = p->left;
        delete p;
        p = tmp;
        size_--;
        return 1;
      } else {
        // both children non null
        TreeNode<T>* &tmp = p->right;
        while (tmp->left != NULL) {
          tmp = tmp->left; }
        p->value = tmp->value;
        return erase(tmp->value,tmp);
      }
    }
  }


// ========================================================
//
// TREE_ITERATOR MEMBER FUNCTIONS
// (version requiring parent pointers)
//
// We will discuss this code in lecture 18!
//
// Note: pre- vs. post- increment are differentiated 
//    by the dummy integer argument
//

  // pre-increment operator, ++itr
  tree_iterator<T> & operator++() { 
    /* Implemented in Lecture 18 */ 
    assert (ptr_ != NULL);
    if (ptr_->right != NULL) { // find the leftmost child of the right node
      ptr_ = ptr_->right;
      while (ptr_->left != NULL) { ptr_ = ptr_->left; }
    } else { // go upwards along right branches...  stop after the first left
      while (ptr_->parent != NULL && ptr_->parent->right == ptr_) { 
        ptr_ = ptr_->parent; 
      }
      ptr_ = ptr_->parent;
    }
    return *this;
  }

  // post-increment operator, itr++
  tree_iterator<T> operator++(int) {
    tree_iterator<T> temp(*this);
    ++(*this);
    return temp;
  }


// ========================================================
