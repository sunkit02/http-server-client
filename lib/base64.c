#include "base64.h"
 
static char encodingTable[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};
static char *decodingTable = NULL;
static int modTable[] = {0, 2, 1};
 
void buildDecodingTable() {
 
    decodingTable = malloc(256);
 
    for (int i = 0; i < 64; i++)
        decodingTable[(unsigned char) encodingTable[i]] = i;
}
 
 
void base64_cleanup() {
    free(decodingTable);
} 
 
char *base64_encode(void *data, size_t inputLength, size_t *outputLength) {
 
    
    unsigned char *inputData = data;
    *outputLength = 4 * ((inputLength + 2) / 3);
 
    // Additional byte for null terminating character
    char *encodedData = malloc(*outputLength + 1);
    if (encodedData == NULL) return NULL;
 
    for (int i = 0, j = 0; i < inputLength;) {
 
        uint32_t octet_a = i < inputLength ? (unsigned char)inputData[i++] : 0;
        uint32_t octet_b = i < inputLength ? (unsigned char)inputData[i++] : 0;
        uint32_t octet_c = i < inputLength ? (unsigned char)inputData[i++] : 0;
 
        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;
 
        encodedData[j++] = encodingTable[(triple >> 3 * 6) & 0x3F];
        encodedData[j++] = encodingTable[(triple >> 2 * 6) & 0x3F];
        encodedData[j++] = encodingTable[(triple >> 1 * 6) & 0x3F];
        encodedData[j++] = encodingTable[(triple >> 0 * 6) & 0x3F];
    }
 
    for (int i = 0; i < modTable[inputLength % 3]; i++)
        encodedData[*outputLength - 1 - i] = '=';
 
    encodedData[*outputLength] = '\0';

    return encodedData;
}
 
 
void *base64_decode(const char *data, size_t inputLength, size_t *outputLength) {
 
    if (decodingTable == NULL) buildDecodingTable();
 
    if (inputLength % 4 != 0) return NULL;
 
    *outputLength = inputLength / 4 * 3;
    if (data[inputLength - 1] == '=') (*outputLength)--;
    if (data[inputLength - 2] == '=') (*outputLength)--;
 
    unsigned char *decodedData = malloc(*outputLength);
    if (decodedData == NULL) return NULL;
 
    for (int i = 0, j = 0; i < inputLength;) {
 
        uint32_t sextet_a = data[i] == '=' ? 0 & i++ : decodingTable[data[i++]];
        uint32_t sextet_b = data[i] == '=' ? 0 & i++ : decodingTable[data[i++]];
        uint32_t sextet_c = data[i] == '=' ? 0 & i++ : decodingTable[data[i++]];
        uint32_t sextet_d = data[i] == '=' ? 0 & i++ : decodingTable[data[i++]];
 
        uint32_t triple = (sextet_a << 3 * 6)
        + (sextet_b << 2 * 6)
        + (sextet_c << 1 * 6)
        + (sextet_d << 0 * 6);
 
        if (j < *outputLength) decodedData[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < *outputLength) decodedData[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < *outputLength) decodedData[j++] = (triple >> 0 * 8) & 0xFF;
    }
 
    return (void *)decodedData;
}
