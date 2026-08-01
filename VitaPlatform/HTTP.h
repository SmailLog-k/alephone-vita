#ifndef HTTP_H
#define HTTP_H

// Minimal HTTP stub for Vita port

#ifdef __cplusplus
extern "C" {
#endif

typedef struct HTTPClient HTTPClient;

void HTTPClient_Init(void);
void HTTPClient_Cleanup(void);

int HTTPClient_Get(const char* url, char** response, int* response_code);
int HTTPClient_Post(const char* url, const char* data, char** response, int* response_code);

#ifdef __cplusplus
}
#endif

#endif