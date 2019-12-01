
// void
// login_cb (struct evhttp_request *req, void *arg){
//     std::cout << "进入login——cb回调" << std::endl;
//     struct evbuffer *evb = NULL;
//     struct evhttp_uri *decoded = NULL;
    // if (evhttp_request_get_command (req) == EVHTTP_REQ_GET){
    //     // printf("GET\n");
    //     struct evbuffer *buf = evbuffer_new();
    //     if (buf == NULL) return;
    //     evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", "text/html; charset=UTF-8");
    //     evhttp_add_header(evhttp_request_get_output_headers(req), "Connection", "Keep-Alive");
    //     char * file = load_file("./html/login.html");
    //     evbuffer_add_printf(buf, "%s", file);
    //     free(file);
    //     evhttp_send_reply(req, HTTP_OK, "OK", buf);
    //     return;
//    }else if (evhttp_request_get_command (req) == EVHTTP_REQ_POST){ 
//    if (evhttp_request_get_command (req) == EVHTTP_REQ_POST){ 
        /* Decode the payload */
        // struct evbuffer *buf = evhttp_request_get_input_buffer (req);
        // evbuffer_add (buf, "", 1);    /* NUL-terminate the buffer */
        // char *payload = (char *) evbuffer_pullup (buf, -1);
        // int post_data_len = evbuffer_get_length(buf);
        // char request_data_buf[4096] = {0};
        // memcpy(request_data_buf, payload, post_data_len);
        // printf("[post_data][%d]=\n %s\n", post_data_len, payload);

        // evhttp_add_header(evhttp_request_get_output_headers(req), "Server", MYHTTPD_SIGNATURE);
        // evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", "text/html; charset=UTF-8");
        // evhttp_add_header(evhttp_request_get_output_headers(req), "Connection", "Keep-Alive");

        // evb = evbuffer_new ();
        // if(do_auth(payload) == 0){
        //     char * file = load_file("./html/login.html");
        //     evbuffer_add_printf(evb, "%s", file);    //将封装好的evbuffer 发送给客户端
        //     free(file);
        // }else{
        //     char * file = load_file("./html/UploadServer-master/html/index.html");
        //     evbuffer_add_printf(evb, "%s", file);    //将封装好的evbuffer 发送给客户端
        //     free(file);    
        // }
        // evhttp_send_reply(req, HTTP_OK, "OK", evb);
        // if (evb)
        //     evbuffer_free (evb);
        // // printf("[response]:\n");

    // }else
    //     return send_page(req, 502);
//}


// void
// state_cb(struct evhttp_request *req, void *arg, int state){
//     if(state = 404){
//         evhttp_add_header(evhttp_request_get_output_headers(req), "Server", MYHTTPD_SIGNATURE);
//         evhttp_add_header(evhttp_request_get_output_headers(req), "Content-Type", "text/html; charset=UTF-8");
//         evhttp_add_header(evhttp_request_get_output_headers(req), "Connection", "close");
//             char * file = load_file("./html/login.html");
//             struct evbuffer * evb = evbuffer_new ();
//             evbuffer_add_printf(evb, "%s", file);    //将封装好的evbuffer 发送给客户端
//             free(file);
//         evhttp_send_reply (req, 404, "NOT FOUND", NULL);
//         return;
//     }else{
//         return;
//     }
// }
