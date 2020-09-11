 ## ¿Permite este mecanismo de control de concurrencia resolver este problema de manera predecible? Explique las diferencias entre usar exclusión mutua y semáforos para resolver este problema
 
 Si, los hilos se ejecutan secuencialmente.
 
 Una de las diferencias observadas es el tiempo de duracion, parece que resolverlos con semaforos tarda menos tiempo que resolverlo con mutexes.
 Segundo ambas estructuras funcionan diferentes, los semaforos esperan mientras que los mutexes se bloquean, ademas los semaforos se postean para liberarlos mientras que los mutexes se desbloquean.
 
 Respecto a lo demas, para resolver este problema pareciera que ambos funcionan igualmente bien.
 
