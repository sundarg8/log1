#include "p5_utils.h"
#include "u3_utils.h"
#include <deque>
#include <map>

static hal_txn_num_t running_txn_num =12;
static hal_msg_num_t running_msg_num =0;

//C1 typedef map<hal_msg_num_t, hal_msg_format_t>  map_t1;
//C1 vector<map_t1> txn_hal_msg_table(5);
map<hal_msg_num_t, hal_msg_format_t> txn_hal_msg_list[HAL_MSG_LIST_MAX_INDEX];
map<hal_txn_num_t, hal_msg_list_index_t> active_hal_txn_map;


#if 1       //f1

hal_txn_num_t  hal_get_next_free_txn() {return ++running_txn_num;}

hal_msg_num_t  hal_get_next_free_msg_num() {return ++running_msg_num;}


hal_msg_list_index_t hal_get_next_free_msg_list_index() {
    for (int i=0 ; i< HAL_MSG_LIST_MAX_INDEX; i++) {
        if (txn_hal_msg_list[i].empty())  {return i;}
    }
    return -1;
}

hal_msg_list_index_t get_txn_msg_index(hal_txn_num_t txn_num) {
    map<hal_txn_num_t, hal_msg_list_index_t>::iterator iter = active_hal_txn_map.find(txn_num);
    if (iter != active_hal_txn_map.end())
        return iter->second;
}
void display_txn_map() {
    map<hal_txn_num_t, hal_msg_list_index_t>::iterator iter; print_assoc_iter(iter, active_hal_txn_map);
}

void display_txn_hal_msg_list(hal_txn_num_t txn_num) {
    hal_msg_list_index_t msg_index = get_txn_msg_index(txn_num);
    map<hal_msg_num_t, hal_msg_format_t>::iterator iter; print_assoc_iter(iter, txn_hal_msg_list[msg_index]);
}



hal_txn_num_t u3_f1_start_txn() {
    hal_txn_num_t  new_txn = hal_get_next_free_txn();
    hal_msg_list_index_t txn_msg_list_index = hal_get_next_free_msg_list_index();

    active_hal_txn_map.insert(pair<hal_txn_num_t, hal_msg_list_index_t>(new_txn, txn_msg_list_index));
    return new_txn;
}

hal_cookie_num_t u3_f2_add_msg_to_txn(hal_txn_num_t txn_num, hal_msg_format_t msg) {
    hal_msg_num_t msg_num;
    msg_num = hal_get_next_free_msg_num();
    hal_msg_list_index_t msg_index = get_txn_msg_index(txn_num);

    txn_hal_msg_list[msg_index].insert(pair<hal_msg_num_t, hal_msg_format_t>(msg_num, msg));

    return msg_num;
}

syserr_t u3_f4_end_txn(hal_txn_num_t txn_num) {
    //display_txn_hal_msg_list(txn_num);
    hal_msg_list_index_t msg_index = get_txn_msg_index(txn_num);
    map<hal_msg_num_t, hal_msg_format_t>::iterator iter;
    iter = txn_hal_msg_list[msg_index].begin();
    for (; iter != txn_hal_msg_list[msg_index].end(); iter++)
        cout << setw(9) << " write to queue:msg " <<  iter->first << " : Payload --> " << iter->second  << endl; NL;


}


void u3_g1_ut_start() {

    int second_txn, third_txn, first_txn = u3_f1_start_txn();
    vector<hal_cookie_num_t> msg_cookies;
    msg_cookies.push_back(u3_f2_add_msg_to_txn(first_txn, "Obj Msg One"));
    msg_cookies.push_back(u3_f2_add_msg_to_txn(first_txn, "Obj Msg Two"));
    msg_cookies.push_back(u3_f2_add_msg_to_txn(first_txn, "Obj Msg Four"));
    msg_cookies.push_back(u3_f2_add_msg_to_txn(first_txn, "Obj Msg Three"));
    msg_cookies.push_back(u3_f2_add_msg_to_txn(first_txn, "Obj Msg Five"));


    second_txn = u3_f1_start_txn();
    msg_cookies.push_back(u3_f2_add_msg_to_txn(second_txn, "Obj Msg Eleven"));
    msg_cookies.push_back(u3_f2_add_msg_to_txn(second_txn, "Obj Msg Thirteen"));
    msg_cookies.push_back(u3_f2_add_msg_to_txn(second_txn, "Obj Msg Fifteen"));
    msg_cookies.push_back(u3_f2_add_msg_to_txn(second_txn, "Obj Msg Seventeen"));

    display_txn_map();
    //display_txn_hal_msg_list( first_txn);
    //display_txn_hal_msg_list(second_txn);

    u3_f4_end_txn(second_txn);
    u3_f4_end_txn(first_txn);

}
#endif
