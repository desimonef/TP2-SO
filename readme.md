# TP2 SO

## Introducción
Trabajo práctico elaborado para la materia _"72.11 - Sistemas Operativos"_, Instituto Teconológico de Buenos Aires. Consiste en la implementación de funcionalidades sobre el trabajo práctico especial realizado en la materia _"72.08 - Arquitectura de Computadoras"_. Algunas de las funcionalidades que se debieron implementar consistieron en:
- _Administración de memoria_ 
- _Priority Based Round Robin Scheduler_
- Mecanismos de Inter Process Communication en la forma de _Pipes_
- Sincronización y control de procesos con _semáforos_ 

Y, a modo de utilizar todo lo mencionado, se requirió una implementación del famoso ["Problema de los Filósofos"](https://www.ee.ryerson.ca/~courses/coe518/Labs/lab4/lisi.edu-dining-Philosopherecture8.pdf).

## Compilación
Para compilar este trabajo práctico, es necesario conectarse a docker. **Se asume que el usuario posee conocimientos para instalar docker en su entorno de ejecución.**

Primero, accedemos a _docker_ mediante el comando:
```
docker run -v ${PWD}:/root --security-opt seccomp:unconfined -ti agodio/itba-so:1.0
```
- Nota: Para utilizar este comando, es necesario haber pulleado la imagen de docker del servidor. Esto se puede realizar haciendo    `docker pull agodio/itba-so:1.0`

Una vez en docker, ejecutamos el siguiente comando: 
```
cd root && make all/buddy && exit
```
- Nota 2: El kernel permite la compilación con 2 administradores de memoria diferentes. Si se desea utilizar el administrador _buddy_, ejecutar indicando `make buddy`. Caso contrario, usar `make all`.

Una vez que se tiene el programa ejecutado, nos podemos ubicar sobre el directorio  `Image` donde debería figurar el archivo `x64BareBonesImage.qcow2`. Para ejecutarlo, es necesario contar con _qemu_ instalado en el entorno de trabajo. **Se asume que el usuario posee conocimientos para instalar qemu en su entorno de trabajo**.

Para ejecutar el trabajo, ejecutamos el comando:
```
qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 
```
o simplemente `./run.sh` (o `./run.bat` en caso que se este trabajando con _WSL2_)

## Uso
El kernel ofrece diferentes funcionalidades, que se encuentran listadas mediante la ejecución del comando `help`. Algunas de las funcionalidades destacadas son:

- **help**: muestra el menu de ayuda
- **clear**: Borra el contenido de la pantalla
- **kill**: termina la ejecución del SO
- **zerodiv**: se genera una excepcion de division por cero
- **invopcode**: se genera una excepcion de codigo de operacion equivocado
- **inforeg**: muestra los valores de los registros en el momento de ejecucion
- **printmem**: muestra el contenido de 32 bytes de memoria a partir de la direccion hexadecimal provista
- **datetime**: muestra la hora y la fecha a la hora de ejecutarse
- **mem**: imprime estado actual de la memoria
- **ps**: imprime procesos
- **loop**: imprime el PID del proceso
- **nice**: cambia prioridad de un proceso
- **kill, block, unblock**: cambia estados de los procesos
- **sem**: imprime semaforos
- **cat**: imprime stdin como lo recibe
- **wc**: cuenta cantidad de lineas del input
- **filter**: filtra vocales del input
- **pipe**: imprime estado actual de los pipes
- **phylo**: implementacion del problema de los filosofos

## Integrantes:
Nombre | Legajo
-------|--------
[De Simone, Franco](https://github.com/desimonef) | 61100
[Dizenhaus, Manuel](https://github.com/ManuelDizen) | 61101
[Cornidez, Milagros](https://github.com/mcornidez) | 61432
