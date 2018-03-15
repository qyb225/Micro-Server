#ifndef HTTPREQ_H
#define HTTPREQ_H
#include <string>

class HttpReq {
private:
    std::string method;
    std::string path;
    std::string version;

    void handle_request(const char *http_request);
public:
    HttpReq(const char *http_request);
    std::string get_method() const;
    std::string get_path() const;
    std::string get_version() const;
};

#endif