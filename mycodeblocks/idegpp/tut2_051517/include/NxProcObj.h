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


#endif //_NXPROCOBJ_H_7A115EC9441A428EA88E8C2C02F04DA7
