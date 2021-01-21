#ifndef OI_SVR_HTTPRESPONSE_H
#define OI_SVR_HTTPRESPONSE_H

#include "../autobuffer.h"



namespace http { namespace response {


void Pack(int _resp_code, AutoBuffer &_out_buff, AutoBuffer &_send_body);


class Parser {
  public:


  private:
  
  
};

}}




#endif //OI_SVR_HTTPRESPONSE_H
