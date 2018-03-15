#include "HttpRes.h"
#include "HttpReq.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>

std::map<std::string, std::string> HttpRes::mmt = {
    { "html",  "text/html" },
	{ "htm",   "text/html" },
	{ "css",   "text/css"  },
	{ "js",	   "application/x-javascript" },
	{ "ico",   "image/x-icon" },
	{ "gif",   "image/gif" },
	{ "jpg",   "image/jpeg" },
	{ "jpeg",  "image/jpeg" },
	{ "png",   "image/png" }
};

std::map<std::string, std::function
        <void(std::string, char *, int *)>>
        HttpRes::method_table = {
    { "/", &get_index },
    { "/next", &get_next}
};

HttpRes::HttpRes(const HttpReq &http_req):
                method(http_req.get_method()),
                path(http_req.get_path()) {}

void HttpRes::res_to_buffer(char *buffer, int *len) {
    if (method_table.find(path) != method_table.end()) {
        method_table[path](method, buffer, len);
    } else {
        get_text_from_file(path, buffer, len);
    }
}

std::string HttpRes::response_status(int status_code) {
    std::string res_status;
    switch (status_code) {
        case 200:
            res_status = "HTTP/1.1 200 OK<\r\n\r\n";
            break;
        case 404:
            res_status = "HTTP/1.1 404 Not Found<\r\n\r\n";
            break;
    }
    return res_status;
}

std::string HttpRes::response_type(std::string exp) {
    std::string res_type = "Content-Type: ";
    if (mmt.find(exp) != mmt.end()) {
        res_type = res_type + mmt[exp] + "\r\n";
    }
    return res_type + "\r\n";
}

void HttpRes::get_text_from_file(std::string path, 
                                 char *buffer, int *len) {
    std::string res = "";
    std::string exp = get_file_exp(path);
    
    std::ifstream fin(path);
    
    if (!fin) {
        res += response_status(404);
        res += "<h1>404 Not Found</h1>\r\n";
    } else {
        res += response_status(200);
        //res += response_type(exp);
        
        std::stringstream ss_buffer;
        ss_buffer << fin.rdbuf();
        res += ss_buffer.str();
        res += "\r\n";

        fin.close();
    }

    *len = res.size();
    strcpy(buffer, res.c_str());
}

std::string HttpRes::get_file_exp(std::string path) {
    int n = path.size();
    std::string exp;
    while (n > 0) {
        --n;
        if (path[n] == ' ') {
            continue;
        }
        if (path[n] == '.') {
            exp = path.substr(n + 1);
            break;
        }
    }
    return exp;
}

/*User define*/
void get_index(std::string req_method, char *buffer, int *len) {
    if (req_method == "GET") {
        HttpRes::get_text_from_file("index.html", buffer, len);
    }
}

void get_next(std::string req_method, char *buffer, int *len) {
    if (req_method == "GET") {
        HttpRes::get_text_from_file("next.html", buffer, len);
    }
}
