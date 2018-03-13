#ifndef HTTPRES_H
#define HTTPRES_H
#include <map>
#include <functional>

class HttpRes {
private:
    std::map<std::string, std::function
            <std::string(std::string)>>
            method_table;
public:
    HttpRes();
    std::string exec(std::string api, std::string method);
};

std::string get_index(std::string req_method);

#endif