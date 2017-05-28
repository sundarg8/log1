#include "txn.h"
#include <unistd.h>

const int NxTxnMgr::MAX_TXN_BUFFER_SZ = 512;

NxTxnMgr::NxTxnMgr() {
    ObjectId_   = 0;
    TxnPyldSz_  = 0;
    memset(TxnBuffer_, 0, sizeof(TxnBuffer_));
}


NxTxnMgr::NxTxnMgr(NxClientApiConnectMode conn_mode, NxClientApiDirection direction) {
    TxnParentApiMode_  = conn_mode;
    TxnMsgDirn_        = direction;
    NxTxnMgr();
}


NxTxnMgr::~NxTxnMgr() {}

int NxTxnMgr::SetNxTxnMgrNum(int val_NxTxnMgr) {
    TxnNo_ = val_NxTxnMgr;

    return NxProcSUCCESS;

}

int NxTxnMgr::GetNxTxnMgrNum() {   //TB_Edited
    return TxnNo_;
}

//TB_Edited
int NxTxnMgr::TxnAddObj(TestObject *obj_data, ClientApiObjAction action_type , ClientApiObjCookie *req_cookie) {
    std::pair<std::map<int,TestObject>::iterator,bool> ret_iter;
    ClientApiObjEncap *obj_encap = new ClientApiObjEncap();
    ++ObjectId_;

    //object_id TO action type map.
    ret_iter = ActionsMap_.insert(pair<int, TestObject&> (ObjectId_, *obj_data));
    obj_encap->objAction    = action_type;
    obj_encap->objCookie    = *req_cookie;
    obj_encap->objPtr       = &(ret_iter.first->second);
    obj_encap->objRetStatus = NxProcFAILURE;

    if (ClientApiObjActionUndefined != action_type)
        ObjEncapMap_.insert(pair<int,ClientApiObjEncap *> (ObjectId_, obj_encap));

    return NxProcSUCCESS;
    //return ObjectId_;
}

int NxTxnMgr::SetClientApiRef(NxClientApi *parent_client) {


    //p_ParentClientApi = parent_client;
    return NxProcSUCCESS;
}
//TB_Edited
int NxTxnMgr::ConvertToBuffer() {
    map<int, TestObject >::iterator  iter;
    TestObject r_obj;
    int ret_length;
    int obj_id;
    char *obj_pld_ptr ;
    ClientApiObjAction obj_action = ClientApiObjActionUndefined;
    syserr_t  obj_ret_status = NxProcFAILURE;

    TxnPayload_t *p_pld = (TxnPayload_t *)TxnBuffer_;
    p_pld->txn_curr_index = TxnNo_;
    p_pld->txn_flags      = 0;
    p_pld->txn_ret_status = 0x01020403;
    p_pld->txn_sz         = sizeof(TxnPayload_t) - sizeof(p_pld->obj_pyld_start);

    //add the header
    obj_pld_ptr = p_pld->obj_pyld_start;

    for (iter = ActionsMap_.begin(); iter != ActionsMap_.end(); iter++ ) {
        r_obj = iter->second;
        obj_id = iter->first;

        if (ObjEncapMap_[iter->first]) {
             obj_action     = (ObjEncapMap_[iter->first])->objAction;
             obj_ret_status = (ObjEncapMap_[iter->first])->objRetStatus;
        }
        ret_length = r_obj.ConvertToBuffer(obj_id,obj_pld_ptr, MAX_TXN_BUFFER_SZ, obj_action, obj_ret_status);
        obj_pld_ptr     += ret_length;
        p_pld->txn_sz   += ret_length;

    }
    //modify the header and footer.

    return p_pld->txn_sz;

}

int NxTxnMgr::ConvBufferToTxn(int recv_bytes, int *rcvd_txn_no) {
    TxnPayload_t    *txn_pld = (TxnPayload_t *)TxnBuffer_;
    ObjPldHeader_t *obj_pld_start;
    int curr_sz = 0;
    //char *buff_ptr;
    int obj_count=0;
    std::set<uint8_t>::iterator set_it;


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

        TestObject  curr_obj;

        curr_obj.ConvertToObjInst((char *)obj_pld_start->unit_pyld_start );
        if (NxClientApiClientMode != TxnParentApiMode_)
            ActionsMap_.insert(pair<int, TestObject&> (obj_pld_start->unit_id, curr_obj));
        else
            RespActionsMap_.insert(pair<int, TestObject&> ((obj_pld_start->unit_id+0), curr_obj)); //HI_UT_DEL

        if (ClientApiObjActionUndefined != obj_pld_start->unit_action) {  //HI_TBD ...do you want to not insert encap-map on client recv ??
            ClientApiObjEncap *obj_encap = new ClientApiObjEncap();
            obj_encap->objAction    = obj_pld_start->unit_action;
            obj_encap->objRetStatus = obj_pld_start->unit_status;

            if (NxClientApiClientMode != TxnParentApiMode_) {
                set_it = InsertFailureInObjActions.find(obj_pld_start->unit_id);
                if (set_it != InsertFailureInObjActions.end()) {
                    obj_encap->objRetStatus = NxProcInsertFailureVal;
                } else {
                    obj_encap->objRetStatus = NxProcSUCCESS;
                }
            }

            //obj_encap->objCookie    = obj_pld_start->unit_cookie;
            obj_encap->objPtr       = &curr_obj;
            if (NxClientApiClientMode != TxnParentApiMode_) {
                ObjEncapMap_.insert(pair<int,ClientApiObjEncap *> (obj_pld_start->unit_id, obj_encap));

            } else {
                RespObjEncapMap_.insert(pair<int,ClientApiObjEncap *> (obj_pld_start->unit_id, obj_encap));

            }

        }

        obj_pld_start = (ObjPldHeader_t *) ((char *)obj_pld_start + obj_pld_start->unit_sz);
        curr_sz       += obj_pld_start->unit_sz;
        obj_count++;
    }

    //commented-may27  PrintPrintMe();

    ///////FindRespCookieAndCallApp();

    *rcvd_txn_no = TxnNo_;

    return NxProcSUCCESS;
}

int NxTxnMgr::FindRespCookieAndCallApp(NxTxnMgr *p_req_txn, ClientCbFn txn_cb_fn) {

    map<int, TestObject >::iterator  req_iter;
    map<int, TestObject >::iterator  iter;

    TestObject r_obj;

    cout << endl;
    if (NxClientApiServerMode == TxnParentApiMode_) cout <<  " Server ";
    else                                            cout << " Client ";

    if (NxClientApiMsgSend == TxnMsgDirn_)  cout << " Send " ;
    else                                    cout << " Recv " ;

    cout << "Txn Number  -->  " << TxnNo_ << endl;
    cout << "-------------------------------------" ;

    for (iter = RespActionsMap_.begin(); iter != RespActionsMap_.end(); iter++ ) {
        int resp_obj_id = iter->first;
        //if (1 ) { /// p_req_txn->ActionsMap_[resp_obj_id])
        req_iter = p_req_txn->ActionsMap_.find(resp_obj_id);
        if (req_iter != p_req_txn->ActionsMap_.end()) {

            if (RespObjEncapMap_[resp_obj_id]) {

                 if (p_req_txn->ObjEncapMap_[resp_obj_id]) {

                    cout << endl << "Object_id " << resp_obj_id
                            << "  CB_Action_Cookie " << *(int *)((p_req_txn->ObjEncapMap_[resp_obj_id])->objCookie.data_ptr)
                            << "  CB_Magic_no. " << (p_req_txn->ObjEncapMap_[resp_obj_id])->objCookie.magic_no
                            << endl;
                    int rrtoken = *(int *)((p_req_txn->ObjEncapMap_[resp_obj_id])->objCookie.data_ptr);
                    txn_cb_fn(rrtoken, RespObjEncapMap_[resp_obj_id]->objRetStatus, &(iter->second),  RespObjEncapMap_[resp_obj_id]->objAction);
                }
            }
        }
    }

    //commented-may27 PrintPrintMe();

    return NxProcSUCCESS;
}

int NxTxnMgr::SendTxnBuffToNano(NanoMsg *p_txnSock, int pld_bytes) {
    int sent_bytes = 0;
    int payload_size = pld_bytes;
    p_txnSock->Send(TxnBuffer_, payload_size, 0, &sent_bytes);

    return NxProcSUCCESS;
}

int NxTxnMgr::RecvTxnBufferFromNano(NanoMsg *p_txnSock, int *recv_bytes) {
    p_txnSock->Recv(TxnBuffer_, 512, 0, recv_bytes);
    //p_txnSock->PrintBytes(TxnBuffer_, *recv_bytes);

    return NxProcSUCCESS;
}



void NxTxnMgr::PrintPrintMe() {
    map<int, TestObject >::iterator  iter;
    //map<int, ClientApiObjEncap *>::iterator encap_iter;
    TestObject r_obj;
    //if (p_ParentClientApi->IsServerMode())
    cout << endl;
    if (NxClientApiServerMode == TxnParentApiMode_) cout <<  " Server ";
    else                                            cout << " Client ";

    if (NxClientApiMsgSend == TxnMsgDirn_)  cout << " Send " ;
    else                                    cout << " Recv " ;

    cout << "Txn Number -->  " << TxnNo_ << endl;

    for (iter = ActionsMap_.begin(); iter != ActionsMap_.end(); iter++ ) {
        cout <<  setw(9) << " ObjectId_ : " << iter->first << "  " ;
        if (ObjEncapMap_[iter->first]) {
            cout <<  " Action " << (ObjEncapMap_[iter->first])->objAction << "  ";
            if (NxClientApiServerMode != TxnParentApiMode_)
                cout <<  " Cookie " << *(int *)(ObjEncapMap_[iter->first])->objCookie.data_ptr << " " ;
            cout <<  " Magic " << (ObjEncapMap_[iter->first])->objCookie.magic_no << " " << endl << "         " ;

        }
        r_obj = iter->second;
        r_obj.PrintPrintMe();

    }
    for (iter = RespActionsMap_.begin(); iter != RespActionsMap_.end(); iter++ ) {
        cout <<  setw(9) << " R-ObjectId_ : " << iter->first << "  " ;
        if (RespObjEncapMap_[iter->first]) {
            cout <<  " R-Action " << (RespObjEncapMap_[iter->first])->objAction << "  ";
            //cout <<  " Cookie " << *(int *)(ObjEncapMap_[iter->first])->objCookie.data_ptr << " " ;
            cout <<  " R-Magic " << (RespObjEncapMap_[iter->first])->objCookie.magic_no << " " << endl << "         " ;

        }
        r_obj = iter->second;
        r_obj.PrintPrintMe();

    }
    cout <<endl;
}



