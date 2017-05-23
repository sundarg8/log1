#ifndef NN_MSG_H
#define NN_MSG_H

#include <iostream>

#include <nnxx/message.h>
#include <nnxx/pair.h>
#include <nnxx/socket.h>

using namespace std;
#define SOCKET_ADDR_SZ 64
//#define SOCKET_BUFFER_LEN 1024

class NxClientApi;

class NanoMsg
{
    public:
        enum ConnType { ConnClient , ConnServer };

        NanoMsg(const char *, ConnType);
        virtual ~NanoMsg();

        int ConnectToEndPoint();
        int Send(const char *);
        int Recv(const char *);
        int Send(const char *buf,  int buf_len, int flags, int *total_sent_bytes); //for app use
        int Recv(char       *buf , int buf_len, int flags, int *total_recv_bytes); //for app use
        int SetClientApiRef(NxClientApi *);
        bool IsClient();

        int RunUT();
        void PrintBytes(const char *pBytes , const int nBytes);


    protected:

    private:

        char            SockAddr_[SOCKET_ADDR_SZ];
        ConnType        ConnType_;
        nnxx::socket    NanoMsgSock_ { nnxx::SP, nnxx::PAIR};
        NxClientApi     *p_ParentClientApi;

        int RunAsServerUT();
        int RunAsClientUT();
        int SendByteStream(char *buf , int len, int flags, int *total_sent_bytes);
        int RecvByteStream(char *buf , int len, int flags, int *total_sent_bytes);

};

#endif // NN_MSG_H
