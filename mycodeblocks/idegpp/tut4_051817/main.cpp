#include <iostream>
#include "sample_object.h"
#include "client_api.h"
#include "nn_msg.h"
using namespace std;

#define SOCKET_ADDR "ipc:///data/pair_xx_cb5.ipc"

void ut1_client(NxClientApi *);
void ut2_nnmsg(int argc, char **argv);
#define tempstr "May20-v5 "


int initInfra(int argc, NxClientApi **p_apiObj)  {
    *p_apiObj               = new NxClientApi();
    NanoMsg     *p_nanoMsg  = nullptr;

    if (argc == 2)       p_nanoMsg = new  NanoMsg(SOCKET_ADDR, NanoMsg::ConnClient);
    else if (argc == 3 ) p_nanoMsg = new  NanoMsg(SOCKET_ADDR, NanoMsg::ConnServer);
    else                 p_nanoMsg = new  NanoMsg(SOCKET_ADDR, NanoMsg::ConnClient);

    (*p_apiObj)->SetupSockConnection(p_nanoMsg);

    return 0;

}

int main(int argc, char**argv) {

    cout << " -- Start of Main() ---- " << endl;
    NxClientApi *p_apiObj = nullptr;

    if (argc < 4) {
        initInfra(argc, &p_apiObj);
        ut1_client(p_apiObj);
    }

    cout << tempstr << "exec of ut1_client "  << endl;
    return 0;
}


void ut1_client(NxClientApi *p_apiObj) {

    cookie  cookies[10];
    TestObject intf1, intf2;

    if (p_apiObj->IsServerMode() ){
        cout <<  tempstr << " -- Start in Server Listen Mode ---- " << endl;

        //Server Mode // while TRUE loop ?
        for  (int i =0 ; i < 1; i++) {
                p_apiObj->StartRecvTxnAndWaitOnRecv();
                //start a new txn and call recv again
        }
        return; //return from server.
    }


    //Client Mode.
    cout <<  tempstr << " -- Start in Client Send Mode ---- " << endl;
    intf1.SetParams("A54012348765",  1000, 2);
    cookies[0] = p_apiObj->PerformActionOnObj(&intf1, CREATE, 0);

    intf1.SetParams("B56", 1003, 5);
    cookies[1] = p_apiObj->PerformActionOnObj(&intf1, MODIFY, 0);

    /*
    p_apiObj.FlushObjActions();

    intf2.SetParams("eth2", 2000, 3);
    cookies[2] = p_apiObj->PerformActionOnObj(&intf2, CREATE, 0);
    intf2.SetParams("eth2", 2001, 6);
    cookies[3] = p_apiObj->PerformActionOnObj(&intf2, MODIFY, 0);
    */

    p_apiObj->FlushObjActions();
    cout << " ------ MAIN OUTPUT ---- " << " Copies --> " << TestObject::NumCopyCtors_ << endl;
}


void ut2_nnmsg(int argc, char **argv) {
    //if (argc == 4)      ut2_nnmsg(argc,argv);

    NanoMsg *p_nanoMsg = nullptr;
    if (argc == 2) {
        p_nanoMsg = new  NanoMsg(SOCKET_ADDR, NanoMsg::ConnClient);
        p_nanoMsg->RunUT();
    }
    if (argc == 4) {
        p_nanoMsg = new  NanoMsg(SOCKET_ADDR, NanoMsg::ConnServer);
        p_nanoMsg->RunUT();
    }
}

