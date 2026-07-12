#pragma once

typedef struct {
    void* ptr;
    bool nil;
} Some;

inline Some some(void* value) {
    Some s = {value, false};
    return s;
}