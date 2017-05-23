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

        cookie  PerformActionOnObj(TestObject *,enum  action_t , cookie);
        void    FlushObjActions();
        int     SetupSockConnection(NanoMsg *ptr);
        int     StartRecvTxnAndWaitOnRecv();
        bool    IsClientMode();
        bool    IsServerMode();
        void    StartTxn();
        void    EndTxnAndSend();



        //abort add

        virtual void PrintPrintMe();
        virtual void PrintReflection() {}
        virtual void LogMe(){}


    protected:

    private:
        map<int, NxTxnMgr*>      txnMap_;
        int                      txnNum_;
        NanoMsg                  * p_nnSock;


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
