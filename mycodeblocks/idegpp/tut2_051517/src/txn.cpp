#include "txn.h"

const int NxClientTxn::MAX_TXN_BUFFER_SZ = 512;

NxClientTxn::NxClientTxn()
{
    ObjectId_ = 0;
    memset(TxnBuffer, 0, sizeof(TxnBuffer));
}

NxClientTxn::~NxClientTxn()
{
}


cookie NxClientTxn::TxnAddObj(TestObject *obj_data, enum action_t action_type , cookie req_cookie) {
    ++ObjectId_;
    ActionsMap_.insert(pair<int, TestObject&> (ObjectId_, *obj_data));
    return ObjectId_;
}


void NxClientTxn::PrintPrintMe() {
    map<int, TestObject >::iterator  iter;
    TestObject r_obj; // = nullptr;
    for (iter = ActionsMap_.begin(); iter != ActionsMap_.end(); iter++ ) {
        cout <<  setw(9) << " ObjectId_ : " << iter->first << "  " ;
        r_obj = iter->second;
        r_obj.PrintPrintMe();
    }
}

void NxClientTxn::ConvertToBuffer() {
    map<int, TestObject >::iterator  iter;
    TestObject r_obj; // = nullptr;
    int ret_length;
    for (iter = ActionsMap_.begin(); iter != ActionsMap_.end(); iter++ ) {
        r_obj = iter->second;
        //r_obj.PrintPrintMe();

        ret_length = r_obj.ConvertToBuffer(TxnBuffer, MAX_TXN_BUFFER_SZ);
    }
}

/* ////////////////////////BACKUP CODE /////////////////////////////////////////////////////////

#if 0
cookie NxClientTxn::TxnAddObj(TestObject *obj_data, enum action_t action_type , cookie req_cookie) {
    //TestObject *curr_obj = new TestObject();
    //(*curr_obj) = obj_data;
    ++ObjectId_;
    //ActionsMap_.insert(pair<int, TestObject> (ObjectId_, *curr_obj));
    ActionsMap_.insert(pair<int, TestObject&> (ObjectId_, *obj_data));
    return ObjectId_;

}
#endif // 0
*/
