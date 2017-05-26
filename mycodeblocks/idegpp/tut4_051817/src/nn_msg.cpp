#include <cstring>
#include <unistd.h>
#include <system_error>
#include <iomanip>

#include "sample_object.h"
#include "nn_msg.h"




NanoMsg::NanoMsg(const char *sock_addr, ConnType type)
{
    strncpy(SockAddr_, sock_addr, SOCKET_ADDR_SZ);
    SockAddr_[SOCKET_ADDR_SZ - 1] = '\0';
    ConnType_ = type;
}

NanoMsg::~NanoMsg()
{
    //dtor
}

int NanoMsg::ConnectToEndPoint() {
    if (ConnClient == ConnType_)
        NanoMsgSock_.bind(SockAddr_);
    else
        NanoMsgSock_.connect(SockAddr_);

    return NxProcSUCCESS;

}

bool NanoMsg::IsClient() {
    return ((ConnClient == ConnType_) ? true : false);
}


int NanoMsg::SetClientApiRef(NxClientApi *parent_client) {
    p_ParentClientApi = parent_client;

    return NxProcSUCCESS;
}

int NanoMsg::Send(const char *buf) {
    //if (p_ParentClientApi->IsServerMode())
    //    cout << "test ";
    NanoMsgSock_.send(buf);

    return NxProcSUCCESS;
}

int NanoMsg::Recv(const char *buf) {
    nnxx::message msg = NanoMsgSock_.recv();

    return NxProcSUCCESS;
}


int NanoMsg::Send(const char *buf, int len, int flags, int *total_sent_bytes) {
    *total_sent_bytes = 0;
    *total_sent_bytes = NanoMsgSock_.send(buf, len, flags);

    return NxProcSUCCESS;
}

int NanoMsg::Recv(char *buf , int buf_len, int flags, int *total_recv_bytes) {
    *total_recv_bytes = 0;
    *total_recv_bytes = NanoMsgSock_.recv(buf, buf_len, flags);

    return NxProcSUCCESS;
}


int NanoMsg::PrintBytes(const char *pBytes , const int nBytes) {

    for (int i = 0; i != nBytes; i++)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<unsigned int>(pBytes[i]) << " ";
    }
    cout << std::dec << std::endl;

    return NxProcSUCCESS;
}
