
#include "client_api.h"

#define SOCKET_ADDR "ipc:///data/pair_xx_cb5.ipc"
#define tempstr "May24-v1 "

using namespace std;
void ut6_client(NxClientApi *, int usage_mode);


int NxClientApiHalInit(int argc, NxClientApi **p_apiObj) {

    Syserr_t                ret_val = NxProcSUCCESS;

    NxClientApiConnectionParams conn_params;
    conn_params.connection_type = NxClientApiNanoMsgSock;
    conn_params.is_blocking_connection = true;
    conn_params.connection_mode =
        (3 == argc) ? NxClientApiServerMode : NxClientApiClientMode;

    strncpy(conn_params.connection_addr, SOCKET_ADDR,
            sizeof(conn_params.connection_addr));


    (*p_apiObj) = new NxClientApi(&conn_params);
    (*p_apiObj)->SetupConnection();

    return ret_val;
}

int main(int argc, char**argv) {

    cout << " -- Start of Main() ---- " << endl;
    NxClientApi *p_apiObj = nullptr;

    NxClientApiHalInit(argc, &p_apiObj);
    for (int mode=1; mode < 5; mode++) {
        ut6_client(p_apiObj,mode);
        sleep(2);
    }
    cout << tempstr << "exec of ut1_client "  << endl;
    return 0;
}



/*
* Modes of App Usage of NxClientApi. -->
    a) No Understanding / Usage of Txn ,, except for call to  FlushAllObjectActions().
    b) Call StartNewTxn() and CloseTxn() with no specific reference to Txn_no and EMPTY params.
    c) Call StartNewTxn(ptr) with pass-value of txn as zero, and expects overwritte value to be allocated Txn_no.
    d) Call StartNewTxn(ptr) with explicitly set value of txn_no.
*/



void ut6_client(NxClientApi *p_apiObj, int user_mode) {

    cookie_t      cookies[10];
    TestObject  intf1, intf2, intf3, intf4;
    int first_txn_no = -1, second_txn_no = -1;
    cout << "  Running Test " << __FUNCTION__ << " user_mode : " << user_mode << endl;

    if (p_apiObj->IsServerMode() ) {
        cout <<  tempstr << " -- Start in Server Listen Mode ---- " << endl;
        for  (int i =0 ; i < 2; i++) {
                p_apiObj->StartNewTxnAndWaitOnRecv();
        }
        return;
    }

    //Client Mode.
    cout <<  tempstr << " -- Start in Client Send Mode ---- " << endl;
    intf1.SetParams((char *)"A54010",  7, 12);
    intf2.SetParams((char *)"B6689", 13, 15);
    intf3.SetParams((char *)"eth3", 19, 13);
    intf4.SetParams((char *)"eth4", 23, 10);


    switch (user_mode) {

        case 1: {
            p_apiObj->AddObjectAction(&intf1, CREATE, &cookies[0]);
            p_apiObj->AddObjectAction(&intf2, MODIFY, &cookies[1]);
            p_apiObj->FlushAllObjectActions();
            p_apiObj->StartNewTxnAndWaitOnRecv();

            p_apiObj->AddObjectAction(&intf3, CREATE, &cookies[2]);
            p_apiObj->AddObjectAction(&intf4, MODIFY, &cookies[3]);
            p_apiObj->FlushAllObjectActions();
            p_apiObj->StartNewTxnAndWaitOnRecv();

            break;

        }

        case 2: {
            if (NxProcFAILURE == p_apiObj->StartNewTxn())  return;
            p_apiObj->AddObjectActionToTxn(&intf1, CREATE, &cookies[0]);
            p_apiObj->AddObjectActionToTxn(&intf2, MODIFY, &cookies[1]);
            p_apiObj->CloseTxn();
            p_apiObj->StartNewTxnAndWaitOnRecv();

            if (NxProcFAILURE == p_apiObj->StartNewTxn())  return;
            p_apiObj->AddObjectActionToTxn(&intf3, CREATE, &cookies[2]);
            p_apiObj->AddObjectActionToTxn(&intf4, MODIFY, &cookies[3]);
            p_apiObj->CloseTxn();
            p_apiObj->StartNewTxnAndWaitOnRecv();

            break;
        }

        case 3: {
            first_txn_no = 0;
            if (NxProcFAILURE == p_apiObj->StartNewTxn(&first_txn_no))  return;
            p_apiObj->AddObjectActionToTxn(first_txn_no, &intf1, CREATE, &cookies[0]);
            p_apiObj->AddObjectActionToTxn(&intf2, MODIFY, &cookies[1]);  // <--
            p_apiObj->CloseTxn(&first_txn_no);
            p_apiObj->StartNewTxnAndWaitOnRecv();

            second_txn_no = 0;
            if (NxProcFAILURE == p_apiObj->StartNewTxn(&second_txn_no))  return;
            p_apiObj->AddObjectActionToTxn(&intf3, CREATE, &cookies[2]);
            p_apiObj->AddObjectActionToTxn(second_txn_no,&intf4, MODIFY, &cookies[3]);
            p_apiObj->CloseTxn(&second_txn_no);
            p_apiObj->StartNewTxnAndWaitOnRecv();

            break;
        }

        case 4: {
            first_txn_no = 105;
            second_txn_no = 245;

            if (NxProcFAILURE == p_apiObj->StartNewTxn(&second_txn_no))  return;
            if (NxProcFAILURE == p_apiObj->StartNewTxn(&first_txn_no))  return;

            p_apiObj->AddObjectActionToTxn(first_txn_no, &intf1, CREATE, &cookies[0]);
            p_apiObj->AddObjectActionToTxn(first_txn_no, &intf2, MODIFY, &cookies[1]);
            p_apiObj->CloseTxn(&first_txn_no);
            p_apiObj->StartNewTxnAndWaitOnRecv();

            p_apiObj->AddObjectActionToTxn(second_txn_no,&intf3, CREATE, &cookies[2]);
            p_apiObj->AddObjectActionToTxn(second_txn_no,&intf4, MODIFY, &cookies[3]);
            p_apiObj->CloseTxn(&second_txn_no);
            p_apiObj->StartNewTxnAndWaitOnRecv();

            break;

        }
        default:{
            break;
        }
    }

    cout << " ------ MAIN OUTPUT ---- " << " Copies --> " << TestObject::NumCopyCtors_ << endl;

}


#if 0

#define explicit_txn

#ifdef explicit_txn
#define replace1 &curr_txn_no
#define replace2 &sec_txn_no
#define replace3 curr_txn_no
#define replace4 sec_txn_no

#else

#define replace1
#define replace2
#define replace3 curr_txn_no
#define replace4 sec_txn_no

#endif



void ut5_client(NxClientApi *p_apiObj) {

    cookie_t      cookies[10];
    TestObject  intf1, intf2;
    int replace3 = 0 ,replace4 = 0;


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


#ifdef explicit_txn
    replace4  = 245 ;
    if (NxProcFAILURE == p_apiObj->StartNewTxn(replace2))
        return;
    replace3 = 105 ;

#endif

    if (NxProcFAILURE == p_apiObj->StartNewTxn(replace1))
        return;

    intf1.SetParams((char *)"A54012348765",  26, 12);
    p_apiObj->AddActionToTxn(replace3, &intf1, CREATE, &cookies[0]);
    intf1.SetParams((char *)"B56", 36, 15);
    p_apiObj->AddActionToTxn(replace3, &intf1, MODIFY, &cookies[1]);
    p_apiObj->CloseTxn(replace1);

    for  (int i =0 ; i < 1; i++) {
        p_apiObj->StartNewTxnAndWaitOnRecv();   ///UT Code
    }

#ifndef explicit_txn
    //replace4  = 245 ;
    if (NxProcFAILURE == p_apiObj->StartNewTxn(replace2))
        return;
#endif


    intf2.SetParams((char *)"eth2", 51, 23);
    p_apiObj->AddActionToTxn(replace4, &intf2, CREATE, &cookies[2]);
    intf2.SetParams((char *)"eth2", 52, 21);
    p_apiObj->AddActionToTxn(replace4, &intf2, MODIFY, &cookies[3]);
    p_apiObj->CloseTxn(replace2);

    for  (int i =0 ; i < 1; i++) {
        p_apiObj->StartNewTxnAndWaitOnRecv();    ///UT Code
    }
    cout << " ------ MAIN OUTPUT ---- " << " Copies --> " << TestObject::NumCopyCtors_ << endl;



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



void ut6_old_client(NxClientApi *p_apiObj) {

    cookie_t      cookies[10];
    TestObject  intf1, intf2;
    NxClientApiTxnParams curr_txn_no , sec_txn_no;
    //curr_txn_no.txn_no = 106;
    //sec_txn_no.txn_no  = 256;

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

    if (NxProcFAILURE == p_apiObj->StartNewTxn(&sec_txn_no)) {
        return;
    }
    if (NxProcFAILURE == p_apiObj->StartNewTxn(&curr_txn_no)) {
        return;
    }

    cout << " Curr   Txn no : " << curr_txn_no.txn_no << "  Priority : " << curr_txn_no.priority <<endl;
    cout << " Sec    Txn no : " <<  sec_txn_no.txn_no << "  Priority : " <<  sec_txn_no.priority <<endl;

    intf1.SetParams((char *)"A54012348765",  26, 12);
    p_apiObj->AddActionToTxn(curr_txn_no.txn_no, &intf1, CREATE, &cookies[0]);
    intf1.SetParams((char *)"B56", 36, 15);
    p_apiObj->AddActionToTxn(curr_txn_no.txn_no, &intf1, MODIFY, &cookies[1]);
    p_apiObj->CloseTxn(&curr_txn_no);

    for  (int i =0 ; i < 1; i++) {
        p_apiObj->StartNewTxnAndWaitOnRecv();   ///UT Code
    }

    intf2.SetParams((char *)"eth2", 51, 23);
    p_apiObj->AddActionToTxn(sec_txn_no.txn_no, &intf2, CREATE, &cookies[2]);
    intf2.SetParams((char *)"eth2", 52, 21);
    p_apiObj->AddActionToTxn(sec_txn_no.txn_no, &intf2, MODIFY, &cookies[3]);
    p_apiObj->CloseTxn(&sec_txn_no);

    for  (int i =0 ; i < 1; i++) {
        p_apiObj->StartNewTxnAndWaitOnRecv();    ///UT Code
    }
    cout << " ------ MAIN OUTPUT ---- " << " Copies --> " << TestObject::NumCopyCtors_ << endl;


}

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
