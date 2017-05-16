#include "txn.h"

txn::txn()
{
    //ctor
    object_id = 0;
    //cout << "inside txn ctor" << endl;
}

txn::~txn()
{
    //dtor
}

cookie txn::add_obj(sample_object &obj_data, enum action_t action_type , cookie req_cookie) {
    ++object_id;
    actions_map.insert(pair<int, sample_object&> (object_id, obj_data));
    return object_id;
}


cookie txn::v2_add_obj(sample_object obj_data, enum action_t action_type , cookie req_cookie) {
    ++object_id;
    v2_actions_map.insert(pair<int, sample_object> (object_id, obj_data));
    return object_id;
}

void txn::display_actions_map() {
    map<int, sample_object &>::iterator iter;
    sample_object r_obj; // = nullptr;
    for (iter = actions_map.begin(); iter != actions_map.end(); iter++ ) {
        cout << __FUNCTION__ << setw(9) << " Object_id : " << iter->first << endl ; /// << " : " << iter->second  << endl;
        r_obj = iter->second;
        r_obj.display_obj();
    }
    cout <<endl;

}

void txn::v2_display_actions_map() {
    map<int, sample_object >::iterator  iter;
    sample_object r_obj; // = nullptr;
    for (iter = v2_actions_map.begin(); iter != v2_actions_map.end(); iter++ ) {
        cout << __FUNCTION__ << setw(9) << " Object_id : " << iter->first << endl ; /// << " : " << iter->second  << endl;
        r_obj = iter->second;
        r_obj.display_obj();
    }
    cout <<endl;

}
