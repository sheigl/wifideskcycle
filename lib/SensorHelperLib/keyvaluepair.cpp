#include "keyvaluepair.h"
#ifndef ARD_H
#define ARD_H
#include <Arduino.h>
#endif

KeyValuePair::KeyValuePair(char *k, char *v) {
  Serial.printf("Creating KeyValuePair(%s, %s)\r\n", k, v);
  this->init();

  this->setKey(k);
  this->setValue(v);
};

KeyValuePair::KeyValuePair() {
  Serial.println("Creating KeyValuePair()");

  this->init();
};

KeyValuePair::~KeyValuePair() {
  Serial.printf("Deconstructing KeyValuePair(%s, %s)\r\n", this->key, this->value);

  if (this->key)
  {
    free(this->key);
  }
  
  if (this->value)
  {
    free(this->value);
  }
};

char *KeyValuePair::getKey() {
  Serial.println("Entering getKey()");

  if (this->key == NULL)
  {
    Serial.println("key is NULL!");
    return NULL;
  }
  
  //Serial.printf("key is %s\r\n", this->key);
  return this->key;
}
char *KeyValuePair::getValue() {
  Serial.println("Entering getValue()");

  if (this->value == NULL)
  {
    Serial.println("value is NULL!");
    return NULL;
  }

  //Serial.printf("value is %s\r\n", this->value);
  return this->value;
}

void KeyValuePair::setKey(char *k) {
  Serial.printf("Entering setKey(%s)\r\n", k);

  this->key = (char *)malloc(sizeof(char) * strlen(k));
  strcpy(this->key, k);
}

void KeyValuePair::setValue(char *v) {
  Serial.printf("Entering setValue(%s)\r\n", v);
  
  this->value = (char *)malloc(sizeof(char) * strlen(v));
  strcpy(this->value, v);
}

void KeyValuePair::init() {
  int size = sizeof(char) * 256;

  Serial.print("Allocating ");
  Serial.print(size);
  Serial.println(" bytes...");

  this->key = (char *)malloc(size);
  this->value = (char *)malloc(size);

  memset(this->key, 0, size);
  memset(this->value, 0, size);
}
