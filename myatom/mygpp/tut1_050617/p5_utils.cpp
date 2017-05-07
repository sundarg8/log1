#include <iostream>
using namespace std;

int fn_print_gpp_version() {
  if( __cplusplus == 201103L ) std::cout << "v6_1 C++11\n" ;
  else if( __cplusplus == 199711L ) std::cout << "v3_2 C++98\n" ;
  else std::cout << "pre-standard C++\n" ;
  return 0;
}
