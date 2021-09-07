#include <stdio.h>
#include <unistd.h>
#include<signal.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <stdlib.h>

int cantidad_cambios = 0;
int num_semaforo;

void escribir_resultados(){
  printf("Resultados semaforo creados\n");

  FILE *fp;
  char nombre_archivo[27];
  sprintf(nombre_archivo, "semaforo_%i.txt", num_semaforo);
  fp = fopen( nombre_archivo, "w");
  fprintf(fp, "%i", cantidad_cambios);
  fclose(fp);

  exit(getpid());
}

int main(int argc, char const *argv[])
{
  // argc me da el numero de variables. podi hacer un if
  int tiempo = atoi(argv[1]);
  num_semaforo = atoi(argv[0]);
  int fabrica_pid = atoi(argv[2]);
  printf("Semaforo numero: %i\n", num_semaforo);
  // printf("Tiempo en cambiar de color: %i\n", tiempo);
  // printf("PID Fabrica: %s\n", argv[2]);
  // printf("I'm the SEMAFORO process and my PID is: %i\n\n", getpid());

  // Verde = 1 Rojo = 0
  // int estado = 1;
  // avisar a fabrica estado de semaforo
  signal(SIGABRT, escribir_resultados);
  while (true)
  {
    sleep(tiempo);
    // if (estado == 1)
    // {
    //   estado = 0;
    //   printf("Estado semaforo %i:  Rojo\n", num_semaforo);
    //   // send_signal_with_int(fabrica_pid, num_semaforo);
    // }
    // else if (estado == 0)
    // {
    //   estado = 1;
    //   printf("Estado semaforo %i:  Verde\n", num_semaforo);
    //   // send_signal_with_int(fabrica_pid, num_semaforo);
    // }
    cantidad_cambios ++;
    send_signal_with_int(fabrica_pid, num_semaforo);
  }
  
  
}
