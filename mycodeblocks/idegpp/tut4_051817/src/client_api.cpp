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

}

int NxClientApi::SetupSockConnection(NanoMsg *ptr) {
    p_nnSock = ptr;
    p_nnSock->ConnectToEndPoint();
    p_nnSock->SetClientApiRef(this);
}

bool NxClientApi::IsClientMode() {
    return (p_nnSock->IsClient());
}

bool NxClientApi::IsServerMode() {
    return (p_nnSock->IsClient() == false);
}

int NxClientApi::StartNewTxnAndWaitOnRecv() {
    NxTxnMgr*       p_NxTxnMgr = new NxTxnMgr;
    int recv_bytes = 0;
    if (p_nnSock !=nullptr) {
        p_NxTxnMgr->RecvTxnBufferFromNano(p_nnSock, &recv_bytes);
        p_NxTxnMgr->ConvBufferToTxn(recv_bytes);
    }
    txnMap_[txnNum_]    =   p_NxTxnMgr;  //TBD ?? TxnNum from payload.

}

int NxClientApi::IncrementToNextTxn() {
    txnNum_ +=1 ;
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

cookie NxClientApi::PerformActionOnObj(TestObject *intf,
            enum  action_t  action_type, cookie req_cookie) {

    map<int, NxTxnMgr*>::iterator    iter;
    NxTxnMgr*                        p_NxTxnMgr = nullptr;
    cookie                           alloted_id;

    iter = txnMap_.find(txnNum_);

    if (iter == txnMap_.end()) {

        p_NxTxnMgr       =    new NxTxnMgr();
        p_NxTxnMgr->SetNxTxnMgrNum(txnNum_);
        txnMap_[txnNum_]    =   p_NxTxnMgr;

    } else {
        p_NxTxnMgr = iter->second;
    }

    alloted_id  = p_NxTxnMgr->TxnAddObj(intf, action_type, req_cookie);
    return alloted_id;
}

void NxClientApi::FlushObjActions() {

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
    sleep(1);
}

int NxClientApi::StartTxn() {
    map<int, NxTxnMgr*>::iterator       iter;
    NxTxnMgr                            *p_NxTxnMgr = nullptr;


    iter = txnMap_.find(txnNum_);
    if (iter != txnMap_.end()) {
        cout << "Abort Needed Here.. New Txn already exists for TxnNum " << txnNum_;
        return -1;
    }

    p_NxTxnMgr          =    new NxTxnMgr();
    p_NxTxnMgr->SetNxTxnMgrNum(txnNum_);
    txnMap_[txnNum_]    =   p_NxTxnMgr;


}

void NxClientApi::FlushTxn() {

    map<int, NxTxnMgr*>::iterator       iter;
    NxTxnMgr                            *p_NxTxnMgr = nullptr;
    int                                 pld_bytes   = 0;

    iter = txnMap_.find(txnNum_);

    if (iter == txnMap_.end()) {
        cout << "Abort Needed Here.. Txn does not exist for TxnNum " << txnNum_;
        return;
    }
    p_NxTxnMgr = iter->second;
    p_NxTxnMgr->PrintPrintMe();
    pld_bytes = p_NxTxnMgr->ConvertToBuffer();

    if (p_nnSock !=nullptr) {
        p_NxTxnMgr->SendTxnBuffToNano(p_nnSock, pld_bytes);
    }
    sleep(1);
    IncrementToNextTxn();

}

int NxClientApi::StartTxn(int *curr_txn_no) {
    map<int, NxTxnMgr*>::iterator       iter;
    NxTxnMgr                            *p_NxTxnMgr = nullptr;

    IncrementToNextTxn();

    iter = txnMap_.find(txnNum_);
    if (iter != txnMap_.end()) {
        cout << "Abort Needed Here.. New Txn already exists for TxnNum " << txnNum_;
        return -1;
    }

    p_NxTxnMgr          =    new NxTxnMgr();
    p_NxTxnMgr->SetNxTxnMgrNum(txnNum_);
    txnMap_[txnNum_]    =   p_NxTxnMgr;
    *curr_txn_no        = txnNum_;
    //IncrementToNextTxn();

}


int NxClientApi::StartTxnWithId(int curr_txn_no) {
    map<int, NxTxnMgr*>::iterator       iter;
    NxTxnMgr                            *p_NxTxnMgr = nullptr;

    iter = txnMap_.find(curr_txn_no);
    if (iter != txnMap_.end()) {
        cout << __FUNCTION__ <<  "Abort Needed Here.. New Txn already exists for TxnNum " << txnNum_;
        return -1;
    }

    p_NxTxnMgr              =   new NxTxnMgr();
    p_NxTxnMgr->SetNxTxnMgrNum(curr_txn_no);
    txnMap_[curr_txn_no]    =   p_NxTxnMgr;
    return 0;

}

void NxClientApi::FlushTxn(int curr_txn_no) {

    map<int, NxTxnMgr*>::iterator       iter;
    NxTxnMgr                            *p_NxTxnMgr = nullptr;
    int                                 pld_bytes   = 0;

    iter = txnMap_.find(curr_txn_no);

    if (iter == txnMap_.end()) {
        cout << __FUNCTION__ << " Abort Needed Here.. Txn does not exist for TxnNum " << curr_txn_no;
        return;
    }
    p_NxTxnMgr = iter->second;
    p_NxTxnMgr->PrintPrintMe();
    pld_bytes = p_NxTxnMgr->ConvertToBuffer();

    if (p_nnSock !=nullptr) {
        p_NxTxnMgr->SendTxnBuffToNano(p_nnSock, pld_bytes);
    }
    sleep(1);
    //IncrementToNextTxn();

}

cookie NxClientApi::AddActionToTxn(int curr_txn_no, TestObject *intf,
            enum  action_t  action_type, cookie req_cookie) {

    map<int, NxTxnMgr*>::iterator    iter;
    NxTxnMgr*                        p_NxTxnMgr = nullptr;
    cookie                           alloted_id;

    iter = txnMap_.find(curr_txn_no);

    if (iter == txnMap_.end()) {
        cout << " " << __FUNCTION__ << "Abort Needed Here.. Txn does not exist for TxnNum " << curr_txn_no;
        return -1;
    } else {
        p_NxTxnMgr = iter->second;
    }

    alloted_id  = p_NxTxnMgr->TxnAddObj(intf, action_type, req_cookie);
    return alloted_id;
}

