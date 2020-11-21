# Tiempo de duracion del programa para encontrar numeros primos
Creado por Fabiola Jimenez y Karen Zamora.

El siguiente cuadro muestra los tiempos de duracion para diferentes estrategias de comunicacion entre hilos. Los tiempos estan dados en segundos.
Las corridas se realizaron con un numero de 100000000 tres veces.

|  ejercicio |  procesos |1 |2|3 |
|---|---|---|---|---|
|count_primes_process|  1 | 89.661409  | 88.585412   | 90.272267   |
|count_primes_process| 16  | 65.545689  | 66.025527   | 69.566219 |
|count_primes_process| 32  |  72.301093  | 69.301179  | 68.773145 |
|count_primes_process| 64  | 69.665522 | 70.269668 | 73.743663  |
|count_primes_reduction|  1 | 88.654710  |87.793338  | 87.885886   |
|count_primes_reduction| 16  | 64.821625  | 66.782605  | 66.349952 |
|count_primes_reduction| 32  | 66.676823  | 68.778798  | 68.576306 |
|count_primes_reduction| 64  | 65.460857  | 66.282078  | 65.992454  |
|count_primes_all_reduce|  1 | 88.876599  |89.297862  | 89.299594  |
|count_primes_all_reduce| 16  | 65.009387  | 65.871123  | 68.81825 |
|count_primes_all_reduce| 32  | 68.818259  | 67.410608  | 72.490510 |
|count_primes_all_reduce| 64  | 66.333065  | 66.142965  | 69.170663  |

