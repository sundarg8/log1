#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;




int printTime();
int printHour();
int pre_run(int argc, char *argv[]);
int post_run();
int nlfl(char *str);

#define lsf     cout << endl << ">>> Enter- fn " << __FUNCTION__ << endl ;
#define lef     cout << endl << ">>> Exit-- fn " << __FUNCTION__ << endl ;

#define NL printf("\n")
#define NLFL(str) \
    cout << ">>>  " << __FUNCTION__ <<":" <<  __LINE__ <<  "  <<<   " << str << "\n"
#define ct(str) cout << __FUNCTION__ << "\t-- "  << #str << " -->\t\t " << str << "\n";
#define ctp(str) cout << __FUNCTION__ << "\t-- "  << str << "\n";

//#define ct(str) cout << str << "\n";
//#define cts(str) cout << str;
