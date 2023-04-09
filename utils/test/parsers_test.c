#include "../parsers.h"
#include "../http.h"
#include <stdbool.h>
#include <string.h>

// Allows color coded the test results 
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// Print red for fail and green for pass
#define LOG_PASS(format, ...) printf(ANSI_COLOR_GREEN format ANSI_COLOR_RESET, ##__VA_ARGS__)
#define LOG_FAIL(format, ...) printf(ANSI_COLOR_RED format ANSI_COLOR_RESET, ##__VA_ARGS__)

// Util function for calling tests
void runTest(char* testName, bool (*testFunction)());

// Defined tests
bool canParseHttpResponseWithBody();
bool canParseHttpResponseWithoutBody();
bool canParseHttpRequestWithBody();
bool canParseHttpRequestWithoutBody();
bool canStringifyHttpRequest();


int main(void) {
    runTest("canStringifyHttpRequest",canStringifyHttpRequest);
    runTest("canParseHttpResponseWithBody", canParseHttpResponseWithBody);
    runTest("canParseHttpResponseWithoutBody", canParseHttpResponseWithoutBody);
    runTest("canParseHttpRequestWithBody", canParseHttpRequestWithBody);
    runTest("canParseHttpRequestWithoutBody", canParseHttpRequestWithoutBody);
}

// Displays the appropriate message depending on whether a test passed or failed
void runTest(char* testName, bool (*testFunction)()) {
    if (testFunction()) {
        LOG_PASS("Test %s passed!\n", testName);
    } else {
        LOG_FAIL("Test %s failed.\n", testName);
    }
}


bool canParseHttpResponseWithBody() {
    // Given
    const char *const rawResponse = "HTTP/1.1 200 OK\r\n"
                                    "key1: value1\r\n"
                                    "key2: value2\r\n"
                                    "key3: value3\r\n\r\n"
                                    "<h1>This is the body!</h1>\r\n\r\n";

    int expectedStatusCode = 200;
    char *expectedStatus = "OK";
    char *expectedBody = "<h1>This is the body!</h1>";

    char *expectedKeys[] = {"key1", "key2", "key3"};
    char *expectedValues[] = {"value1", "value2", "value3"};
    size_t n = sizeof(expectedKeys) / sizeof(expectedKeys[0]);


    // When
    HttpResponse *response = parseHttpResponse(rawResponse);


    // Then

    // Check if parse was successful
    if (response == NULL) {
        LOG_FAIL("Failed to parse http response.\n");

        return false;
    }

    // Check status code
    if (response->statusCode != expectedStatusCode) {
        LOG_FAIL("Wrong status code. Expected: %d, Got: %d\n",
                 expectedStatusCode, response->statusCode);

        return false;
    }

    // Check status
    if (response->status == NULL ||
        strcmp(response->status, expectedStatus) != 0) {
        LOG_FAIL("Wrong status. Expected: '%s', Got: '%s'\n",
                 expectedStatus, response->status);

        return false;
    }

    // Check headers
    if (response->headerList == NULL) {
        LOG_FAIL("Expected %zu headers. Got (null)\n", n);

        return false;
    }

    HttpHeader *header;
    for (size_t i = 0; i < n; i++) {
        header = httpHeaderListGet(response->headerList, expectedKeys[i]);
        if (header == NULL) {
            LOG_FAIL("Expected header with key: '%s' not found\n", 
                     expectedKeys[i]);

            return false;
        }

        if (strcmp(header->value, expectedValues[i]) != 0) {
            LOG_FAIL("Wrong value for header with key: '%s'."
                     "Expected: '%s', Got: '%s'\n",
                     expectedKeys[i],
                     expectedValues[i], header->value);

            return false;
        }
    }

    // Check body
    if (response->body == NULL ||
        strcmp(response->body, expectedBody) != 0) {
        LOG_FAIL("Wrong body.\n"
                 "Expected:\n"
                 "--------\n"
                 "%s\n"
                 "---------\n"
                 "Got:\n"
                 "--------\n"
                 "%s\n"
                 "---------\n",
                 expectedBody, response->body);

        return false;
    }

    return true;
}


bool canParseHttpResponseWithoutBody() {
    // Given
    const char *const rawResponse = "HTTP/1.1 200 OK\r\n"
                                    "key1: value1\r\n"
                                    "key2: value2\r\n"
                                    "key3: value3\r\n\r\n";

    int expectedStatusCode = 200;
    char *expectedStatus = "OK";
    char *expectedBody = NULL;

    char *expectedKeys[] = {"key1", "key2", "key3"};
    char *expectedValues[] = {"value1", "value2", "value3"};
    size_t n = sizeof(expectedKeys) / sizeof(expectedKeys[0]);


    // When
    HttpResponse *response = parseHttpResponse(rawResponse);


    // Then

    // Check if parse was successful
    if (response == NULL) {
        LOG_FAIL("Failed to parse http response.\n");

        return false;
    }

    // Check status code
    if (response->statusCode != expectedStatusCode) {
        LOG_FAIL("Wrong status code. Expected: %d, Got: %d\n",
                 expectedStatusCode, response->statusCode);

        return false;
    }

    // Check status
    if (response->status == NULL ||
        strcmp(response->status, expectedStatus) != 0) {
        LOG_FAIL("Wrong status. Expected: '%s', Got: '%s'\n",
                 expectedStatus, response->status);

        return false;
    }

    // Check headers
    if (response->headerList == NULL) {
        LOG_FAIL("Expected %zu headers. Got (null)\n", n);

        return false;
    }

    HttpHeader *header;
    for (size_t i = 0; i < n; i++) {
        header = httpHeaderListGet(response->headerList, expectedKeys[i]);
        if (header == NULL) {
            LOG_FAIL("Expected header with key: '%s' not found\n", 
                     expectedKeys[i]);

            return false;
        }

        if (strcmp(header->value, expectedValues[i]) != 0) {
            LOG_FAIL("Wrong value for header with key: '%s'."
                     "Expected: '%s', Got: '%s'\n",
                     expectedKeys[i],
                     expectedValues[i], header->value);

            return false;
        }
    }

    // Check body
    if (response->body != NULL) {
        LOG_FAIL("Wrong body.\n"
                 "Expected:\n"
                 "--------\n"
                 "%s\n"
                 "---------\n"
                 "Got:\n"
                 "--------\n"
                 "%s\n"
                 "---------\n",
                 expectedBody, response->body);

        return false;
    }

    return true;
}


bool canParseHttpRequestWithBody() {
    // Given
    char *rawRequest = "POST /post-data HTTP/1.1\r\n"
                       "key1: value1\r\n"
                       "key2: value2\r\n"
                       "key3: value3\r\n\r\n"
                       "{\"name\": \"Eunice\", \"age\": 20}\r\n"
                       "{\"name\": \"Sun Kit\", \"age\": 21}\r\n\r\n";

    HttpMethods expectedMethod = POST;
    char *expectedUrl = "/post-data";
    char *expectedBody = "{\"name\": \"Eunice\", \"age\": 20}\r\n"
                         "{\"name\": \"Sun Kit\", \"age\": 21}";

    HttpHeader expectedHeaders[] = {
        {.key = "key1", .value = "value1"},
        {.key = "key2", .value = "value2"},
        {.key = "key3", .value = "value3"},
    };
    size_t expectedNumOfHeaders = 3;


    // When
    HttpRequest *request = parseHttpRequest(rawRequest);


    // Then

    // Check if parse was successful
    if (request == NULL) {
        LOG_FAIL("Failed to parse http request.\n");

        return false;
    }

    // Check method
    if (request->method != expectedMethod) {
        LOG_FAIL("Wrong method. Expected: %d, Got: %d\n",
                 expectedMethod, request->method);

        return false;
    } 

    // Check url
    if (strcmp(request->url, expectedUrl) != 0) {
        LOG_FAIL("Wrong url. Expected: '%s', Got: '%s'\n",
                 expectedUrl, request->url);

        return false;
    }

    // Check body
    if (request->body == NULL ||
        strcmp(request->body, expectedBody) != 0) {
        LOG_FAIL("Wrong body.\n"
                 "Expected:\n"
                 "--------\n"
                 "%s\n"
                 "---------\n"
                 "Got:\n"
                 "--------\n"
                 "%s\n"
                 "---------\n",
                 expectedBody, request->body);
        
        return false;
    }

    // Check headers
    for (size_t i = 0; i < expectedNumOfHeaders; i++) {
        HttpHeader *header = httpHeaderListGet(request->headerList, expectedHeaders[i].key);
        if (header == NULL) {
            LOG_FAIL("Missing header with key: '%s'\n", expectedHeaders[i].key);

            return false;
        }

        if (strcmp(header->value, expectedHeaders[i].value)) {
            LOG_FAIL("Wrong header.\n"
                     "Expected: HttpHeader(key='%s', value='%s')\n"
                     "Got:      HttpHeader(key='%s', value='%s')\n",
                     expectedHeaders[i].key, expectedHeaders[i].value,
                     header->key, header->value);

            return false;
        }
    }

    // Return true if all checks have passed
    return true;
}


bool canParseHttpRequestWithoutBody() {
    // Given
    char *rawRequest = "POST /post-data HTTP/1.1\r\n"
                       "key1: value1\r\n"
                       "key2: value2\r\n"
                       "key3: value3\r\n\r\n";

    HttpMethods expectedMethod = POST;
    char *expectedUrl = "/post-data";
    char *expectedBody = NULL;

    HttpHeader expectedHeaders[] = {
        {.key = "key1", .value = "value1"},
        {.key = "key2", .value = "value2"},
        {.key = "key3", .value = "value3"},
    };
    size_t expectedNumOfHeaders = 3;


    // When
    HttpRequest *request = parseHttpRequest(rawRequest);
    // if (request->body == NULL) puts("True");
    // else puts("False");


    // Then

    // Check if parse was successful
    if (request == NULL) {
        LOG_FAIL("Failed to parse http request.\n");

        return false;
    }

    // Check method
    if (request->method != expectedMethod) {
        LOG_FAIL("Wrong method. Expected: %d, Got: %d\n",
                 expectedMethod, request->method);

        return false;
    } 

    // Check url
    if (strcmp(request->url, expectedUrl) != 0) {
        LOG_FAIL("Wrong url. Expected: '%s', Got: '%s'\n",
                 expectedUrl, request->url);

        return false;
    }

    // Check body
    if (request->body != NULL) {
        LOG_FAIL("Wrong body.\n"
                 "Expected:\n"
                 "--------\n"
                 "%s\n"
                 "---------\n"
                 "Got:\n"
                 "--------\n"
                 "%s\n"
                 "---------\n",
                 expectedBody, request->body);
        
        return false;
    }

    // Check headers
    for (size_t i = 0; i < expectedNumOfHeaders; i++) {
        HttpHeader *header = httpHeaderListGet(request->headerList, expectedHeaders[i].key);
        if (header == NULL) {
            LOG_FAIL("Missing header with key: '%s'\n", expectedHeaders[i].key);

            return false;
        }

        if (strcmp(header->value, expectedHeaders[i].value)) {
            LOG_FAIL("Wrong header.\n"
                     "Expected: HttpHeader(key='%s', value='%s')\n"
                     "Got:      HttpHeader(key='%s', value='%s')\n",
                     expectedHeaders[i].key, expectedHeaders[i].value,
                     header->key, header->value);

            return false;
        }
    }

    // Return true if all checks have passed
    return true;
}


bool canStringifyHttpRequest() {
    // Given
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


    // When
    char *stringifiedRequest = stringifyHttpRequest(&request);


    // Then
    // Check for the correct stringified request
    if (strcmp(expectedStringifiedRequest, stringifiedRequest) != 0) {
        LOG_FAIL("Wrong stringified request\n"
                 "Expected:\n"
                 "%s\n\n"
                 "Got:\n"
                 "%s\n\n",
                 expectedStringifiedRequest,
                 stringifiedRequest); 

        return false;
    }

    // Free dynamically allocated memory
    free(stringifiedRequest);

    return true;
}

