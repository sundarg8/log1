#include <iostream>
#include <unistd.h>

#include "sample_object.h"
#include "client_api.h"
#include "nn_msg.h"
#include <system_error>


#if 0
#include <nnxx/message.h>
#include <nnxx/pair.h>
#include <nnxx/socket.h>
#endif // 0

#define SOCKET_ADDR "ipc:///data/pair_xx_cb3.ipc"
#define NODE0 "client"
#define NODE1 "server"


using namespace std;




int nanomsg_server(NanoMsg *p_nanoObj) {
  try {
    p_nanoObj->SetupServer();

    while (1) {
        p_nanoObj->RecvAndPrintOnServer();
        usleep(1 * 1000 * 1000);
    }
    return 0;
  }
  catch (const std::system_error &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}

int nanomsg_client(NanoMsg *p_nanoObj) {
  try {
    p_nanoObj->SetupClient();
    p_nanoObj->SendFromClient("TUT4--LIB -- msg1 -- Hello World-my-V2!");
    cout << "\n after send1 " << endl;
    usleep(2 * 1000 * 1000);

    p_nanoObj->SendFromClient("msg2 -- Hello World-my-V2!");
    cout << "\n after send2 " << endl;
    usleep(2 * 1000 * 1000);

    p_nanoObj->SendFromClient("msg3 -- Hello World-my-V2!");
    cout << "\n after send3 " << endl;
    usleep(2 * 1000 * 1000);


    p_nanoObj->SendFromClient("msg4 -- Hello World-my-V2!");
    cout << "\n after send4 " << endl;
    usleep(2 * 1000 * 1000);

    return 0;
  }
  catch (const std::system_error &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}

int nanomsg_main(int argc, char **argv, NanoMsg *p_nanoObj) {
        if (argc == 2 && strncmp(NODE0, argv[1], strlen(NODE0)) == 0) {
                return nanomsg_client(p_nanoObj);
        } else if (argc == 2 && strncmp (NODE1, argv[1], strlen (NODE1)) == 0) {
                return nanomsg_server(p_nanoObj);
        } else {
                fprintf (stderr, "Usage: pair %s|%s <ARG> ...\n", NODE0, NODE1);
                return 1;
        }

}

void ut1_client(NxClientApi &apiObj) {

    cookie  cookies[10];
    TestObject intf1, intf2;

    intf1.SetParams("A54",  1000, 2);
    cookies[0] = apiObj.PerformActionOnObj(&intf1, CREATE, 0);


    intf1.SetParams("B56", 1003, 5);
    cookies[1] = apiObj.PerformActionOnObj(&intf1, MODIFY, 0);
    apiObj.FlushObjActions();


    intf2.SetParams("eth2", 2000, 3);
    cookies[2] = apiObj.PerformActionOnObj(&intf2, CREATE, 0);
    intf2.SetParams("eth2", 2001, 6);
    cookies[3] = apiObj.PerformActionOnObj(&intf2, MODIFY, 0);

    apiObj.FlushObjActions();


    cout << " ------ MAIN OUTPUT ---- " << " Copies --> " << TestObject::NumCopyCtors_ << endl;
}


int main(int argc, char**argv)
{
    cout << " -- Start of Main() ---- " << endl;


    NxClientApi  apiObj;
    //ut1_client(apiObj);
    ///nnxx_main(argc, argv);

    NanoMsg  nanoObj(SOCKET_ADDR);
    //nanoObj.SendFromClient();
    nanomsg_main(argc, argv, &nanoObj);

    cout << "May17-v8  exec of ut1_client "  << endl;

    return 0;
}
