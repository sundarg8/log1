#ifndef NxClientApi_H
#define NxClientApi_H

#include <iostream>
#include <unistd.h>
#include <map>
#include "NxProcObj.h"
#include "sample_object.h"
#include "txn.h"
#include "nn_msg.h"

using namespace std;

typedef int cookie_t;

int NxClientApiHalInit(int argc, NxClientApi **p_apiObj);

enum NxClientApiConnectType {
        NxClientApiInvalidConnectType,
        NxClientApiNanoMsgSock,
        NxClientApiNamedPipe,
};

enum NxClientApiConnectMode {
        NxClientApiInvalidConnectMode,
        NxClientApiClientMode,
        NxClientApiServerMode
};

typedef struct NxClientApiConnectionParams_ {
    NxClientApiConnectType  connection_type;
    NxClientApiConnectMode  connection_mode;
    char    connection_addr[48];
    bool    is_blocking_connection;
} NxClientApiConnectionParams;


typedef struct NxClientApiTxnParams_ {
    int     priority = -1;
    int     queue_id = -1;
} NxClientApiTxnParams;

typedef struct ClientApiObjCookie_ {
    void *data_ptr    = nullptr ;
    uint16_t magic_no = 0 ;
} ClientApiObjCookie;


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
        virtual int  AddObjectActionToTxn(IN TestObject *obj, IN enum  action_t , OUT cookie_t *);
        virtual int  AddObjectActionToTxn(IN int txn_no, IN TestObject *obj, IN enum  action_t , OUT cookie_t *);

        virtual int  AddObjectAction(TestObject *,enum  action_t , cookie_t *);
        virtual int  FlushAllObjectActions();

        int     StartNewTxnAndWaitOnRecv();
        bool    IsClientMode();
        bool    IsServerMode();


        //abort add

        //Inherited APIs
        virtual void PrintPrintMe();
        virtual void PrintReflection() {}
        virtual void LogMe(){}



    protected:
        NxClientApi() ;
        virtual int   SetTxnParams(IN NxClientApiTxnParams *p_txn_params){ return 0; }


        //To Be Deleted..
        enum EndPointType { Undefined, NanoMsgSock , UnixPipe };// TSD
        int     SetupClientEndPoint(IN const char *,IN EndPointType );
        int     SetupServerEndPoint(IN const char *,IN EndPointType );


        //Below Public Apis -- Deprecated for App use.
        int     SetupSockConnection(NanoMsg *ptr);
        int     PerformActionOnObj(TestObject *,enum  action_t , cookie_t *);
        int     FlushObjActions();
        int     StartTxn();
        int     FlushTxn();
        int     StartTxn(int *curr_txn_no);
        int     StartTxnWithId(IN int curr_txn_no);
        int     AddActionToTxn(IN int curr_txn_no, IN TestObject *obj,IN enum  action_t , OUT cookie_t *);
        int     FlushTxn(IN int curr_txn_no);


    private:
        map<int, NxTxnMgr*>      txnMap_;
        int                      txnNum_;
        EndPointType             endPtType_;
        NanoMsg                  * p_nnSock;
        NxClientApiConnectionParams connParams_;
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
