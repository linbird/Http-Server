#include "https_server.h"
#include <openssl/crypto.h>
#include "my_config.h"
#include <unistd.h>
extern CONF_INFO config;

std::map<std::string, std::string> 
content_type {
    { "txt", "text/plain" },
        { "c", "text/plain" },
        { "cpp", "text/plain" },
        { "h", "text/plain" },
        { "html", "text/html" },
        { "htm", "text/htm" },
        { "css", "text/css" },
        { "gif", "image/gif" },
        { "jpg", "image/jpeg" },
        { "jpeg", "image/jpeg" },
        { "png", "image/png" },
        { "ico", "image/x-icon"},
        { "pdf", "application/pdf" },
        { "ps", "application/postsript" }};

void
login_action(struct evhttp_request *req){
    std::string uuid;
    evkeyvalq* headers = evhttp_request_get_input_headers(req);
    struct evkeyval *header;
    for (header = headers->tqh_first; header; header = header->next.tqe_next) {
        if(std::string(header->key) == "Cookie"){
            uuid = header->value;
            break;
        }
    }
    // std::cout << "value= " << uuid << std::endl;
    size_t start = uuid.find("UUID=");
    uuid = uuid.substr(start+5, uuid.length()-start-5);
    // std::cout << "UUID= " << uuid << std::endl;
    if(uuid.empty()){
        if(auth(req) == 1){
            boost::uuids::uuid a_uuid = boost::uuids::random_generator()(); 
            std::string uuid_string = boost::uuids::to_string(a_uuid);
            std::string cookie = "UUID=" + uuid_string + ";Path=/";
            struct evbuffer *buf = evbuffer_new();
            if (buf == NULL) 
                return;
            std::string head_tail = "text/html; charset=UTF-8";
            evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", head_tail.c_str());
            evhttp_add_header(evhttp_request_get_output_headers(req), "Set-Cookie", cookie.c_str());
            evhttp_add_header(evhttp_request_get_output_headers(req), "Connection", "Keep-Alive");
            return send_page(req,
                    (config.root + config.source_root + config.default_page).c_str());
        }else
            return send_page(req, 
                    (config.root + config.source_root + config.index_page).c_str());
    }
    else
        return send_page(req,
                (config.root + config.source_root + config.default_page).c_str());
}

void
file_action(struct evhttp_request *req){
    if (evhttp_request_get_command (req) == EVHTTP_REQ_POST)
        return file_upload(req);
    else if(evhttp_request_get_command (req) == EVHTTP_REQ_GET)
        return file_download(req);
    else
        return send_page(req, 502);    
}

void
media_action(struct evhttp_request *req){
}

int 
auth(struct evhttp_request *req){
    if (evhttp_request_get_command (req) == EVHTTP_REQ_POST){
        struct evbuffer *buf = evhttp_request_get_input_buffer (req);
        evbuffer_add(buf, "", -1);    /* NUL-terminate the buffer */
        char *payload = (char *) evbuffer_pullup (buf, -1);
        // int post_data_len = evbuffer_get_length(buf);
        // char request_data_buf[4096] = {0};
        // memcpy(request_data_buf, payload, post_data_len);
        std::vector<std::string> sub_paths;
        boost::split(sub_paths, payload, boost::is_any_of( "&=" ), boost::token_compress_on);
        // printf("[username=%s ][password=%s\n", sub_paths[1].c_str(), sub_paths[3].c_str());
        if(sub_paths[1] == sub_paths[3])
            return 1;
        return 0;
    }
    return 0;
}

void action_cdn(struct evhttp_request *req){
    struct evbuffer *evb = NULL;
    std::string URI(evhttp_request_get_uri (req));
    std::vector<std::string> sub_paths;
    boost::split(sub_paths, URI, boost::is_any_of( ",/." ), boost::token_compress_on);
    if(sub_paths[2] == "login")
        return login_action(req);
    else if(sub_paths[2] == "file")
        return file_action(req);
    else if(sub_paths[2] == "media")
        return media_action(req);
    else
        return send_page(req, 502);
}

char *
load_file(const char * filename){
    // std::cout << "希望读取文件" << filename << std::endl;
    FILE * file = fopen(filename,"r");
    if(file == nullptr)
        return nullptr;
    struct stat statbuf;
    stat(filename,&statbuf);
    char * html = (char *)malloc(statbuf.st_size);
    if(html == nullptr)
        return nullptr;
    memset(html, 0 , statbuf.st_size);
    fread(html, sizeof(char), statbuf.st_size, file);
    return html;
}

void 
file_download(struct evhttp_request *req){
    struct evbuffer *buf = evhttp_request_get_input_buffer (req);
    evbuffer_add(buf, "", -1);    /* NUL-terminate the buffer */
    std::string head_info((char *)evbuffer_pullup (buf, -1));
    std::string filename;
    std::string path = config.root + config.data_root + filename;
    std::cout << "下载读出:" << path << std::endl;

}

void 
file_upload(struct evhttp_request *req){
    struct evbuffer *buf = evhttp_request_get_input_buffer(req);
    char *payload = (char *) evbuffer_pullup (buf, -1);
    std::string head_info(payload);
    int post_data_len = evbuffer_get_length(buf);
    std::vector<std::string> lines;
    boost::split(lines, head_info, boost::is_any_of( "\n" ), boost::token_compress_on);
    std::vector<std::string> names;
    boost::split(names, lines[1], boost::is_any_of( "\"" ), boost::token_compress_on);
    std::string path = config.root + config.data_root + names[names.size()-2];
    std::ofstream file(path);
    std::cout << "上传写入" << path << std::endl;
    
    if (file.is_open()){
        for(auto it = lines.begin()+4; it < lines.end()-2; ++it)
            file << (*it+ "\n");   
    
        // char cbuf[1024];
        // while(evbuffer_get_length(buf)) {
        //     int n = evbuffer_remove(buf, cbuf, sizeof(buf)-1);
        //     if(n > 0)
        //        file << cbuf;
        //         // fwrite(cbuf, 1, n, stdout);
        //         for(int i = 0; i < n; ++i)
        //         printf("%X",cbuf+i);
        // }
    file.close();
    }
    struct evbuffer *rbuf = evbuffer_new();
    evbuffer_add_printf(rbuf, "received file %s", names[names.size()-2].c_str());
    evhttp_send_reply(req, HTTP_OK, "SUCCESS", rbuf);
}

void
action_login(struct evhttp_request *req){
    if(evhttp_request_get_command (req) == EVHTTP_REQ_POST){
        struct evbuffer *evb = NULL;
        const char *uri = evhttp_request_get_uri (req);
        struct evhttp_uri *decoded = NULL;
        char * file_name = NULL;
        // int http_state = check_valid(uri, &file_name);
        // struct evbuffer *buf = evbuffer_new();
        struct evbuffer *buf = evhttp_request_get_input_buffer (req);
        std::stringstream data;
        data << (char *) evbuffer_pullup (buf, -1);
        std::string line;
        struct evbuffer *rbuf = evbuffer_new();
        evbuffer_add_printf(rbuf, "%s", "OK");
        evhttp_send_reply(req, HTTP_OK, "NULL", rbuf);
    }else{
        send_page(req, 502);
    }
}

void file_cdn(struct evhttp_request *req){
    std::string URI(evhttp_request_get_uri (req));
    std::vector<std::string> sub_paths;
    boost::split(sub_paths, URI, boost::is_any_of( ",/." ), boost::token_compress_on);
    size_t start = URI.find(".", URI.find(".")+1);
    std::string sub_path = URI.substr(start+1);
    //     std::cout << "file_cdn(%s)" << URI << std::endl;
    // for(auto s : sub_paths)
    //     std::cout << "sub " << s << " | ";
    // std::cout << std::endl;
    if(URI.size() <= 1)
        return send_page(req, (config.root + config.default_page).c_str());
    else{
        std::string url = config.root + config.source_root;
        if(sub_paths[2] == "login")
            url += (config.login + sub_path);
        else if(sub_paths[2] == "file")
            url += (config.file + sub_path);
        else if(sub_paths[2] == "states")
            url += (config.states + sub_path);
        else
            url += (config.test + sub_path);
        if(-1 == access(url.c_str(), 0))
            return send_page(req, 404);
        // else if(sub_paths[1] != config.source_root)
        //     return send_page(req, 403);
        else{
            std::cout << "url: " << url << std::endl;
            return send_page(req, url);
        }
    }
}

void
default_cb(struct evhttp_request *req, void *arg){
    // std::cout << "请求地址: " << evhttp_request_uri(req) << std::endl;
    // evhttp_send_error(req, 404, "404");
    // struct evkeyvalq * kv = evhttp_request_get_input_headers(req);
    // printf("%s\n", evhttp_find_header(kv, "SetCookie"));
    struct evbuffer *evb = NULL;
    std::string URI(evhttp_request_get_uri (req));
    std::vector<std::string> sub_paths;
    boost::split(sub_paths, URI, boost::is_any_of( ",/." ), boost::token_compress_on);
    // if(action_auth(req) == 0 )

    if(sub_paths[1] == "action")
        return action_cdn(req);
    else if(sub_paths[1] == "file")
        return file_cdn(req);
    else
        return send_page(req, (config.root + config.source_root + config.index_page).c_str());

    // return send_page(req, 403);
}

void 
send_page(struct evhttp_request *req, std::string page){
    struct evbuffer *buf = evbuffer_new();
    if (buf == NULL) 
        return;
    std::cout << "send_page::page=" << page << std::endl; 
    std::vector<std::string> sub_paths;
    boost::split(sub_paths, page, boost::is_any_of( ",/." ), boost::token_compress_on);
    std::string head_tail = "text/" + sub_paths.back() + "; charset=UTF-8";
    evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", head_tail.c_str());
    evhttp_add_header(evhttp_request_get_output_headers(req), "Connection", "Keep-Alive");
    // std::cout << "尝试读取文件" << page.c_str() << std::endl;
    // std::cout << "当前路径" << getcwd(nullptr,0) << std::endl;    
    // char * file = load_file(page.c_str());
    char * file = load_file(page.c_str());
    evbuffer_add_printf(buf, "%s", file);
    // std::cout << "发送数据: \n" << buf << std::endl;
    // printf("发送数据: \n %s",buf);
    free(file);
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
}

void send_page(struct evhttp_request *req, int states){
    struct evbuffer *buf = evbuffer_new();
    if (buf == NULL) return;
    evhttp_send_error(req, states, std::to_string(states).c_str());
}
