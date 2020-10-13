 
# Lectores y escritores.
## Karen Zamora, Fabiola Jiménez.

### Descripción del problema / patrón de sincronización.
El problema de los lectores-escritores se da cuando se tiene una estructura de datos que es leída y modificada por hilos concurrentes. Cuando un hilo la modifica, se debe prevenir que otros hilos la lean, de manera que se evadan situaciones como la lectura de datos incorrectos o incompletos.
La manera de sincronización será la siguientes:
Un hilo escritor no puede entrar mientras otros estén leyendo.
Los hilos lectores no pueden entrar mientras algún hilo esté escribiendo.
El patrón de exclusión es un patrón categórico de exclusión, donde no necesariamente un hilo excluye a los demás, a menos de que pertenezcan a diferentes categorías.

### Conteste la pregunta: ¿Se puede presentar alguna situación de deadlock o starvation? Explique.
Se podría presentar un problema de starvation con alguno de los escritores, esto debido a que si un hilo cuya función es la de escribir llega a la sección crítica y se encuentra con lectores podría esperar por siempre mientras hilos lectores van y vienen, ya que si continúan llegando lectores mientras haya al menos uno dentro de esta zona el escritor no podrá entrar. Esto no se considera como deadlock, ya que algunos hilos tienen progresos, sin embargo esto no es deseable.
En este caso, es necesario implementar una solución de forma en la que al llegar el escritor, los hilos dentro de la zona crítica puedan finalizar, pero no puedan entrar hilos adicionales. 
### Agregue alguna ilustración o pseudocódigo que permita visualizar la solución al problema.

Para los escritores:

```
Semaforoescritores.wait()
#sección crítica para escritores
Semaforoescritores.post()
```


Para los lectores:

```
Si hay lectores:
  Semaforoescritores.wait()
  mutexlectores.lock()
    #seccion critica para lectores
  Si ya no hay lectores:
  Semaforoescritores.post()
  mutexlectores.unlock();`
```

 
### Implemente un programa en C utilizando Pthreads donde se haga una simulación de la solución al problema o patrón asignado. Asegúrese de que al ejecutar su programa se pueda observar de forma clara lo que ocurre en cada hilo (Puede hacer uso de funciones de sleep aleatorios en hilos).

Ver codigo adjunto
