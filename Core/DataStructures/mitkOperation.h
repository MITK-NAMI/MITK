#ifndef OPERATION_H_HEADER_INCLUDED_C16E7D9E
#define OPERATION_H_HEADER_INCLUDED_C16E7D9E

#include "mitkCommon.h"
//##ModelId=3E9B07B10043
typedef int OperationType ;
	
static const int ROI = 10;
static const int VIEW = 20;

namespace mitk {
//##ModelId=3E5F682C0289
class Operation
{
  public:
    //##ModelId=3E7830E70054
    Operation(OperationType operationType);

    //##ModelId=3E7831B600CA
    OperationType GetOperationType();

    //##ModelId=3E7F4D7D01AE
    virtual void Execute();

  private:
    //##ModelId=3E5F6A9001E9
    OperationType m_OperationType;

};
}//namespace mitk


#endif /* OPERATION_H_HEADER_INCLUDED_C16E7D9E */
