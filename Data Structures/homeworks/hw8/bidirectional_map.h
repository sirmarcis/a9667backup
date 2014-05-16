#ifndef BIDIRECTIONAL_MAP_H_INCLUDED
#define BIDIRECTIONAL_MAP_H_INCLUDED

#include<iostream>
#include <vector>
#include <utility>

template<class T, class V> class Node{
public:
    Node() : left(NULL), right(NULL), parent(NULL) {}
    Node(const T& init) : value(init), left(NULL), right(NULL), parent(NULL){}
    Node* operator= (const Node* newNode);

    const T& getValue() const { return value; }
    void setValue(const T& newVal) { value = newVal; }
    Node* getLink() const { return link; }

    Node* left;
    Node* right;
    Node* parent;
    Node<V, T>* link;

    T value;
};

template<class T, class V> class bidirectional_iterator{
public:
    bidirectional_iterator() : ptr_(NULL) {}
    bidirectional_iterator(Node<T, V>* p) : ptr_(p) {}
    bidirectional_iterator(const bidirectional_iterator& old) : ptr_(old.ptr_) {}
    ~bidirectional_iterator() {}
    bidirectional_iterator& operator=(const bidirectional_iterator& old) { ptr_ = old.ptr_; return *this; }
    const std::pair<T, V> operator*() const{ return std::make_pair(ptr_->value, ptr_->link->value); }
    bidirectional_iterator<T,V>& operator++() { //taken from lab 11
        if (ptr_->right != NULL) { // find the leftmost child of the right node
            ptr_ = ptr_->right;
            while (ptr_->left != NULL) { ptr_ = ptr_->left; }
        } else { // go upwards along right branches...  stop after the first left
            while (ptr_->parent != NULL && ptr_->parent->right == ptr_) { ptr_ = ptr_->parent; }
            ptr_ = ptr_->parent;
        }
        return *this;
    }
    bidirectional_iterator<T,V> operator++(int){
        bidirectional_iterator<T,V> temp(*this);
        ++(*this);
        return temp;
    }
    bidirectional_iterator<T,V>& operator--(){ //method the same as lab11, the "phantom node" method
        if(ptr_->left != NULL){
            ptr_ = ptr_->left;
            while(ptr_->right != NULL) { ptr_ = ptr_->right; }
        }
        else{
            while(ptr_->parent != NULL && ptr_->parent->left == ptr_) { ptr_ = ptr_->parent; }
            ptr_ = ptr_->parent;
        }
        return *this;
    }
    bidirectional_iterator<T,V> operator--(int){
        bidirectional_iterator<T,V> temp(*this);
        --(*this);
        return temp;
    }
    bidirectional_iterator<V,T> follow_link() { return bidirectional_iterator<V,T>(ptr_->link); }
    Node<V,T>* getLink() { return ptr_->link; } //minor useful function, feel free to ignore ;) (i could replace this with follow_link, but I'm lazy)

    bool operator==(const bidirectional_iterator& rgt) { return ptr_ == rgt.ptr_; }
    bool operator!=(const bidirectional_iterator& rgt) { return ptr_ != rgt.ptr_; }

private:
    Node<T, V>* ptr_;
};

template <class T, class V> class bidirectional_map{
public:
    bidirectional_map() : key_root_(NULL), value_root_(NULL), size_(0) { keyEndElt = new Node<T,V>(); valueEndElt = new Node<V,T>(); }
    bidirectional_map(const bidirectional_map<T ,V>& old) : size_(old.size_) {
        if(old != *this){
            //std::cout <<"got to here" <<std::endl;
            std::pair<Node<T, V>*, Node<V, T>* > new_roots = copy_tree(old.key_root_, old.value_root_, NULL, NULL);
            key_root_ = new_roots.first;
            value_root_ = new_roots.second;
            size_ = old.size_;
        }
    }
    ~bidirectional_map() { this->destroy_map(key_root_, value_root_); }
    bidirectional_map& operator=(const bidirectional_map& old) {
        if(old != *this){
            this->destroy_map(key_root_, value_root_);
            std::pair<Node<T, V>*, Node<V, T>* > new_roots = copy_tree(old.key_root_, old.value_root_, NULL, NULL);
            key_root_ = new_roots.first;
            value_root_ = new_roots.second;
            size_ = old.size_;
        }
        return *this;
    }
    V& operator[](const T& key_value);
    const V& operator[](const T& key_value) const;
    T& operator[](const V& key_value);
    const T& operator[](const V& key_value) const;

    typedef bidirectional_iterator<T,V> key_iterator;
    typedef bidirectional_iterator<V,T> value_iterator;
    int size() const { return size_; }
    bool operator!=(const bidirectional_map& old) const { return (old.key_root_ != this->key_root_ && old.value_root_ != this->value_root_); }
    bool operator==(const bidirectional_map& old) const { return (old.key_root_ == this->key_root_ && old.value_root_ == this->value_root_); }
    //these are the find, insert, and erase functions

    key_iterator find(const T& key_value) { return find1(key_value, key_root_); } //find a key
    bidirectional_iterator<V,T> find(const V& key_value) { return find2(key_value, value_root_); }  //find a value
    std::pair<key_iterator, bool> insert(const std::pair<T, V>& insert_pair) { return insert(insert_pair, key_root_, value_root_, NULL); }
    int erase(T const& key_value) { return erase(key_value, key_root_); }

    key_iterator key_begin() const{
        if(!key_root_) return key_iterator(NULL);
        Node<T, V>* p = key_root_;
        while(p->left) p = p->left;
        return key_iterator(p);
    }
    key_iterator key_end() const { //method the same as lab11, the "phantom node" method
        if(!key_root_) return key_iterator(NULL);
        Node<T,V>* p = key_root_;
        while (p->right) p = p->right;
        if(p != keyEndElt){
            keyEndElt->parent = p;
            p->right = keyEndElt;
        }
        return key_iterator(keyEndElt);
    }
    bidirectional_iterator<V,T> value_begin() const{
        if(!value_root_) return bidirectional_iterator<V,T>(NULL);
        Node<V,T>* p = value_root_;
        while(p->left) p = p->left;
        return bidirectional_iterator<V,T>(p);
    }
    bidirectional_iterator<V,T> value_end() const { //method the same as lab11, the "phantom node" method
        if(!value_root_) return bidirectional_iterator<V,T>(NULL);
        Node<V,T>* p = value_root_;
        while (p->right) p = p->right;
        if(p != valueEndElt){
            valueEndElt->parent = p;
            p->right = valueEndElt;
        }
        return bidirectional_iterator<V,T>(valueEndElt);
    }

    void destroy(){
        destroy_map(key_root_, value_root_);
    }

    void destroy_map(Node<T,V>* p, Node<V,T>* q){
        killKeys(p);
        killValues(q);
    }

    void print(std::ostream& ostr) const { //the print function
        ostr << "key tree: " << std::endl;
        printSideways(ostr, key_root_, 0);
        ostr << std::endl << "value tree: " <<std::endl;
        printSideways2(ostr, value_root_, 0); }
private:
    Node<T, V>* key_root_;
    Node<V, T>* value_root_;

    //end tracers used to help with iteration, each represents the end of their respective trees
    Node<T,V>* keyEndElt;
    Node<V,T>* valueEndElt;
    int size_;

    std::pair<Node<T, V>*, Node<V, T>* > copy_tree(Node<T, V>* old_key_root, Node<V, T>* old_value_root,
                                                   Node<T, V>* key_parent, Node<V, T>* value_parent){  //this is long as shit, but still needed
        Node<T, V>* new_key_node = old_key_root;
        Node<V, T>* new_value_node = old_value_root;
        if(old_key_root && old_value_root){ //if both pointers are not null, copy hte current level
            new_key_node = new Node<T, V>();  //make new nodes for both key and value trees
            new_value_node = new Node<V, T>();
            new_key_node->value = old_key_root->value;
            new_value_node->value = old_value_root->value;
            new_key_node->link = new_value_node; //make new links between the new nodes, pointing at each other
            new_value_node->link = new_key_node;
            std::pair<Node<T, V>*, Node<V, T>* > left_nodes = copy_tree(old_key_root->left, old_value_root->left, new_key_node, new_value_node); //go deeper...
            std::pair<Node<T,V>*, Node<V,T>* > right_nodes = copy_tree(old_key_root->right, old_value_root->right, new_key_node, new_value_node);
            new_key_node->left = left_nodes.first; // relay over the newly found values
            new_value_node->left = left_nodes.second;
            new_key_node->right = right_nodes.first;
            new_value_node->right = right_nodes.second;
            new_key_node->parent = key_parent; //set the parent nodes for both trees
            new_value_node->parent = value_parent;
        }
        return std::make_pair(new_key_node, new_value_node); //return the newly made node pair
    }
    void killKeys(Node<T,V>* p){ //called by destroy map to delete the key tree
        if(p != NULL){
            killKeys(p->left);
            killKeys(p->right);
            delete p;
        }
    }
    void killValues(Node<V, T>* p){ //called by destroy map to delete the value tree
        if(p != NULL){
            killValues(p->left);
            killValues(p->right);
            delete p;
        }
    }

    key_iterator find1(const T& search_value, Node<T,V>* p){ //recursivly finds a key node
        if(!p) return key_iterator(keyEndElt);
        if (p->value > search_value)
            return find1(search_value, p->left);
        else if (p->value < search_value)
            return find1(search_value, p->right);
        else
            return key_iterator(p);
    }

    bidirectional_iterator<V,T> find2(const V& search_value, Node<V,T>* p){ //recursivly finds a key node
        if(!p || p == valueEndElt) return bidirectional_iterator<V,T>(valueEndElt);
        if (p->value > search_value)
            return find2(search_value, p->left);
        else if (p->value < search_value)
            return find2(search_value, p->right);
        else
            return bidirectional_iterator<V,T>(p);
    }

    Node<V,T>* insertValue(const V& val, Node<V,T>*& value_root, Node<V,T>* value_parent){ //called by insert to add the value to the sppropriate point in the value tree
        if(!value_root || value_root == valueEndElt){
            value_root = new Node<V,T>(val);
            value_root->parent = value_parent;
            return value_root;
        }
        else if(val < value_root->value)
            return insertValue(val, value_root->left, value_root);
        else if(val > value_root->value)
            return insertValue(val, value_root->right, value_root);
        else
            return NULL;
    }

    std::pair<key_iterator, bool> insert(const std::pair<T, V>& insert_pair, Node<T,V>*& key_root, Node<V,T>*& value_root, Node<T,V>* key_parent){
        if(!key_root || key_root == keyEndElt){ //if its at he end of a branch, make a new node
            key_root = new Node<T,V>(insert_pair.first);
            Node<V,T>* newValNode = insertValue(insert_pair.second, value_root, NULL);
            key_root->link = newValNode; //make the links between trees
            newValNode->link = key_root;
            key_root->parent = key_parent;
            size_++;
            return std::pair<key_iterator, bool>(key_root, true);
        }
        else if(insert_pair.first < key_root->value) //else, recurse down where it should depending on the key value
            return insert(insert_pair, key_root->left, value_root, key_root);
        else if(insert_pair.first > key_root->value)
            return insert(insert_pair, key_root->right, value_root, key_root);
        else
            return std::pair<key_iterator, bool>(key_iterator(key_root), false); //if theres a node thats already there, don't add it
    }

    void eraseValue(V const& key_value, Node<V,T>* &p){  //taken from lab11, come at me bro
        if(!p) return;

        if(p->value < key_value)
            return eraseValue(key_value, p->right);
        else if(p->value > key_value)
            return eraseValue(key_value, p->left);
        if (!p->left && !p->right) { // leaf
            delete p;
            p=NULL;
        }
        else if (!p->left) { // no left child
            Node<V,T>* q = p;
            p=p->right;
            assert (p->parent == q);
            p->parent = q->parent;
            delete q;
        }
        else if (!p->right) { // no right child
            Node<V,T>* q = p;
            p=p->left;
            assert (p->parent == q);
            p->parent = q->parent;
            delete q;
        }
        else { // Find rightmost node in left subtree
            Node<V,T>* q = p->left;
            while (q->right) q = q->right;
            p->value = q->value;
            // recursively remove the value from the left subtree
            eraseValue(q->value, p->left);
        }
    }

    int erase(T const& key_value, Node<T,V>* &p){  //taken from lab11
        if(!p) return 0;

        if(p->value < key_value)
            return erase(key_value, p->right);
        else if(p->value > key_value)
            return erase(key_value, p->left);

        assert(p->value == key_value);
        eraseValue(p->link->value, value_root_);
        if (!p->left && !p->right) { // leaf
            delete p;
            p=NULL;
            this->size_--;
        }
        else if (!p->left) { // no left child
            Node<T,V>* q = p;
            p=p->right;
            assert (p->parent == q);
            p->parent = q->parent;
            delete q;
            this->size_--;
        }
        else if (!p->right) { // no right child
            Node<T,V>* q = p;
            p=p->left;
            assert (p->parent == q);
            p->parent = q->parent;
            delete q;
            this->size_--;
        }
        else { // Find rightmost node in left subtree
            Node<T,V>* q = p->left;
            while (q->right) q = q->right;
            p->value = q->value;
            // recursively remove the value from the left subtree
            int check = erase(q->value, p->left);
            assert (check == 1);
        }
        return 1;
    }

    void printSideways(std::ostream& ostr, const Node<T, V>* p, int depth) const { //used to print the key tree
        if(p && p != keyEndElt){
            printSideways(ostr, p->right, depth+1);
            for (int i=0; i<depth; ++i) ostr << "    ";
            ostr << p->value << "\n";  //this gives a memory leak, I dont know why
            printSideways(ostr, p->left, depth+1);
        }
    }
    void printSideways2(std::ostream& ostr, const Node<V,T>* p, int depth) const {  //used to print the value tree
        if(p && p != valueEndElt){
            printSideways2(ostr, p->right, depth+1);
            for (int i=0; i<depth; ++i) ostr << "    ";
            ostr << p->value << ", " << p->link->value << "\n";
            printSideways2(ostr, p->left, depth+1);
        }
    }
};

//the two following methods deal with accessing a key node
template <class T, class V> V& bidirectional_map<T,V>::operator[](const T& key_value){
    bidirectional_map<T,V>::key_iterator it = find(key_value);
    if(it != NULL)
        return it.getLink()->value;
}

template <class T, class V> const V& bidirectional_map<T,V>::operator[](const T& key_value) const{
    bidirectional_map<T,V>::key_iterator it = find(key_value);
    if(it != NULL)
        return it.getLink()->value;
}

//the two following methods deal with accessing a value node
template <class T, class V> T& bidirectional_map<T,V>::operator[](const V& key_value){
    bidirectional_iterator<V,T> it = find(key_value);
    if(it != NULL)
        return it.getLink()->value;
}

template <class T, class V> const T& bidirectional_map<T,V>::operator[](const V& key_value) const{
    bidirectional_iterator<V,T> it = find(key_value);
    if(it != NULL)
        return it.getLink()->value;
}


#endif // BIDIRECTIONAL_MAP_H_INCLUDED
