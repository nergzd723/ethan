#include <string.h>
#define UNALIGNED(X, Y) (((long)X & (sizeof (long) - 1)) | ((long)Y & (sizeof (long) - 1)))

/* How many bytes are copied each iteration of the word copy loop.  */
#define LBLOCKSIZE (sizeof (long))

/* Threshhold for punting to the byte copier.  */
#define TOO_SMALL(LEN)  ((LEN) < LBLOCKSIZE)

int memcmp (const void* s1, const void* s2, size_t n)
{
  unsigned char *d1 = (unsigned char *) s1;
  unsigned char *d2 = (unsigned char *) s2;

  while (n--)
    {
      if (*d1 != *d2)
	{
	  return *d1 - *d2;
	}
      d1++;
      d2++;
    }
  return 0;
}