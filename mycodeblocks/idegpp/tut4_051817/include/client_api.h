#ifndef NxClientApi_H
#define NxClientApi_H

#include "NxProcObj.h"
#include "sample_object.h"
#include "txn.h"
#include "nn_msg.h"
#include <map>

using namespace std;

typedef int cookie_t;


class NxClientApi : public NxProcObj
{
    public:
        enum EndPointType { Undefined, NanoMsgSock , UnixPipe };

                NxClientApi() ;
        virtual ~NxClientApi();
        int     SetupClientEndPoint(IN const char *,IN EndPointType );
        int     SetupServerEndPoint(IN const char *,IN EndPointType );
        int     StartNewTxnAndWaitOnRecv();
        bool    IsClientMode();
        bool    IsServerMode();

        //ut4
        int     StartTxnWithId(IN int curr_txn_no);
        int     AddActionToTxn(IN int curr_txn_no, IN TestObject *obj,IN enum  action_t , OUT cookie_t *);
        int     FlushTxn(IN int curr_txn_no);
        //abort add



        virtual void PrintPrintMe();
        virtual void PrintReflection() {}
        virtual void LogMe(){}

        //Below Public Apis -- Deprecated for App use.
        int     SetupSockConnection(NanoMsg *ptr);
        //ut1
        int     PerformActionOnObj(TestObject *,enum  action_t , cookie_t *);
        int     FlushObjActions();
        //ut2
        int     StartTxn();
        int     FlushTxn();
        //ut3
        int     StartTxn(int *curr_txn_no);



    protected:

    private:
        map<int, NxTxnMgr*>      txnMap_;
        int                      txnNum_;
        EndPointType             endPtType_;
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
