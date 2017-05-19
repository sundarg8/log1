#ifndef NN_MSG_H
#define NN_MSG_H

#include <iostream>
#include <nnxx/message.h>
#include <nnxx/pair.h>
#include <nnxx/socket.h>

using namespace std;
#define SOCKET_ADDR_SZ 64

class NanoMsg
{
    public:
        enum ConnType { ConnClient , ConnServer };

        NanoMsg(const char *, ConnType);
        virtual ~NanoMsg();

        int SetupConnection();
        int Send(const char *);
        int Recv(const char *);
        int RunUT();

    protected:

    private:

        char            SockAddr_[SOCKET_ADDR_SZ];
        ConnType        ConnType_;
        nnxx::socket    NanoMsgSock_ { nnxx::SP, nnxx::PAIR};

        int RunAsServerUT();
        int RunAsClientUT();

};

#endif // NN_MSG_H
