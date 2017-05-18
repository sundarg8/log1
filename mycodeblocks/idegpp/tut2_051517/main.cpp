#include <iostream>
#include "sample_object.h"
#include "client_api.h"

using namespace std;


void ut1_client(NxClientApi &apiObj) {

    cookie  cookies[10];
    TestObject intf1, intf2;

    intf1.SetParams("A53",  1000, 2);
    cookies[0] = apiObj.PerformActionOnObj(&intf1, CREATE, 0);

    /*
    intf1.SetParams("B56", 1003, 5);
    cookies[1] = apiObj.PerformActionOnObj(&intf1, MODIFY, 0);
    apiObj.FlushObjActions();


    intf2.SetParams("eth2", 2000, 3);
    cookies[2] = apiObj.PerformActionOnObj(&intf2, CREATE, 0);
    intf2.SetParams("eth2", 2001, 6);
    cookies[3] = apiObj.PerformActionOnObj(&intf2, MODIFY, 0);
    */
    apiObj.FlushObjActions();


    cout << " ------ MAIN OUTPUT ---- " << cookies[3] << " Copies --> " << TestObject::NumCopyCtors_ << endl;
}


int main()
{
    cout << " -- Start of Main() ---- " << endl;
    NxClientApi  apiObj;
    ut1_client(apiObj);


    //apiObj.PrintPrintMe();

    cout << "May17-v8  exec of ut1_client "  << endl;

    return 0;
}
