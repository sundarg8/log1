#ifndef CLIENT_API_H
#define CLIENT_API_H

#include "sample_object.h"
#include "txn.h"
#include <map>
#include <iomanip>

using namespace std;

typedef int cookie;

class client_api
{
    public:
        client_api() ;
        virtual ~client_api();
        static cookie v3_perform_action_on_obj(sample_object *,enum  action_t , cookie);

        static void flush_obj_actions();
        static void display_client_txn_map();

    protected:
    private:
        static int thread_local_txn_num;
        static map<int, txn*> client_txn_map;

};

#endif // CLIENT_API_H
