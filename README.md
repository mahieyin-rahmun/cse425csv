# This is the Assignment 2 for CSE425 course.

### Problem Description: A CSV file is given, we are to implement a module that will search the CSV files based on the specified criteria, such as search by disease, total death by year, etc. The search parameters are passed by command line interface.


##### Search Criteria set so far:
**1. by cause**

**2. by state**

**3. by year**

**4. death by year**

**5. by number of deaths**

**6. a mixture of all five**

**C++ Compilation Instructions**

###### We have made use of C++ boost library so they need to be present on the system.
###### The command is 
##### **$ g++ main.cpp -o <output file name> -lboost_program_options**

###### All of the implementations can be run using this command structure:
##### **Python: $ python3 main.py <path to the csv file> -<name of argument>=<value of argument>
##### **Shell: $ main.sh <path to the csv file> -<name of argument>=<value of argument>
##### **C++: After building the binary file -> $ <.o file> --<name of argument>=<value of argument>

