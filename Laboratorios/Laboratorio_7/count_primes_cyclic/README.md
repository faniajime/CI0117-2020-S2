# Contador ciclico de numeros primos

El siguiente cuadro muestra los tiempos de duracion para diferentes estrategias de scheduling y diferentes tamanos de chunks. Los tiempos estan dados en milisegundos. El procesador de pruebas que se utilizo corria por deafult con 4 hilos.

|  tipo |  1 |  4 |  2 | 50 000 000 |
|---|---|---|---|---|---|
|  static | 188 005.68  | 180 284.62   | 182 387.16   |  123 466.05  | 
| dynamic  | 187 799.01  | 181 877.66    | 182 279.16 |  116 909.45   |
| guided  | 188 096.01  | 181 266.22  | 183 416.84  | 124 792.51  |

El tiempo de duracion del programa serial fue de 144 912, 98 y del paralelo simple 165 432, 77.

##### Cual tuvo menor duracion y por que

Parece que el tipo de scheduling con menor tiempo de duracion fue el dinamico con tamanos de chunk grandes, esto puesto que la manera en la que se distribuyen los chunks de manera dinamica son mas eficientes y porque al tener tamanos de chunks mas grandes el programa no tiene que estar distribuyendo iteraciones, parece que esto afecta el rendimiento. 

#### Scheduling guiado y ejemplo

