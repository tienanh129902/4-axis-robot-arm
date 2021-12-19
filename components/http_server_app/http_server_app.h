#ifndef __HTTP_SERVER_APP_H
#define __HTTP_SERVER_APP_H

typedef void (*http_post_callback_t) (char* data, int len);
typedef void (*http_get_callback_t) (void); 
typedef void (*http_get_data_callback_t) (char* data, int len); 

void start_webserver(void);
void stop_webserver(void);
void http_set_page_load_callback(void *cb);
void http_set_callback_slider1(void *cb);
void http_set_callback_slider2(void *cb);
void http_set_callback_slider3(void *cb);
void http_set_callback_slider4(void *cb);

#endif