#include <iostream>
#include "sample_object.h"
#include "client_api.h"

using namespace std;


void ut1_client() {

    cookie  cookies[10];
    sample_object intf1, intf2;

    intf1.set_obj_params("eth1", 1000, 50);
    cookies[0] = client_api::v2_perform_action_on_obj(intf1, CREATE, 0);

    intf1.set_obj_params("eth1", 1001, 53);
    cookies[1] = client_api::v2_perform_action_on_obj(intf1, MODIFY, 0);
    intf1.display_obj();
    client_api::flush_obj_actions();


    intf2.set_obj_params("eth2", 2000, 60);
    cookies[2] = client_api::v2_perform_action_on_obj(intf2, CREATE, 0);
    intf2.set_obj_params("eth2", 2001, 64);
    cookies[3] = client_api::v2_perform_action_on_obj(intf2, MODIFY, 0);
    client_api::flush_obj_actions();

    cout << " ------ MAIN OUTPUT ---- " << cookies[3] << endl;
    client_api::display_client_txn_map();
}

int main()
{
    ut1_client();

     return 0;
}
