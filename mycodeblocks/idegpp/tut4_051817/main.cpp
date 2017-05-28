
#include "NxServerApi.h"


#define SOCKET_ADDR "ipc:///data/pair_xx_cb6.ipc"
#define tempstr "May27-v2 "
using namespace std;
void ut6_client(NxClientApi *, int test_case);
void ut6_server(NxClientApi *, int test_case);
std::set<uint8_t> InsertFailureInObjActions;


int app_cb_handler(int rrtoken, int return_status,
        TestObject *p_obj, ClientApiObjAction obj_action) {
    cout << "CB fn call for cookie : " << rrtoken ;
    cout << " with ret_status : " << return_status;
    cout << " for ObjAction : " << obj_action  << endl;

    p_obj->PrintPrintMe();

    return NxProcSUCCESS;
}

int NxClientInitializeConnectionParams(NxClientApiConnectionParams *p_conn_params,
                NxClientApiConnectMode app_mode = NxClientApiClientMode) {

    p_conn_params->connection_type = NxClientApiNanoMsgSock;
    p_conn_params->is_blocking_connection = true;
    strncpy(p_conn_params->connection_addr, SOCKET_ADDR,
            sizeof(p_conn_params->connection_addr));

    p_conn_params->connection_mode = app_mode;

    return NxProcSUCCESS;
}

int app_in_client_mode (std::vector<uint8_t> const &tests) {
    NxClientApi *p_apiObj = nullptr;
    NxClientApiConnectionParams conn_params;


    ClientCbFn  my_cb= &app_cb_handler;
    NxClientInitializeConnectionParams(&conn_params);
    p_apiObj = new NxClientApi(&conn_params);
    p_apiObj->SetupConnection();

    p_apiObj->SetAppCallbackFn(my_cb);

    for (auto n : tests)   ut6_client(p_apiObj, n);

    return NxProcSUCCESS;
}

int app_in_server_mode (std::vector<uint8_t> const &tests) {
    NxClientApi *p_apiObj = nullptr;
    NxClientApiConnectionParams conn_params;

    NxClientInitializeConnectionParams(&conn_params, NxClientApiServerMode );
    p_apiObj = new NxServerApi(&conn_params);
    p_apiObj->SetupConnection();

    InsertFailureInObjActions.insert(5);

    for (auto n : tests)   ut6_server(p_apiObj, n);

    return NxProcSUCCESS;
}

int main(int argc, char**argv) {

    //std::vector<uint8_t> tests { 1, 2 , 3 , 4};
    std::vector<uint8_t> tests {4};


    if (0) {            ///dual process mode.
        if (3 != argc) {
            app_in_client_mode(tests);
        } else {
            app_in_server_mode(tests);
        }
    } else {            ///Single process, multi-thread mode
        pid_t pid = fork();
        if (pid == 0) {
            app_in_client_mode(tests);
        } else {
            app_in_server_mode(tests);
        }
    }

    return 0;
}



int NxProcClientApiInit(NxClientApi **p_apiObj) {

    NxClientApiConnectionParams conn_params;
    NxClientInitializeConnectionParams(&conn_params);

    (*p_apiObj) = new NxClientApi(&conn_params);
    (*p_apiObj)->SetupConnection();

    return NxProcSUCCESS;
}

int NxProcServerApiInit(NxClientApi **p_apiObj) {

    NxClientApiConnectionParams conn_params;
    NxClientInitializeConnectionParams(&conn_params, NxClientApiServerMode );

    (*p_apiObj) = new NxServerApi(&conn_params);
    (*p_apiObj)->SetupConnection();

    return NxProcSUCCESS;
}



/*
  Modes of App Usage of NxClientApi. -->
    TestCase->
    1) No Understanding / Usage of Txn ,, except for call to  FlushAllObjectActions().
    2) Call StartNewTxn() and CloseTxn() with no specific reference to Txn_no and EMPTY params.
    3) Call StartNewTxn(ptr) with pass-value of txn as zero, and expects overwritte value to be allocated Txn_no.
    4) Call StartNewTxn(ptr) with explicitly set value of txn_no.
*/


int rrtoken[10];
void ut6_client(NxClientApi *p_apiObj, int test_case) {

    ClientApiObjCookie cookies[10];



    for (int j=0; j<10; j++) {
        rrtoken[j] = (j+1) * 7;
        cookies[j].data_ptr = &rrtoken[j];
        cookies[j].magic_no = 135+j;
    }

    TestObject  intf1, intf2, intf3, intf4;
    int first_txn_no = -1, second_txn_no = -1;

    cout << "  Running Test " << __FUNCTION__ << " user_mode : " << test_case << endl;

    intf1.SetParams((char *)"A54010",  7, 12);
    intf2.SetParams((char *)"B6689", 13, 15);
    intf3.SetParams((char *)"eth3", 19, 13);
    intf4.SetParams((char *)"eth4", 23, 10);


    switch (test_case) {

        case 1: {
            p_apiObj->AddObjectAction(&intf1, ClientApiObjActionCreate, &cookies[0]);
            p_apiObj->AddObjectAction(&intf2, ClientApiObjActionModify, &cookies[1]);
            p_apiObj->FlushAllObjectActions();
            p_apiObj->StartNewTxnAndWaitOnRecv();

            p_apiObj->AddObjectAction(&intf3, ClientApiObjActionCreate, &cookies[2]);
            p_apiObj->AddObjectAction(&intf4, ClientApiObjActionModify, &cookies[3]);
            p_apiObj->FlushAllObjectActions();
            p_apiObj->StartNewTxnAndWaitOnRecv();

            break;

        }

        case 2: {
            if (NxProcFAILURE == p_apiObj->StartNewTxn())  return;
            p_apiObj->AddObjectActionToTxn(&intf1, ClientApiObjActionCreate, &cookies[0]);
            p_apiObj->AddObjectActionToTxn(&intf2, ClientApiObjActionModify, &cookies[1]);
            p_apiObj->CloseTxn();
            p_apiObj->StartNewTxnAndWaitOnRecv();

            if (NxProcFAILURE == p_apiObj->StartNewTxn())  return;
            p_apiObj->AddObjectActionToTxn(&intf3, ClientApiObjActionCreate, &cookies[2]);
            p_apiObj->AddObjectActionToTxn(&intf4, ClientApiObjActionModify, &cookies[3]);
            p_apiObj->CloseTxn();
            p_apiObj->StartNewTxnAndWaitOnRecv();

            break;
        }

        case 3: {
            first_txn_no = 0;
            if (NxProcFAILURE == p_apiObj->StartNewTxn(&first_txn_no))  return;
            p_apiObj->AddObjectActionToTxn(first_txn_no, &intf1, ClientApiObjActionCreate, &cookies[0]);
            p_apiObj->AddObjectActionToTxn(&intf2, ClientApiObjActionModify, &cookies[1]);  // <--
            //p_apiObj->AddObjectActionToTxn(first_txn_no, &intf1, ClientApiObjActionUndefined, &cookies[0]);
            //p_apiObj->AddObjectActionToTxn(&intf2, ClientApiObjActionModify, &cookies[1]);  // <--
            p_apiObj->CloseTxn(&first_txn_no);
            p_apiObj->StartNewTxnAndWaitOnRecv();


            second_txn_no = 0;
            if (NxProcFAILURE == p_apiObj->StartNewTxn(&second_txn_no))  return;
            p_apiObj->AddObjectActionToTxn(&intf3, ClientApiObjActionCreate, &cookies[2]);
            p_apiObj->AddObjectActionToTxn(second_txn_no,&intf4, ClientApiObjActionDelete, &cookies[3]);
            p_apiObj->CloseTxn(&second_txn_no);
            p_apiObj->StartNewTxnAndWaitOnRecv();


            break;
        }

        case 4: {
            first_txn_no = 105;
            second_txn_no = 245;

            if (NxProcFAILURE == p_apiObj->StartNewTxn(&second_txn_no))  return;
            if (NxProcFAILURE == p_apiObj->StartNewTxn(&first_txn_no))  return;

            p_apiObj->AddObjectActionToTxn(first_txn_no, &intf1, ClientApiObjActionDelete, &cookies[0]);
            p_apiObj->AddObjectActionToTxn(first_txn_no, &intf2, ClientApiObjActionCreate, &cookies[1]);
            p_apiObj->CloseTxn(&first_txn_no);
            p_apiObj->StartNewTxnAndWaitOnRecv();

            p_apiObj->AddObjectActionToTxn(second_txn_no,&intf3, ClientApiObjActionModify, &cookies[2]);
            p_apiObj->AddObjectActionToTxn(second_txn_no,&intf4, ClientApiObjActionDelete, &cookies[3]);
            p_apiObj->CloseTxn(&second_txn_no);
            p_apiObj->StartNewTxnAndWaitOnRecv();

            break;

        }
        default:{
            break;
        }
    }

    sleep(1);

    cout << endl << "------ MAIN OUTPUT ---- " << tempstr << " Copies --> " << TestObject::NumCopyCtors_ << endl;

}


void ut6_server(NxClientApi *p_apiObj, int test_case) {
    cout << "  Running Test " << __FUNCTION__ << " user_mode : " << test_case << endl;
    for  (int i =0 ; i < 2; i++) {
        p_apiObj->StartNewTxnAndWaitOnRecv();
    }

    sleep(1);
    return;
}
