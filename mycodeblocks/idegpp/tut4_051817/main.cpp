#include <iostream>
#include "sample_object.h"
#include "client_api.h"
#include "nn_msg.h"

#define SOCKET_ADDR "ipc:///data/pair_xx_cb3.ipc"

using namespace std;

void ut1_client(int argc, char **argv) {

    NanoMsg *p_nanoMsg = nullptr;
    if (argc == 2) {
        p_nanoMsg = new  NanoMsg(SOCKET_ADDR, NanoMsg::ConnClient);
    } else if (argc ==3 ) {
        p_nanoMsg = new  NanoMsg(SOCKET_ADDR, NanoMsg::ConnServer);
    } else { //argc ==1  ..no nanomsg

    }

    NxClientApi  apiObj;
    cookie  cookies[10];
    TestObject intf1, intf2;

    if (argc > 1) {
        apiObj.SetApiNanoMsg(p_nanoMsg);
        p_nanoMsg->SetupConnection();
    }


    intf1.SetParams("A54",  1000, 2);
    cookies[0] = apiObj.PerformActionOnObj(&intf1, CREATE, 0);
    /*
    intf1.SetParams("B56", 1003, 5);
    cookies[1] = apiObj.PerformActionOnObj(&intf1, MODIFY, 0);
    apiObj.FlushObjActions();


    intf2.SetParams("eth2", 2000, 3);
    cookies[2] = apiObj.PerformActionOnObj(&intf2, CREATE, 0);
    intf2.SetParams("eth2", 2001, 6);
    cookies[3] = apiObj.PerformActionOnObj(&intf2, MODIFY, 0);
    */

    apiObj.FlushObjActions();
    cout << " ------ MAIN OUTPUT ---- " << " Copies --> " << TestObject::NumCopyCtors_ << endl;
}

void ut2_nnmsg(int argc, char **argv) {
    NanoMsg *p_nanoMsg = nullptr;
   if (argc == 2) {
        p_nanoMsg = new  NanoMsg(SOCKET_ADDR, NanoMsg::ConnClient);
        p_nanoMsg->RunUT();
    }
    if (argc == 3) {
        p_nanoMsg = new  NanoMsg(SOCKET_ADDR, NanoMsg::ConnServer);
        p_nanoMsg->RunUT();
    }
}


int main(int argc, char**argv) {
    cout << " -- Start of Main() ---- " << endl;

    //if (argc ==1)    ut1_client(argc, argv);
    //if (argc > 1)    ut2_nnmsg (argc, argv);

    if (argc < 3)       ut1_client(argc, argv);
    if (argc == 3)      ut2_nnmsg(argc,argv);

    cout << "May19-v2  exec of ut1_client "  << endl;
    return 0;
}
