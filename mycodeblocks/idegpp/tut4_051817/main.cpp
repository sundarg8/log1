#include <iostream>
#include <unistd.h>
#include "sample_object.h"
#include "client_api.h"
using namespace std;

#define SOCKET_ADDR "ipc:///data/pair_xx_cb5.ipc"

int initInfra(int argc, NxClientApi **p_apiObj);
int v2InitInfra(int argc, NxClientApi **p_apiObj);

void ut1_client(NxClientApi *);
void ut2_client(NxClientApi *);
void ut3_client(NxClientApi *);
void ut4_client(NxClientApi *);


void ut50_nnmsg(int argc, char **argv);
#define tempstr "May23-v5 "


int v2InitInfra(int argc, NxClientApi **p_apiObj) {

    *p_apiObj               = new NxClientApi();
    Syserr_t                ret_val = NxProcSUCCESS;

    if (argc == 3)
        ret_val = (*p_apiObj)->SetupServerEndPoint(
                    SOCKET_ADDR, NxClientApi::NanoMsgSock);
    else
        ret_val = (*p_apiObj)->SetupClientEndPoint(
                    SOCKET_ADDR, NxClientApi::NanoMsgSock);

    return ret_val;
}

int main(int argc, char**argv) {

    cout << " -- Start of Main() ---- " << endl;
    NxClientApi *p_apiObj = nullptr;

    if (argc < 4) {
        v2InitInfra(argc, &p_apiObj);
        ut4_client(p_apiObj);
    }

    sleep(2);
    cout << tempstr << "exec of ut1_client "  << endl;
    return 0;
}


void ut4_client(NxClientApi *p_apiObj) {

    cookie_t      cookies[10];
    TestObject  intf1, intf2;
    int curr_txn_no = 0 ,sec_txn_no = 0;

    cout << "  Running Test " << __FUNCTION__ << endl;

    if (p_apiObj->IsServerMode() ) {
        cout <<  tempstr << " -- Start in Server Listen Mode ---- " << endl;
        for  (int i =0 ; i < 2; i++) {
                p_apiObj->StartNewTxnAndWaitOnRecv();
        }
        return;
    }

    //Client Mode.
    cout <<  tempstr << " -- Start in Client Send Mode ---- " << endl;

    curr_txn_no = 105 ;
    sec_txn_no  = 248 ;
    if (NxProcFAILURE == p_apiObj->StartTxnWithId(sec_txn_no))
        return;
    if (NxProcFAILURE == p_apiObj->StartTxnWithId(curr_txn_no))
        return;

    intf1.SetParams((char *)"A54012348765",  26, 12);
    p_apiObj->AddActionToTxn(curr_txn_no, &intf1, CREATE, &cookies[0]);
    intf1.SetParams((char *)"B56", 36, 15);
    p_apiObj->AddActionToTxn(curr_txn_no, &intf1, MODIFY, &cookies[1]);
    p_apiObj->FlushTxn(curr_txn_no);

    for  (int i =0 ; i < 1; i++) {
        p_apiObj->StartNewTxnAndWaitOnRecv();   ///UT Code
    }

    intf2.SetParams((char *)"eth2", 51, 23);
    p_apiObj->AddActionToTxn(sec_txn_no, &intf2, CREATE, &cookies[2]);
    intf2.SetParams((char *)"eth2", 52, 21);
    p_apiObj->AddActionToTxn(sec_txn_no, &intf2, MODIFY, &cookies[3]);
    p_apiObj->FlushTxn(sec_txn_no);

    for  (int i =0 ; i < 1; i++) {
        p_apiObj->StartNewTxnAndWaitOnRecv();    ///UT Code
    }
    cout << " ------ MAIN OUTPUT ---- " << " Copies --> " << TestObject::NumCopyCtors_ << endl;

}

#if 0
void ut3_client(NxClientApi *p_apiObj) {

    cookie_t  cookies[10];
    TestObject intf1, intf2;
    int curr_txn_no = 0;

    cout << "  Running Test " << __FUNCTION__ << endl;
    if (p_apiObj->IsServerMode() ) {
        cout <<  tempstr << " -- Start in Server Listen Mode ---- " << endl;
        for  (int i =0 ; i < 2; i++) {
                p_apiObj->StartNewTxnAndWaitOnRecv();
                //start a new txn and call recv again
        }
        return;
    }


    //Client Mode.
    p_apiObj->StartTxn(&curr_txn_no);
    cout <<  tempstr << " -- Start in Client Send Mode ---- " << endl;
    intf1.SetParams("A54012348765",  25, 2);
    cookies[0] = p_apiObj->AddActionToTxn(curr_txn_no, &intf1, CREATE, &cookies[0]);
    intf1.SetParams("B56", 35, 5);
    cookies[1] = p_apiObj->AddActionToTxn(curr_txn_no, &intf1, MODIFY, &cookies[0]);
    p_apiObj->FlushTxn(curr_txn_no);


    p_apiObj->StartTxn(&curr_txn_no);
    intf2.SetParams("eth2", 211, 3);
    cookies[2] = p_apiObj->AddActionToTxn(curr_txn_no, &intf2, CREATE, &cookies[0]);
    intf2.SetParams("eth2", 311, 6);
    cookies[3] = p_apiObj->AddActionToTxn(curr_txn_no, &intf2, MODIFY, &cookies[0]);
    p_apiObj->FlushTxn(curr_txn_no);

    cout << " ------ MAIN OUTPUT ---- " << " Copies --> " << TestObject::NumCopyCtors_ << endl;
}

void ut2_client(NxClientApi *p_apiObj) {

    cookie_t  cookies[10];
    TestObject intf1, intf2;
    cout << "  Running Test " << __FUNCTION__ << endl;

    if (p_apiObj->IsServerMode() ){
        cout <<  tempstr << " -- Start in Server Listen Mode ---- " << endl;
        for  (int i =0 ; i < 2; i++) {
                p_apiObj->StartNewTxnAndWaitOnRecv();
                //start a new txn and call recv again
        }
        return;
    }


    //Client Mode.
    p_apiObj->StartTxn();
    cout <<  tempstr << " -- Start in Client Send Mode ---- " << endl;
    intf1.SetParams("A54012348765",  25, 2);
    cookies[0] = p_apiObj->PerformActionOnObj(&intf1, CREATE, &cookies[0]);
    intf1.SetParams("B56", 35, 5);
    cookies[1] = p_apiObj->PerformActionOnObj(&intf1, MODIFY, &cookies[0]);
    p_apiObj->FlushTxn();


    p_apiObj->StartTxn( );
    intf2.SetParams("eth2", 211, 3);
    cookies[2] = p_apiObj->PerformActionOnObj(&intf2, CREATE, &cookies[0]);
    intf2.SetParams("eth2", 311, 6);
    cookies[3] = p_apiObj->PerformActionOnObj(&intf2, MODIFY, &cookies[0]);
    p_apiObj->FlushTxn();

    cout << " ------ MAIN OUTPUT ---- " << " Copies --> " << TestObject::NumCopyCtors_ << endl;
}

void ut1_client(NxClientApi *p_apiObj) {

    cookie_t  cookies[10];
    TestObject intf1, intf2;

    cout << "  Running Test " << __FUNCTION__ << endl;


    if (p_apiObj->IsServerMode() ){
        cout <<  tempstr << " -- Start in Server Listen Mode ---- " << endl;

        //Server Mode // while TRUE loop ?
        for  (int i =0 ; i < 2; i++) {
                p_apiObj->StartNewTxnAndWaitOnRecv();
                //start a new txn and call recv again
        }
        return; //return from server.
    }


    //Client Mode.
    cout <<  tempstr << " -- Start in Client Send Mode ---- " << endl;
    intf1.SetParams("A54012348765",  25, 2);
    cookies[0] = p_apiObj->PerformActionOnObj(&intf1, CREATE, &cookies[0]);

    intf1.SetParams("B56", 35, 5);
    cookies[1] = p_apiObj->PerformActionOnObj(&intf1, MODIFY, &cookies[0]);


    p_apiObj->FlushObjActions();

    intf2.SetParams("eth2", 211, 3);
    cookies[2] = p_apiObj->PerformActionOnObj(&intf2, CREATE, &cookies[0]);
    intf2.SetParams("eth2", 311, 6);
    cookies[3] = p_apiObj->PerformActionOnObj(&intf2, MODIFY, &cookies[0]);


    p_apiObj->FlushObjActions();
    cout << " ------ MAIN OUTPUT ---- " << " Copies --> " << TestObject::NumCopyCtors_ << endl;
}

void ut50_nnmsg(int argc, char **argv) {
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

int initInfra(int argc, NxClientApi **p_apiObj)  {


    *p_apiObj               = new NxClientApi();
    NanoMsg     *p_nanoMsg  = nullptr;

    if (argc == 2)       p_nanoMsg = new  NanoMsg(SOCKET_ADDR, NanoMsg::ConnClient);
    else if (argc == 3 ) p_nanoMsg = new  NanoMsg(SOCKET_ADDR, NanoMsg::ConnServer);
    else                 p_nanoMsg = new  NanoMsg(SOCKET_ADDR, NanoMsg::ConnClient);

    (*p_apiObj)->SetupSockConnection(p_nanoMsg);

    return 0;

}

#endif
