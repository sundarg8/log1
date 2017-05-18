#include "client_api.h"
#include <iostream>



NxClientApi::NxClientApi() {
    txnNum_ = 22;
}

NxClientApi::~NxClientApi() {
}



cookie NxClientApi::PerformActionOnObj(TestObject *intf,
            enum  action_t  action_type, cookie req_cookie) {

    map<int, NxClientTxn*>::iterator iter;
    NxClientTxn* p_NxClientTxn = nullptr;
    cookie alloted_id;

    iter = txnMap_.find(txnNum_);

    if (iter == txnMap_.end()) {
        p_NxClientTxn = new NxClientTxn();
        p_NxClientTxn->SetNxClientTxnNum(txnNum_);
        txnMap_[txnNum_] = p_NxClientTxn;
    } else {
        p_NxClientTxn = iter->second;
    }

    alloted_id= p_NxClientTxn->TxnAddObj(intf, action_type, req_cookie);
    return alloted_id;
}



void NxClientApi::FlushObjActions() {
    ///call transport send() for the current active NxClientTxn..
    map<int, NxClientTxn*>::iterator iter;
    NxClientTxn* p_NxClientTxn = nullptr;

    iter = txnMap_.find(txnNum_);
    cout << " ---- Start of Flush TXN  --> "  <<  txnNum_ << "\n\n";
    if (iter != txnMap_.end()) {
        p_NxClientTxn = iter->second;
        p_NxClientTxn->PrintPrintMe();
        p_NxClientTxn->ConvertToBuffer();
    }
    cout <<  " \n #### End of Flush TXN  --> "  << txnNum_ <<  "\n\n\n";
    ++txnNum_;

}



void NxClientApi::PrintPrintMe() {
    map<int, NxClientTxn*>::iterator iter;
    NxClientTxn* p_NxClientTxn = nullptr;

    for (iter = txnMap_.begin(); iter != txnMap_.end(); iter++ ) {
        cout << __FUNCTION__ << setw(9) << iter->first << " : " << iter->second  << endl;
        p_NxClientTxn = iter->second;
        p_NxClientTxn->PrintPrintMe();
        cout << endl;
    }


}


