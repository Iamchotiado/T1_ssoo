#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdbool.h>


int turnos_sem1 = -1;
int turnos_sem2 = -1;
int turnos_sem3 = -1;
int turnos_bodega = -1;
int numero_repartidor;

int estado_s1;
int estado_s2;
int estado_s3;
int escrito = 0;

void handler_repartidor_sigabrt() {
  printf("Escribimos resultados repartidores\n");
  if (escrito == 0)
  {
    
    FILE *fp;
    char nombre_archivo[27];
    sprintf(nombre_archivo, "repartidor_%i.txt", numero_repartidor);
    fp = fopen( nombre_archivo, "w");
    fprintf(fp, "%i,%i,%i,%i\n", turnos_sem1, turnos_sem2, turnos_sem3, turnos_bodega);
    fclose(fp);
    escrito = 1;
  }
  exit(getpid());
};

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
  numero_repartidor = atoi(argv[4]);
  signal(SIGABRT, handler_repartidor_sigabrt);
  estado_s1 = argv[5][0] - '0';
  estado_s2 = argv[5][1] - '0';
  estado_s3 = argv[5][2] - '0';
  printf("\nESTADO SEMAFORO 1 AL CREAR: %i\n", estado_s1);
  printf("\nESTADO SEMAFORO 2 AL CREAR: %i\n", estado_s2);
  printf("\nESTADO SEMAFORO 3 AL CREAR: %i\n", estado_s3);
  int repartidores_por_crear = atoi(argv[6]);
  int pid_fabrica = atoi(argv[7]);
  printf("Numero repartidor: %i, Por crear: %i\n", numero_repartidor, repartidores_por_crear);

  int llego = false;
  int turnos = 0;
  int posicion = 0;
  connect_sigaction(SIGUSR1, chequear_semaforo);
  

  while (llego == false)
  {
    sleep(1);
    // preguntar si estoy en un semaforo y si este esta verde
    // turnos++;
    
    if (posicion == dis_semaforo1 - 1)
    {
      
      if (estado_s1 == 0)
      {
        turnos++;
      }
      if (estado_s1 == 1)
      {
        posicion++;
        turnos++;
        turnos_sem1 = turnos;
      }
    }
    else if (posicion == dis_semaforo2 - 1)
    {
      
      if (estado_s2 == 0)
      {
        turnos++;
      }
      if (estado_s2 == 1)
      {
        posicion++;
        turnos++;
        turnos_sem2 = turnos;
      }
    }
    else if (posicion == dis_semaforo3 - 1)
    {
      
      if (estado_s3 == 0)
      {
        turnos++;
      }
      if (estado_s3 == 1)
      {
        posicion++;
        turnos++;
        turnos_sem3 = turnos;
      }
    }
    
    
    else if (posicion == dis_bodega - 1)
    {
      posicion++;
      turnos++;
      // guardar estadisticas en archivo
      turnos_bodega = turnos;
      llego = true;
      printf("Posicion repartidor %i(en bodega): %i,    Turno: %i\n", numero_repartidor, posicion, turnos);
      break;
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
  escrito = 1;

  if (numero_repartidor == repartidores_por_crear)
  {
    // avisamos a la fabrica que llego a la bodega el ultimo repartidor
    printf("Termino ultimo repartidor\n");
    // kill(SIGUSR2, getppid());
    kill(pid_fabrica, SIGINT);
  };
  while (1)
  {
    /* code */
  };
  
  // exit(getpid());
  // printf("Termino repartidor con ID: %i\n", getpid());
}
