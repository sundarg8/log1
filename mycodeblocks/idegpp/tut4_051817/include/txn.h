#ifndef NxTxnMgr_H
#define NxTxnMgr_H

#include "NxProcObj.h"
#include <iostream>
#include <map>
#include "sample_object.h"
#include "nn_msg.h"
using namespace std;


//#define MAX_TXN_BUFFER_SZ 512
typedef int cookie;

class NxTxnMgr : public NxProcObj
{
    public:
        NxTxnMgr();
        virtual ~NxTxnMgr();

        void SetNxTxnMgrNum(int val_NxTxnMgr);
        int TxnAddObj(TestObject *, enum action_t  , cookie);
        void ConvertToBuffer();
        void SendTxnBuffer(NanoMsg *p_txnSock);
        void RecvTxnBuffer(NanoMsg *p_txnSock);

        virtual void PrintPrintMe();
        virtual void PrintReflection() {}
        virtual void LogMe(){}


    protected:
    private:
        map<int, TestObject>    ActionsMap_;
        int                     TxnNo_;
        int                     ObjectId_;
        char                    TxnBuffer[512];

        static const int        MAX_TXN_BUFFER_SZ;
        int GetNxTxnMgrNum();



};

#endif // NxTxnMgr_H
