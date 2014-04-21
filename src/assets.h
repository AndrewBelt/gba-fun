#ifndef ASSETS_H
#define ASSETS_H

#define ASSET(sym) \
  extern char _binary_##sym##_start, _binary_##sym##_end, _binary_##sym##_size

#define ASSET_START(sym) ((void *) &_binary_##sym##_size)
#define ASSET_END(sym) ((void *) &_binary_##sym##_size)
#define ASSET_SIZE(sym) ((size_t) &_binary_##sym##_size)

#endif