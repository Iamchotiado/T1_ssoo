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
int pid_repartidores[200];
int pid_semaforos[3];
int fabrica_id_rep;
char fabrica_id_rep_str[20];
int termino_fabrica = 0;
int crear = 0;
int estado_s1 = 1;
int estado_s2 = 1;
int estado_s3 = 1;
InputFile *data_in;
char** tiempos;

void handler_fabrica_sigabrt(){
  for (int i = 0; i < repartidores_creados; i++)
  {
    printf("MATANDO A REPARTIDOR: %i\n", pid_repartidores[i]);
    kill(pid_repartidores[i], SIGABRT);
    waitpid(pid_repartidores[i], NULL, 0);
  }
  exit(getpid());
};

void handler_main_sigint(){
  printf("Mataremos a la fabrica, repartidores y los semaforos\n");
  kill(fabrica_id_rep, SIGABRT);
  waitpid(fabrica_id_rep, NULL, 0);
  for (int i = 0; i < 3; i++)
  {
    printf("MATANDO SEMAFORO %i\n", i);
    // matamos a los semaforos
    kill(pid_semaforos[i], SIGABRT);
    waitpid(pid_semaforos[i], NULL, 0);
  }
  input_file_destroy(data_in);
  free(tiempos);
  exit(getpid());
};

void avisar_repartidor(int sig, siginfo_t *siginfo, void *ucontext){
  int semaforo = siginfo -> si_value.sival_int;
  // cambiamos el estado de los semaforos para los repartidores que van a ser creados
  // Si el ultimo repartidor llego
  // printf("EL NUMERO DE SEMAFORO ES: %i\n", semaforo);
  if (semaforo == 4)
  {
    printf("Nunca entra aca\n");
  }
  else
  {
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

    for (int i = 0; i < repartidores_creados; i++)
    { 
      // if de bajo era para ver si los pid guardados en el array eran los correctos
      // if (repartidores_creados == 4)
      // {
      //   printf("PID REPARTIDOR %i: %i\n", i, pid_repartidores[i]);
      // }
      
      send_signal_with_int(pid_repartidores[i], semaforo);
    }
  }
};
  
  

void crear_repartidor(){
  crear = 1;
};


int main(int argc, char const *argv[])
{
  printf("I'm the DCCUBER process and my PID is: %i\n", getpid());

  char *filename = argv[1];
  data_in = read_file(filename); // Lee

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
  tiempos = malloc(3 * sizeof(char*));
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

  // signal sigint
  signal(SIGINT, handler_main_sigint);
  // CREAR HIJOS
  // Crear proceso Fábrica y 3 semáforos
  char main_id_str[(int)((ceil(log10(getpid()))+1)*sizeof(char))];
  sprintf(main_id_str, "%i", getpid());

  int fabrica_id = fork();
  if (fabrica_id == 0)
  {
    // printf("Se crea fabrica: %i \n\n", fabrica_id);
    // Creamos a los repartidores
    signal(SIGABRT, handler_fabrica_sigabrt);
    signal(SIGALRM, crear_repartidor);
    alarm(tiempo_repartidores); 
    // alarm(1);
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
          // repartidores creados hasta el minuto
          char repartidores_creados_str[(int)((ceil(log10(repartidores_creados))+1)*sizeof(char))];
          sprintf(repartidores_creados_str, "%i", repartidores_creados);
          // repartidores que hay que crear
          // char num_repartidores_str[(int)((ceil(log10(num_repartidores))+1)*sizeof(char))];
          // sprintf(num_repartidores_str, "%i", num_repartidores);
          // id de la fabrica
          // char fabrica_id_rep_str[(int)((ceil(log10(fabrica_id_rep))+1)*sizeof(char))];
          // sprintf(fabrica_id_rep_str, "%i", fabrica_id_rep);

          // Le mandamos ademas el estado actual de los semaforos
          if (estado_s1 == 1 && estado_s2 == 1 && estado_s3 == 1)
          {
            char states[3] = "111";
            execlp("./repartidor", distancia_s1, distancia_s2, distancia_s3, distancia_b, repartidores_creados_str, states, num_repartidores_str, main_id_str, NULL);
          }
          else if (estado_s1 == 1 && estado_s2 == 1 && estado_s3 == 0)
          {
            char states[3] = "110";
            execlp("./repartidor", distancia_s1, distancia_s2, distancia_s3, distancia_b, repartidores_creados_str, states, num_repartidores_str, main_id_str, NULL);
          }
          else if (estado_s1 == 1 && estado_s2 == 0 && estado_s3 == 1)
          {
            char states[3] = "101";
            execlp("./repartidor", distancia_s1, distancia_s2, distancia_s3, distancia_b, repartidores_creados_str, states, num_repartidores_str, main_id_str, NULL);
          }
          else if (estado_s1 == 1 && estado_s2 == 0 && estado_s3 == 0)
          {
            char states[3] = "100";
            execlp("./repartidor", distancia_s1, distancia_s2, distancia_s3, distancia_b, repartidores_creados_str, states, num_repartidores_str, main_id_str, NULL);
          }
          else if (estado_s1 == 0 && estado_s2 == 1 && estado_s3 == 0)
          {
            char states[3] = "010";
            execlp("./repartidor", distancia_s1, distancia_s2, distancia_s3, distancia_b, repartidores_creados_str, states, num_repartidores_str, main_id_str, NULL);
          }
          else if (estado_s1 == 0 && estado_s2 == 0 && estado_s3 == 1)
          {
            char states[3] = "001";
            execlp("./repartidor", distancia_s1, distancia_s2, distancia_s3, distancia_b, repartidores_creados_str, states, num_repartidores_str, main_id_str, NULL);
          }
          else if (estado_s1 == 0 && estado_s2 == 0 && estado_s3 == 0)
          {
            char states[3] = "000";
            execlp("./repartidor", distancia_s1, distancia_s2, distancia_s3, distancia_b, repartidores_creados_str, states, num_repartidores_str, main_id_str, NULL);
          }
          else if (estado_s1 == 0 && estado_s2 == 1 && estado_s3 == 1)
          {
            char states[3] = "011";
            execlp("./repartidor", distancia_s1, distancia_s2, distancia_s3, distancia_b, repartidores_creados_str, states, num_repartidores_str, main_id_str, NULL);
          }
          

          // execlp("./repartidor", distancia_s1, distancia_s2, distancia_s3, distancia_b, repartidores_creados_str, NULL);
        }
        pid_repartidores[repartidores_creados - 1] = repartidor_id;
        printf("REPARTIDOR CREADO ID: %i\n", repartidor_id);
        if (repartidores_creados < num_repartidores)
        {
          alarm(tiempo_repartidores);
          // alarm(1);
        }
      }
    

    }
      
  }
    
  else if (fabrica_id > 0)
  {
    char fabrica_id_str[(int)((ceil(log10(fabrica_id))+1)*sizeof(char))];
    sprintf(fabrica_id_str, "%i", fabrica_id);
    // id de fabrica que sera enviado al repartidor
    fabrica_id_rep = fabrica_id;
    sprintf(fabrica_id_rep_str, "%i", fabrica_id_rep);

    for (int i = 0; i < 3; i++)
    {
      pid_semaforo = fork();
      if (pid_semaforo == 0) {
        int num_semaforo = i + 1;
        char num_semaforo_str[2];
        sprintf(num_semaforo_str, "%i", num_semaforo);
        execlp("./semaforo", num_semaforo_str, tiempos[i], fabrica_id_str, NULL);
        printf("CHILD: Exec done\n");
      }
      pid_semaforos[i] = pid_semaforo;
    }
    
    

  
  // Espero hasta que fábrica termine para destruir semaforos
  
  
  wait(NULL);
  // free(tiempos);
  // DESCOMENTAR CUANDO SE HAGA WAIT
  // printf("Liberando memoria...\n");
  // input_file_destroy(data_in); 
  }

  // Esperar fábrica
  // Destruir semaforos
}

