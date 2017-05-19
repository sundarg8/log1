#include "nn_msg.h"
#include <cstring>
#include <unistd.h>
#include <system_error>


NanoMsg::NanoMsg(const char *sock_addr, ConnType type)
{
    strncpy(SockAddr_, sock_addr, SOCKET_ADDR_SZ);
    SockAddr_[SOCKET_ADDR_SZ-1] = '\0';
    ConnType_ = type;
}

NanoMsg::~NanoMsg()
{
    //dtor
}

int NanoMsg::SetupConnection() {
    if (ConnClient == ConnType_)
        NanoMsgSock_.bind(SockAddr_);
    else
        NanoMsgSock_.connect(SockAddr_);
}

int NanoMsg::Send(const char *buf) {
    NanoMsgSock_.send(buf);
}

int NanoMsg::Recv(const char *buf) {
    nnxx::message msg = NanoMsgSock_.recv();
    std::cout << __FUNCTION__ <<  msg << std::endl;
    return 0;
}


int NanoMsg::RunAsServerUT() {
    const char *buf;
  try {
    SetupConnection();

    while (1) {
        Recv(buf);
        usleep(1 * 1000 * 1000);
    }
    return 0;
  }
  catch (const std::system_error &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}

int NanoMsg::RunAsClientUT() {
  try {
    SetupConnection();

    Send("TUT4_3--Class -- msg1 -- Hello World-my-V2!");
    cout << "\n after send1 " << endl;
    usleep(2 * 1000 * 1000);
    Send("msg2 -- Hello World-my-V2!");
    cout << "\n after send2 " << endl;
    usleep(2 * 1000 * 1000);
    Send("msg3 -- Hello World-my-V2!");
    cout << "\n after send3 " << endl;
    usleep(2 * 1000 * 1000);
    Send("msg4 -- Hello World-my-V2!");
    cout << "\n after send4 " << endl;
    usleep(2 * 1000 * 1000);

    return 0;
  }
  catch (const std::system_error &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}

int NanoMsg::RunUT() {
    if  (ConnClient == ConnType_)
        RunAsClientUT();
    else
        RunAsServerUT();

}

/// //////////////////////////////////////////////////////////////////
#if 0

/*


int NanoMsg::SetupClient() {
    ClientSock_.bind(SockAddr_);
    return 0;
}

int NanoMsg::SetupServer() {
    ServerSock_.connect(SockAddr_);
    return 0;

}
int NanoMsg::SendFromClient(const char *buf) {
    ClientSock_.send(buf);
    return 0;
}

int NanoMsg::RecvOnServer(char *buf) {
    nnxx::message msg = ServerSock_.recv();
    std::cout << msg << std::endl;
    return 0;
}

int NanoMsg::RecvAndPrintOnServer() {
    nnxx::message msg = ServerSock_.recv();
    std::cout << "Library" << msg << std::endl;
    return 0;
}

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
    p_nanoObj->SendFromClient("TUT4_2--LIB -- msg1 -- Hello World-my-V2!");
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
*/
#endif
