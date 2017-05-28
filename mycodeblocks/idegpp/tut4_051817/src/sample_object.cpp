#include "sample_object.h"

int TestObject::NumCopyCtors_ = 0;

TestObject::TestObject()
{
    memset(IntfName_,0, nameSz);
    IntfName_[7] = '\0';
}

TestObject::TestObject(const TestObject &obj) {
    strncpy(IntfName_, obj.IntfName_, nameSz);
    Speed_     = obj.Speed_;
    Stats_     = obj.Stats_;
    ++NumCopyCtors_;
}


TestObject::~TestObject()
{

}

int TestObject::SetParams(char  *val_name, int val_Speed_, int val_Stats_) {
    strncpy(IntfName_, val_name, nameSz);
    Speed_ = val_Speed_;
    Stats_ = val_Stats_;
    return NxProcSUCCESS;
}

int TestObject::PrintBytes(const char *pBytes , const int nBytes) {

    for (int i = 0; i != nBytes; i++)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<unsigned int>(pBytes[i]) << " ";
    }
    cout << std::dec << std::endl;
    return NxProcSUCCESS;

}

void TestObject::PrintPrintMe() {
    cout << setw(10) << "obj_print--> \tAttr_name: " << IntfName_
            << "  Attr_Speed_: " << Speed_
            << "   Attr_Stats_: " << Stats_ <<  endl;
}

int TestObject::ConvertToObjInst(char *buf, TestObject **ptr_to_new_obj) {
    *ptr_to_new_obj = new TestObject();
    (*ptr_to_new_obj)->Speed_ = (int)*buf;  buf += sizeof(Speed_);
    (*ptr_to_new_obj)->Stats_ = (int)*buf;  buf += sizeof(Stats_);
    strncpy((*ptr_to_new_obj)->IntfName_ , buf, sizeof(IntfName_));
    return NxProcSUCCESS;
}

int TestObject::ConvertToObjInst(char *buf, TestObject *ptr_alloc_obj) {
    (ptr_alloc_obj)->Speed_ = (int)*buf;  buf += sizeof(Speed_);
    (ptr_alloc_obj)->Stats_ = (int)*buf;  buf += sizeof(Stats_);
    strncpy((ptr_alloc_obj)->IntfName_ , buf, sizeof(IntfName_));
    return NxProcSUCCESS;

}

int TestObject::ConvertToObjInst(char *buf) {
    Speed_ = (int)*buf;  buf += sizeof(Speed_);
    Stats_ = (int)*buf +100 ;  buf += sizeof(Stats_);
    strncpy(IntfName_ , buf, sizeof(IntfName_));
    return NxProcSUCCESS;
}

int TestObject::ConvertToBuffer(int objectId, char *buf,  int max_length, ClientApiObjAction obj_action, syserr_t obj_status) {
    std::size_t count = 0; //sizeof(*this);
    ObjPldHeader_t obj_pld;
    char *tmpbuf = buf;
    int tmp_sz = 0;

    //Call Serialize data and get obj_buffer && size.
    int obj_data_sz = sizeof(Speed_) + sizeof(Stats_) + sizeof(IntfName_);

    obj_pld.unit_sz = sizeof(ObjPldHeader_t) + obj_data_sz;
    obj_pld.unit_id = objectId;
    obj_pld.unit_action = obj_action ; //TBC
    obj_pld.unit_cookie = 0x11223344 ; //Can be ignored or deleted from code.
    obj_pld.unit_status = obj_status;

    //cout << "start -- buffer  -- " << count  << endl;

    tmp_sz = sizeof(ObjPldHeader_t);
    std::memcpy(buf, &obj_pld, tmp_sz); buf += tmp_sz; count += tmp_sz;

    tmp_sz = sizeof(Speed_);
    std::memcpy(buf, &Speed_, tmp_sz); buf += tmp_sz; count += tmp_sz;

    std::memcpy(buf, &Stats_, sizeof(Stats_)); buf += sizeof(Stats_);
    count += sizeof(Stats_);

    std::memcpy(buf, IntfName_, sizeof(IntfName_));  buf+= sizeof(IntfName_);
    count += sizeof(IntfName_);

    buf = tmpbuf; // Is it needed ??

    //PrintBytes(buf, count);

    //cout << "end   ---- buffer  "  << count <<  endl;
    return count;
}
