#ifndef NxClientApi_H
#define NxClientApi_H

#include <iostream>
#include <unistd.h>
#include <map>
#include <vector>

#include "NxProcObj.h"
#include "sample_object.h"
#include "txn.h"
#include "nn_msg.h"

using namespace std;

typedef int cookie_t;

//int NxProcClientApiInit(int argc, NxClientApi **p_apiObj);

class NxClientApi : public NxProcObj
{
    public:

        NxClientApi(IN NxClientApiConnectionParams *params);
        virtual ~NxClientApi();
        int     SetupConnection();

        /*
         *   StartNewTxn
         *      Starts a Txn in the process context
         *      @p_txn_no
         *          - if NULL

         */
        virtual int  StartNewTxn(IN_OUT int *p_txn_no = nullptr, IN NxClientApiTxnParams *p_params = nullptr);
        virtual int  CloseTxn   (IN int *p_txn_no = nullptr);
        virtual int  AddObjectActionToTxn(IN TestObject *obj, IN ClientApiObjAction , OUT ClientApiObjCookie *);
        virtual int  AddObjectActionToTxn(IN int txn_no, IN TestObject *obj, IN ClientApiObjAction , OUT ClientApiObjCookie *);

        virtual int  AddObjectAction(TestObject *,ClientApiObjAction , ClientApiObjCookie *);
        virtual int  FlushAllObjectActions();

        virtual int  StartNewTxnAndWaitOnRecv();
        bool IsServerMode();
        virtual int  SetAppCallbackFn(IN ClientCbFn app_cb);



        //abort add

        //Inherited APIs
        virtual void PrintPrintMe();
        virtual void PrintReflection() {}
        virtual void LogMe(){}



    protected:
        NxClientApi() ;
        NxClientApiConnectionParams connParams_; //TBD

        virtual int   SetTxnParams(IN NxClientApiTxnParams *p_txn_params){ return 0; }
        virtual bool IsClientMode();

        //To Be Deleted..
        enum EndPointType { Undefined, NanoMsgSock , UnixPipe };// TSD
        int     SetupClientEndPoint(IN const char *,IN EndPointType );
        int     SetupServerEndPoint(IN const char *,IN EndPointType );

        //Below Public Apis -- Deprecated for App use.
        int     SetupSockConnection(NanoMsg *ptr);
        int     PerformActionOnObj(TestObject *,ClientApiObjAction , ClientApiObjCookie *);
        int     FlushObjActions();
        int     StartTxn();
        int     FlushTxn();
        int     StartTxn(int *curr_txn_no);
        int     StartTxnWithId(IN int curr_txn_no);
        int     AddActionToTxn(IN int curr_txn_no, IN TestObject *obj,IN ClientApiObjAction , OUT ClientApiObjCookie *);
        int     FlushTxn(IN int curr_txn_no);

        NanoMsg                  * p_nnSock;
        map<int, NxTxnMgr*>      ApiTxnMap_;
        map<int, NxTxnMgr*>      ApiRespTxnMap_;
        ClientCbFn               ApiCbFn_;


    private:
        int                      txnNum_;
        EndPointType             endPtType_;
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
