#include "nn_msg.h"
#include <cstring>
#include <unistd.h>
#include <system_error>
#include <iomanip>



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

int NanoMsg::ConnectToEndPoint() {
    if (ConnClient == ConnType_)
        NanoMsgSock_.bind(SockAddr_);
    else
        NanoMsgSock_.connect(SockAddr_);
}

bool NanoMsg::IsClient() {
    return ((ConnClient == ConnType_) ? true : false);
}


int NanoMsg::SetClientApiRef(NxClientApi *parent_client) {
    p_ParentClientApi = parent_client;
}

int NanoMsg::Send(const char *buf) {
    NanoMsgSock_.send(buf);
}

int NanoMsg::Recv(const char *buf) {
    nnxx::message msg = NanoMsgSock_.recv();
    //std::cout << __FUNCTION__ <<  msg << std::endl;
    return 0;
}


int NanoMsg::Send(const char *buf, int len, int flags, int *total_sent_bytes) {
    *total_sent_bytes = 0;
    *total_sent_bytes = NanoMsgSock_.send(buf, len, flags);
    //cout <<  " Sent Bytes -- "  << *total_sent_bytes << endl;
    return 0;
}

int NanoMsg::Recv(char *buf , int buf_len, int flags, int *total_recv_bytes) {
    *total_recv_bytes = 0;
    *total_recv_bytes = NanoMsgSock_.recv(buf, buf_len, flags);
    //cout <<  " Actual Rcvd  Bytes -- "  << *total_recv_bytes << endl;
    return 0;
}


int NanoMsg::PrintBytes(const char *pBytes , const int nBytes) {

    for (int i = 0; i != nBytes; i++)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0')
            << static_cast<unsigned int>(pBytes[i]) << " ";
    }
    cout << std::dec << std::endl;

}

/// ///////////////////////////////////////////////////////////////////////

int NanoMsg::RunUT() {
    char *buf = nullptr;
    int bytes=16;

    if  (ConnClient == ConnType_) {
        bytes = 16;
        ConnectToEndPoint();        //RunAsClientUT();
        ///SendByteStream(SockBuffer_, SOCKET_BUFFER_LEN, 0, &bytes);

    }
    else {
        bytes = 8;
        ConnectToEndPoint();        //RunAsServerUT();
        ///RecvByteStream(SockBuffer_, SOCKET_BUFFER_LEN, 0, &bytes);
    }
    return 0;
}

/*
int NanoMsg::SendByteStream(char *buf , int len, int flags, int *sent_bytes) {
    char byteArr[64] = {0x18,0x03,0x00,0x00,0x02,0x00,0x00,0x00,
                        0x41,0x35,0x34,0x00,0x00,0x00,0x00,0x00 };
    len = 16;
    for (int i =0 ; i< 5; i ++) {
        byteArr[15] =i*2;
        Send(byteArr, len, 0, sent_bytes);
        PrintBytes(byteArr, *sent_bytes); cout << endl;
        sleep(1);
    }
    sleep(5);

    //Send(buf, len, flags, sent_bytes);
}

int NanoMsg::RecvByteStream(char *buf , int len, int flags, int *recv_bytes)  {
    for  (int i =0 ; i < 5; i++) {
        ///Recv(SockBuffer_, SOCKET_BUFFER_LEN, 0, recv_bytes);
        ///PrintBytes(SockBuffer_, *recv_bytes);
        sleep(2);
    }
    sleep(2);
}
*/

int NanoMsg::RunAsServerUT() {
    const char *buf;
  try {
    //ConnectToEndPoint();

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
    //ConnectToEndPoint();

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
    //e8 03 00 00 02 00 00 00 41 35 34 00 00 00 00 00
    return 1;
  }
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
