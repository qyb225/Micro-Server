#include "HttpReq.h"

HttpReq::HttpReq(const char *http_request)
                : method("GET"), api("/") {
    std::string http_req(http_request);
    int i = 0;
    int count = 0;
    int lo = 0, hi;
    while (i < http_req.size() && 
           count < 2 && http_req[i] != '.') {
        if (http_req[i] == ' ') {
            ++count;
            hi = i;
            if (count == 1) {
                method = http_req.substr(lo, hi - lo);
                lo = hi + 1;
            } else {
                api = http_req.substr(lo, hi - lo);
            }
        }
        ++i;
    }
}

std::string HttpReq::get_method() const {
    return method;
}

std::string HttpReq::get_api() const {
    return api;
}