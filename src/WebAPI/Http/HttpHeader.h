#ifndef HttpHeader_h
#define HttpHeader_h

#include <Arduino.h>

class HttpHeader {
  protected:
  private:
    String Date;
    String Content_Type;
    String Transfer_Encoding;
    String Connection;
    String CF_Ray;
    String Access_Control_Allow_Origin;
    String Set_Cookie;
    String Vary;
    String Via;
    String CF_Cache_Status;
    String X_Kong_Proxy_Latency;
    String X_Kong_Upstream_Latency;
    String Server;

  public:
    HttpHeader();
    ~HttpHeader();

    void add(String name, String value);

    const String checkDate() { return Date; }
    const String checkContentType() { return Content_Type; }
    const String checkTransferEncoding() { return Transfer_Encoding; }
    const String checkConnection() { return Connection; }
    const String checkCFRay() { return CF_Ray; }
    const String checkAccessControlAllowOrigin() { return Access_Control_Allow_Origin; }
    const String checkSetCookie() { return Set_Cookie; }
    const String checkVary() { return Vary; }
    const String checkVia() { return Via; }
    const String checkCFCacheStatus() { return CF_Cache_Status; }
    const String checkXKongProxyLatency() { return X_Kong_Proxy_Latency; }
    const String checkXKongUpstreamLatency() { return X_Kong_Upstream_Latency; }
    const String checkServer() { return Server; }
};

#endif