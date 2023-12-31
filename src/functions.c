#include "../include/morse.h"

char morse_num[10][6] = {
    "-----",
    ".----",
    "..---",
    "...--",
    "....-", 
    ".....",
    "-....",
    "--...",
    "---..",
    "----."
};



char morse_alpha[27][6] = {//ultima coluna para o \0
    ".-", //a
    "-...", //b
    "-.-.",//c
    "-..",//d
    ".",//e
    "..-.",//f
    "--.",//g
    "....",//h
    "..",//i
    ".---",//j
    "-.-",//k
    ".-..",//l
    "--",//m
    "-.",//n
    "---",//o
    ".--.",//p
    "--.-",//q
    ".-.",//r
    "...",//s
    "-",//t
    "..-",//u
    "...-",//v
    ".--",//w
    "-..-",//x
    "-.--",//y
    "--..",//z
    "/"
    };

// lerPorClique: lê um código morse através do input (teclado) por cliques e o coloca em palavra
// deve-se utilizar a tecla enter
// '.' -> pressione o enter uma vez
// '-' -> segure a tecla até que ao menos mais um enter saia no input
// deve haver um intervalo de aproximadamente meio segundo entre um dígito e outro
// ' ' -> manter a tecla solta por aproximadamente 3 segundos
// para parar a leitura, digite qualquer letra e pressiona enter
void lerPorClique(char *palavra) {
    // essa struct utiliza da biblioteca sys/time.h para calcular o tempo entre os inputs do teclado
    struct timeval begin, end;
    double tempoSec;
    int soma = 0, i = 0;

    getchar();
    gettimeofday(&begin, 0);
    
    // esse loop externo roda enquanto houverem caracteres a serem lidos
    while (1) {
        int parar = 0;

        // esse loop roda enquanto estiver em um mesmo dígito
        while (1) {
            if (getchar() != '\n') {
                parar = 1;
                break;
            }

            gettimeofday(&end, 0);
            tempoSec = (double) (end.tv_sec - begin.tv_sec) + (end.tv_usec - begin.tv_usec) * 1e-6;
            if (tempoSec > 0.4) {
                break;
            }

            // a quantidade de enters é usada para saber se é um ponto ou um traço
            soma++;

            gettimeofday(&begin, 0);
        }
        if (soma == 0) {
            palavra[i++] = '.';
        } else {
            palavra[i++] = '-';
        }

        if (parar == 1) {
            palavra[i] = '\0';
            break;
        }

        if (tempoSec > 2) {
            palavra[i++] = ' ';
        }

        if (tempoSec > 4) {
            palavra[i++] = '/';
            palavra[i++] = ' ';
        }
        
        soma = 0;
        gettimeofday(&begin, 0);
    }
}

// morseAudio: recebe uma string contendo um codigo morse
// e o reproduz em formato de audio
void morseAudio(char *morse) {
    int tam;
    for (int i = 0; i < strlen(morse); i++) {
        if (morse[i] == ' ') {
            sleep(2);
        } else if (morse[i] == '-') {
            tam = 2000;
        } else {
            tam = 1000;
        }

        #ifdef __linux__
            while (tam--) {
                printf("\a");
            }
        #elif _WIN32
            BEEP(800, tam);
        #endif

        sleep(1);
        tam = 0;
    }
}

//função para a conversão de morse para alfanumérico
void conv_MorseParaAlfaNumerico(char *morse){
    // funçao strtok divide a string original (morse) em substrings (caracteres individuais) separadas por espaços
    char *caractere = strtok((char *)morse, " ");

    // loop principal que percorre toda a string original
    int i;
    int encontrado;
    while (caractere != NULL){
        encontrado = 0;
        // loop que compara linearmente cada caractere da substring com a referencia (alfabeto) e realiza a conversão
        for (i = 0; i < 27; i++) {
            if (strcmp(caractere, morse_alpha[i]) == 0) {
                if (i == 26) {//se morse_alpha[i] é a barra que indica espaço
                    printf(" ");
                    encontrado = 1;
                    break;
                } else {
                    printf("%c", 'A' + i);
                    encontrado = 1;
                    break;
                }
            }
        }

        // loop que compara linearmente cada caractere da substring com a referencia (numeros) e realiza a conversão
        for (i = 0; i < 10; i++) {
            if (strcmp(caractere, morse_num[i]) == 0) {
                printf("%d", i);
                encontrado = 1;
                break;
            }
        }

        // imprime um símbolo caso o caractere não tenha sido reconhecido em nenhuma das duas strings bases (alfabeto e numeros)
        if (!encontrado) {
            printf("(?)");
        }

        // relização da passagem para o proximo caractere 
        caractere = strtok(NULL, " ");
    }
}

//funções de conversão de alfanumérico para morse
void toLower(char **string_ref, int tam){//string minuscula
    int i;
    char c;

    char* string = *string_ref;

    for(i=0; i<tam; i++){
        c = string[i];
        if(c>='A' && c<='Z') string[i]= c + 32;
    }

}

char* conv_AlfaNumericoToMorse(char *string){
    char *morseCriado;
    int tam = strlen(string);
    do {
        morseCriado = (char*)malloc(sizeof(char) * tam * 6);
    } while (morseCriado == NULL);

    toLower(&string, tam);

    char c;
    for (int i = 0; i < tam; i++){
        c = string[i];
        if(c >= '0' && c <= '9') {
            strcat(morseCriado, morse_num[c - '0']);
            strcat(morseCriado, " ");
        } else if(c == ' ') {
            strcat(morseCriado, "/ ");
        } else {
           strcat(morseCriado, morse_alpha[c-'a']);
           strcat(morseCriado, " "); 
        }
    }

    return morseCriado;
}



//função para limpar a tela
void clear(){
    #ifdef __linux__
        system("clear");
    #elif _WIN32
        system("cls");
    #endif
}

//função para mostar o menu
void menu() {
    printf(ROXO"\t\t========= CONVERSOR MORSE =========\n\n");
    printf("1 - "FIM_COR);
    printf("Alfanumerico para Morse\n");
    printf(ROXO"2 - "FIM_COR);
    printf("Morse para Alfanumerico\n");
    printf(ROXO"3 - "FIM_COR);
    printf("Como funciona o codigo morse?\n");
    printf(ROXO"0 - "FIM_COR);
    printf("Sair\n");
}

//função esperar para que o usuário pressione enter(interface)
void esperar() {
    printf(ROXO"\nPressione Enter para continuar..."FIM_COR);
    limparBuffer();
    getchar();  
}

// essa função limpa o buffer, para evitar problemas com o scanf
void limparBuffer() {
    while (getchar() != '\n');
}