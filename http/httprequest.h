#ifndef OI_SVR_HTTPREQUSET_H
#define OI_SVR_HTTPREQUSET_H

#include "requestline.h"
#include "../autobuffer.h"
#include <map>


namespace http { namespace request {


void Pack(const std::string &_host, const std::string &_url, const std::map<std::string, std::string> _headers,
          AutoBuffer& _send_body, AutoBuffer &_out_buff);



}}


#endif //OI_SVR_HTTPREQUSET_H
