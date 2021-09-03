#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
  // argc me da el numero de variables. podi hacer un if
  // printf(argv[0]);
  printf("I'm the SEMAFORO process and my PID is: %i\n", getpid());
}
