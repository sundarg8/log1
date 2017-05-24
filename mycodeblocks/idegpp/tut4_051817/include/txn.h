#ifndef NxTxnMgr_H
#define NxTxnMgr_H

#include "NxProcObj.h"
#include <iostream>
#include <map>
#include "sample_object.h"
#include "nn_msg.h"
using namespace std;


//#define MAX_TXN_BUFFER_SZ 512

typedef int cookie_t;

typedef struct  TxnPayload_ {
    int txn_sz;
    int txn_ret_status;

    int txn_curr_index;
    int txn_type;
    int txn_flags;
    char obj_pyld_start[512];
} TxnPayload_t;


class NxTxnMgr : public NxProcObj
{
    public:
                NxTxnMgr();
        virtual ~NxTxnMgr();

        int     SetNxTxnMgrNum(IN int val_NxTxnMgr);
        int     TxnAddObj(IN TestObject *, IN enum action_t  , OUT cookie_t *);

        int     ConvertToBuffer();
        int     ConvBufferToTxn(IN int recv_bytes, OUT int *rcvd_txn_no);

        int     SendTxnBuffToNano(IN NanoMsg *p_txnSock, IN int pld_bytes);
        int     RecvTxnBufferFromNano(IN NanoMsg *p_txnSock, OUT int *recv_bytes);

        virtual void    PrintPrintMe();
        virtual void    PrintReflection() {}
        virtual void    LogMe(){}


    protected:
    private:
        static const int        MAX_TXN_BUFFER_SZ;

        map<int, TestObject>    ActionsMap_;
        int                     TxnNo_;
        int                     ObjectId_;
        char                    TxnBuffer_[512];
        int                     TxnPyldSz_;

        int                     GetNxTxnMgrNum();

};

#endif // NxTxnMgr_H
