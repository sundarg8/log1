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



cookie txn::v3_add_obj(sample_object *obj_data, enum action_t action_type , cookie req_cookie) {
    //sample_object *curr_obj = new sample_object();
    //(*curr_obj) = obj_data;
    ++object_id;
    //v3_actions_map.insert(pair<int, sample_object> (object_id, *curr_obj));
    v3_actions_map.insert(pair<int, sample_object&> (object_id, *obj_data));
    return object_id;
}


void txn::v3_display_actions_map() {
    map<int, sample_object >::iterator  iter;
    sample_object r_obj; // = nullptr;
    for (iter = v3_actions_map.begin(); iter != v3_actions_map.end(); iter++ ) {
        cout <<  setw(9) << " Object_id : " << iter->first << endl ; /// << " : " << iter->second  ;
        r_obj = iter->second;
        r_obj.display_obj();
    }
    //cout <<endl;

}
