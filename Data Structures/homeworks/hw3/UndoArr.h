#ifndef UNDOARR_H_INCLUDED
#define UNDOARR_H_INCLUDED

template <class T> class UndoArray{
public:
    //typedefs
    typedef unsigned int size_type;

    //Constructors and whatnot
    UndoArray(){ this->create(); }
    UndoArray(size_type n, const T& t = T()){this->create(n, t);}
    UndoArray(const UndoArray& a){ copy(a);}
    UndoArray& operator=(const UndoArray& a);
    ~UndoArray(){destroy();}

    //member functions
    T& operator[] (size_type i){return m_data[i]; }
    const T& operator[] (size_type i) const{ return m_data[i];}
    void set(int i, T val) const;
    T get(int i) const;
    void undo(int i) const;
    bool initialized(int i) const;
    void push_back(const T& t); //this is for extra credit
    void pop_back();
    void resize(size_type n, const T& fill_in_value = T());
    void clear() { delete [] m_data; create(); }
    bool empty() const { return m_size == 0; }
    size_type size() const { return m_size; }
    void print();

private:
    //member functions
    void create();
    void create(size_type n, const T& val);
    void copy(const UndoArray<T>& a);
    void destroy();

    //representation
    T** m_data;
    size_type m_size;
    size_type m_alloc;
    int* counts_;

};

template <class T> void UndoArray<T>::create(){
    m_data = NULL;
    m_size = m_alloc = 0;
    counts_ = new int[0];
}

template <class T> void UndoArray<T>::create(size_type n, const T& val){
    m_data = new T*[n];
    m_size = m_alloc = n;
    counts_ = new int[n];
    for(int* x = counts_; x != counts_ + m_size; ++x)
        *x = 0;
    for(T** p = m_data; p != m_data + m_size; ++p){
        *p = new T[1];
        *p[0] = val;
    }
}

template <class T> UndoArray<T>& UndoArray<T>::operator=(const UndoArray<T>& a){
    if(this != &a){
        delete [] m_data;
        delete [] counts_;
        this->copy(a);
    }
    return *this;
}

template <class T> void UndoArray<T>::destroy(){
    delete [] m_data;
    delete [] counts_;
}

template <class T> void UndoArray<T>::copy(const UndoArray<T>& a){
    m_size = a.m_size;
    m_alloc = a.m_alloc;
    m_data = new T*[a.m_alloc];
    counts_ = new int[a.m_size];
    for(size_type x = 0; x < m_size; ++x){
        counts_[x] = a.counts_[x];
        m_data[x] = a.m_data[x];
    }
}

template <class T> void UndoArray<T>::set(int i, T val) const{
    //check size of array at that point in the M_data array, make a temp array one larger for that point, copy over any values that may exist, then add new
    //at the end (remember to delete the old array)
    if(i < m_alloc){
        int array_at_index = counts_[i];
        T* new_array_at;
        if(array_at_index == 0)
            new_array_at = new T[1];
        else{
            //std::cout << "I'm Alive!" << std::endl;
            new_array_at = new T[array_at_index +1];
            T* old_arr = m_data[(size_type)i];
            for(size_type x = 0; x < array_at_index; ++x){
                new_array_at[x] = old_arr[x];
                //std::cout << old_arr[x] << " ";
            }
        }
        new_array_at[array_at_index] = val;
        m_data[i] = new_array_at;
        counts_[i] = array_at_index + 1;
    }
}

template<class T> T UndoArray<T>::get(int i) const{
    if(i < m_size){
        int array_at_index = counts_[i];
        if(array_at_index > 0){
            //T* old_arr = m_data[(size_type)i];
            return m_data[(size_type)i][array_at_index - 1];
        }
    }
}

template <class T> void UndoArray<T>::undo(int i) const{
    if(i < m_alloc){
        int array_at_index = counts_[i];
        T* new_array_at;
        if(array_at_index > 0){
            //std::cout << "I'm Alive!" << std::endl;
            new_array_at = new T[array_at_index -1];
            T* old_arr = m_data[(size_type)i];
            for(size_type x = 0; x < array_at_index -1; ++x){
                new_array_at[x] = old_arr[x];
                //std::cout << old_arr[x] << " ";
            }
            m_data[i] = new_array_at;
            counts_[i] = array_at_index - 1;
        }

    }
}

template <class T> bool UndoArray<T>::initialized (int i) const{
    if(i < m_alloc){
        int array_at_index = counts_[i];
        if(array_at_index > 0) return true;
        else return false;
    }
    else return false;

}

template <class T> void UndoArray<T>::push_back(const T& val){
    if(m_size == m_alloc){
        if(m_alloc == 0)
            m_alloc = 1;
        m_alloc *= 2;
        T** new_data = new T*[m_alloc];
        int* new_counts_ = new int[m_size + 1];
        for(size_type x = 0; x < m_size; x++){
            new_counts_[x] = counts_[x];
            for(int y = 0; y < counts_[x]; ++y){
                new_data[x][y] = m_data[x][y];
            }
        }
        delete [] m_data;
        delete [] counts_;
        m_data = new_data;
        counts_ = new_counts_;
    }
    m_data[m_size] = new T[1];
    m_data[m_size][0] = val;
    counts_[(int)m_size] = 1;
    ++ m_size;
}

template <class T> void UndoArray<T>::pop_back(){
    if(m_size > 0){
        //delete m_data[m_size -1];
        T** new_data = new T*[m_alloc];
        int* new_counts_ = new int[m_size - 1];
        for(size_type x = 0; x < m_size - 1; x++){
            new_counts_[x] = counts_[x];
            for(int y = 0; y < counts_[x]; ++y){
                new_data[x][y] = m_data[x][y];
            }
        }
        delete [] m_data;
        delete [] counts_;
        m_data = new_data;
        counts_ = new_counts_;
        m_size--;
        /*
        int* new_counts_ = new int[m_size - 1];
        for(int x = 0; x < (int)m_size - 1; ++x){
            new_counts_[x] = counts_[x];
        }
        delete [] counts_;
        counts_ = new_counts_;
        */
    }
}

template <class T> void UndoArray<T>::resize(size_type n, const T& fill_in_value){

}

template <class T> void UndoArray<T>::print(){
    for(unsigned int x = 0; x < m_size; ++x){
        T* old_arr = m_data[x];
        unsigned int size_at_arr = (unsigned int)counts_[x];
        for(size_type y = 0; y < size_at_arr; ++y){
            std::cout << old_arr[y] << " ";
        }
        std::cout << std::endl;
    }
}

#endif // UNDOARR_H_INCLUDED
