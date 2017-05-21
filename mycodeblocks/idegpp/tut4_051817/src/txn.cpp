#include "txn.h"
#include <unistd.h>

const int NxTxnMgr::MAX_TXN_BUFFER_SZ = 512;

NxTxnMgr::NxTxnMgr() {
    ObjectId_   = 0;
    TxnPyldSz_  = 0;
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

int NxTxnMgr::ConvertToBuffer() {
    map<int, TestObject >::iterator  iter;
    TestObject r_obj; // = nullptr;
    int ret_length;
    char *obj_pld_ptr ;

    TxnPayload_t *p_pld = (TxnPayload_t *)TxnBuffer;
    p_pld->txn_curr_index = TxnNo_;
    p_pld->txn_flags      = 0;
    p_pld->txn_ret_status = 0x01020403;
    p_pld->txn_sz         = sizeof(TxnPayload_t) - sizeof(p_pld->obj_pyld_start);
    cout << "Sundar ::: " << p_pld->txn_sz << endl;

    //add the heade
    obj_pld_ptr = p_pld->obj_pyld_start;

    //TxnBuffer[0] =
    for (iter = ActionsMap_.begin(); iter != ActionsMap_.end(); iter++ ) {
        r_obj = iter->second;
        //ret_length = r_obj.ConvertToBuffer(TxnBuffer, MAX_TXN_BUFFER_SZ);
        ret_length = r_obj.ConvertToBuffer(obj_pld_ptr, MAX_TXN_BUFFER_SZ);

        obj_pld_ptr     += ret_length;
        p_pld->txn_sz   += ret_length;

    }
    //modify the header and footer.

    return p_pld->txn_sz;

}

void NxTxnMgr::SendTxnBuffer(NanoMsg *p_txnSock, int pld_bytes) {
    int sent_bytes = 0;
    int payload_size = pld_bytes;
    p_txnSock->Send(TxnBuffer, payload_size, 0, &sent_bytes);
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
