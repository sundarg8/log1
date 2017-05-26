#include "NxServerApi.h"

NxServerApi::NxServerApi() : NxClientApi()
{
    //ctor
}

NxServerApi::~NxServerApi()
{
    //dtor
}

NxServerApi::NxServerApi(IN NxClientApiConnectionParams *params)
    : NxClientApi(params)
{
}

int NxServerApi::StartNewTxnAndWaitOnRecv() {
    NxTxnMgr*       p_NxTxnMgr = new NxTxnMgr(connParams_.connection_mode, NxClientApiMsgRecv);
    int rcvd_txn_num =0 ;
    int recv_bytes = 0;
    if (p_nnSock !=nullptr) {
        cout << __FUNCTION__ <<  "  -- Server Waiting to Recv Data... "  << endl;

        p_NxTxnMgr->RecvTxnBufferFromNano(p_nnSock, &recv_bytes);
        p_NxTxnMgr->ConvBufferToTxn(recv_bytes, &rcvd_txn_num);

    }
    txnMap_[rcvd_txn_num]    =   p_NxTxnMgr;

    CloseTxn(&rcvd_txn_num);

    return NxProcSUCCESS;
}

