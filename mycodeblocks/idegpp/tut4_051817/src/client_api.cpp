#include "client_api.h"
#include <iostream>
//123456789012345678901234567890123456789012345678901234567890123456789012345678


NxClientApi::NxClientApi() { txnNum_ = 22;}

NxClientApi::~NxClientApi() {}


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
    NxTxnMgr*                           p_NxTxnMgr = nullptr;

    iter = txnMap_.find(txnNum_);
    cout << " ---- Start of Flush TXN  --> "  <<  txnNum_ << "\n\n";
    if (iter != txnMap_.end()) {
        p_NxTxnMgr = iter->second;
        p_NxTxnMgr->PrintPrintMe();
        p_NxTxnMgr->ConvertToBuffer();
    }
    cout <<  " \n #### End of Flush TXN  --> "  << txnNum_ <<  "\n\n\n";
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


