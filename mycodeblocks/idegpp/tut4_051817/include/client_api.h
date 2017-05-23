#ifndef NxClientApi_H
#define NxClientApi_H

#include "NxProcObj.h"
#include "sample_object.h"
#include "txn.h"
#include "nn_msg.h"
#include <map>

using namespace std;

typedef int cookie;


class NxClientApi : public NxProcObj
{
    public:
        NxClientApi() ;
        virtual ~NxClientApi();
        int     SetupSockConnection(NanoMsg *ptr);
        int     StartRecvTxnAndWaitOnRecv();
        bool    IsClientMode();
        bool    IsServerMode();

        //ut1
        cookie  PerformActionOnObj(TestObject *,enum  action_t , cookie);
        void    FlushObjActions();

        //ut2
        int     StartTxn();
        void    FlushTxn();

        //ut3
        int     StartTxn(int *curr_txn_no);
        cookie  AddActionToTxn(int curr_txn_no, TestObject *obj,enum  action_t , cookie);
        void    FlushTxn(int curr_txn_no);

        //ut4
        int     StartTxnWithId(int curr_txn_no);


        //abort add

        virtual void PrintPrintMe();
        virtual void PrintReflection() {}
        virtual void LogMe(){}


    protected:

    private:
        map<int, NxTxnMgr*>      txnMap_;
        int                      txnNum_;
        NanoMsg                  * p_nnSock;
        int                      IncrementToNextTxn();


};

/**
//typdef struct NxClientConnection_ {
//    uint nm_handle;

//}NxClientConenction;

//class NxClinetApiUtil {
//    Send(NxCLientConnection *)
//    Recv(NxClientConnection*)
//}
*/

#endif // NxClientApi_H
