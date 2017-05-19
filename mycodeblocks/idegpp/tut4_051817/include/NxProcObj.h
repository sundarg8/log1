#ifndef _NXPROCOBJ_H_7A115EC9441A428EA88E8C2C02F04DA7
#define _NXPROCOBJ_H_7A115EC9441A428EA88E8C2C02F04DA7

#include "sys/types.h"

///namespace nxproc {

class NxProcObj {
public:
  virtual ~NxProcObj()=0;
  /*
   * Print self to target using printf style function provided
  */
  virtual void PrintPrintMe()=0;
  /*
   * Print Self to Tagged Object output format
  */
  virtual void PrintReflection()=0;
  /*
   *  Log self to local object logger target
  */
  virtual void LogMe()=0;

protected:
  /* Should matter if we had this or not*/
  NxProcObj();
};

///} //namespace

/// ///////////////////////////////////////////////////////////////


#if 0 // CDEL

int client() {
  try {
    nnxx::socket s1 { nnxx::SP, nnxx::PAIR };
    nnxx::socket s2 { nnxx::SP, nnxx::PAIR };
    const char *addr = SOCKET_ADDR; //"inproc://example";

    s1.bind(addr);
    //s2.connect(addr);

    s1.send("Hello World-my! ");
    usleep(2 * 1000 * 1000);

    nnxx::message msg = s2.recv();
    std::cout << msg << std::endl;
    return 0;
  }
  catch (const std::system_error &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}

int client() {
  try {
    nnxx::socket s1 { nnxx::SP, nnxx::PAIR };
    //nnxx::socket s2 { nnxx::SP, nnxx::PAIR };
    const char *addr = SOCKET_ADDR; //"inproc://example";

    s1.bind(addr);
    //s2.connect(addr);

    s1.send("TUT4 -- msg1 -- Hello World-my-V2!");
    cout << "\n after send1 " << endl;
    usleep(2 * 1000 * 1000);

    s1.send("msg2 -- Hello World-my-V2!");
    cout << "\n after send2 " << endl;
    usleep(2 * 1000 * 1000);

    s1.send("msg3 -- Hello World-my-V2!");
    cout << "\n after send3 " << endl;
    usleep(2 * 1000 * 1000);


    s1.send("msg4 -- Hello World-my-V2!");
    cout << "\n after send4 " << endl;
    usleep(2 * 1000 * 1000);

    return 0;
  }
  catch (const std::system_error &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}

int server() {
  try {
    nnxx::socket s2 { nnxx::SP, nnxx::PAIR };
    const char *addr = SOCKET_ADDR; //"inproc://example";

    s2.connect(addr);

    while (1) {
        nnxx::message msg = s2.recv();
        std::cout << msg << std::endl;
        usleep(1 * 1000 * 1000);
    }
    return 0;
  }
  catch (const std::system_error &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}

int nnxx_main(int argc, char **argv) {

       if (argc == 2 && strncmp(NODE0, argv[1], strlen(NODE0)) == 0) {
                return client();
        } else if (argc == 2 && strncmp (NODE1, argv[1], strlen (NODE1)) == 0) {
                return server();
        } else {
                fprintf (stderr, "Usage: pair %s|%s <ARG> ...\n", NODE0, NODE1);
                return 1;
        }

}

#endif // 0


#endif //_NXPROCOBJ_H_7A115EC9441A428EA88E8C2C02F04DA7

