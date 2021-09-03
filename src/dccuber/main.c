#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "../file_manager/manager.h"

int pid_semaforo;

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
  

  // CREAR HIJOS
  // Crear proceso Fábrica y 3 semáforos
  
  int fabrica_id = fork();
  if (fabrica_id == 0)
  {
    printf("Se crea fabrica: %i \n\n", fabrica_id);
    // Creamos a los repartidores
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
        execlp("./semaforo", num_semaforo_str, tiempos[i], NULL);
        printf("CHILD: Exec done\n");
      }
    }
  
  // Espero hasta que fábrica termine para destruir semaforos
  // wait(NULL);

  // DESCOMENTAR CUANDO SE HAGA WAIT
  //printf("Liberando memoria...\n");
  //input_file_destroy(data_in); 
  }

  // Esperar fábrica
  // Destruir semaforos
}

