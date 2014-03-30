#ifndef ASSETS_H
#define ASSETS_H

#define ASSET(sym) \
  extern char _binary_##sym##_start; \
  extern char _binary_##sym##_end; \
  extern char _binary_##sym##_size

ASSET(assets_fun_txt);
ASSET(pokemon_pal_bin);
ASSET(pokemon_img_bin);

#endif