#include "some.h"
#include "null.h"

inline Some some(void *value) {
  return (Some){value, value == NULL ? true : false};
}
