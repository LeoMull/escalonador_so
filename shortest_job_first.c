#include <stdio.h>
#include <stdlib.h>

typedef struct tarefa {
    char id[2];
    int tempo;
} TAREFA;
typedef struct processador {
    char id[2];
    int tempo;
    int tempoInicio;
    int tarefasProntas;
} PROCESSADOR;
typedef struct processos{
    char id[2];
    int processador;
    int start;
    int end;
} PROCESSOS;

int compareTarefasEncrease(const void* a, const void* b) {
    TAREFA* tarefaA = (TAREFA*)a;
    TAREFA* tarefaB = (TAREFA*)b;
    return tarefaA->tempo - tarefaB->tempo;
}
int compareTarefasDecrease(const void* a, const void* b) {
    TAREFA* tarefaA = (TAREFA*)a;
    TAREFA* tarefaB = (TAREFA*)b;
    return  tarefaB->tempo - tarefaA->tempo;
}

void readAndOrganizeFile(const char* filename, TAREFA* tarefas, int numTarefas) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return;
    }

    char line[100];
    int i = 0;
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%s %d", tarefas[i].id, &tarefas[i].tempo);
        i++;
    
        if (i == numTarefas) {
            break;
        }
    }
    fclose(file);
    
    qsort(tarefas, numTarefas, sizeof(TAREFA), compareTarefasEncrease);
}
void organizeProcess(PROCESSOS *processos, char* id, int start, int end, int processador, int pos){
        for(int j=0; j<3;j++){
            processos[pos].id[j] = id[j];   
        }
        processos[pos].processador = processador;
        processos[pos].start = start;
        processos[pos].end = end;
    return;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input_file> <number_of_processors>\n", argv[0]);
        return 1;
    }
    int numProcessadores = atoi(argv[2]), numTarefas = 0, proxTarefa = 0, finished = 0, time=0, counterProcessos=0;
    PROCESSADOR processador[numProcessadores];
    char line[100];

    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    while (fgets(line, sizeof(line), file)){
        numTarefas++;
    }
    fclose(file);   

    TAREFA tarefas[numTarefas];
    PROCESSOS processos[numTarefas];

    readAndOrganizeFile(argv[1], tarefas, numTarefas);
    for(int i=0; i<numTarefas; i++){
        printf("%s %d\n", tarefas[i].id, tarefas[i].tempo);
    }
    FILE* file2 = fopen("saida.txt", "a");

    for(int i=0; i<numProcessadores; i++){
        processador[i].tempo = 0;
        processador[i].tempoInicio = 0;
        processador[i].tarefasProntas = 0;
    }

    while(finished == 0){
        finished = 1;
        for(int i=0; i<numProcessadores; i++){
            if(processador[i].tempo == 0 && processador[i].tarefasProntas > 0){
                organizeProcess(processos, processador[i].id, processador[i].tempoInicio, time, i, counterProcessos);
                counterProcessos++;
            }

            if(processador[i].tempo == 0 && proxTarefa < numTarefas){
                for(int j=0; j<3;j++){
                    processador[i].id[j] = tarefas[proxTarefa].id[j];
                }
                processador[i].tempo = tarefas[proxTarefa].tempo;
                processador[i].tempoInicio = time;
                processador[i].tarefasProntas++;
                proxTarefa++;
            }
            if(processador[i].tempo >= 0){
                processador[i].tempo--;
            }
        }
        for(int i=0; i<numProcessadores; i++){
            if(processador[i].tempo >= 0){
                finished = 0;
                break;
            }
        }

        time++;
    }
    
    for(int i=0; i<numProcessadores; i++){
        fprintf(file2, "Processador %d :\n", i);
        for(int j=0; j<numTarefas; j++){
            if(processos[j].processador == i){
                fprintf(file2,"%s;%d;%d\n", processos[j].id, processos[j].start, processos[j].end);
            }   
        }    
    }
    fclose(file2);
    return 0;
}

