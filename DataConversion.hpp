#ifndef __H_DATA_CONVERSION__
#define __H_DATA_CONVERSION__

static unsigned int le_bytes_to_u32(unsigned char c1, unsigned char c2, unsigned char c3, unsigned char c4)
{
  return (c4 << 24) + (c3 << 16) + (c2 << 8) + (c1 << 0);
}

static unsigned short le_bytes_to_u16(unsigned char c1, unsigned char c2)
{
  return ((c2 << 8) | c1);
}

#endif
