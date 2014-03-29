#ifndef ASSETS_H
#define ASSETS_H

#define ASSET(sym) \
  extern const void* _binary_##sym##_start; \
  extern const void* _binary_##sym##_end; \
  extern const void _binary_##sym##_size

ASSET(assets_fun_txt);

#endif