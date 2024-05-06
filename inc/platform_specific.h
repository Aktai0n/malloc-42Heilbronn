#pragma once

#if defined(__GNUC__) || defined(__clang__)

#define REGISTER_CONSTRUCTOR(func) \
    static void func(void) __attribute__ ((constructor))
#define REGISTER_DESTRUCTOR(func) \
    static void func(void) __attribute__ ((destructor))

#else
#error "Unsupported platform. Please use GCC or Clang!"
#endif