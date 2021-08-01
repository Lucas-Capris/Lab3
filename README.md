# Medições de performance:

Usando a média de 5 ocorrências de cada caso a seguir, foi encontrado os seguintes valores:

- n = 10^5 com 2 threads:
    - Tempo do calculo sequencial: 0.001000
    - Tempo do calculo concorrente: 0.001500
    - Ganho de desempenho: 0.666...
- n = 10^5 com 4 threads:
    - Tempo do calculo sequencial: 0.000200
    - Tempo do calculo concorrente: 0.000600
    - Ganho de desempenho: 0.333...
      
- n = 10^7 com 2 threads: 
    - Tempo do calculo sequencial: 0.044000
    - Tempo do calculo concorrente: 0.024000
    - Ganho de desempenho: 1.8333...
    
- n = 10^7 com 4 threads:
    - Tempo do calculo sequencial: 0.044000
    - Tempo do calculo concorrente: 0.016000
    - Ganho de desempenho: 2.75

Nos casos de n = 10^9, meu PC continuamente retornava "Erro-malloc", implicando que não há memória suficiente para que eu possa fazer testes com esses valores. Optei por fazer tetes com n = 10^8 no lugar, já que não há retorno de erro nesse caso.

- n = 10^8 com 2 threads:
    - Tempo do calculo sequencial: 0.426000
    - Tempo do calculo concorrente: 0.221000
    - Ganho de desempenho: 1.9276...
    
- n = 10^8 com 4 threads:
    - Tempo do calculo sequencial: 0.424000
    - Tempo do calculo concorrente: 0.144000
    - Ganho de desempenho: 2.9444...
    
