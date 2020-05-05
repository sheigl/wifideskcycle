
#include "headercollection.h"
#ifndef ARD_H
#define ARD_H
#include <Arduino.h>
#endif

int HeaderCollection::length() {
  Serial.println("Entering length()");
  return this->sizeOfArray;
};

HeaderCollection::HeaderCollection() {
  Serial.println("Creating HeaderCollection()");

  int size = sizeof(KeyValuePair *) * 20;

  Serial.print("Allocating ");
  Serial.print(size);
  Serial.println(" bytes...");

  this->headers = (KeyValuePair **)malloc(size);
  memset(this->headers, 0, size);
};

HeaderCollection::~HeaderCollection() {
  Serial.println("Deconstructing HeaderCollection()");
  
  if (this->headers)
  {
    free(this->headers);
  } 
}

KeyValuePair *HeaderCollection::getHeader(int index) {
  Serial.println("Entering getHeader()");
  return headers[index];
};

void HeaderCollection::addHeader(char *headerName, char *headerValue) {
  Serial.printf("Entering addHeader(%s, %s)\r\n", headerName, headerValue);
  Serial.println("Adding existing headers to new collection...");

  this->sizeOfArray++;
  
  Serial.println("Adding new header to end of array...");
  KeyValuePair *header = (KeyValuePair *)malloc(sizeof(KeyValuePair) * 1);
  header->setKey(headerName);
  header->setValue(headerValue);
  
  this->headers[this->sizeOfArray - 1] = header;

  Serial.print("Size of headers is now ");
  Serial.println(this->sizeOfArray);
};