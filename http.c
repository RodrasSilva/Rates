
#include "./http.h"

static size_t write_to_memory(char *data, size_t size, size_t nmemb, void *context)
{
    size_t byteCount = size * nmemb;
    Buffer *bufptr = (Buffer *)context;

    /* grow the buffer in order to hold the new bytes */
    bufptr->data = realloc(bufptr->data, bufptr->size + byteCount + 1);
    if (bufptr->data == NULL) {
        /* out of memory! */
        fprintf(stderr, "***error: not enough memory\n");
        return 0;
    }
    /* copy the new data block */
    memcpy(bufptr->data + bufptr->size, data, byteCount);

    /* update size and close the C string */
    bufptr->size += byteCount;
    bufptr->data[bufptr->size] = '\0';
    return byteCount;
}

char *http_get_data(const char *url, size_t *data_size)
{
    static int curl_global_init_called = 0;
    CURL *curl_handle;
    CURLcode curl_result;
    Buffer buffer = { .size=0, .data = NULL };
    char *result = NULL;

    if(!curl_global_init_called){
        curl_global_init(CURL_GLOBAL_ALL);
        atexit(curl_global_cleanup);
        curl_global_init_called=1;
    }

    curl_handle= curl_easy_init();
    if(!curl_handle){
        fprintf(stderr,"***error: curl_easy_init() failed \n");
        return result;
    }

    curl_easy_setopt(curl_handle,CURLOPT_URL,url);
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT,"libcurl-agent/1.0");
    curl_easy_setopt(curl_handle,CURLOPT_FOLLOWLOCATION,1L); 
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION,write_to_memory);  
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &buffer);
    curl_result = curl_easy_perform(curl_handle);
    if (curl_result != CURLE_OK) {
        fprintf(stderr, "***error: curl_easy_perform() failed: %s\n",
                curl_easy_strerror(curl_result));
		goto cleanup_handle;
    }

    result = buffer.data;
    *data_size = buffer.size;

 cleanup_handle:
    curl_easy_cleanup(curl_handle);
	return result;
}