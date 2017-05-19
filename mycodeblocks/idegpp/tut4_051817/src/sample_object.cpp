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

int TestObject::ConvertToBuffer(char *buf,  int max_length ) {
    std::size_t count = 0; //sizeof(*this);
    char *tmpbuf = buf;


    cout << "start -- buffer  -- " << count  << endl;

    std::memcpy(buf, &Speed_, sizeof(Speed_)); buf += sizeof(Speed_);
    count += sizeof(Speed_);
    std::memcpy(buf, &Stats_, sizeof(Stats_)); buf += sizeof(Stats_);
    count += sizeof(Stats_);
    std::memcpy(buf, IntfName_, sizeof(IntfName_));  buf+= sizeof(IntfName_);
    count += sizeof(IntfName_);

    buf = tmpbuf;

    PrintBytes(buf, count);

    cout << "end   ---- buffer  "  << count <<  endl;
    return count;
}
