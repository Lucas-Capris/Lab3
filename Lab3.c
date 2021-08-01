#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<time.h>

int nthreads;
long long int n;
int randLim = 1000;
float *vetor;

typedef struct{
    float min;
    float max;
} tArgs;

void *minmax (void*arg){

    tArgs* args = (tArgs*) malloc(sizeof(tArgs));
    args->min = randLim; //Valor inicial para o min: o maior valor possível que ele poderia ter para que possa ser quase garantidamente trocado depois
    args->max = 0; //Valor inicial para o max: o menor valor possível que ele poderia ter para que possa ser quase garantidamente trocado depois

    long long int id = (long long int) arg;

    long int tamBloco = n/nthreads;
    long int start = id * tamBloco;
    long int end;

    if(id == nthreads-1){
        end = n;
    } else{
        end = start + tamBloco;
    }
    
    for(long long int i = start; i<end; i++){
        
        if(args->max < vetor[i]){
            args->max = vetor[i];
        }
        if(args->min > vetor[i]){
            args->min = vetor[i];
        }
    }

    pthread_exit((void*)args);
}

int main(int argc, char *argv[]){
   
    srand(time(NULL));
    pthread_t *tid;
    tArgs* retorno;
    tArgs* args;

    float minSeq = randLim; //Preenchido com a mesma lógica do args->min
    float maxSeq = 0; //Preenchido com a mesma lógica do args->max

    float minCon = randLim; //Preenchido com a mesma lógica do args->min
    float maxCon = 0; //Preenchido com a mesma lógica do args->max

    clock_t inicio, fim;
    double delta;

    //Início da medição do tempo de inicialização
    inicio = clock();
    
    //Check de input
    if(argc<3){
        fprintf(stderr,"A entrada deve ser: ./'nome do programa' <dimensao do vetor> <n de threads>\n");
        return 1;
    }

    //Armazenando inputs
    n = atoi(argv[1]);          
    nthreads = atoi(argv[2]);

    //Inicializando vetores que serão usados para a reposta
    float vetorResMin[nthreads];
    float vetorResMax[nthreads];

    //Alocação da memória para ponteiros do programa com checks de erro
    vetor  = (float *) malloc(sizeof(float)*n);
    if (vetor == NULL){
        fprintf(stderr,"Erro-malloc\n");
        return 2;
    }

    retorno = (tArgs*) malloc(sizeof(tArgs));
    if (retorno == NULL){
        fprintf(stderr,"Erro-malloc\n");
        return 2;
    }

    args = (tArgs*) malloc(sizeof(tArgs));
    if (args == NULL){
        fprintf(stderr,"Erro-malloc\n");
        return 2;
    }

    tid = (pthread_t*) malloc(sizeof(pthread_t) * nthreads);
    if(tid == NULL){
        fprintf(stderr,"Erro-malloc\n");
        return 2;
    }    

    //Preenchendo o vetor
    for(long long int i = 0; i < n; i++){
        vetor[i] = (float)rand()/(RAND_MAX) * randLim; //Que varia entre 0 e randLim
    }

    //Fim da medição do tempo de inicialização
    fim = clock();
    delta = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("Tempo de inicializacao: %lf\n",delta);

    //Início da medição do tempo de cálculo sequencial
    inicio = clock();

    //Cálculo sequencial para comparar posteriormente
    for(long long int i = 0; i < n; i++){
        if (vetor[i] < minSeq){
            minSeq = vetor[i];
        }

        if (vetor[i] > maxSeq){
            maxSeq = vetor[i];
        }
    }

    //Fim da medição do tempo de cálculo sequencial
    fim = clock();
    delta = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("Tempo do calculo sequencial: %lf\n",delta);

    //Início da medição do tempo de cálculo concorrente
    inicio = clock();

    //Criando as threads
    for(int i = 0; i < nthreads; i++){

        if(pthread_create(tid+i,NULL,minmax,(void*)i)){
            fprintf(stderr,"Erro--pthread_create\n");
            return 3;
        }
    }

    //Aguardando término das threads
    for(long long int i = 0; i < nthreads; i++){
        pthread_join(*(tid+i),(void**) &retorno);
        
        vetorResMin[i] = retorno->min;
        vetorResMax[i] = retorno->max;
    }

    //Fim da medição do tempo de cálculo concorrente
    fim = clock();
    delta = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("Tempo do calculo concorrente: %lf\n",delta);

    //Obtendo resultados
    for(int i = 0; i < nthreads; i++){
        if (vetorResMin[i] < minCon){
            minCon = vetorResMin[i];
        }

        if (vetorResMax[i] > maxCon){
            maxCon = vetorResMax[i];
        }
    }

    //Comparando resultados do sequencial e do concorrente
    if(minCon != minSeq || maxCon != maxSeq){
        printf("minCon: %lf, minSeq: %lf, maxCon: %lf, maxSeq: %lf\n", minCon, minSeq, maxCon, maxSeq);
        printf("ERRO -- incompatibilidade entre forma concorrente e sequencial");
        return 4;
    }

    puts("Confirmada compatibilidade entre forma concorrente e sequencial!");

    //Início da medição do tempo de finalização
    inicio = clock();

    //Liberando o espaço de memória
    free(vetor);
    free(tid);
    free(args);
    free(retorno);

    //Fim da medição do tempo de finalização
    fim = clock();
    delta = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("Tempo de finalizacao: %lf\n",delta);

    return 0;
}
