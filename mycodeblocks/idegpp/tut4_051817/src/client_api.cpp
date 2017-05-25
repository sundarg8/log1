#include "client_api.h"
#include <iostream>
#include <unistd.h>
//123456789012345678901234567890123456789012345678901234567890123456789012345678


NxClientApi::NxClientApi() {
    txnNum_         = 22;   //Default Txn Start Number.
    p_nnSock        = nullptr;
}

NxClientApi::NxClientApi(IN NxClientApiConnectionParams *params) {
    txnNum_     = 22;       //Default Txn Start Number.
    p_nnSock    = nullptr;
    std::memcpy(&connParams_, params, sizeof(NxClientApiConnectionParams));
}

NxClientApi::~NxClientApi() {}

int NxClientApi::SetupConnection() {
    if (NxClientApiNanoMsgSock == connParams_.connection_type) {
        NanoMsg     *p_nanoMsg  = nullptr;
        if (NxClientApiServerMode == connParams_.connection_mode) {
            p_nanoMsg = new  NanoMsg(connParams_.connection_addr, NanoMsg::ConnServer);
        } else {
            p_nanoMsg = new  NanoMsg(connParams_.connection_addr, NanoMsg::ConnClient);
        }
        SetupSockConnection(p_nanoMsg);
    }
    return NxProcSUCCESS;
}



int NxClientApi::StartNewTxn(int *p_txn_no, NxClientApiTxnParams *p_params) {
    if (nullptr == p_txn_no) {
        StartTxn();
        return NxProcSUCCESS;
    }
    if (0 == *p_txn_no) {
        StartTxn(p_txn_no); //IN_OUT case
    } else {
        StartTxnWithId(*p_txn_no);  //IN case
    }

    return NxProcSUCCESS;
}

int NxClientApi::CloseTxn(int *p_txn_no) {
    if (nullptr == p_txn_no) {
        FlushObjActions();
        return NxProcSUCCESS;
    } else if (0 != *p_txn_no) {
        FlushTxn(*p_txn_no);
    } else {
        FlushTxn();
    }
    return NxProcSUCCESS;
}

int NxClientApi::AddObjectAction(TestObject *intf,
            enum  action_t  action_type, cookie_t *cookie) {

    return PerformActionOnObj(intf,action_type, cookie);

}

int NxClientApi::FlushAllObjectActions() {
    return FlushObjActions();
}

int NxClientApi::AddObjectActionToTxn(IN int txn_no, IN TestObject *obj,IN enum  action_t action_type, OUT cookie_t *req_cookie) {
    return AddActionToTxn(txn_no,  obj, action_type, req_cookie);
}


int NxClientApi::AddObjectActionToTxn(IN TestObject *obj,IN enum  action_t action_type , OUT cookie_t *req_cookie) {
    return AddActionToTxn(0,  obj, action_type, req_cookie);
}

int NxClientApi::SetupClientEndPoint(const char *sock_addr, EndPointType end_type) {
    if (end_type == NanoMsgSock) {
        NanoMsg     *p_nanoMsg  = nullptr;
        p_nanoMsg   = new  NanoMsg(sock_addr, NanoMsg::ConnClient);
        SetupSockConnection(p_nanoMsg);
    } else {
        cout << " TBD ... "  << endl;
    }

    endPtType_ = end_type;


    return NxProcSUCCESS;
}

int NxClientApi::SetupServerEndPoint(const char * sock_addr, EndPointType end_type) {
    if (end_type == NanoMsgSock) {
        NanoMsg     *p_nanoMsg  = nullptr;
        p_nanoMsg   = new  NanoMsg(sock_addr, NanoMsg::ConnServer);
        SetupSockConnection(p_nanoMsg);
    } else {
        cout << " TBD ... "  << endl;
    }

    endPtType_ = end_type;


    return NxProcSUCCESS;
}

int NxClientApi::SetupSockConnection(NanoMsg *ptr) {
    p_nnSock = ptr;
    p_nnSock->ConnectToEndPoint();
    p_nnSock->SetClientApiRef(this);


    return NxProcSUCCESS;

}

bool NxClientApi::IsClientMode() {
    return (p_nnSock->IsClient());
}

bool NxClientApi::IsServerMode() {
    return (p_nnSock->IsClient() == false);
}

int NxClientApi::StartNewTxnAndWaitOnRecv() {
    NxTxnMgr*       p_NxTxnMgr = new NxTxnMgr;
    int rcvd_txn_num =0 ;
    int recv_bytes = 0;
    if (p_nnSock !=nullptr) {
        p_NxTxnMgr->RecvTxnBufferFromNano(p_nnSock, &recv_bytes);
        p_NxTxnMgr->ConvBufferToTxn(recv_bytes, &rcvd_txn_num);

    }
    txnMap_[rcvd_txn_num]    =   p_NxTxnMgr;

    //UT_code.. To loop the msg back to Client.TBD.
    if (IsServerMode() )    { FlushTxn(rcvd_txn_num); }

    return NxProcSUCCESS;
}

int NxClientApi::IncrementToNextTxn() {
    txnNum_ +=1 ;

    return NxProcSUCCESS;

}

void NxClientApi::PrintPrintMe() {
    map<int, NxTxnMgr*>::iterator    iter;
    NxTxnMgr*                        p_NxTxnMgr = nullptr;

    for (iter = txnMap_.begin(); iter != txnMap_.end(); iter++ ) {
        cout    << __FUNCTION__ << setw(9)
                << iter->first << " : " << iter->second  << endl;

        p_NxTxnMgr = iter->second;
        p_NxTxnMgr->PrintPrintMe();
        cout << endl;
    }
}

int NxClientApi::PerformActionOnObj(TestObject *intf,
            enum  action_t  action_type, cookie_t *cookie) {

    map<int, NxTxnMgr*>::iterator    iter;
    NxTxnMgr*                        p_NxTxnMgr = nullptr;

    iter = txnMap_.find(txnNum_);

    if (iter == txnMap_.end()) {

        p_NxTxnMgr       =    new NxTxnMgr();
        p_NxTxnMgr->SetNxTxnMgrNum(txnNum_);
        txnMap_[txnNum_]    =   p_NxTxnMgr;

    } else {
        p_NxTxnMgr = iter->second;
    }

    p_NxTxnMgr->TxnAddObj(intf, action_type, cookie);
    return NxProcSUCCESS;
}

int NxClientApi::FlushObjActions() {

    map<int, NxTxnMgr*>::iterator       iter;
    NxTxnMgr                            *p_NxTxnMgr = nullptr;

    iter = txnMap_.find(txnNum_);
    if (iter != txnMap_.end()) {
        p_NxTxnMgr = iter->second;
        p_NxTxnMgr->PrintPrintMe();
        int pld_bytes = p_NxTxnMgr->ConvertToBuffer();
        if (p_nnSock !=nullptr) {
            p_NxTxnMgr->SendTxnBuffToNano(p_nnSock, pld_bytes);
        }
    }
    IncrementToNextTxn();

    return NxProcSUCCESS;
}

int NxClientApi::StartTxn() {
    map<int, NxTxnMgr*>::iterator       iter;
    NxTxnMgr                            *p_NxTxnMgr = nullptr;


    iter = txnMap_.find(txnNum_);
    if (iter != txnMap_.end()) {
        cout << "Abort Needed Here.. New Txn already exists for TxnNum " << txnNum_;
        return NxProcFAILURE;
    }

    p_NxTxnMgr          =    new NxTxnMgr();
    p_NxTxnMgr->SetNxTxnMgrNum(txnNum_);
    txnMap_[txnNum_]    =   p_NxTxnMgr;

    //new1 IncrementToNextTxn();

    return NxProcSUCCESS;
}

int NxClientApi::FlushTxn() {

    map<int, NxTxnMgr*>::iterator       iter;
    NxTxnMgr                            *p_NxTxnMgr = nullptr;
    int                                 pld_bytes   = 0;

    iter = txnMap_.find(txnNum_);

    if (iter == txnMap_.end()) {
        cout << "Abort Needed Here.. Txn does not exist for TxnNum " << txnNum_;
        return NxProcFAILURE;
    }
    p_NxTxnMgr = iter->second;
    p_NxTxnMgr->PrintPrintMe();
    pld_bytes = p_NxTxnMgr->ConvertToBuffer();

    if (p_nnSock !=nullptr) {
        p_NxTxnMgr->SendTxnBuffToNano(p_nnSock, pld_bytes);
    }
    //new1 IncrementToNextTxn();

    return NxProcSUCCESS;
}

int NxClientApi::StartTxn(int *curr_txn_no) {
    map<int, NxTxnMgr*>::iterator       iter;
    NxTxnMgr                            *p_NxTxnMgr = nullptr;

    IncrementToNextTxn();

    iter = txnMap_.find(txnNum_);
    if (iter != txnMap_.end()) {
        cout << "Abort Needed Here.. New Txn already exists for TxnNum " << txnNum_;
        return NxProcFAILURE;
    }

    p_NxTxnMgr          =    new NxTxnMgr();
    p_NxTxnMgr->SetNxTxnMgrNum(txnNum_);
    txnMap_[txnNum_]    =   p_NxTxnMgr;
    *curr_txn_no        = txnNum_;

    return NxProcSUCCESS;
}


int NxClientApi::StartTxnWithId(int curr_txn_no) {
    map<int, NxTxnMgr*>::iterator       iter;
    NxTxnMgr                            *p_NxTxnMgr = nullptr;

    iter = txnMap_.find(curr_txn_no);
    if (iter != txnMap_.end()) {
        cout << __FUNCTION__ <<  "Abort Needed Here.. New Txn already exists for TxnNum " << txnNum_;
        return NxProcFAILURE;
    }

    p_NxTxnMgr              =   new NxTxnMgr();
    p_NxTxnMgr->SetNxTxnMgrNum(curr_txn_no);
    txnMap_[curr_txn_no]    =   p_NxTxnMgr;


    return NxProcSUCCESS;
}

int NxClientApi::FlushTxn(int curr_txn_no) {

    map<int, NxTxnMgr*>::iterator       iter;
    NxTxnMgr                            *p_NxTxnMgr = nullptr;
    int                                 pld_bytes   = 0;

    iter = txnMap_.find(curr_txn_no);

    if (iter == txnMap_.end()) {
        cout << __FUNCTION__ << " Abort Needed Here.. Txn does not exist for TxnNum " << curr_txn_no;
        return NxProcFAILURE;
    }
    p_NxTxnMgr = iter->second;
    p_NxTxnMgr->PrintPrintMe();
    pld_bytes = p_NxTxnMgr->ConvertToBuffer();

    if (p_nnSock !=nullptr) {
        p_NxTxnMgr->SendTxnBuffToNano(p_nnSock, pld_bytes);
    }

    return NxProcSUCCESS;
}

int NxClientApi::AddActionToTxn(int curr_txn_no, TestObject *intf,
            enum  action_t  action_type, cookie_t *req_cookie) {

    map<int, NxTxnMgr*>::iterator    iter;
    NxTxnMgr*                        p_NxTxnMgr = nullptr;

    //new1
    if (0 == curr_txn_no)  curr_txn_no = txnNum_;

    iter = txnMap_.find(curr_txn_no);

    if (iter == txnMap_.end()) {
        cout << " " << __FUNCTION__ << "Abort Needed Here.. Txn does not exist for TxnNum " << curr_txn_no;
        return NxProcFAILURE;
    } else {
        p_NxTxnMgr = iter->second;
    }


    p_NxTxnMgr->TxnAddObj(intf, action_type, req_cookie);
    return NxProcSUCCESS;
}



/*
int NxClientApi::StartNewTxn(NxClientApiTxnParams *p_txn_params) {
    if (nullptr == p_txn_params) {
        StartTxn();
    }
    if (0 == p_txn_params->txn_no) {
        StartTxn(&(p_txn_params->txn_no)); //IN_OUT case
    } else {
        StartTxnWithId(p_txn_params->txn_no);  //IN case
    }

    return NxProcSUCCESS;
}

int NxClientApi::CloseTxn(NxClientApiTxnParams *p_txn_params) {
    if (nullptr == p_txn_params) {
        FlushObjActions();
    } else if (0 != p_txn_params->txn_no) {
        FlushTxn(p_txn_params->txn_no);
    } else {
        FlushTxn();
    }
    return NxProcSUCCESS;
}



*/
