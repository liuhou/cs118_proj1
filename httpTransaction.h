#ifndef HTTPTRANSACTION_H
#define HTTPTRANSACTION_H

#include <map>
#include <vector>
#include <string>
#include <istream>

typedef std::map<std::string, std::string> HeaderMap;
typedef std::vector<std::uint8_t> ByteVector;

class HttpTransaction {
private:
	HeaderMap headers;
	std::string httpVersion;

public:

	void setHeaders(HeaderMap &headers);
	HeaderMap getHeaders();

    void setHttpVersion(std::string &);
    std::string getHttpVersion();
};

class HttpRequest: public HttpTransaction {
private:
    std::string method;
    std::string requestUri;
public:
    // TODO: build a constructor that takes vector/array of bytes as input

    void setRequestUri(std::string &);
    std::string getRequestUri();

    void setMethod(std::string &);
    std::string getMethod();

    std::string toRequestString();
    void parseFromMessageString(std::string &);

    std::vector<uint8_t> encode();
};

class HttpResponse: public HttpTransaction {
private:
    int status;
public:
    // TODO: build a constructor that takes vector/array of bytes as input

    static const int SC_OK = 200;
    static const int SC_BAD_REQUEST = 400;
    static const int SC_NOT_FOUND = 404;

    void setStatus(int);
    int getStatus();

    std::vector<uint8_t> encode();

    // use std::istream type to represent file in the content
};

#endif
