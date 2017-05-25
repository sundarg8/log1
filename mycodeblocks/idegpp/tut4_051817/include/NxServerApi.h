#ifndef NXSERVERAPI_H
#define NXSERVERAPI_H

#include "client_api.h"

class NxServerApi : public NxClientApi
{
    public:
        NxServerApi(IN NxClientApiConnectionParams *params);
        virtual ~NxServerApi();

        virtual int  StartNewTxnAndWaitOnRecv();

    protected:
        NxServerApi();

    private:
};

#endif // NXSERVERAPI_H
