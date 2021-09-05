#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdbool.h>

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
  int turnos_sem1;
  int turnos_sem2;
  int turnos_sem3;
  int turnos_bodega;
  while (llego == false)
  {
    sleep(1);
    // preguntar si estoy en un semaforo y si este esta verde
    turnos++;
    if (posicion == dis_semaforo1)
    {
      // si esta en rojo no hago nada
      // si esta en verde aumento la posicion
      turnos_sem1 = turnos;
    }
    if (posicion == dis_semaforo2)
    {
      /* code */
      turnos_sem2 = turnos;
    }
    if (posicion == dis_semaforo3)
    {
      /* code */
      turnos_sem3 = turnos;
    }
    
    

    printf("posicion repartidor %i: %i\n", numero_repartidor, turnos);
    if (turnos == dis_bodega)
    {
      // guardar estadisticas en archivo
      turnos_bodega = turnos;
      llego = true;
    }
    
  }

}
