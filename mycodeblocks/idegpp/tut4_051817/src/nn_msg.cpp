#include "nn_msg.h"
#include <cstring>

/*
NanoMsg::NanoMsg() //const char *sock_addr)
{
    const char *sock_addr = "ipc:///data/pair_xx_cb4_def.ipc";
    strncpy(SockAddr_, sock_addr, SOCKET_ADDR_SZ);
    SockAddr_[SOCKET_ADDR_SZ-1] = '\0';
    SetupClient();
    SetupServer();
}
*/

NanoMsg::NanoMsg(const char *sock_addr)
{
    strncpy(SockAddr_, sock_addr, SOCKET_ADDR_SZ);
    SockAddr_[SOCKET_ADDR_SZ-1] = '\0';
    //SetupClient();
    //SetupServer();
}

NanoMsg::~NanoMsg()
{
    //dtor
}

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
