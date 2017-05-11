#include <iostream>
#include "p5_utils.h"
#include "p4_classes.h"
#include "u2_utils.h"

using namespace std;

void u1_fns() {
    //fn3_ex_vector();NL;NL;
    //fn4_ex_my2vector();NL;NL;
    fn5_ex_my3_T_vector();
}

void u2_fns() {
    u2_f1_test();
}

int main(int argc, char **argv)  {
    pre_run(argc , argv);

    u2_fns();

    post_run();
    return 0;
 }

