#include "txn.h"
#include <unistd.h>

const int NxTxnMgr::MAX_TXN_BUFFER_SZ = 512;

NxTxnMgr::NxTxnMgr() {
    ObjectId_   = 0;
    TxnPyldSz_  = 0;
    memset(TxnBuffer_, 0, sizeof(TxnBuffer_));
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
    //object_id TO action type map.
    ActionsMap_.insert(pair<int, TestObject&> (ObjectId_, *obj_data));
    return ObjectId_;
}


void NxTxnMgr::PrintPrintMe() {
    map<int, TestObject >::iterator  iter;
    TestObject r_obj;
    cout << endl << " Txn Number -->  " << TxnNo_ << endl;
    for (iter = ActionsMap_.begin(); iter != ActionsMap_.end(); iter++ ) {
        cout <<  setw(9) << " ObjectId_ : " << iter->first << "  " ;
        r_obj = iter->second;
        r_obj.PrintPrintMe();
    }
    cout <<endl;
}

int NxTxnMgr::ConvertToBuffer() {
    map<int, TestObject >::iterator  iter;
    TestObject r_obj; // = nullptr;
    int ret_length;
    int obj_id;
    char *obj_pld_ptr ;

    TxnPayload_t *p_pld = (TxnPayload_t *)TxnBuffer_;
    p_pld->txn_curr_index = TxnNo_;
    p_pld->txn_flags      = 0;
    p_pld->txn_ret_status = 0x01020403;
    p_pld->txn_sz         = sizeof(TxnPayload_t) - sizeof(p_pld->obj_pyld_start);
    //cout << "Sundar ::: " << p_pld->txn_sz << endl;

    //add the header
    obj_pld_ptr = p_pld->obj_pyld_start;

    for (iter = ActionsMap_.begin(); iter != ActionsMap_.end(); iter++ ) {
        r_obj = iter->second;
        obj_id = iter->first;

        ret_length = r_obj.ConvertToBuffer(obj_id,obj_pld_ptr, MAX_TXN_BUFFER_SZ);
        obj_pld_ptr     += ret_length;
        p_pld->txn_sz   += ret_length;

    }
    //modify the header and footer.

    return p_pld->txn_sz;

}

void NxTxnMgr::SendTxnBuffToNano(NanoMsg *p_txnSock, int pld_bytes) {
    int sent_bytes = 0;
    int payload_size = pld_bytes;
    p_txnSock->Send(TxnBuffer_, payload_size, 0, &sent_bytes);
    //cout << __FUNCTION__ << " Sent Bytes to Svr : " << sent_bytes << endl;

}

void NxTxnMgr::RecvTxnBufferFromNano(NanoMsg *p_txnSock, int *recv_bytes) {
    static int Recv_txn_count = 0;
    p_txnSock->Recv(TxnBuffer_, 512, 0, recv_bytes);
    //cout <<  " \n Recv Buf- " << Recv_txn_count++ << "  : " << "Recv Bytes from  Client : " << *recv_bytes << endl;
    //p_txnSock->PrintBytes(TxnBuffer_, *recv_bytes);

    sleep(1);

}

int NxTxnMgr::ConvBufferToTxn(int recv_bytes) {
    TxnPayload_t    *txn_pld = (TxnPayload_t *)TxnBuffer_;
    ObjPldHeader_t *obj_pld_start;
    int curr_sz = 0;
    char *buff_ptr;
    int obj_count=0;


    if (txn_pld->txn_sz != recv_bytes) {
        //cout << __FUNCTION__ << " ASSERT here pld sz " << txn_pld->txn_sz
        //    << "not matching recv bytes" << recv_bytes << endl;
        ///TBD ?? Assert ??
    } else {
        //cout << __FUNCTION__ << " Good  here pld sz " << txn_pld->txn_sz
        //    << "  IS matching recv bytes" << recv_bytes << endl;
    }
    TxnNo_              =  txn_pld->txn_curr_index;
    curr_sz             =  sizeof(TxnPayload_t) - sizeof(txn_pld->obj_pyld_start);
    obj_pld_start       =  (ObjPldHeader_t *)txn_pld->obj_pyld_start;


    while ((curr_sz + obj_pld_start->unit_sz) <= recv_bytes && (obj_pld_start->unit_sz != 0)) {
        //cout << "Hello -- "  << obj_pld_start->unit_id << " sz :: "
        //        << obj_pld_start->unit_sz << endl;

        TestObject  curr_obj;
        curr_obj.ConvertToObjInst((char *)obj_pld_start->unit_pyld_start );
        //curr_obj.PrintPrintMe();
        ActionsMap_.insert(pair<int, TestObject&> (obj_pld_start->unit_id, curr_obj));
        obj_pld_start = (ObjPldHeader_t *) ((char *)obj_pld_start + obj_pld_start->unit_sz);
        curr_sz       += obj_pld_start->unit_sz;
        obj_count++;

    }

    PrintPrintMe();

    return 0;
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
