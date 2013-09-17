#ifndef MIPTEX_H_
#define MIPTEX_H_

#define MIPLEVELS 4
typedef struct miptex_s
{
   char     name[16];
   unsigned width, height;
   unsigned offsets[MIPLEVELS];  // four mip maps stored
} miptex_t;

#endif /*MIPTEX_H_*/
