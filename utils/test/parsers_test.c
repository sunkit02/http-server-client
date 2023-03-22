#include "../parsers.h"
#include "../http.h"
#include <stdbool.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


bool canParseHttpResponse();


int main(void) {

    if (canParseHttpResponse()) {
        puts("Test canParseHttpResponse() passed!");
    } else {
        puts("Test canParseHttpResponse() failed.");
    }

}


bool canParseHttpResponse() {
    const char *const rawResponse = 
        "HTTP/1.1 200 OK\r\n"
        "key1: value1\r\n"
        "key2: value2\r\n"
        "key3: value3\r\n\r\n"
        "<h1>This is the body!</h1>\r\n";

    HttpResponse *response = parseHttpResponse(rawResponse);

    int expectedStatusCode = 200;
    char *expectedStatus = "OK";
    char *expectedBody = "<h1>This is the body!</h1>";

    if (response->statusCode != expectedStatusCode) {
        printf(ANSI_COLOR_RED "Wrong status code. Expected: %d, Got: %d\n" ANSI_COLOR_RESET,
               expectedStatusCode, response->statusCode);
        return false;
    }
    if (response->status == NULL ||
        strcmp(response->status, expectedStatus) != 0) {
        printf(ANSI_COLOR_RED "Wrong status. Expected: '%s', Got: '%s'\n" ANSI_COLOR_RESET,
               expectedStatus, response->status);
        return false;
    }
    if (response->status == NULL ||
        strcmp(response->body, expectedBody) != 0) {
        printf(ANSI_COLOR_RED "Wrong body. Expected: '%s', Got: '%s'\n" ANSI_COLOR_RESET,
               expectedBody, response->body);
        return false;
    }

    // Check headers
    char *expectedKeys[] = {"key1", "key2", "key3"};
    char *expectedValues[] = {"value1", "value2", "value3"};
    size_t n = sizeof(expectedKeys) / sizeof(expectedKeys[0]);

    if (response->headerList == NULL) {
        printf(ANSI_COLOR_RED "Expected %zu headers. Got 0" ANSI_COLOR_RESET, n);
        return false;
    }

    HttpHeader *header;
    for (size_t i = 0; i < n; i++) {
        header = httpHeaderListGet(response->headerList, expectedKeys[i]);
        if (header == NULL) {
            printf(ANSI_COLOR_RED "Expected header with key: '%s' not found\n" ANSI_COLOR_RESET,
                    expectedKeys[i]);
            return false;
        }
        if (strcmp(header->value, expectedValues[i]) != 0) {
            printf(ANSI_COLOR_RED "Wrong value for header with key: '%s'."
                   "Expected: '%s', Got: '%s'\n" ANSI_COLOR_RESET,
                    expectedKeys[i], expectedValues[i], header->value);
            return false;
        }
    }

    return true;
}
