#ifndef WAD_H_
#define WAD_H_

#include "miptex.h"

#define MAX_TEX_WIDTH 512
#define MAX_TEX_HEIGHT 512
#define MAX_TEX_SIZE (MAX_TEX_WIDTH * MAX_TEX_HEIGHT)
#define MAX_MIP_SIZE (MAX_TEX_SIZE+(MAX_TEX_SIZE/4)+(MAX_TEX_SIZE/16)+(MAX_TEX_SIZE/64))
#define MAX_LUMP_SIZE (MAX_MIP_SIZE + sizeof(miptex_t) + 2 + (256*3) + 2)

#define CMP_NONE  0
#define CMP_LZSS  1

#define TYP_NONE  0
#define TYP_LABEL 1
#define TYP_LUMPY 64    // 64 + grab command number

typedef struct
{
   char  identification[4];   // should be WAD2 or 2DAW
   int   numlumps;
   int   infotableofs;
} tWadHeader;

typedef struct
{
   int   filepos;          // file offset of mip
   int   disksize;         // mip size
   int   size;             // uncompressed
   char  type;             // 0x43 = WAD3 mip (Half-Life)
   char  compression;      // not used?
   char  pad1, pad2;       // not used?
   char  name[16];         // must be null terminated
} tWadLump;

#endif /*WAD_H_*/
