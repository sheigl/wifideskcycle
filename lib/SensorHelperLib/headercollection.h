#include "keyvaluepair.h"

class HeaderCollection {
  public:    
    void addHeader(char *headerName, char *headerValue);
    int length();
    KeyValuePair *getHeader(int index);
    HeaderCollection();
    ~HeaderCollection();
  private:
    KeyValuePair **headers;
    int sizeOfArray = 0;
};