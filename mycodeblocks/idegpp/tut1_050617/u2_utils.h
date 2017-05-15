#include "p5_utils.h"
#include <vector>
#include <iterator>

#if 1       //f1
void u2_f1_test();
class u2_test {
    private:
        int x;
        string name;
    public:
        u2_test(int val) {
            x = val;
        }
        void setName() {
            ctp("Enter the name of u2_test");
            getline(cin, name);
        }
        void display() {
            ctw(x);
            ctw(name);
        }
};
#endif

#if 1       //f2 std::vector
void u2_f2_stdVector_test();
#endif

#if 1       //f3 pointers
void u2_f3_pointers_test();
#endif      // f2

#if 1       //f4 generic prog
void u2_f4_generic_prog_test();
#endif      // f2

#if 1       //f5 sequential containers.
void u2_f5_seq_containers_test();
#endif // 1

#if 1       //f6 assoc. containers.
void u2_f6_associative_containers_test();
#endif

#if 1
void u2_f7_generic_algos_test();
#endif



