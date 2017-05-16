#ifndef TXN_H
#define TXN_H

#include <iostream>
#include <map>
#include "sample_object.h"
using namespace std;

typedef int cookie;

class txn
{
    public:
        txn();
        virtual ~txn();
        void set_txn_num(int val_txn) {
            txn_no = val_txn;
        }
        int get_txn_num() {
            return txn_no;
        }
        cookie add_obj(sample_object &, enum action_t  , cookie);
        cookie v2_add_obj(sample_object, enum action_t  , cookie);
        void display_actions_map();
        void v2_display_actions_map();

    protected:
    private:
        int txn_no;
        int object_id;
        map<int, sample_object &> actions_map;
        map<int, sample_object>   v2_actions_map;

};

#endif // TXN_H
