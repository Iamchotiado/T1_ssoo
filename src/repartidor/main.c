#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdbool.h>


int turnos_sem1;
int turnos_sem2;
int turnos_sem3;
int turnos_bodega;

int estado_s1 = 1;
int estado_s2 = 1;
int estado_s3 = 1;


void chequear_semaforo(int sig, siginfo_t *siginfo, void *ucontext) {
  
  int semaforo = siginfo -> si_value.sival_int;
  if (semaforo == 1)
  {
    if (estado_s1 == 1)
    {
      estado_s1 = 0;
    }
    else if (estado_s1 == 0)
    {
      estado_s1 = 1;
    }
  }

  if (semaforo == 2)
  {
    if (estado_s2 == 1)
    {
      estado_s2 = 0;
    }
    else if (estado_s2 == 0)
    {
      estado_s2 = 1;
    }
  }

  if (semaforo == 3)
  {
    if (estado_s3 == 1)
    {
      estado_s3 = 0;
    }
    else if (estado_s3 == 0)
    {
      estado_s3 = 1;
    }
  }
}
int main(int argc, char const *argv[])
{
  printf("I'm the REPARTIDOR process and my PID is: %i\n", getpid());
  int dis_semaforo1 = atoi(argv[0]);
  int dis_semaforo2 = atoi(argv[1]);
  int dis_semaforo3 = atoi(argv[2]);
  int dis_bodega = atoi(argv[3]);
  int numero_repartidor = atoi(argv[4]);

  int llego = false;
  int turnos = 0;
  int posicion = 0;
  connect_sigaction(SIGUSR1, chequear_semaforo);
  while (llego == false)
  {
    sleep(1);
    // preguntar si estoy en un semaforo y si este esta verde
    // turnos++;
    
    if (posicion == dis_semaforo1)
    {
      
      if (estado_s1 == 0)
      {
        turnos++;
      }
      if (estado_s1 == 1)
      {
        posicion++;
        turnos++;
      }
      turnos_sem1 = turnos;
    }
    else if (posicion == dis_semaforo2)
    {
      
      if (estado_s2 == 0)
      {
        turnos++;
      }
      if (estado_s2 == 1)
      {
        posicion++;
        turnos++;
      }
      turnos_sem2 = turnos;
    }
    else if (posicion == dis_semaforo3)
    {
      connect_sigaction(SIGUSR1, chequear_semaforo);
      if (estado_s3 == 0)
      {
        turnos++;
      }
      if (estado_s3 == 1)
      {
        posicion++;
        turnos++;
      }
      turnos_sem3 = turnos;
    }
    
    
    else if (posicion == dis_bodega)
    {
      // guardar estadisticas en archivo
      turnos_bodega = turnos;
      llego = true;
    }
    else {
      posicion++;
      turnos++;
    }
    printf("Posicion repartidor %i: %i,    Turno: %i\n", numero_repartidor, posicion, turnos);
    
  }
  FILE *fp;
  char nombre_archivo[27];
  sprintf(nombre_archivo, "repartidor_%i.txt", numero_repartidor);
  fp = fopen( nombre_archivo, "w");
  fprintf(fp, "%i,%i,%i,%i\n", turnos_sem1, turnos_sem2, turnos_sem3, turnos_bodega);
  fclose(fp);
  
  exit(0);
  printf("Termino repartidor con ID: %i\n", getpid());
}
