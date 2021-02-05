#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h> 

typedef struct _Buffer{
    size_t size;
    char *data;
}Buffer;

char *http_get_data(const char *url, size_t *data_size);
