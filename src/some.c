#include "some.h"
#include "null.h"

Some some(void *value) {

  Some s = {value, value == NULL ? true : false};
  return s;
}
