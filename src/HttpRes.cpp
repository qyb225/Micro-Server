#include "HttpRes.h"
#include <fstream>
#include <iostream>
#include <sstream>  


HttpRes::HttpRes() {
    method_table.insert(make_pair("/qubic", &get_index));
}

std::string HttpRes::exec(std::string api, std::string method) {
    std::string res = "";
    if (method_table.find(api) != method_table.end()) {
        res = method_table[api](method);
    }
    return res;
}

/*User define*/
std::string get_index(std::string req_method) {
    std::string res = "";
    if (req_method == "GET") {
        res += "HTTP/1.0 200 OK<\r\n\r\n";
        std::ifstream fin("./index.html");
        std::stringstream buffer;
        buffer << fin.rdbuf();
        res += buffer.str();
        res += "\r\n";
    }
    return res;
}
