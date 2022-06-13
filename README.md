# Carrera-de-caballos-con -hebras

- Tarea Numero 2 sobre un hipodromo
  - Introducción
  - uso del programa

### Introducción

Lenguaje en C#
Autores; Nahuelhual Rayen - Pizarro Patricio

El programa se trata sobre una carrera de caballos el cual tiene una cantidad de 2 a 7 caballos, donde el ganador sera elegido aleatoreamente.

-Se crea una estructura para el caballo, una funcion que crea el caballo para que se vea en pantalla 
se crean los caballos a traves de Threads

-Se simula una carrera

-se usa el mutex para evitar condiciones criticas

### Uso del programa

Al correr el programa por consola se vera secuencialmente el avance de cada caballo hasta llegar al final de la pista dando por escrito el caballo ganador.
//para compilar usar: gcc -o carrera carrera.c -lpthread -lncurses -DMUTEX
