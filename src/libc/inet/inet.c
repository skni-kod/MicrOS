#include "inet.h"

in_addr_t inet_addr(const char* s)
{
  in_addr_t ip = { 0 };

  char* p = strtok((char*)s, ".");

  uint8_t i = 0;
  while (p != NULL && i < 4) {
    *((uint8_t*)&ip + i++) = atoi(p);
    p = strtok(NULL, ".");
  }

  return ip;
}
