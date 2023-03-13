#ifndef URLPARSER_H
#define URLPARSER_H

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "server.h"

int strfchar(char *str, char c);
const char *findBodyStart(const char *const rawRequest);
HttpRequest parseRequest(const char *const rawRequest);

#endif // !DEBUG
