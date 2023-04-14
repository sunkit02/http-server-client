#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "http.h"


// Constructs an empty HttpHeaderList
// Returns NULL if failed to construct the list
HttpHeaderList *constructHttpHeaderList(size_t capacity) {
    HttpHeaderList *list = malloc(sizeof(HttpHeaderList));
    list->size = 0;
    list->capacity = capacity;
    list->headers = malloc(capacity * sizeof(HttpHeader *));

    if (list->headers == NULL) return NULL;

    return list;
}


// Adds to the end of the HttpHeaderList
bool httpHeaderListAdd(HttpHeaderList *list, char *key, char *value) {
    // Resize list when reaching capacity
    if (list->size >= list->capacity) {
        list->capacity *= 2;
        HttpHeader **temp = realloc(list->headers, 
                                    list->capacity * sizeof(HttpHeader *));
        if (temp == NULL) return false;
        list->headers = temp;
    }

    // Construct HttpHeader by creating a copy of key value pair
    // in memory and assigning to new header struct
    HttpHeader *header = malloc(sizeof(HttpHeader));

    header->key = malloc(strlen(key) + 1);
    header->value = malloc(strlen(value) + 1);
    if (header->key == NULL || header->value == NULL) {
        httpHeaderDestroy(header);
        return false;
    }

    strcpy(header->key, key);
    strcpy(header->value, value);
    

    list->headers[list->size++] = header;
    return true;
}


// Gets the HttpHeader in HttpHeaderList if key exists. 
// Returns NULL if key doesn't exist
HttpHeader *httpHeaderListGet(HttpHeaderList *list, char *key) {
    if (list->size == 0) return NULL;

    for (size_t i = 0; i < list->size; i++) {
        HttpHeader *header = list->headers[i];
        if (strcmp(header->key, key) == 0) {
            return header;
        }
    }

    return NULL;
}


// Removes HttpHeader in list with specified key
// Returns true if removed successfully, false if key doesn't exist
// Note: Removing HttpHeader from list frees its memory
bool httpHeaderListRemove(HttpHeaderList *list, char *key) {
    HttpHeader *target;
    // Find header with key
    for (size_t i = 0; i < list->size; i++) {
            target = list->headers[i];
        if (strcmp(key, target->key) == 0) {
            // Free memory of target header
            httpHeaderDestroy(target);

            // Shift everything behind one index forward
            for (size_t j = i; j < list->size - 1; j++) {
                list->headers[j] = list->headers[j + 1];
            }

            list->size--;

            return true;
        }
    }
    return false;
}


// Checks if an HttpHeader with specified key exists in list
bool httpHeaderListContainsHeader(HttpHeaderList *list, char *key) {
    if (list == NULL) {
        perror("NULL pointer to HttpHeaderList");
        exit(EXIT_FAILURE);
    }

    if (key == NULL) {
        perror("NULL pointer to key");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < list->size; i++) {
        if (strcmp(list->headers[i]->key, key) == 0) {
            return true;
        }
    }
    return false;
}


// Frees all memory used by HttpHeader
void httpHeaderDestroy(HttpHeader *header) {
    if (header == NULL) return;

    if (header->key != NULL) 
        free(header->key);
    if (header->value != NULL)
        free(header->value);

    free(header);
}

// Frees all memory used by HttpHeaderList
void httpHeaderListDestroy(HttpHeaderList *list) {
    if (list->headers != NULL) {
        // Free all headers
        for (size_t i = 0; i < list->size; i++) {
            httpHeaderDestroy(list->headers[i]);
        }

        // Free header array
        free(list->headers);
    }

    // Free HttpHeaderList
    free(list);
}
