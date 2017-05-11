///#ifndef __u2_utils__
///#define __u2_utils__

#include "p5_utils.h"


#if 1       //f1
void u2_f1_test();
class u2_test {
    private:
        int x;
        string name;
    public:
        u2_test(int val) {x = val; }
        void setName() {
            ctp("Enter the name of u2_test");
            getline(cin, name);
        }
        void display() {
            ct(x); NL;
            ct(name);
        }
};
#endif

#if 1       //f2
#endif      // f2

///#endif // __u2_utils__
