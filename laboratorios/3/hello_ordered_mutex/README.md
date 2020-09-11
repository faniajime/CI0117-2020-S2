 
### ¿Permite este mecanismo de control de concurrencia resolver este problema de manera predecible?

No, inclusive utilizando un arreglo de mutexes, los hilos no se sincronizan, dado que en el tiempo que le toma al hilo cero bloquear todos los mutexes, los hilos ya terminaron de imprimir. 

