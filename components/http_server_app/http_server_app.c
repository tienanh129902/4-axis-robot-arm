#include "http_server_app.h"
#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <esp_system.h>
#include <sys/param.h>
#include "esp_netif.h"
#include "esp_eth.h"
#include "stdio.h"
#include <esp_http_server.h>

/* A simple example that demonstrates how to create GET and POST
 * handlers for the web server.
 */

static const char *TAG = "HTTP_SERVER";
static httpd_handle_t server = NULL;
// static httpd_req_t *REG;

extern const uint8_t index_html_start[] asm("_binary_index_html_start");
extern const uint8_t index_html_end[] asm("_binary_index_html_end");

static http_post_callback_t http_post_slider1_callback = NULL;
static http_post_callback_t http_post_slider2_callback = NULL;
static http_post_callback_t http_post_slider3_callback = NULL;
static http_post_callback_t http_post_slider4_callback = NULL;
static http_get_callback_t http_get_page_load_callback = NULL;

/* An HTTP POST handler */
static esp_err_t slider1_post_handler(httpd_req_t *req)
{
    char buf[100];
    /* Read the data for the request */
    httpd_req_recv(req, buf, req->content_len);
    http_post_slider1_callback(buf, req->content_len);

    // End response
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

/* An HTTP POST handler */
static esp_err_t slider2_post_handler(httpd_req_t *req)
{
    char buf[100];
    /* Read the data for the request */
    httpd_req_recv(req, buf, req->content_len);
    http_post_slider2_callback(buf, req->content_len);

    // End response
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

/* An HTTP POST handler */
static esp_err_t slider3_post_handler(httpd_req_t *req)
{
    char buf[100];
    /* Read the data for the request */
    httpd_req_recv(req, buf, req->content_len);
    http_post_slider3_callback(buf, req->content_len);

    // End response
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

/* An HTTP POST handler */
static esp_err_t slider4_post_handler(httpd_req_t *req)
{
    char buf[100];
    /* Read the data for the request */
    httpd_req_recv(req, buf, req->content_len);
    http_post_slider4_callback(buf, req->content_len);

    // End response
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

static const httpd_uri_t slider1_post_data = {
    .uri = "/slider1",
    .method = HTTP_POST,
    .handler = slider1_post_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx = NULL};

static const httpd_uri_t slider2_post_data = {
    .uri = "/slider2",
    .method = HTTP_POST,
    .handler = slider2_post_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx = NULL};

static const httpd_uri_t slider3_post_data = {
    .uri = "/slider3",
    .method = HTTP_POST,
    .handler = slider3_post_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx = NULL};

static const httpd_uri_t slider4_post_data = {
    .uri = "/slider4",
    .method = HTTP_POST,
    .handler = slider4_post_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx = NULL};

/* An HTTP GET handler */
static esp_err_t page_get_handler(httpd_req_t *req)
{
    // const char* resp_str = (const char*) "Hello World";
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, (const char *)index_html_start, index_html_end - index_html_start);
    // httpd_resp_set_type(req, "text/css");
    // httpd_resp_send(req, (const char *)style_css_start, style_css_end - style_css_start);
    // httpd_resp_set_type(req, "text/js");
    // httpd_resp_send(req, (const char *)main_js_start, main_js_end - main_js_start);
    return ESP_OK;
}

static const httpd_uri_t get_page = {
    .uri = "/robot",
    .method = HTTP_GET,
    .handler = page_get_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx = NULL};

esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err)
{
    if (strcmp("/robot", req->uri) == 0)
    {
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "/robot URI is not available");
        /* Return ESP_OK to keep underlying socket open */
        return ESP_OK;
    }
    /* For any other URI send 404 and close socket */
    httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Some 404 error message");
    return ESP_FAIL;
}

void start_webserver(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK)
    {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &get_page);
        httpd_register_uri_handler(server, &slider1_post_data);
        httpd_register_uri_handler(server, &slider2_post_data);
        httpd_register_uri_handler(server, &slider3_post_data);
        httpd_register_uri_handler(server, &slider4_post_data);
        httpd_register_err_handler(server, HTTPD_404_NOT_FOUND, http_404_error_handler);
    }
    else
    {
        ESP_LOGI(TAG, "Error starting server!");
    }
}

void stop_webserver(void)
{
    httpd_stop(server);
}

void http_set_page_load_callback(void *cb)
{
    http_get_page_load_callback = cb;
}

void http_set_callback_slider1(void *cb)
{
    http_post_slider1_callback = cb;
}

void http_set_callback_slider2(void *cb)
{
    http_post_slider2_callback = cb;
}

void http_set_callback_slider3(void *cb)
{
    http_post_slider3_callback = cb;
}

void http_set_callback_slider4(void *cb)
{
    http_post_slider4_callback = cb;
}
