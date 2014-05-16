// ==================================================================
// Important Note: You are encouraged to read through this provided
//   code carefully and follow this structure.  You may modify the
//   file as needed to complete your implementation.
// ==================================================================

#include <fstream>
#include <iostream>
#include <string>
#include <list>
#include <cassert>
#include "dvd.h"
#include "customer.h"

// several type defs to keep things neat
// (hint, these are STL lists)
typedef std::list<DVD> inventory_type;
typedef std::list<Customer> customers_type;
typedef std::list<std::string> preference_type;


// ==================================================================

// Helper function prototypes
std::string read_customer_name(std::istream &istr);
std::string read_dvd_name(std::istream &istr);
inventory_type::iterator find_DVD(inventory_type &inventory, const std::string &dvd_name);
customers_type::iterator find_Customer(customers_type &customers, const std::string &dvd_name);
bool dvd_exists(inventory_type &inventory, const std::string &dvd_name);
bool customer_exists(customers_type &customers, const std::string &dvd_name);

// The main algorithm for DVD distribution
void shipping_algorithm(inventory_type &inventory, customers_type &customers, std::ostream &ostr);
void return_dvd(inventory_type &inventory, customers_type &customers, std::ostream &ostr, bool oldest, std::string &customer_name);
void add_customer_preference(inventory_type &inventory, customers_type &customers, std::string & customer_name, std::string dvd_name, std::ostream &ostr);
void sweep_preference_list(Customer & new_c, inventory_type &inventory, std::ostream &ostr);

// ==================================================================


void usage(const char* program_name) {
  std::cerr << "Usage: " << program_name << " <input_file> <output_file>" << std::endl;
  std::cerr << " -or-  " << program_name << " <input_file> <output_file> --analysis" << std::endl;
  std::cerr << " -or-  " << program_name << " <input_file> <output_file> --improved" << std::endl;
  std::cerr << " -or-  " << program_name << " <input_file> <output_file> --improved --analysis" << std::endl;
  //exit(1);
}


int main(int argc, char* argv[]) {

  if (argc < 3 || argc > 5) {
    usage(argv[0]);
  }

  // open input and output file streams
  std::ifstream istr(argv[1]);
  if (!istr) {
    std::cerr << "ERROR: Could not open " << argv[1] << " for reading." << std::endl;
    usage(argv[0]);
  }
  std::ofstream ostr(argv[2]);
  if (!ostr) {
    std::cerr << "ERROR: Could not open " << argv[2] << " for writing." << std::endl;
    usage(argv[0]);
  }

  // optional parameters for extra credit
  bool use_improved_algorithm = false;
  bool print_analysis = false;
  for (int i = 3; i < argc; i++) {
    if (std::string(argv[i]) == "--improved") {
      use_improved_algorithm = true;
    } else if (std::string(argv[i]) == "--analysis") {
      print_analysis = true;
    } else {
      usage(argv[0]);
    }
  }


  // stores information about the DVDs
  inventory_type inventory;
  // stores information about the customers
  customers_type customers;


  // read in and handle each of the 8 keyword tokens
  std::string token;
  while (istr >> token) {
    if (token == "dvd") {
      std::string dvd_name = read_dvd_name(istr);
      int copies;
      istr >> copies;
      if(copies == 1)
        ostr << copies << " copy of " << dvd_name << " added" <<std::endl;
      else
        ostr << copies << " copies of " << dvd_name << " added" <<std::endl;
        DVD new_dvd(dvd_name, copies);
        inventory.push_back(new_dvd);

    } else if (token == "customer") {
      std::string customer_name = read_customer_name(istr);
      int num_movies;
      istr >> num_movies;
      if(!(customer_exists(customers, customer_name))){
        Customer new_c(customer_name);
        for (int i = 0; i < num_movies; i++) {
            std::string dvd_name = read_dvd_name(istr);
            //updates database with one less of the movie specified
            //updates customer info with this movie added and when it was added
            //inventory_type::iterator it_tester = find_DVD(inventory, dvd_name);
            if(dvd_exists(inventory, dvd_name)){
                //std::cout << "dvd match found of " << dvd_name <<std::endl;
                new_c.add_preference(dvd_name, ostr);
            }
            else{
                ostr << "WARNING: No movie named " << dvd_name << " in the inventory" << std::endl;
            }
        }
        //std::cout << new_c.get_name() << " " << new_c.preference_list_empty() << std::endl;
        customers.push_back(new_c);
        ostr << "new customer: " << customer_name <<std::endl;
        }
      else{
        for (int i = 0; i < num_movies; i++)
            std::string dvd_name = read_dvd_name(istr);
        ostr << "WARNING: Already have a customer named " << customer_name <<std::endl;
      }
    } else if (token == "ship") {

      shipping_algorithm(inventory,customers,ostr);

    } else if (token == "return_oldest") {
      std::string customer_name = read_customer_name(istr);
      return_dvd(inventory, customers, ostr, true, customer_name);

    } else if (token == "return_newest") {
      std::string customer_name = read_customer_name(istr);
      return_dvd(inventory, customers, ostr, false, customer_name);

    } else if (token == "print_customer") {
      std::string customer_name = read_customer_name(istr);
      customers_type::iterator cus_it = find_Customer(customers, customer_name);
      if(customer_exists(customers, customer_name))
        (*cus_it).print_customer(ostr);
        else
            ostr << "WARNING: No customer named " << customer_name << std::endl;

      /* YOU MUST FINISH THIS IMPLEMENTATION */

    } else if (token == "print_dvd") {
      std::string dvd_name = read_dvd_name(istr);
      inventory_type::iterator inv_it = find_DVD(inventory, dvd_name);
      if(dvd_exists(inventory, dvd_name))
        (*inv_it).print_dvd(ostr);
      else
        ostr << "WARNING: No movie named " << dvd_name << " in the inventory" <<std::endl;

      /* YOU MUST FINISH THIS IMPLEMENTATION */

    } else if (token == "add_preference") {
      std::string customer_name = read_customer_name(istr);
      std::string dvd_name = read_dvd_name(istr);
      if(customer_exists(customers, customer_name))
        add_customer_preference(inventory, customers, customer_name, dvd_name, ostr);
      else
        ostr << "WARNING: No customer named " << customer_name << std::endl;
    } else {
      std::cerr << "ERROR: Unknown token " << token << std::endl;
      return 1;
    }
  }
}


// ==================================================================


// A customer name is simply two strings, first & last name
std::string read_customer_name(std::istream &istr) {
  std::string first, last;
  istr >> first >> last;
  return first + " " + last;
}


// A movie name is one or more strings inside of double quotes
std::string read_dvd_name(std::istream &istr) {
  std::string token;
  istr >> token;
  assert (token[0] == '"');
  std::string answer = token;
  while (answer[answer.size()-1] != '"') {
    istr >> token;
    answer += " " + token;
  }
  return answer;
}


// A helper function to find a DVD from the inventory
inventory_type::iterator find_DVD(inventory_type &inventory, const std::string &dvd_name) {
  for (inventory_type::iterator inventory_itr = inventory.begin();
       inventory_itr != inventory.end(); inventory_itr++) {
    if (inventory_itr->get_name() == dvd_name) {
      return inventory_itr;
    }
  }
  // if the DVD is not found, return the .end() iterator
  return inventory.end();
}

bool dvd_exists(inventory_type &inventory, const std::string &dvd_name){
    for (inventory_type::iterator inventory_itr = inventory.begin();
       inventory_itr != inventory.end(); inventory_itr++) {
    if (inventory_itr->get_name() == dvd_name) {
      return true;
    }
  }
  // if the DVD is not found, return the .end() iterator
  return false;
}

customers_type::iterator find_Customer(customers_type &customers, const std::string &dvd_name) {
  for (customers_type::iterator inventory_itr = customers.begin();
       inventory_itr != customers.end(); inventory_itr++) {
    if (inventory_itr->get_name() == dvd_name) {
      return inventory_itr;
    }
  }
  // if the DVD is not found, return the .end() iterator
  return customers.end();
}

bool customer_exists(customers_type &customers, const std::string &dvd_name){
for (customers_type::iterator inventory_itr = customers.begin();
       inventory_itr != customers.end(); inventory_itr++) {
    if (inventory_itr->get_name() == dvd_name) {
      return true;
    }
  }
  // if the DVD is not found, return the .end() iterator
  return false;
}


// ==================================================================

//
// A basic algorithm for determining which movies to ship to each customer
//
void shipping_algorithm(inventory_type &inventory, customers_type &customers, std::ostream &ostr) {


  ostr << "Ship DVDs" << std::endl;

  // Loop over the customers in priority order
  //
  // Note that we edit the customers list as we go, to add customers
  // to the back of the list if they receive a DVD.  This allows a
  // customer to receive multiple DVDs in one shipment cycle, but only
  // after all other customers have had a chance to receive DVDs.
  //
  customers_type::iterator customer_itr = customers.begin();
  while (customer_itr != customers.end()) {

    // skip this customer if they already have 3 movies or if their
    // preference list is empty (no outstanding requests)
    //std::cout << "completed one iteration where [" << customer_itr->has_max_num_movies() << "] [" << customer_itr->preference_list_empty() << "]" <<std::endl;
    if (customer_itr->has_max_num_movies() || customer_itr->preference_list_empty()) {
      // move on to the next customer
      customer_itr++;
      continue;
    }

    // a helper flag variable
    bool sent_dvd = false;
    // loop over the customer's preferences
    const preference_type &preferences = customer_itr->get_preferences();
    for (preference_type::const_iterator preferences_itr = preferences.begin();
         preferences_itr != preferences.end(); preferences_itr++) {
      // locate this DVD in the inventory
      inventory_type::iterator inventory_itr = find_DVD(inventory,*preferences_itr);
      if (inventory_itr != inventory.end() && inventory_itr->available()) {
        // if the DVD is available, ship it to the customer!
        ostr << "  " << customer_itr->get_name() << " receives " << *preferences_itr << std::endl;
        inventory_itr->shipped();
        if((*customer_itr).find_rental(*preferences_itr))
            ostr << "WARNING: " << (*customer_itr).get_name() << " currently has " << (*preferences_itr) << "!" <<std::endl;
        (*customer_itr).receives((*preferences_itr), ostr);
        // move this customer to the back of the priority queue
        // they will get a chance to receive another DVD, but only
        // after everyone else gets a chance
        customers.push_back(*customer_itr);
        customer_itr = customers.erase(customer_itr);

        // after setting the flag to true, leave the iteration over preferences
        sent_dvd = true;
        break;
      }
    }

    // if no DVD was sent to this customer, then we move on to the next customer
    // (do not change this customer's priority for tomorrow's shipment)
    if (!sent_dvd) {
      customer_itr++;
    }
  }
}

void return_dvd(inventory_type &inventory, customers_type &customers, std::ostream &ostr, bool oldest, std::string &customer_name){
    customers_type::iterator it;
    if(customer_exists(customers, customer_name)){
    for(it = customers.begin(); it != customers.end(); it++){
        std::string curr_cus_name = (*it).get_name();
        if(curr_cus_name == customer_name){
            inventory_type::iterator inv_it;
            std::string dvd_name;
            if((*it).get_num_movies() > 0){
                if(oldest)
                    dvd_name = (*it).return_oldest();
                else
                    dvd_name = (*it).return_newest();
                for(inv_it = inventory.begin(); inv_it != inventory.end(); inv_it++){
                    if((*inv_it).get_name() == dvd_name)
                        (*inv_it).return_copy();
                    continue;
                }
                ostr << customer_name << " returns " << dvd_name <<std::endl;

                return;
            }
            else{
                ostr << "WARNING: " << customer_name << " has no DVDs to return!" <<std::endl;
                return;
            }
        }
    }
    }
    else
        ostr << "WARNING: No customer named " << customer_name <<std::endl;

}

void add_customer_preference(inventory_type &inventory, customers_type &customers, std::string & customer_name, std::string dvd_name, std::ostream &ostr){
    customers_type::iterator cus_it;
    inventory_type::iterator inv_it = find_DVD(inventory, dvd_name);

    if(dvd_exists(inventory, dvd_name)){
        for(cus_it = customers.begin(); cus_it != customers.end(); cus_it++){
            if((*cus_it).get_name() == customer_name){
                if((*cus_it).find_rental(dvd_name) != true)
                    (*cus_it).add_preference((*inv_it).get_name(), ostr);
                else
                    ostr << "WARNING: " << (*cus_it).get_name() << " currently has " << dvd_name << "!" << std::endl;
                return;
            }
        }
    }
    else
        ostr << "WARNING: No movie named " << dvd_name << " in the inventory" << std::endl;
}

void sweep_preference_list(Customer & new_c, inventory_type &inventory, std::ostream &ostr){
    std::list<std::string> new_c_preference_list = new_c.get_preferences();
    std::list<std::string>::iterator it;

}

// ==================================================================
