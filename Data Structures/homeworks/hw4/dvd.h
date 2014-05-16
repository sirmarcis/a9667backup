#ifndef DVD_H_INCLUDED
#define DVD_H_INCLUDED


class DVD{
public:
    DVD(){dvd_name = ""; tot_copies = avalible_copies = 1; }
    DVD(std::string nDvd_name) { dvd_name = nDvd_name; tot_copies = avalible_copies  = 1; }
    DVD(std::string nDvd_name, int nNum_copies) { dvd_name = nDvd_name; tot_copies = avalible_copies = nNum_copies; }

    //Accessors
    std::string get_name() const{return dvd_name; }
    int get_num_copies() const {return avalible_copies; }
    bool copies_avalible() const{ if(avalible_copies > 0) return true; else return false; }
    bool available() const;

    //editors
    void set_name(std::string nName) {dvd_name = nName; }
    void return_copy() { if(avalible_copies < tot_copies) avalible_copies += 1; }
    void shipped() { if(avalible_copies > 0) avalible_copies -= 1; }
    void print_dvd(std::ostream &ostr);

private:
    std::string dvd_name;
    int avalible_copies;
    int tot_copies;
};

bool DVD::available() const{
    if(avalible_copies > 0)
        return true;
    return false;
}

void DVD::print_dvd(std::ostream &ostr){
    ostr << dvd_name << ":" <<std::endl;
    std::string copy_aval_str = " copy ";
    std::string tot_copy_str = " copy ";
    if((tot_copies - avalible_copies) > 1)
        copy_aval_str = " copies ";
    if(avalible_copies > 1)
        tot_copy_str = " copies ";
    if((tot_copies - avalible_copies) > 0 && avalible_copies > 0){
        ostr << "  " << (tot_copies - avalible_copies) << copy_aval_str << "checked out and " << avalible_copies << tot_copy_str << "available" <<std::endl;
    }
    else if((tot_copies - avalible_copies) == 0)
        ostr << "  " << tot_copies << tot_copy_str << "available" <<std::endl;
    else
        ostr << "  " << (tot_copies - avalible_copies) << copy_aval_str << "checked out" <<std::endl;
}


#endif // DVD_H_INCLUDED
