#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
  // argc me da el numero de variables. podi hacer un if
  int tiempo = atoi(argv[1]);
  printf("Semaforo numero: %s\n", argv[0]);
  printf("Tiempo en cambiar de color: %i\n", tiempo);
  printf("PID Fabrica: %s\n", argv[2]);
  printf("I'm the SEMAFORO process and my PID is: %i\n\n", getpid());
}
