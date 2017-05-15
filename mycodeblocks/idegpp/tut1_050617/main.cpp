#include <iostream>
#include "p5_utils.h"
#include "p4_classes.h"
#include "u2_utils.h"

using namespace std;

void u1_fns() {
    //fn3_ex_vector();NL;NL;
    //fn4_ex_my2vector();NL;NL;
    //fn5_ex_my3_T_vector();
}

void u2_fns() {
    //u2_f1_test();
    //u2_f2_stdVector_test();
    //u2_f3_pointers_test();
    //u2_f4_generic_prog_test();
    //u2_f5_seq_containers_test();
    u2_f6_associative_containers_test();
    //u2_f7_generic_algos_test();
}

void u3_fns() {
    //u3_f1_start_txn();
    //u3_f2_add_msg_to_txn();
    //u3_f3_enqueue();
    //u3_f4_txn_end();
    //u3_f5_enqueue_and_send();
}

int y1_global;
int main(int argc, char **argv)  {
    pre_run(argc , argv);

    u2_fns();

    post_run();
    return 0;
 }

