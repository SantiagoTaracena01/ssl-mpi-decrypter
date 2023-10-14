# ssl-mpi-decrypter

Este proyecto se centra en la búsqueda y recuperación de llaves de cifrado DES (Estándar de Cifrado de Datos) utilizando diferentes enfoques y estrategias de cómputo paralelo. El objetivo es evaluar la eficiencia de varias técnicas y comparar sus ventajas y desventajas para la búsqueda de llaves de cifrado DES en un entorno distribuido.

## Miembros del Equipo

- José Rodrigo Barrera García (20807)
- Marco Pablo Orozco Saravia (20857)
- Santiago Taracena Puga (20017)

## Descripción del Proyecto

El proyecto se divide en varias etapas, cada una de las cuales se enfoca en una estrategia diferente para buscar llaves de cifrado DES. Las estrategias incluyen:

1. **Enfoque Naive**: Un enfoque sencillo de fuerza bruta que prueba cada llave en un rango dado.
2. **Enfoque de Arriba hacia Abajo**: Una estrategia optimizada que inicia la búsqueda desde la llave máxima y se desplaza hacia abajo.
3. **Enfoque de Dos Llaves a la Vez**: Una estrategia teóricamente eficiente que revisa dos llaves adyacentes simultáneamente.

## Implementación

El proyecto está implementado en lenguaje C y hace uso de las siguientes tecnologías y bibliotecas:

- Código en C para la manipulación de llaves DES y el cómputo paralelo.
- Biblioteca OpenSSL para operaciones de cifrado y descifrado DES.
- MPI (Interfaz de Paso de Mensajes) para la programación paralela y distribuida.

## Instrucciones de Uso

1. Asegúrate de tener instalado MPI y la biblioteca OpenSSL en tu sistema.

2. Compila el código fuente utilizando el compilador C. Puedes usar el siguiente comando:

```bash
mpicc -o main main.c -lcrypto
```

Ejecuta el programa MPI con el número de nodos deseados. Por ejemplo, para ejecutar con 4 nodos:

```bash
mpirun -np 4 ./main <archivo-de-entrada> <llave-privada> <archivo-a-buscar>
```

El programa se encargará de iniciar la búsqueda de la llave y proporcionará resultados una vez encontrada.

## Resultados

Después de ejecutar el programa con diferentes enfoques y estrategias, se recopilaron datos y se realizaron análisis comparativos. Los resultados se detallan en el informe del proyecto adjunto.

## Contribuciones

Siéntete libre de contribuir o mejorar este proyecto. Puedes hacerlo a través de bifurcaciones (forks) y solicitudes de extracción (pull requests).

## Licencia

Este proyecto se publica bajo la Licencia MIT. Consulta el archivo LICENSE para obtener más detalles.

Este proyecto se realizó como parte de un curso universitario (CC3069 - Computación Paralela y Distribuida) en la Universidad del Valle de Guatemala.

Por supuesto, asegúrate de personalizar el archivo `README.md` con información específica de tu proyecto, como los resultados reales que obtuviste y cualquier detalle adicional que desees compartir con los usuarios y colaboradores potenciales.
