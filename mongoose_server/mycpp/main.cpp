// #include <iostream>
// #include "basic_http.h"

// using namespace std;

// int main(void)
// {
//     BasicHttp bshttp = new BasicHttp();

//     if (!bshttp.Start())
//     {
//         cout << "初始化失败" << endl;
//         return -1;
//     }

//     return 0;
// }



// Copyright (c) 2015 Cesanta Software Limited
// All rights reserved


#include "WebServer.h"

int main(void) 
{
  CWebServer myServer;
  
  myServer.runServer();

  return 0;
}