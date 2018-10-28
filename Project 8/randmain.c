#include <stdio.h>
#include <stdlib.h>
#include "randcpuid.h"
#include <stdbool.h>
#include <errno.h>
#include <dlfcn.h>

static bool
writebytes (unsigned long long x, int nbytes)
{
  int ndigits = nbytes * 2;
  do
    {
      if (putchar ("0123456789abcdef"[x & 0xf]) < 0)
	return false;
      x >>= 4;
      ndigits--;
    }
  while (0 < ndigits);

  return 0 <= putchar ('\n');
}

int main(int argc, char **argv){

   /* Check arguments.  */
  bool valid = false;
  long long nbytes;
  if (argc == 2)
    {
      char *endptr;
      errno = 0;
      nbytes = strtoll (argv[1], &endptr, 10);
      if (errno)
	perror (argv[1]);
      else
	valid = !*endptr && 0 <= nbytes;
    }
  if (!valid)
    {
      fprintf (stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
      return 1;
    }

  /* If there's no work to do, don't worry about which library to use.  */
  if (nbytes == 0)
    return 0;

  /* Now that we know we have work to do, arrange to use the
     appropriate library.  */
  
  // void (*initialize) (void);
  unsigned long long (*rand64) (void);
  // void (*finalize) (void);
  //create ptr to library so we can load the corresponding library
  void* dl_handle;
  
  if (rdrand_supported ())
    {
      dl_handle= dlopen("./randlibhw.so", RTLD_LAZY);
      if(!dl_handle){
	printf("dlopen() error - failed to open randlibhw.so:  %s\n", dlerror());
	return 1;
      }
      rand64 = dlsym(dl_handle, "hardware_rand64");
      if(dlerror()){
	printf("could not locate hardware_rand64: %s\n",dlerror());
	return 1;
      }
    }
  else //if linking randlibhw doesnt work, use randlibsw
    {
      dl_handle = dlopen("./randlibsw.so", RTLD_LAZY);
      if(dl_handle == NULL)
	{
	  printf("failed to open randlibsw.so: %s\n", dlerror());
	}
      rand64 = dlsym(dl_handle, "software_rand64");
      if(dlerror()){
	printf("could not locate software_rand64: %s\n", dlerror());
	return 1;
      }
    }

  int wordsize = sizeof rand64 ();
  int output_errno = 0;

  do
    {
      unsigned long long x = rand64 ();
      int outbytes = nbytes < wordsize ? nbytes : wordsize;
      if (!writebytes (x, outbytes))
	{
	  output_errno = errno;
	  break;
	}
      nbytes -= outbytes;
    }
  while (0 < nbytes);

  if (fclose (stdout) != 0)
    output_errno = errno;

  if (output_errno)
    {
      errno = output_errno;
      perror ("output");
      //finalize ();
      return 1;
    }

  dlclose(dl_handle);
  return 0;
}

