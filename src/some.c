#include "some.h"
#include <stddef.h>

inline Some some(void *value) {
  return (Some){value, value == NULL ? true : false};
}
