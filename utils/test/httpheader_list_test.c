#include "../http.h"
#include <stdio.h>
#include <string.h>

void printHttpHeaderList(HttpHeaderList *list);

int main(void) {
    HttpHeaderList *list = constructHttpHeaderList(10);

    for (size_t i = 0; i < 22; i++) {
        char key[15];
        char value[15];
        sprintf(key, "Key %d", (int)i);
        sprintf(value, "Value %d", (int)i);

        if (!httpHeaderListAdd(list, key, value)) {
            printf("failed to add at index %zu\n", i);
            break;
        }

        printf("key=%s, value=%s\n", key, value);
        printf("Stored: key=%s, value%s\n", 
                list->headers[i]->key, list->headers[i]->value);
        printf("List contains header: %s\n\n",
               httpHeaderListContainsHeader(list, key) ? "true" : "false");
    }
    puts("finished adding");

    char *nonExistentHeader = "doesn't exist";
    printf("Contains header: '%s' = %s\n", nonExistentHeader,
           httpHeaderListContainsHeader(list, nonExistentHeader) ? "true" : "false");

    char headerStr[50] = { };
    HttpHeader *header = list->headers[0];
    printf("key=%s\n", header->key);
    printf("value=%s\n", header->value);

    printHttpHeaderList(list);

    size_t index = 10;
    printf("\nkey=%s, value=%s\n",
           list->headers[index]->key, list->headers[index]->value);

    size_t half = list->size / 2;
    puts("\n\nRemoving");
    size_t size = list->size;
    for (size_t i = 0; i < size; i++) {
        char key[10];
        sprintf(key, "Key %d", (int)i);
        printf("Removing %s\n", key);

        httpHeaderListRemove(list, key);
    }
    printHttpHeaderList(list);

    for (size_t i = 0; i < 22; i++) {
        char key[15];
        char value[15];
        sprintf(key, "Key %d", (int)i);
        sprintf(value, "Value %d", (int)i);

        if (!httpHeaderListAdd(list, key, value)) {
            printf("failed to add at index %zu\n", i);
            break;
        }

        printf("key=%s, value=%s\n", key, value);
        printf("Stored: key=%s, value%s\n\n", 
                list->headers[i]->key, list->headers[i]->value);
    }
}

void printHttpHeaderList(HttpHeaderList *list) {
    printf("%s", "[");
    for (size_t i = 0; i < list->size; i++) {
        // printf("\nPrinting index %zu\n", i);
        
        // puts("Getting header");
        HttpHeader *header = list->headers[i];

        // puts("Printing header");
        printf("Header(key=%s, value=%s)%s",
               header->key, header->value,
               i == list->size - 1 ? "" : ",\n");
    }
    printf("%s", "]\n");
}
