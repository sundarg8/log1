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
        NanoMsg(const char *);
        //NanoMsg();
        virtual ~NanoMsg();
        int SetupClient();
        int SetupServer();
        int SendFromClient(const char *);
        int RecvOnServer(char *);
        int RecvAndPrintOnServer();

    protected:
    private:
        nnxx::socket ClientSock_ { nnxx::SP, nnxx::PAIR };
        nnxx::socket ServerSock_ { nnxx::SP, nnxx::PAIR };
        char SockAddr_[SOCKET_ADDR_SZ];

};

#endif // NN_MSG_H
