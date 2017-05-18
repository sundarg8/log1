#ifndef NxClientTxn_H
#define NxClientTxn_H

#include "NxProcObj.h"
#include <iostream>
#include <map>
#include "sample_object.h"
using namespace std;

//#define MAX_TXN_BUFFER_SZ 512
typedef int cookie;

class NxClientTxn : public NxProcObj
{
    public:
        NxClientTxn();
        virtual ~NxClientTxn();
        static const int MAX_TXN_BUFFER_SZ;

        void SetNxClientTxnNum(int val_NxClientTxn) {
            TxnNo_ = val_NxClientTxn;
        }
        int GetNxClientTxnNum() {
            return TxnNo_;
        }
        cookie TxnAddObj(TestObject *, enum action_t  , cookie);

        virtual void PrintPrintMe();
        virtual void PrintReflection() {}
        virtual void LogMe(){}
        void ConvertToBuffer();


    protected:
    private:
        int TxnNo_;
        int ObjectId_;
        map<int, TestObject>   ActionsMap_;
        char TxnBuffer[512];


};

#endif // NxClientTxn_H
