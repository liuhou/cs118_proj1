/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   httpTransaction.cpp
 * Author: HOU
 *
 * Created on 2016年10月14日, 下午8:04
 */

#include <cstdlib>
#include <string>
#include "httpTransaction.h"

using namespace std;

/*
 * 
 */
HttpTransaction :: HttpTransaction(){
    httpVersion = "";
    headers.clear();
}
void HttpTransaction :: setHeaders(string& key, string& value){
    headers[key] = value;
}
string HttpTransaction ::getHeaders(){
    string result = "";
    map<string, string>::iterator iter = headers.begin();
    while (iter != headers.end())
	{
            result = result + iter->first + ": " + iter->second + "\r\n";
            iter++;
	}
	result = result + "\r\n";
	return result;
}
string HttpTransaction ::getHeader(string& key){
    if(headers.count(key) > 0){
        return headers[key];
    }else{
        return "none";
    }
    
}
void HttpTransaction ::setHttpVersion(string& version){
    httpVersion = version;
}
string HttpTransaction::getHttpVersion(){
    return httpVersion;
}

void HttpRequest::setRequestUrl(string& uri){
    requestUrl = uri;
}
string HttpRequest::getRequestUrl(){
    return requestUrl;
}
void HttpRequest::setMethod(string& newMethod){
    method = newMethod;
}
string HttpRequest::getMethod(){
    return method;
}
string HttpRequest::toRequestString(){
    string result = "";
    result = result + getMethod() + " " + getRequestUrl() + " " + getHttpVersion() + "\r\n";
    result = result + getHeaders();
    return result;
}
vector<uint8_t> HttpRequest::encode(){
    string str = toRequestString();
    vector<uint8_t> result;
    result.assign(str.begin(), str.end());
    return result;
}
bool HttpRequest::decodeFirstline(ByteVector& first){
    string str = "";
    int numOfSp = 0;
    for(int i = 0; first[i] != '\r'; i++){
        if(first[i] == ' '){
            numOfSp++;
            if(numOfSp == 1){
                setMethod(str);
                str = "";
            }else if(numOfSp == 2){
                setRequestUrl(str);
                str = "";
            }
            if(i > 0 && first[i - 1] == ' ' && numOfSp > 2){
                return false;
            }
            continue;
        }
        str = str.append(first[i]);
    }
    setHttpVersion(str);
    if(getMethod().compare("GET") != 0){
        return false;
    }
    if(getHttpVersion().compare("HTTP/1.0") != 0){
        return false;
    }
    return true;
}
void HttpResponse::setStatus(int responsestatus){
    status = responsestatus;
    switch(status){
        case 200 :
            statusDef = "OK";
            break;
        case 400 :
            statusDef = "Bad request";
            break;
        case 404 :
            statusDef = "Not found";
    }
}
void HttpResponse::getStatus(){
    return status;
}
string HttpResponse::getStatusDefinition(){
    return statusDef;
}
/*string HttpResponse::toResponseString(){
    string result;
    
}*/
vector<uint8_t> HttpResponse::encode(){
    vector<uint8_t> result;
    string str = "";
    str = str + getHttpVersion() + " " + getStatus() + " " + getStatusDefinition() + "\r\n";
    str = str + getHeaders();
    result.assign(str.begin(), str.end());
    
    return result;
}
bool HttpResponse::decodeFirstline(ByteVector& first){
    string str = "";
    int numOfSp = 0;
    for(int i = 0; first[i] != '\r'; i++){
        if(first[i] == ' '){
            numOfSp++;
            if(numOfSp == 1){
                setHttpVersion(str);
                str = "";
            }else if(numOfSp == 2){
                setStatus(atoi(str.c_str()));
                str = "";
            }
            continue;
        }
        str.append(first[i]);
    }
    return true;
}