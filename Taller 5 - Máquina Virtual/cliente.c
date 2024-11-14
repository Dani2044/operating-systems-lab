/************************************************************************************************
Fecha: 24/10/2024
Autor: Daniel Castro
Materia: Sistemas Operativos
Tema: Taller 5 - Máquina Virtual

Descripción:
        Este programa permite la comunicación bidireccional de un cliente a un servidor usando 
        NamePipes y un archivo FiFO.
************************************************************************************************
        CLIENTE
************************************************************************************************/

#include <stdio.h> // Para entradas y salidas estándar
#include <sys/stat.h> // Para gestionar propiedades de archivos
#include <sys/types.h> // Para definir tipos de datos como pid_t
#include <fcntl.h> // Para controlar archivos
#include <unistd.h> // Para llamadas al sistema como fork()
#include <string.h> // Para manejo de cadenas de caracteres

#define FIFO_FILE "/tmp/fifo_twoway" // Defición del nombre del archivo FIFO

int main() {
   int fd; // Descriptor para el archivo FIFO
   int end_process; // Indicador para terminar el proceso
   int stringlen; // Longitud de la cadena de entrada
   int read_bytes; // Lector de bytes
   char readbuf[80]; // Buffer de 80 caracteres para lectura de la entrada
   char end_str[5]; // Cadena "end" para finalizar
   printf("FIFO_CLIENT: Send messages, infinitely, to end enter \"end\"\n"); // Muestra mensaje inicial
   fd = open(FIFO_FILE, O_CREAT|O_RDWR); // Abre el archivo FIFO en modo escritura y lectura
   strcpy(end_str, "end"); // Inicializa la cadena de terminación
   
   while (1) { // Ciclo infinito para enviar mensajes
      printf("Enter string: "); // Solicita entrada
      fgets(readbuf, sizeof(readbuf), stdin); // Lee la entrada del usuario 
      stringlen = strlen(readbuf); // Calcula la longitud de la cadena
      readbuf[stringlen - 1] = '\0'; // Ingresa el caracter de final
      end_process = strcmp(readbuf, end_str); // Compara la entrada con "end"
      
      //printf("end_process is %d\n", end_process);
      if (end_process != 0) {
         write(fd, readbuf, strlen(readbuf)); // Escribe el mensaje en el FIFO
         printf("FIFOCLIENT: Sent string: \"%s\" and string length is %d\n", readbuf, (int)strlen(readbuf)); // Imprime mensaje de envío y su longitud
         read_bytes = read(fd, readbuf, sizeof(readbuf)); // Lee los datos del cliente
         readbuf[read_bytes] = '\0'; // Termina la cadena leída con '\0'
         printf("FIFOCLIENT: Received string: \"%s\" and length is %d\n", readbuf, (int)strlen(readbuf)); // Imprime mensaje recibido y su longitud
      } else {
         write(fd, readbuf, strlen(readbuf)); // Escribe el mensaje en el FIFO
         printf("FIFOCLIENT: Sent string: \"%s\" and string length is %d\n", readbuf, (int)strlen(readbuf)); // Imprime mensaje de envío y su longitud
         close(fd); // Cierra descriptor del archivo
         break; // Termina el ciclo infinito
      }
   }
   return 0; // Finaliza el programa
}