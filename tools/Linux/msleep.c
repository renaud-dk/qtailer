#include <unistd.h>
#include <stdlib.h>

int main (int argc, char **argv)
{
  int a = atoi(argv[1]);
  
  usleep(a * 1000);

  return 0;
}

