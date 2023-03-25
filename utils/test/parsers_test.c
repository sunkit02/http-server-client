#include "../parsers.h"
#include "../http.h"
#include <stdbool.h>

// Allows color coded the test results 
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


// Defined tests
bool canParseHttpResponse();
bool canStringifyHttpRequest();


int main(void) {

    if (canParseHttpResponse()) {
        puts(ANSI_COLOR_GREEN 
             "Test canParseHttpResponse() passed!" 
             ANSI_COLOR_RESET);
    } else {
        puts(ANSI_COLOR_RED
             "Test canParseHttpResponse() failed." 
             ANSI_COLOR_RESET);
    }

    if (canStringifyHttpRequest()) {
        puts(ANSI_COLOR_GREEN 
             "Test canStringifyHttpRequest() passed!" 
             ANSI_COLOR_RESET);
    } else {
        puts(ANSI_COLOR_RED
             "Test canParseHttpResponse() failed." 
             ANSI_COLOR_RESET);
    }

}


bool canParseHttpResponse() {
    const char *const rawResponse = "HTTP/1.1 200 OK\r\n"
                                    "key1: value1\r\n"
                                    "key2: value2\r\n"
                                    "key3: value3\r\n\r\n"
                                    "<h1>This is the body!</h1>\r\n";

    HttpResponse *response = parseHttpResponse(rawResponse);
    if (response == NULL) {
        puts(ANSI_COLOR_RED 
             "Failed to parse http response."
             ANSI_COLOR_RESET);

        return false;
    };

    int expectedStatusCode = 200;
    char *expectedStatus = "OK";
    char *expectedBody = "<h1>This is the body!</h1>";

    // Check for status and statusCode 
    if (response->statusCode != expectedStatusCode) {
        printf(ANSI_COLOR_RED 
               "Wrong status code. Expected: %d, Got: %d\n" 
               ANSI_COLOR_RESET,
               expectedStatusCode, response->statusCode);

        return false;
    }
    if (response->status == NULL ||
        strcmp(response->status, expectedStatus) != 0) {
        printf(ANSI_COLOR_RED
               "Wrong status. Expected: '%s', Got: '%s'\n" 
               ANSI_COLOR_RESET,
               expectedStatus, response->status);

        return false;
    }

    // Check headers
    char *expectedKeys[] = {"key1", "key2", "key3"};
    char *expectedValues[] = {"value1", "value2", "value3"};
    size_t n = sizeof(expectedKeys) / sizeof(expectedKeys[0]);

    if (response->headerList == NULL) {
        printf(ANSI_COLOR_RED
               "Expected %zu headers. Got (null)\n" 
               ANSI_COLOR_RESET, n);

        return false;
    }

    HttpHeader *header;
    for (size_t i = 0; i < n; i++) {
        header = httpHeaderListGet(response->headerList, expectedKeys[i]);
        if (header == NULL) {
            printf(ANSI_COLOR_RED 
                   "Expected header with key: '%s' not found\n"
                   ANSI_COLOR_RESET,
                   expectedKeys[i]);

            return false;
        }

        if (strcmp(header->value, expectedValues[i]) != 0) {
            printf(ANSI_COLOR_RED 
                   "Wrong value for header with key: '%s'."
                   "Expected: '%s', Got: '%s'\n" 
                   ANSI_COLOR_RESET,
                   expectedKeys[i],
                   expectedValues[i], header->value);

            return false;
        }
    }

    // Check body
    if (response->body == NULL ||
        strcmp(response->body, expectedBody) != 0) {
        printf(ANSI_COLOR_RED 
               "Wrong body. Expected: '%s', Got: '%s'\n" 
               ANSI_COLOR_RESET,
               expectedBody, response->body);

        return false;
    }

    return true;
}

bool canStringifyHttpRequest() {
    // Initialize test request
    HttpHeaderList *headerList = constructHttpHeaderList(5);
    HttpRequest request;
    request.method = GET;
    request.url = "/data";
    request.headerList = headerList;
    request.body = "{\"name\": \"Sun Kit\"}";

    httpHeaderListAdd(headerList, "Key1", "Value1");
    httpHeaderListAdd(headerList, "Key2", "Value2");
    httpHeaderListAdd(headerList, "Key3", "Value3");

    char *expectedStringifiedRequest = 
        "GET /data HTTP/1.1\r\n"
        "Key1:Value1\r\n"
        "Key2:Value2\r\n"
        "Key3:Value3\r\n"
        "\r\n"
        "{\"name\": \"Sun Kit\"}\r\n";

    char *stringifiedRequest = stringifyHttpRequest(&request);

    // Check for the correct stringified request
    if (strcmp(expectedStringifiedRequest, stringifiedRequest) != 0) {
        printf(ANSI_COLOR_RED 
               "Wrong stringified request\n"
               "Expected:\n"
               "%s\n\n"
               "Got:\n"
               "%s\n\n"
               ANSI_COLOR_RESET,
               expectedStringifiedRequest,
               stringifiedRequest); 

        return false;
    }

    // Free dynamically allocated memory
    free(stringifiedRequest);

    return true;
}
