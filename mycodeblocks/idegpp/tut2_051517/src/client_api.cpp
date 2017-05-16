#include "client_api.h"
#include <iostream>


int client_api::thread_local_txn_num = 22;
map<int, txn*> client_api::client_txn_map;

client_api::client_api() {
    //ctor
}

client_api::~client_api() {
    //dtor
}


cookie client_api::perform_action_on_obj(sample_object &intf,
            enum  action_t  action_type, cookie req_cookie) {

    map<int, txn*>::iterator iter;
    txn* p_txn = nullptr;
    cookie alloted_id;

    iter = client_txn_map.find(thread_local_txn_num);
    if (iter == client_txn_map.end()) {
        //create txn object and initialize.
        //cout << " \n Creating new txnObj for txn_id " << thread_local_txn_num << endl;
        p_txn = new txn();
        p_txn->set_txn_num(thread_local_txn_num);
        client_txn_map[thread_local_txn_num] = p_txn;
    } else {
        //cout << "\n Adding to existing txn obj for txn_id " << thread_local_txn_num << endl;
        p_txn = iter->second;
    }

    alloted_id= p_txn->add_obj(intf, action_type, req_cookie);
    return alloted_id;
}

cookie client_api::v2_perform_action_on_obj(sample_object intf,
            enum  action_t  action_type, cookie req_cookie) {

    map<int, txn*>::iterator iter;
    txn* p_txn = nullptr;
    cookie alloted_id;

    iter = client_txn_map.find(thread_local_txn_num);
    if (iter == client_txn_map.end()) {
        //create txn object and initialize.
        ///cout << " \n Creating new txnObj for txn_id " << thread_local_txn_num << endl;
        p_txn = new txn();
        p_txn->set_txn_num(thread_local_txn_num);
        client_txn_map[thread_local_txn_num] = p_txn;
    } else {
        ///cout << "\n Adding to existing txn obj for txn_id " << thread_local_txn_num << endl;
        p_txn = iter->second;
    }

    alloted_id= p_txn->v2_add_obj(intf, action_type, req_cookie);
    return alloted_id;
}

void client_api::flush_obj_actions() {
    ///call transport send() for the current active txn..

    ++thread_local_txn_num;
    cout << "\n Incrementing txn_id to " << thread_local_txn_num << endl;

}



void client_api::display_client_txn_map() {

    map<int, txn*>::iterator iter;
    txn* p_txn = nullptr;
    for (iter = client_txn_map.begin(); iter != client_txn_map.end(); iter++ ) {
        cout << __FUNCTION__ << setw(9) << iter->first << " : " << iter->second  << endl;
        p_txn = iter->second;
        //p_txn->display_actions_map();
        p_txn->v2_display_actions_map();

    }

}
