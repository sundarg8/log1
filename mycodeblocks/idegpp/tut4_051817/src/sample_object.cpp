#include "sample_object.h"

int TestObject::NumCopyCtors_ = 0;

TestObject::TestObject()
{
    memset(IntfName_,0, nameSz);
    IntfName_[7] = '\0';
}


TestObject::~TestObject()
{

}

void TestObject::PrintBytes(const char *pBytes , const int nBytes) {

    for (int i = 0; i != nBytes; i++)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<unsigned int>(pBytes[i]) << " ";
    }
    cout << std::dec << std::endl;

}

int TestObject::ConvertToBuffer(int objectId, char *buf,  int max_length) {
    std::size_t count = 0; //sizeof(*this);
    ObjPldHeader_t obj_pld;
    char *tmpbuf = buf;
    int tmp_sz = 0;

    //Call Serialize data and get obj_buffer && size.
    int obj_data_sz = sizeof(Speed_) + sizeof(Stats_) + sizeof(IntfName_);

    obj_pld.unit_sz = sizeof(ObjPldHeader_t) + obj_data_sz;
    obj_pld.unit_id = objectId;
    obj_pld.unit_action = MODIFY ; //TBC
    obj_pld.unit_cookie = 0x11223344 ; //TBC

    cout << "start -- buffer  -- " << count  << endl;

    tmp_sz = sizeof(ObjPldHeader_t);
    std::memcpy(buf, &obj_pld, tmp_sz); buf += tmp_sz; count += tmp_sz;

    tmp_sz = sizeof(Speed_);
    std::memcpy(buf, &Speed_, tmp_sz); buf += tmp_sz; count += tmp_sz;

    std::memcpy(buf, &Stats_, sizeof(Stats_)); buf += sizeof(Stats_);
    count += sizeof(Stats_);

    std::memcpy(buf, IntfName_, sizeof(IntfName_));  buf+= sizeof(IntfName_);
    count += sizeof(IntfName_);

    buf = tmpbuf; // Is it needed ??

    PrintBytes(buf, count);

    cout << "end   ---- buffer  "  << count <<  endl;
    return count;
}
