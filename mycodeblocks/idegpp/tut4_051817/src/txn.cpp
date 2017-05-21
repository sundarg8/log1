#include "txn.h"
#include <unistd.h>

const int NxTxnMgr::MAX_TXN_BUFFER_SZ = 512;

NxTxnMgr::NxTxnMgr() {
    ObjectId_ = 0;
    memset(TxnBuffer, 0, sizeof(TxnBuffer));
}

NxTxnMgr::~NxTxnMgr() {}

void NxTxnMgr::SetNxTxnMgrNum(int val_NxTxnMgr) {
    TxnNo_ = val_NxTxnMgr;
}

int NxTxnMgr::GetNxTxnMgrNum() {
    return TxnNo_;
}

int NxTxnMgr::TxnAddObj(TestObject *obj_data, enum action_t action_type , cookie req_cookie) {
    ++ObjectId_;
    ActionsMap_.insert(pair<int, TestObject&> (ObjectId_, *obj_data));
    return ObjectId_;
}


void NxTxnMgr::PrintPrintMe() {
    map<int, TestObject >::iterator  iter;
    TestObject r_obj; // = nullptr;
    for (iter = ActionsMap_.begin(); iter != ActionsMap_.end(); iter++ ) {
        cout <<  setw(9) << " ObjectId_ : " << iter->first << "  " ;
        r_obj = iter->second;
        r_obj.PrintPrintMe();
    }
}

void NxTxnMgr::ConvertToBuffer() {
    map<int, TestObject >::iterator  iter;
    TestObject r_obj; // = nullptr;
    int ret_length;
    for (iter = ActionsMap_.begin(); iter != ActionsMap_.end(); iter++ ) {
        r_obj = iter->second;
        ret_length = r_obj.ConvertToBuffer(TxnBuffer, MAX_TXN_BUFFER_SZ);
    }
}

void NxTxnMgr::SendTxnBuffer(NanoMsg *p_txnSock) {
    int sent_bytes = 0;
    p_txnSock->Send(TxnBuffer, 32, 0, &sent_bytes);
    cout << __FUNCTION__ << " Sent Bytes to Svr : " << sent_bytes << endl;

}

void NxTxnMgr::RecvTxnBuffer(NanoMsg *p_txnSock) {
    int recv_bytes = 0;
    static int Recv_txn_count = 0;
    p_txnSock->Recv(TxnBuffer, 512, 0, &recv_bytes);
    cout <<  " \n Recv Buf- " << Recv_txn_count++ << "  : " << "Recv Bytes from  Client : " << recv_bytes << endl;
    p_txnSock->PrintBytes(TxnBuffer, recv_bytes);
    sleep(1);
}

/* ////////////////////////BACKUP CODE /////////////////////////////////////////////////////////

#if 0
cookie NxTxnMgr::TxnAddObj(TestObject *obj_data, enum action_t action_type , cookie req_cookie) {
    //TestObject *curr_obj = new TestObject();
    //(*curr_obj) = obj_data;
    ++ObjectId_;
    //ActionsMap_.insert(pair<int, TestObject> (ObjectId_, *curr_obj));
    ActionsMap_.insert(pair<int, TestObject&> (ObjectId_, *obj_data));
    return ObjectId_;

}
#endif // 0
*/
