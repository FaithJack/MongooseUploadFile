#include "WebServer.h"

char* CWebServer::m_pHttpPort = "12001";
struct mg_serve_http_opts CWebServer::m_HttpServerOpts;

// static void ev_handler(struct mg_connection *nc, int ev, void *p)
// {
//     if (ev == MG_EV_HTTP_REQUEST) {
//         mg_serve_http(nc, (struct http_message *)p, CWebServer::m_HttpServerOpts);
//     }
// }

struct mg_str cb(struct mg_connection *c, struct mg_str file_name) {
    // Return the same filename. Do not actually do this except in test!
    // fname is user-controlled and needs to be sanitized.
    return file_name;
}

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
    // if (ev == MG_EV_HTTP_REQUEST) {
    //     mg_serve_http(nc, (struct http_message *)ev_data, CWebServer::m_HttpServerOpts);
    // }
    struct file_writer_data *data = (struct file_writer_data *) nc->user_data;
    struct http_message *hm = (struct http_message *)ev_data;

    switch (ev)
    {
    case MG_EV_HTTP_REQUEST:
        if (mg_vcmp(&hm->uri, "/index") == 0)
        {
            mg_serve_http(nc, (struct http_message *) ev_data, CWebServer::m_HttpServerOpts);
            //mg_http_serve_file(nc, hm, "index.html", mg_mk_str("text/plain"), mg_mk_str(""));
        }
        else if (mg_vcmp(&hm->uri, "/api/gethtml") == 0)
        {
            mg_http_serve_file(nc, hm, "file.txt", mg_mk_str("text/plain"), mg_mk_str(""));
        }
        else if (mg_vcmp(&hm->uri, "api/hello") == 0)
        {
            (void) ev; 
            (void) ev_data;
            mg_printf(nc, "HTTP/1.0 200 OK\r\n\r\n[I am Hello2]");
            nc->flags |= MG_F_SEND_AND_CLOSE;
        }
        else if (mg_vcmp(&hm->uri, "api/upload") == 0)
        {
            struct mg_http_multipart_part *mp = (struct mg_http_multipart_part *)ev_data;
            mg_file_upload_handler(nc, ev, ev_data, cb);

            // if (data == NULL)
            // {
            //     data = calloc(1, sizeof(struct file_writer_data));
            //     //data->fp = tmpfile();
            //     data->fp = fopen(mp->file_name, "w+");
            //     data->bytes_written = 0;
            //     if (data->fp == NULL) {
            //         mg_printf(nc, "%s",
            //                   "HTTP/1.1 500 Failed to open a file\r\n"
            //                   "Content-Length: 0\r\n\r\n");
            //         nc->flags |= MG_F_SEND_AND_CLOSE;
            //     }
            //     nc->user_data = (void *)data;

            //     if (fwrite(mp->data.p, 1, mp->data.len, data->fp) != mp->data.len) {
            //         mg_printf(nc, "%s",
            //                   "HTTP/1.1 500 Failed to write to a file\r\n"
            //                   "Content-Length: 0\r\n\r\n");
            //         nc->flags |= MG_F_SEND_AND_CLOSE;
            //         return;
            //     }
            //     data->bytes_written += mp->data.len;
                
            // }
        }

        break;
        
    default:
        break;
    }

}

// static void handle_upload(struct mg_connection *nc, int ev, void *p)
// {
//     struct file_writer_data *data = (struct file_writer_data *)nc->user_data;
//     struct mg_http_multipart_part *mp = (struct mg_http_multipart_part *)p;

//     printf("接收中\n ev=%d\n", ev);
//     switch (ev) {
//         case 122/*MG_EV_HTTP_PART_BEGIN*/: {
//             printf("开始接收upload数据\n");
//             if (data == NULL) {
//                 data = (struct file_writer_data *)calloc(1, sizeof(struct file_writer_data));
//                 data->fp = tmpfile();
//                 data->bytes_written = 0;
    
//                 if (data->fp == NULL) {
//                     mg_printf(nc, "%s",
//                             "HTTP/1.1 500 Failed to open a file\r\n"
//                             "Content-Length: 0\r\n\r\n");
//                     nc->flags |= MG_F_SEND_AND_CLOSE;
//                     return;
//                 }
//                 nc->user_data = (void *) data;
//             }
//         break;
//         }
//         case 123/*MG_EV_HTTP_PART_DATA*/: {
//           if (fwrite(mp->data.p, 1, mp->data.len, data->fp) != mp->data.len) {
//             mg_printf(nc, "%s",
//                       "HTTP/1.1 500 Failed to write to a file\r\n"
//                       "Content-Length: 0\r\n\r\n");
//             nc->flags |= MG_F_SEND_AND_CLOSE;
//             return;
//           }
//           data->bytes_written += mp->data.len;
//           break;
//         }
//         case 124/*MG_EV_HTTP_PART_END*/: {
//           mg_printf(nc,
//                     "HTTP/1.1 200 OK\r\n"
//                     "Content-Type: text/plain\r\n"
//                     "Connection: close\r\n\r\n"
//                     "Written %ld of POST data to a temp file\n\n",
//                     (long) ftell(data->fp));
//           nc->flags |= MG_F_SEND_AND_CLOSE;
//           fclose(data->fp);
//           free(data);
//           nc->user_data = NULL;
//           break;
//         }
//         default:
//             break;
//     }
// }

CWebServer::CWebServer()
{
    InitServer();
}

CWebServer::~CWebServer()
{
    mg_mgr_free(&m_mgr);
}

bool CWebServer::InitServer()
{
    struct mg_connection *m_pnc;

    mg_mgr_init(&m_mgr, NULL);
    cout << "服务端已启用 端口号为: " << m_pHttpPort << endl;

    //绑定端口号
    m_pnc = mg_bind(&m_mgr, m_pHttpPort, ev_handler);
    if (m_pnc == NULL) {
        cout << "Failed to create listener" << endl;
        return false;
    }
    //当前文件目录
    m_HttpServerOpts.document_root = ".";
    m_HttpServerOpts.enable_directory_listing = "yes";
    
    //设置服务端参数
    //mg_register_http_endpoint(m_pnc, "/upload", handle_upload MG_UD_ARG(NULL)); //设置url接收地址
    mg_set_protocol_http_websocket(m_pnc);

    return true;
}

bool CWebServer::runServer()
{
    for (;;)
    {
        mg_mgr_poll(&m_mgr, 1000);
    }
}