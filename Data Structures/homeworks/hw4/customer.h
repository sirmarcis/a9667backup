#ifndef CUSTOMER_H_INCLUDED
#define CUSTOMER_H_INCLUDED

class Customer{
public:
    Customer() {customer_name = ""; }
    Customer(std::string nCustomer_name) { customer_name = nCustomer_name; }

    //Accessors
    std::string get_name() const{ return customer_name; }
    std::list<std::string> get_preferences() const{ return preference_list; }
    bool find_rental(std::string dvd_name);
    int get_num_movies();
    bool has_max_num_movies();
    bool preference_list_empty();

    //editors
    void add_preference(std::string dvd_name, std::ostream &ostr);
    void receives (std::string dvd_name, std::ostream &ostr);
    void remove_rental(std::string dvd_name);
    std::string return_oldest();
    std::string return_newest();
    void print_customer(std::ostream &ostr);

private:
    std::string customer_name;
    std::list<std::string> preference_list;
    std::list<std::string> rented_dvds;

};

bool Customer::find_rental(std::string dvd_name){
    std::list<std::string>::iterator it;
    for(it = rented_dvds.begin(); it != rented_dvds.end(); it++){
        if((*it) == dvd_name){
            return true;
        }
    }
    return false;
}

void Customer::add_preference(std::string dvd_name, std::ostream &ostr){
    std::list<std::string>::iterator it;
    for(it = preference_list.begin(); it != preference_list.end(); it++){
        std::string curr_dvd = *it;
        if(curr_dvd.compare(dvd_name) == 0){
            ostr << "WARNING: " << customer_name << " already has " << dvd_name << " on his/her preference list!" <<std::endl;
            return;
        }
    }

    preference_list.push_back(dvd_name);
}

bool Customer::has_max_num_movies(){
    std::list<std::string>::iterator it;
    int cnts = 0;
    for(it = rented_dvds.begin(); it != rented_dvds.end(); it++){
        cnts += 1;
    }
    if(cnts >= 3)
        return true;
    else
        return false;
}

int Customer::get_num_movies(){
    std::list<std::string>::iterator it;
    int cnts = 0;
    for(it = rented_dvds.begin(); it != rented_dvds.end(); it++){
        cnts += 1;
    }
    return cnts;
}

bool Customer::preference_list_empty(){
    std::list<std::string>::iterator it;
    int cnts = 0;
    for(it = preference_list.begin(); it != preference_list.end(); it++){
        cnts += 1;
    }
    if(cnts == 0)
        return true;
    else
        return false;
}

void Customer::receives (std::string dvd_name, std::ostream &ostr){

        rented_dvds.push_back(dvd_name);
        std::list<std::string>::iterator it;
        for(it = preference_list.begin(); it != preference_list.end(); it++){
            std::string curr_dvd = *it;
            if(curr_dvd.compare(dvd_name) == 0){
                it = preference_list.erase(it);
                return;
            }
        }
        //preference_list.remove(dvd_name);


}

void Customer::remove_rental(std::string dvd_name){
    std::list<std::string>::iterator it;
    for(it = rented_dvds.begin(); it != rented_dvds.end(); it++){
        std::string curr_dvd = *it;
        if(curr_dvd.compare(dvd_name) == 0){
            it = rented_dvds.erase(it);
            return;
        }
    }
}

std::string Customer::return_oldest(){
    std::list<std::string>::iterator it = rented_dvds.begin();
    std::string dvd_name = *it;
    remove_rental(dvd_name);
    return dvd_name;
}

std::string Customer::return_newest(){
    std::list<std::string>::iterator it;
    std::string dvd_name;
    for(it = rented_dvds.begin(); it != rented_dvds.end(); it++){
        dvd_name = *it;
    }
    remove_rental(dvd_name);
    return dvd_name;
}

void Customer::print_customer(std::ostream &ostr){
    std::list<std::string>::iterator it;
    int cnts = 0;
    for(it = rented_dvds.begin(); it != rented_dvds.end(); it++){
        cnts += 1;
    }
    std::string print_mesg;
    if(cnts > 0)
        ostr << customer_name << " has " << cnts << " movies:" <<std::endl;

    else{
        ostr << customer_name << " has no movies" <<std::endl;
    }
    if(cnts > 0){
        for(it = rented_dvds.begin(); it != rented_dvds.end(); it++){
            ostr << "    " << (*it) <<std::endl;
        }
    }
    if(preference_list.size() > 0){
        ostr << "  preference list:" <<std::endl;
        for(it = preference_list.begin(); it != preference_list.end(); it++){
            ostr << "    " << (*it) <<std::endl;
        }
    }

}

#endif // CUSTOMER_H_INCLUDED
