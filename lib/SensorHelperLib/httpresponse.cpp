#include "httpresponse.h"
#ifndef ARD_H
#define ARD_H
#include <Arduino.h>
#endif
HttpResponse::HttpResponse() {
  Serial.println("Creating HttpResponse()");

  int size = sizeof(char) * 512;

  Serial.print("Allocating ");
  Serial.print(size);
  Serial.println(" bytes...");

  this->body = (char *)malloc(size);
  memset(this->body, 0, size);

  this->headers = (HeaderCollection *)malloc(sizeof(HeaderCollection) * 1);
  memset(this->headers, 0, sizeof(HeaderCollection) * 1);

  this->request = (HttpRequest *)malloc(sizeof(HttpRequest) * 1);
  memset(this->request, 0, sizeof(HttpRequest) * 1);
}

HttpResponse::~HttpResponse() {
  Serial.println("Deconstructing HttpResponse()");

  if (this->body)
  {
    free(this->body);
  }

  if (this->headers)
  {
    free(this->headers);
  }

  if (this->request)
  {
    free(this->request);
  }
}