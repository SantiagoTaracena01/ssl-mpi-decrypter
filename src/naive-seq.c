/**
 * Universidad del Valle de Guatemala
 * (CC3069) Computación Paralela y Distrubida
 * Proyecto 2 - Memoria Distribuida
 * 
 * Miembros del equipo de trabajo:
 * - José Rodrigo Barrera García (20807)
 * - Marco Pablo Orozco Saravia (20857)
 * - Santiago Taracena Puga (20017)
 */

// Librerías requeridas para el proyecto.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <openssl/des.h>

// Función que encripta un mensaje.
void encrypt(char* source, char* destiny, DES_key_schedule schedule) {

  // Ciclo que encripta el texto completo en cadenas de 8 bytes.
  for (int i = 0; i < strlen(source); i += 8) {

    // Array con el texto de 8 bytes inicial original.
    char originalSentence[8] = { source[i], source[(i + 1)], source[(i + 2)], source[(i + 3)], source[(i + 4)], source[(i + 5)], source[(i + 6)], source[(i + 7)] };

    // Array temporal para colocar el resultado encriptado.
    char encryptedSentence[8] = { "" };

    // Proceso de encriptado de la oración.
    DES_ecb_encrypt((const_DES_cblock*)originalSentence, (DES_cblock*)encryptedSentence, &schedule, DES_ENCRYPT);

    // Mapping del resultado encriptado al destino.
    for (int j = 0; j < 8; j++) {
      destiny[(i + j)] = encryptedSentence[j];
    }
  }
}

// Función que desencripta un mensaje.
void decrypt(char* source, char* destiny, DES_key_schedule schedule) {

  // Ciclo que desencripta el texto completo en cadenas de 8 bytes.
  for (int i = 0; i < strlen(source); i += 8) {

    // Array con el texto de 8 bytes inicial encriptado.
    char encryptedSentence[8] = { source[i], source[(i + 1)], source[(i + 2)], source[(i + 3)], source[(i + 4)], source[(i + 5)], source[(i + 6)], source[(i + 7)] };

    // Array temporal para colocar el resultado desencriptado.
    char decryptedSentence[8] = { "" };

    // Proceso de desencriptado de la oración.
    DES_ecb_encrypt((const_DES_cblock*)encryptedSentence, (DES_cblock*)decryptedSentence, &schedule, DES_DECRYPT);

    // Mapping del resultado desencriptado al destino.
    for (int j = 0; j < 8; j++) {
      destiny[(i + j)] = decryptedSentence[j];
    }
  }
}

// Función que prueba una llave.
int tryKey(long keyToTest, char* source, char* searchedText) {

  // Llave en formato de string.
  char stringKey[256];
  sprintf(stringKey, "%ld", keyToTest);

  // Variables de DES temporales.
  DES_cblock temporalKeyToTest;
  DES_key_schedule temporalSchedule;

  // Configuración de la llave temporal.
  DES_string_to_key(stringKey, &temporalKeyToTest);
  DES_set_key((const_DES_cblock*)&temporalKeyToTest, &temporalSchedule); 

  // Instancia del texto temporal.
  char temporalText[strlen(source)];

  // Desencriptación del texto con la llave a probar.
  decrypt(source, temporalText, temporalSchedule);

  // Retorno si el texto encontrado es correcto.
  return (strstr((char*)temporalText, searchedText) != NULL);
}

// Función main, que ejecuta el código del programa.
int main(int argc, char* argv[]) {

  // Verificación de los argumentos pasados en consola.
  if (argc < 4) {
    perror("Uso del programa: ./naive-par <input-file> <private-key> <searched-file>\n");
    return EXIT_FAILURE;
  }

  /**
   * Llaves probadas:
   * - 123456
   * - 1234567
   * - 12345678
   * - 18014398509481983 (2 ** 56 / 4)
   * - 18014398509481984 (2 ** 56 / 4 + 1)
   */

  // Path al archivo a usar y llave privada usada.
  char* filePath = argv[1];
  char* inputKey = argv[2];
  char* searchedTextPath = argv[3];

  // Upper bound de la búsqueda, es decir, el límite de las llaves.
  long upperBound = (1L << 56); 

  // Instancia del archivo leído.
  FILE* inputFile = fopen(filePath, "rb");

  // Verificación de la existencia del archivo leído.
  if (!inputFile) {
    perror("Error abriendo el archivo.\n");
    return EXIT_FAILURE;
  }

  // Obtención de la longitud del archivo.
  fseek(inputFile, 0, SEEK_END);
  long fileLength = ftell(inputFile);
  rewind(inputFile);

  // Asignación de memoria para el archivo.
  char* inputText = (char*)malloc(fileLength);

  // Verificación de la asignación de memoria.
  if (!inputText) {
    perror("Error asignando memoria para el archivo.\n");
    fclose(inputFile);
    return EXIT_FAILURE;
  }

  // Lectura y almacenamiento del archivo.
  if (fread(inputText, 1, fileLength, inputFile) != fileLength) {
    perror("Error leyendo el archivo.\n");
    free(inputText);
    fclose(inputFile);
    return EXIT_FAILURE;
  }

  // Cierre del archivo de entrada.
  fclose(inputFile);

  // Instancia del archivo con texto a buscar.
  FILE* searchedTextFile = fopen(searchedTextPath, "rb");

  // Verificación de la existencia del archivo leído.
  if (!searchedTextFile) {
    perror("Error abriendo el archivo.\n");
    return EXIT_FAILURE;
  }

  // Obtención de la longitud del archivo.
  fseek(searchedTextFile, 0, SEEK_END);
  long searchedFileLength = ftell(searchedTextFile);
  rewind(searchedTextFile);

  // Asignación de memoria para el archivo.
  char* searchedText = (char*)malloc(searchedFileLength);

  // Verificación de la asignación de memoria.
  if (!searchedText) {
    perror("Error asignando memoria para el archivo.\n");
    fclose(searchedTextFile);
    return EXIT_FAILURE;
  }

  // Lectura y almacenamiento del archivo.
  if (fread(searchedText, 1, searchedFileLength, searchedTextFile) != searchedFileLength) {
    perror("Error leyendo el archivo.\n");
    free(searchedText);
    fclose(searchedTextFile);
    return EXIT_FAILURE;
  }

  // Cierre del archivo del texto a buscar.
  fclose(searchedTextFile);

  // Llave y calendarización de DES.
  DES_cblock key;
  DES_key_schedule schedule;

  // Configuración de la llave.
  DES_string_to_key(inputKey, &key);
  DES_set_key((const_DES_cblock*)&key, &schedule); 

  // Instancia del texto encrpitado y desencriptado.
  char encryptedText[strlen(inputText)];
  char decryptedText[strlen(inputText)];

  // Función para encriptar el texto.
  encrypt(inputText, encryptedText, schedule);

  // Llave a encontrar.
  long foundKey = 0L;
  int keyHasBeenFound = 0;

  // Ciclo for que busca la llave en el límite dado.
  for (long i = 0; i < upperBound; i++) {

    // Verificación de la llave.
    if (tryKey(i, encryptedText, searchedText)) {

      // Intercambio con la llave encontrada.
      foundKey = i;

      // Impresión del resultado.
      printf("Se ha encontrado la llave: %ld\n", foundKey);

      // Salida del ciclo.
      break;
    }
  }

  // Cambio del schedule para la nueva llave.
  DES_key_schedule foundKeySchedule;
  DES_set_key_unchecked((DES_cblock*)&foundKey, &foundKeySchedule);

  // Desencripción con la llave encontrada.
  decrypt(encryptedText, decryptedText, schedule);

  // Impresión de los resultados.
  printf("Texto original: %s\n", inputText);
  printf("Texto cifrado: %s\n", encryptedText);
  decryptedText[(int)strlen(decryptedText) - (int)strlen(encryptedText)] = '\0';
  printf("Texto descifrado: %s\n", decryptedText);

  // Salida correcta del programa.
  return EXIT_SUCCESS;
}
