#ifndef TestObject_H
#define TestObject_H
#include <string>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <set>


#include "NxProcObj.h"
using namespace std;

#define NxProcSUCCESS 0
#define NxProcFAILURE -1
#define NxProcInsertFailureVal 4
#define NxProcStatusDefault -3

extern std::set<uint8_t> InsertFailureInObjActions;

enum NxClientApiConnectType {
        NxClientApiInvalidConnectType,
        NxClientApiNanoMsgSock,
        NxClientApiNamedPipe,
};

typedef enum NxClientApiConnectMode_ {
        NxClientApiInvalidConnectMode,
        NxClientApiClientMode,
        NxClientApiServerMode,
} NxClientApiConnectMode;

typedef enum NxClientApiDirection_ {
    NxClientApiInvalidDirection,
    NxClientApiMsgSend,
    NxClientApiMsgRecv,
} NxClientApiDirection;

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
    uint32_t magic_no = 0 ;
} ClientApiObjCookie;


enum  ClientApiObjAction  {
    ClientApiObjActionUndefined,
    ClientApiObjActionGet,
    ClientApiObjActionCreate,
    ClientApiObjActionModify,
    ClientApiObjActionDelete,
};
#define nameSz 12
typedef  int syserr_t;
#define IN
#define OUT
#define IN_OUT


typedef struct tag_ObjPldHeader {
    int         unit_sz;
    int         unit_id;
    ClientApiObjAction    unit_action;
    int         unit_cookie;
    syserr_t    unit_status;
    int         unit_pyld_start[0];
} ObjPldHeader_t;


class TestObject : NxProcObj {
    public:

        TestObject();
        virtual ~TestObject();
        TestObject(const TestObject &obj);

        static int NumCopyCtors_;

        int ConvertToBuffer(int objectId, char *buf,  int max_length ,
                ClientApiObjAction action = ClientApiObjActionUndefined,
                syserr_t obj_status = NxProcStatusDefault);
        int ConvertToObjInst(char *buf, TestObject **ptr_to_new_obj);
        int ConvertToObjInst(char *buf, TestObject *ptr_alloc_obj);
        int ConvertToObjInst(char *buf);

        int PrintBytes(const char *buf , const int bytes);
        int SetParams(char  *val_name, int val_Speed_, int val_Stats_);


        virtual void PrintPrintMe();
        virtual void    PrintReflection() {}
        virtual void    LogMe(){}


    protected:
    private:
        unsigned int     Speed_;
        unsigned int     Stats_;
        char IntfName_[nameSz];

};


typedef int (*ClientCbFn) (int rrtoken, int magic_no, int return_status,
        TestObject *p_obj, ClientApiObjAction obj_action);

#endif // TestObject_H
