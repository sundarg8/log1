#include "client_api.h"
#include <iostream>
#include <unistd.h>
//123456789012345678901234567890123456789012345678901234567890123456789012345678


NxClientApi::NxClientApi() {
    txnNum_         = 22;
    p_nnSock        = nullptr;
}

NxClientApi::~NxClientApi() {}

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
    txnMap_[rcvd_txn_num]    =   p_NxTxnMgr;  //TBD ?? TxnNum from payload.

    //UT_code...TBD.
    if (IsServerMode() )     FlushTxn(rcvd_txn_num);

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
    //cookie_t                         alloted_id;

    iter = txnMap_.find(txnNum_);

    if (iter == txnMap_.end()) {

        p_NxTxnMgr       =    new NxTxnMgr();
        p_NxTxnMgr->SetNxTxnMgrNum(txnNum_);
        txnMap_[txnNum_]    =   p_NxTxnMgr;

    } else {
        p_NxTxnMgr = iter->second;
    }

    //alloted_id  = p_NxTxnMgr->TxnAddObj(intf, action_type, *cookie);

    p_NxTxnMgr->TxnAddObj(intf, action_type, cookie);
    //*cookie     = alloted_id;
    return NxProcSUCCESS;
}

int NxClientApi::FlushObjActions() {

    map<int, NxTxnMgr*>::iterator       iter;
    NxTxnMgr                            *p_NxTxnMgr = nullptr;

    iter = txnMap_.find(txnNum_);
    //cout << " ---- Start of Flush TXN  --> "  <<  txnNum_ << "\n\n";
    if (iter != txnMap_.end()) {
        p_NxTxnMgr = iter->second;
        p_NxTxnMgr->PrintPrintMe();
        int pld_bytes = p_NxTxnMgr->ConvertToBuffer();
        if (p_nnSock !=nullptr) {
            p_NxTxnMgr->SendTxnBuffToNano(p_nnSock, pld_bytes);
        }
    }
    IncrementToNextTxn();

    //cout <<  " \n #### End of Flush TXN  --> "  << txnNum_ <<  "\n\n ..Sleeping for 2 secs\n";
    //My23 sleep(1);

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
    //My23  sleep(1);
    IncrementToNextTxn();

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
    //IncrementToNextTxn();

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
    //My23 sleep(1);
    //IncrementToNextTxn();

    return NxProcSUCCESS;
}

int NxClientApi::AddActionToTxn(int curr_txn_no, TestObject *intf,
            enum  action_t  action_type, cookie_t *req_cookie) {

    map<int, NxTxnMgr*>::iterator    iter;
    NxTxnMgr*                        p_NxTxnMgr = nullptr;
    //cookie_t                           alloted_id;

    iter = txnMap_.find(curr_txn_no);

    if (iter == txnMap_.end()) {
        cout << " " << __FUNCTION__ << "Abort Needed Here.. Txn does not exist for TxnNum " << curr_txn_no;
        return NxProcFAILURE;
    } else {
        p_NxTxnMgr = iter->second;
    }

    //alloted_id  = p_NxTxnMgr->TxnAddObj(intf, action_type, *req_cookie);

    p_NxTxnMgr->TxnAddObj(intf, action_type, req_cookie);
    //*req_cookie = alloted_id;
    return NxProcSUCCESS;
}

