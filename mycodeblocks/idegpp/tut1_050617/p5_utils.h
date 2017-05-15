#include <iostream>
#include <iomanip>
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

#define NL printf("\n");
#define NLFL(str) \
    cout << ">>>  " << __FUNCTION__ <<":" <<  __LINE__ <<  "  <<<   " << str << "\n";
#define ct(str) cout << __FUNCTION__ << "\t-- "  << #str << " -->\t\t " << str << "\n";
#define ctp(str) cout << __FUNCTION__ << "\t-- "  << str << "\n";
#define ctwn(str,w) cout << __FUNCTION__ << "\t-- " << setfill('.')<< setw(w) << #str << " -->\t " << str << "\n";
#define ctw(str) ctwn(str,9)

#define print_iter(iter, container) \
            NL;for (iter = container.begin(); iter != container.end(); iter++ ) \
                   cout << *iter  << endl; NL; \

#define print_assoc_iter(iter, container) \
            NL;for (iter = container.begin(); iter != container.end(); iter++ ) \
                   cout << __FUNCTION__ << setw(9) << iter->first << " : " << iter->second  << endl; NL; \

#define multi_line_print_iter(iter, container) \
        do {  \
            for (iter = container.begin(); iter != container.end(); iter++ ) \
                cout << *iter  << endl; \
        } while (0);
//#define ct(str) cout << str << "\n";
//#define cts(str) cout << str;
