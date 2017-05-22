#ifndef TestObject_H
#define TestObject_H
#include <string>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <NxProcObj.h>
using namespace std;



enum  action_t  { GET, CREATE , MODIFY , DELETE };
#define nameSz 12


typedef struct tag_ObjPldHeader {
    int         unit_sz;
    int         unit_id;
    action_t    unit_action;
    int         unit_cookie;
    int         unit_pyld_start[0];
} ObjPldHeader_t;


///class TestObject : NxProcObj
class TestObject
{
    public:

        TestObject();
        virtual ~TestObject();

        //virtual void PrintPrintMe() {}
        ///virtual void PrintReflection() {}
        ///virtual void LogMe(){}

        TestObject(const TestObject &obj) {
            //IntfName_ =  obj.IntfName_;
            strncpy(IntfName_, obj.IntfName_, nameSz);
            Speed_     = obj.Speed_;
            Stats_     = obj.Stats_;
            ++NumCopyCtors_;
        }

        int ConvertToBuffer(int objectId, char *buf,  int max_length );
        int ConvertToObjInst(char *buf, TestObject **ptr_to_new_obj);
        int ConvertToObjInst(char *buf, TestObject *ptr_alloc_obj);
        int ConvertToObjInst(char *buf);

        void PrintBytes(const char *buf , const int bytes);

        ///void SetParams(string val_name, int val_Speed_, int val_Stats_) {
        ///void SetParams( int val_Speed_, int val_Stats_) {
            ///IntfName_ = val_name;
        void SetParams(char  *val_name, int val_Speed_, int val_Stats_) {

            strncpy(IntfName_, val_name, nameSz);

            Speed_ = val_Speed_;
            Stats_ = val_Stats_;
        }

        ///virtual void PrintPrintMe() {
        void PrintPrintMe() {
            cout << setw(10) << "Attr_name: " << IntfName_
                             << "  Attr_Speed_: " << Speed_
                             << "   Attr_Stats_: " << Stats_ <<  endl;
        }
        static int NumCopyCtors_;

    protected:
    private:
        unsigned int     Speed_;
        unsigned int     Stats_;
        //string  IntfName_;
        char IntfName_[nameSz];

};

#endif // TestObject_H
