#ifndef NxClientApi_H
#define NxClientApi_H

#include "NxProcObj.h"
#include "sample_object.h"
#include "txn.h"
#include <map>
#include <iomanip>

using namespace std;

typedef int cookie;


class NxClientApi : public NxProcObj
{
    public:
        NxClientApi() ;
        virtual ~NxClientApi();
        cookie PerformActionOnObj(TestObject *,enum  action_t , cookie);
        void FlushObjActions();

        virtual void PrintPrintMe();  //{}
        virtual void PrintReflection() {}
        virtual void LogMe(){}


    protected:
    private:
        int txnNum_;
        map<int, NxClientTxn*> txnMap_;

};

#endif // NxClientApi_H
