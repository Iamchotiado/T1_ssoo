#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <sys/wait.h>
#include "../file_manager/manager.h"

int pid_semaforo;
int repartidores_creados = 0;
int num_repartidores;
int tiempo_repartidores;
int status;
char* distancia_s1;
char* distancia_s2;
char* distancia_s3;
char* distancia_b;
// cambiar el 10 por numero muy alto para asegurar
int pid_repartidores[10];
int crear = 0;

void avisar_repartidor(int sig, siginfo_t *siginfo, void *ucontext){
  // printf("LLLLLLLLLLLLLLLLLLLLLLLLL\n");
  int semaforo = siginfo -> si_value.sival_int;
  for (int i = 0; i < repartidores_creados; i++)
  { 
    // if de bajo era para ver si los pid guardados en el array eran los correctos
    // if (repartidores_creados == 4)
    // {
    //   printf("PID REPARTIDOR %i: %i\n", i, pid_repartidores[i]);
    // }
    
    send_signal_with_int(pid_repartidores[i], semaforo);
  }
};

void crear_repartidor(){
  crear = 1;
}


int main(int argc, char const *argv[])
{
  printf("I'm the DCCUBER process and my PID is: %i\n", getpid());

  char *filename = "input.txt";
  InputFile *data_in = read_file(filename); // Lee

  printf("Leyendo el archivo %s...\n", filename);
  printf("- Lineas en archivo: %i\n", data_in->len); // Se accede al largo
  printf("- Contenido del archivo:\n");

  printf("\t- ");
  for (int i = 0; i < 4; i++)
  {
    printf("%s, ", data_in->lines[0][i]); // Se accede al contenido de las líneas
  }
  printf("\n");

  printf("\t- ");
  for (int i = 0; i < 5; i++)
  {
    printf("%s, ", data_in->lines[1][i]);
  }
  printf("\n");


  // guardamos en un arreglo los tiempos entre cambios de luces en un semaforo
  char** tiempos = malloc(3 * sizeof(char*));
  char* t_semaforo1 = data_in->lines[1][2 + 0];
  char* t_semaforo2 = data_in->lines[1][2 + 1];
  char* t_semaforo3 = data_in->lines[1][2 + 2];
  tiempos[0] = t_semaforo1;
  tiempos[1] = t_semaforo2;
  tiempos[2] = t_semaforo3;
  
  // guardamos el tiempo de creacion entre cada repartidor
  // y la cantidad de envios a realizar o repartidores por crear

  char* tiempo_creacion = data_in->lines[1][0];
  char* num_repartidores_str = data_in->lines[1][1];

  tiempo_repartidores = atoi(tiempo_creacion);
  num_repartidores = atoi(num_repartidores_str);

  // array con los pid de los repartidores
  // int pid_repartidores[num_repartidores];
  
  // distancias a semaforos y bodegas
  distancia_s1= data_in->lines[0][0];
  distancia_s2 = data_in->lines[0][1];
  distancia_s3 = data_in->lines[0][2];
  distancia_b = data_in->lines[0][3];

  
  // CREAR HIJOS
  // Crear proceso Fábrica y 3 semáforos

  int fabrica_id = fork();
  if (fabrica_id == 0)
  {
    // printf("Se crea fabrica: %i \n\n", fabrica_id);
    // Creamos a los repartidores
    signal(SIGALRM, crear_repartidor);
    alarm(tiempo_repartidores); 
    connect_sigaction(SIGUSR1, avisar_repartidor);
    while (1)
    {
      
      if (crear == 1)
      {
        crear = 0;
        int repartidor_id = fork();
        repartidores_creados ++;

        if (repartidor_id == 0)
        {
          char repartidores_creados_str[(int)((ceil(log10(repartidores_creados))+1)*sizeof(char))];
          sprintf(repartidores_creados_str, "%i", repartidores_creados);
          execlp("./repartidor", distancia_s1, distancia_s2, distancia_s3, distancia_b, repartidores_creados_str, NULL);
        }
        pid_repartidores[repartidores_creados - 1] = repartidor_id;
        printf("REPARTIDOR CREADO ID: %i\n", repartidor_id);
        if (repartidores_creados < num_repartidores)
        {
          alarm(tiempo_repartidores);
        }
      }
    

    }
      
  }
    
  else if (fabrica_id > 0)
  {

    for (int i = 0; i < 3; i++)
    {
      pid_semaforo = fork();
      if (pid_semaforo == 0) {
        
        int num_semaforo = i + 1;
        char num_semaforo_str[2];
        sprintf(num_semaforo_str, "%i", num_semaforo);
        char fabrica_id_str[(int)((ceil(log10(fabrica_id))+1)*sizeof(char))];
        sprintf(fabrica_id_str, "%i", fabrica_id);
        execlp("./semaforo", num_semaforo_str, tiempos[i], fabrica_id_str, NULL);
        printf("CHILD: Exec done\n");
      }
    
    }
    
    

  
  // Espero hasta que fábrica termine para destruir semaforos
  wait(NULL);

  // DESCOMENTAR CUANDO SE HAGA WAIT
  // printf("Liberando memoria...\n");
  //input_file_destroy(data_in); 
  }

  // Esperar fábrica
  // Destruir semaforos
}

