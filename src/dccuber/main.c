#include <stdio.h>
#include <unistd.h>

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

  // CREAR HIJOS
  // Crear proceso Fábrica
  
  int fabrica_id = fork();
  
  if (fabrica_id == 0)
  {
    // Creamos a los repartidores
    
    printf("Se crea fabrica: %i \n", fabrica_id);
  }
  else if (fabrica_id > 0)
  {
    for (int i = 0; i < 3; i++)
    {
      pid_semaforo = fork();
      if (pid_semaforo == 0) {
        // printf("CHILD: El pid del semaforo %i\n", pid_semaforo);
        int num_semaforo = i + 1;
        printf("%s, ", data_in->lines[1][i]);
        execlp("./semaforo", num_semaforo, data_in->lines[1][2 + i], NULL);
        printf("CHILD: Exec done\n");
      }
    }

  printf("Liberando memoria...\n");
  input_file_destroy(data_in); // Libera memoria
  }
  
  // Esperar fábrica
  // Destruir semaforos
}

