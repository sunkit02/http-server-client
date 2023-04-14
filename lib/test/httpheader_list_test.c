#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../http.h"

#include "test_utils.h"


// Util functions
void printHttpHeaderList(HttpHeaderList *list);
bool httpHeaderListsAreEqual(HttpHeaderList *list1, HttpHeaderList *list2);
HttpHeaderList *initializeHttpHeaderList(HttpHeader *headers, size_t size);
char *httpHeaderListToString(HttpHeaderList *list);

// Test declaration
bool canAddToHttpHeaderList();
bool canRemoveFromHttpHeaderList();
bool canAddToThenRemoveFromHttpHeaderList();
bool canRemoveFromThenAddToHttpHeaderList();
bool canResizeHttpHeaderListWhenAdding();


int main(void) {
    runTest("canAddToHttpHeaderList", canAddToHttpHeaderList);
    runTest("canRemoveFromHttpHeaderList", canRemoveFromHttpHeaderList);
    runTest("canAddToThenRemoveFromHttpHeaderList", canAddToThenRemoveFromHttpHeaderList);
    runTest("canRemoveFromThenAddToHttpHeaderList", canRemoveFromThenAddToHttpHeaderList);
    runTest("canResizeHttpHeaderListWhenAdding", canResizeHttpHeaderListWhenAdding);
}


bool canAddToHttpHeaderList() {
    // Given
    size_t expectedNumOfHeaders = 5;
    HttpHeaderList *underTest = constructHttpHeaderList(expectedNumOfHeaders);

    HttpHeader expectedHeaders[] = {
        {"Key1", "Value1"},
        {"Key2", "Value2"},
        {"Key3", "Value3"},
        {"Key4", "Value4"},
        {"Key5", "Value5"},
    };

    HttpHeaderList *expectedHeaderList = 
        initializeHttpHeaderList(expectedHeaders, expectedNumOfHeaders);


    // When

    // Add all the expected headers to HttpHeaderList underTest
    httpHeaderListAdd(underTest, "Key1", "Value1");
    httpHeaderListAdd(underTest, "Key2", "Value2");
    httpHeaderListAdd(underTest, "Key3", "Value3");
    httpHeaderListAdd(underTest, "Key4", "Value4");
    httpHeaderListAdd(underTest, "Key5", "Value5");


    // Then
    if (!httpHeaderListsAreEqual(underTest, expectedHeaderList)) {
        char *expectedListString = httpHeaderListToString(expectedHeaderList);
        char *underTestListString = httpHeaderListToString(underTest);

        LOG_FAIL("HttpHeaderLists are not equal\n"
                 "Expected:\n"
                 "%s\n\n"
                 "Got:\n"
                 "%s\n\n",
                 expectedListString,
                 underTestListString);

        free(expectedListString);
        free(underTestListString);

        return false;
    }
    
    return true;
}

bool canRemoveFromHttpHeaderList() {
    // Given
    HttpHeaderList *underTest = constructHttpHeaderList(5);

    httpHeaderListAdd(underTest, "Key1", "Value1");
    httpHeaderListAdd(underTest, "Key2", "Value2");
    httpHeaderListAdd(underTest, "Key3", "Value3");
    httpHeaderListAdd(underTest, "Key4", "Value4");
    httpHeaderListAdd(underTest, "Key5", "Value5");

    size_t expectedNumOfHeaders = 3;
    HttpHeader expectedHeaders[] = {
        {"Key1", "Value1"},
        {"Key3", "Value3"},
        {"Key5", "Value5"},
    };

    HttpHeaderList *expectedHeaderList =
            initializeHttpHeaderList(expectedHeaders, expectedNumOfHeaders);


    // When
    httpHeaderListRemove(underTest, "Key2");
    httpHeaderListRemove(underTest, "Key4");


    // Then
    if (!httpHeaderListsAreEqual(underTest, expectedHeaderList)) {
        char *expectedListString = httpHeaderListToString(expectedHeaderList);
        char *underTestListString = httpHeaderListToString(underTest);

        LOG_FAIL("HttpHeaderLists are not equal\n"
                 "Expected:\n"
                 "%s\n\n"
                 "Got:\n"
                 "%s\n\n",
                 expectedListString,
                 underTestListString);

        free(expectedListString);
        free(underTestListString);

        return false;
    }

    return true;
}

bool canAddToThenRemoveFromHttpHeaderList() {
    // Given
    HttpHeaderList *underTest = constructHttpHeaderList(5);

    httpHeaderListAdd(underTest, "Key1", "Value1");
    httpHeaderListAdd(underTest, "Key2", "Value2");
    httpHeaderListAdd(underTest, "Key3", "Value3");
    httpHeaderListAdd(underTest, "Key4", "Value4");
    httpHeaderListAdd(underTest, "Key5", "Value5");

    size_t expectedNumOfHeaders = 5;
    HttpHeader expectedHeaders[] = {
        {"Key1", "Value1"},
        {"Key3", "Value3"},
        {"Key5", "Value5"},
        {"Key7", "Value7"},
        {"Key9", "Value9"},
    };

    HttpHeaderList *expectedHeaderList =
            initializeHttpHeaderList(expectedHeaders, expectedNumOfHeaders);


    // When
    httpHeaderListRemove(underTest, "Key2");
    httpHeaderListRemove(underTest, "Key4");

    httpHeaderListAdd(underTest, "Key7", "Value7");
    httpHeaderListAdd(underTest, "Key9", "Value9");


    // Then
    if (!httpHeaderListsAreEqual(underTest, expectedHeaderList)) {
        char *expectedListString = httpHeaderListToString(expectedHeaderList);
        char *underTestListString = httpHeaderListToString(underTest);

        LOG_FAIL("HttpHeaderLists are not equal\n"
                 "Expected:\n"
                 "%s\n\n"
                 "Got:\n"
                 "%s\n\n",
                 expectedListString,
                 underTestListString);

        free(expectedListString);
        free(underTestListString);

        return false;
    }

    return true;
}

bool canRemoveFromThenAddToHttpHeaderList() {
    // Given
    HttpHeaderList *underTest = constructHttpHeaderList(5);

    httpHeaderListAdd(underTest, "Key1", "Value1");
    httpHeaderListAdd(underTest, "Key2", "Value2");
    httpHeaderListAdd(underTest, "Key3", "Value3");

    size_t expectedNumOfHeaders = 5;
    HttpHeader expectedHeaders[] = {
        {"Key1", "Value1"},
        {"Key2", "Value2"},
        {"Key3", "Value3"},
        {"Key4", "Value4"},
        {"Key5", "Value5"},
    };

    HttpHeaderList *expectedHeaderList =
            initializeHttpHeaderList(expectedHeaders, expectedNumOfHeaders);


    // When
    // Remove all headers (empty the HttpHeaderList)
    httpHeaderListRemove(underTest, "Key1");
    httpHeaderListRemove(underTest, "Key2");
    httpHeaderListRemove(underTest, "Key3");

    httpHeaderListAdd(underTest, "Key1", "Value1");
    httpHeaderListAdd(underTest, "Key2", "Value2");
    httpHeaderListAdd(underTest, "Key3", "Value3");
    httpHeaderListAdd(underTest, "Key4", "Value4");
    httpHeaderListAdd(underTest, "Key5", "Value5");


    // Then
    if (!httpHeaderListsAreEqual(underTest, expectedHeaderList)) {
        char *expectedListString = httpHeaderListToString(expectedHeaderList);
        char *underTestListString = httpHeaderListToString(underTest);

        LOG_FAIL("HttpHeaderLists are not equal\n"
                 "Expected:\n"
                 "%s\n\n"
                 "Got:\n"
                 "%s\n\n",
                 expectedListString,
                 underTestListString);

        free(expectedListString);
        free(underTestListString);

        return false;
    }

    return true;
}

bool canResizeHttpHeaderListWhenAdding() {
    HttpHeaderList *underTest = constructHttpHeaderList(2); // Capacity = 2
    httpHeaderListAdd(underTest, "Key1", "Value1"); // Capacity = 2
    httpHeaderListAdd(underTest, "Key2", "Value2"); // Capacity = 2

    if (underTest->capacity != 2) {
        LOG_FAIL("Wrong capacity. Expected: %d, Got: %zu\n",
                 2, underTest->capacity);
        return false;
    }

    ////////////////////// Resize //////////////////////
    httpHeaderListAdd(underTest, "Key3", "Value3"); // Capacity = 4
    httpHeaderListAdd(underTest, "Key4", "Value4"); // Capacity = 4
    
    if (underTest->capacity != 4) {
        LOG_FAIL("Wrong capacity. Expected: %d, Got: %zu\n",
                 4, underTest->capacity);
        return false;
    }


    ////////////////////// Resize //////////////////////
    httpHeaderListAdd(underTest, "Key4", "Value4"); // Capacity = 8
    httpHeaderListAdd(underTest, "Key6", "Value6"); // Capacity = 8
    
    if (underTest->capacity != 8) {
        LOG_FAIL("Wrong capacity. Expected: %d, Got: %zu\n",
                 8, underTest->capacity);
        return false;
    }

    return true;
}

HttpHeaderList *initializeHttpHeaderList(HttpHeader *headers, size_t size) {
    HttpHeaderList *list = constructHttpHeaderList(size);
    for (size_t i = 0; i < size; i++) {
        httpHeaderListAdd(list, headers[i].key, headers[i].value);
    }

    return list;
}

char *httpHeaderListToString(HttpHeaderList *list) {
    size_t capacity = list->size > 0 ? (list->size * 20) + 3 : 3;

    char *result = malloc(capacity);

    strcat(result, "[");

    char headerStringBuffer[100] = {0};

    for (size_t i = 0; i < list->size; i++) {
        sprintf(headerStringBuffer, "(%s, %s)%s",
                list->headers[i]->key, list->headers[i]->value,
                i == (list->size - 1) ? "" : ", ");

        if (strlen(result) + strlen(headerStringBuffer) >= capacity) {
            capacity *= 2;
            char *temp = realloc(result, capacity);
            if (temp == NULL) return NULL;
            result = temp;
        }

        strcat(result, headerStringBuffer);
    }

    if (strlen(result) + 1 >= (capacity - 1)) {
        capacity += 1;
        char *temp = realloc(result, capacity);
        if (temp == NULL) return NULL;
        result = temp;
    }

    strcat(result, "]");

    return result;
}


void printHttpHeaderList(HttpHeaderList *list) {
    printf("%s", "[");
    for (size_t i = 0; i < list->size; i++) {
        HttpHeader *header = list->headers[i];

        printf("Header(key=%s, value=%s)%s",
               header->key, header->value,
               i == list->size - 1 ? "" : ",\n");
    }
    printf("%s", "]\n");
}

// Checks whether two HttpHeaderList have the exact same list of HttpHeaders 
// (keys and value pairs) order doesn't matter
bool httpHeaderListsAreEqual(HttpHeaderList *list1, HttpHeaderList *list2) {
    // Ensure an equal number of entries
    if (list1->size != list2->size) return false;

    // Compare all entries
    for (size_t i = 0; i < list1->size; i++) {
        HttpHeader *header1 = list1->headers[i];
        HttpHeader *header2 = httpHeaderListGet(list2, header1->key);

        if (header2 == NULL || strcmp(header1->value, header2->value) != 0)
            return false;
    }

    return true;
}
