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

        NanoMsg(IN const char *, IN ConnType);
        virtual ~NanoMsg();

        int ConnectToEndPoint();
        int Send(IN const char *);
        int Recv(OUT const char *);
        int Send(IN const char *buf, IN int buf_len,
                 IN int flags, OUT int *total_sent_bytes); //for app use
        int Recv(OUT char *buf , IN int buf_len,
                 IN int flags, OUT int *total_recv_bytes); //for app use
        int SetClientApiRef(IN NxClientApi *);
        bool IsClient();

        int RunUT();
        int PrintBytes(IN const char *pBytes , IN const int nBytes);


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
