#include <iostream>
#include "sample_object.h"
#include "client_api.h"

using namespace std;


void ut1_client() {

    cookie  cookies[10];
    sample_object intf1, intf2;

    intf1.set_obj_params("eth1", 1070, 50);
    cookies[0] = client_api::v3_perform_action_on_obj(&intf1, CREATE, 0);
    intf1.set_obj_params("eth1", 1071, 53);
    cookies[1] = client_api::v3_perform_action_on_obj(&intf1, MODIFY, 0);
    client_api::flush_obj_actions();


    intf2.set_obj_params("eth2", 2000, 60);
    cookies[2] = client_api::v3_perform_action_on_obj(&intf2, CREATE, 0);
    intf2.set_obj_params("eth2", 2001, 64);
    cookies[3] = client_api::v3_perform_action_on_obj(&intf2, MODIFY, 0);
    client_api::flush_obj_actions();


    cout << " ------ MAIN OUTPUT ---- " << cookies[3] << " Copies --> " << sample_object::no_of_copy_ctors << endl;
}

int main()
{
    cout << " -- Start of Main() ---- " << endl;
    ut1_client();
    cout << "May16-v8  exec of ut1_client "  << endl;

    client_api::display_client_txn_map();
     return 0;
}
