
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using std::vector;
using std::string ;
using std::cin;
using std::cout;
using std::stringstream ;


#include "DBCore.h"

int main() {

    //DBCore db;
    //db.RUN();

    vector<ROW> vec(10,ROW(vector<ELEMENT>(),0));

    return 0;
}


/*
 *          {   ----------------------------
 *              |TABLE                      |
 *              ----------------------------
 *              |   |Statement              |
 *              |   -------------------------
 *              |       |       ROW         |
 *              |       ---------------------
 *              |           |    ELEMENT    |
 *              |           -----------------   }
 *
 */