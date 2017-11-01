#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include "stdafx.h"

class CWebServer
{
public:
    CWebServer();
    ~CWebServer();

public:
    bool runServer();
    bool InitServer();

public:
    static char *m_pHttpPort;
    static struct mg_serve_http_opts m_HttpServerOpts;
    struct mg_mgr m_mgr;
};

#endif

