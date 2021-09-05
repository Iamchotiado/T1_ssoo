#include <stdio.h>
#include <unistd.h>
#include<signal.h>
#include <stdbool.h>
#include <wait.h>

void cambiar_estado(int signum){
  printf("llego \n");
}

int main(int argc, char const *argv[])
{
  // argc me da el numero de variables. podi hacer un if
  int tiempo = atoi(argv[1]);
  char* num_semaforo = argv[0];
  int fabrica_pid = atoi(argv[2]);
  printf("Semaforo numero: %s\n", num_semaforo);
  // printf("Tiempo en cambiar de color: %i\n", tiempo);
  // printf("PID Fabrica: %s\n", argv[2]);
  // printf("I'm the SEMAFORO process and my PID is: %i\n\n", getpid());

  // Verde = 1 Rojo = 0
  int estado = 1;
  // avisar a fabrica estado de semaforo

  while (true)
  {
    sleep(tiempo);
    if (estado == 1)
    {
      estado = 0;
      printf("Estado semaforo %s:  Rojo\n", num_semaforo);
      send_signal_with_int(fabrica_pid, estado);
    }
    else if (estado == 0)
    {
      estado = 1;
      printf("Estado semaforo %s:  Verde\n", num_semaforo);
      send_signal_with_int(fabrica_pid, estado);
    }
    // avisar a fabrica el estado del semaforo
  }
  
  
}
