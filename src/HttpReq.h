#ifndef HTTPREQ_H
#define HTTPREQ_H
#include <string>

class HttpReq {
private:
    std::string method;
    std::string api;
public:
    HttpReq(const char *http_request);
    std::string get_method() const;
    std::string get_api() const;
};

#endif