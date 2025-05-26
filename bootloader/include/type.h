#ifndef TYPE_H
#define TYPE_H

#if defined(__aarch64__)  /* AArch64 64-bit  */
  typedef unsigned long uintptr_t;
#elif defined(__arm__)    /* 32-bit ARM  */
  typedef unsigned int  uintptr_t;
#else
  #error "Unknown pointer width"
#endif

typedef unsigned int uint32_t;
typedef unsigned char uint8_t;

#endif