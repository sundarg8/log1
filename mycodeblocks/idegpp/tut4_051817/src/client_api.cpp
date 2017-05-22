#include "client_api.h"
#include <iostream>
#include <unistd.h>


//123456789012345678901234567890123456789012345678901234567890123456789012345678


NxClientApi::NxClientApi() {
    txnNum_         = 22;
    p_nnSock        = nullptr;
}

NxClientApi::~NxClientApi() {}

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


int NxClientApi::StartRecvTxnAndWaitOnRecv() {
    NxTxnMgr*       p_NxTxnMgr = new NxTxnMgr;
    int recv_bytes = 0;
    if (p_nnSock !=nullptr) {
        p_NxTxnMgr->RecvTxnBuffer_(p_nnSock, &recv_bytes);
        p_NxTxnMgr->ConvBufferToTxn(recv_bytes);
    }
    txnMap_[txnNum_]    =   p_NxTxnMgr;  //TBD ?? TxnNum from payload.

}

 cookie NxClientApi::PerformActionOnObj(TestObject *intf,
            enum  action_t  action_type, cookie req_cookie) {

    map<int, NxTxnMgr*>::iterator    iter;
    NxTxnMgr*                        p_NxTxnMgr = nullptr;
    cookie                              alloted_id;

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
    ///call transport send() for the current active NxTxnMgr..
    map<int, NxTxnMgr*>::iterator       iter;
    NxTxnMgr                            *p_NxTxnMgr = nullptr;

    iter = txnMap_.find(txnNum_);
    cout << " ---- Start of Flush TXN  --> "  <<  txnNum_ << "\n\n";
    if (iter != txnMap_.end()) {
        p_NxTxnMgr = iter->second;
        p_NxTxnMgr->PrintPrintMe();
        int pld_bytes = p_NxTxnMgr->ConvertToBuffer();
        if (p_nnSock !=nullptr) {
            p_NxTxnMgr->SendTxnBuffer_(p_nnSock, pld_bytes);
        }
    }
    cout <<  " \n #### End of Flush TXN  --> "  << txnNum_ <<  "\n\n ..Sleeping for 2 secs\n";
    sleep(2);
    ++txnNum_;
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


