#include <iostream>
#include <vector>
#include <list>


void reverse_vector(std::vector<int> &vec){
    int reverse_pos;
    for(int x = 0; x < vec.size(); x++){
        reverse_pos = vec.size() - 1 - x;
        if(reverse_pos != x && (vec.size() / 2) > x){
            int temp = vec[x];
            vec[x] = vec[reverse_pos];
            vec[reverse_pos] = temp;
        }
    }
}

void reverse_list(std::list<int> &lst){
    int cnts = 0;
    std::list<int>::iterator fwd_it = lst.begin();
    std::list<int>::reverse_iterator rev_it;
    for(rev_it = lst.rbegin(); rev_it != lst.rend(); ++rev_it){
        if((lst.size() / 2) > cnts){
            int temp = *fwd_it;
            *fwd_it = *rev_it;
            *rev_it = temp;
        }
        cnts += 1;
        fwd_it++;
    }
}

void print_vec(std::vector<int> &vec){
    std::cout << "vector size: "  << vec.size() <<std::endl;
    std::cout << "vector contents: ";
    for(int x = 0; x < vec.size(); x++){
        std::cout << vec[x] << ", ";
    }
    std::cout <<std::endl;
}

void print_list(std::list<int> &lst){
    std::list<int>::iterator it;
    std::cout << "list size: " << lst.size() <<std::endl;
    for(it = lst.begin(); it != lst.end(); it++){
        std::cout << (*it) <<", ";
    }
    std::cout <<std::endl;
}

int
main(){
    std::vector<int> test1;

    test1.push_back(2);
    test1.push_back(3);
    test1.push_back(4);
    test1.push_back(5);
    test1.push_back(6);
    print_vec(test1);
    reverse_vector(test1);
    print_vec(test1);

    std::list<int> lst1;
    lst1.push_back(1);
    lst1.push_back(2);
    lst1.push_back(3);
    lst1.push_back(4);
    lst1.push_back(5);
    print_list(lst1);
    reverse_list(lst1);
    print_list(lst1);
    return 0;
}
