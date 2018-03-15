#ifndef HTTPRES_H
#define HTTPRES_H
#include <map>
#include <functional>
#include "HttpReq.h"

#define HTTP_STATUS_OK 200
#define HTTP_STATUS_NOT_FOUND 404

class HttpRes {
private:
    std::string path;
    std::string method;

    static std::map<std::string, std::function
                   <void(std::string, char *, int *)>>
                   method_table;
    static std::map<std::string, std::string> mmt;

    static std::string get_file_exp(std::string path);
    static std::string response_status(int status_code);
    static std::string response_type(std::string exp);
public:
    HttpRes(const HttpReq &http_req);
    static void get_text_from_file(std::string path, 
                                   char *buffer, int *len);
    void res_to_buffer(char *buffer, int *len);
};

/*User define*/
void get_index(std::string req_method, char *buffer, int * len);
void get_next(std::string req_method, char *buffer, int *len);

#endif