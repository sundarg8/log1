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

        void SetNxClientTxnNum(int val_NxClientTxn);
        cookie TxnAddObj(TestObject *, enum action_t  , cookie);
        void ConvertToBuffer();

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
        int GetNxClientTxnNum();



};

#endif // NxClientTxn_H
