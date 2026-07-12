#include "some.h"

Some some(void *value, bool nil) {
  Some s = {value, nil};
  return s;
}
