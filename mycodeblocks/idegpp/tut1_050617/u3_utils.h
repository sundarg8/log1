#include "p5_utils.h"
#include <vector>
#include <iterator>
typedef unsigned long int  hal_txn_num_t;
typedef unsigned short int hal_msg_list_index_t;
typedef unsigned long int  hal_msg_num_t;
typedef unsigned long int  hal_cookie_num_t;
typedef string             hal_msg_format_t;
typedef unsigned int       syserr_t;

#define HAL_MSG_LIST_MAX_INDEX 10


#if 1       //f1
void u3_g1_ut_start();
hal_txn_num_t u3_f1_start_txn();
hal_cookie_num_t u3_f2_add_msg_to_txn(hal_txn_num_t txn_num , hal_msg_format_t msg);

    //u3_f3_enqueue();
syserr_t u3_f4_end_txn(hal_txn_num_t);
    //u3_f5_enqueue_and_send();
#endif
