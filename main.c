/**      @file: Vampiros.c
 *     @author: João Vítor Arantes Cabral, Matrícula: 17/0126048
 * @disciplina: Algoritmos e Programação de Computadores
 *Um jogo que simula um "RPG" no qual o jogador deve duelar contra vários vampiros
 * */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define ESPACO 0
#define PAREDE 1
#define PERSONAGEM 2
#define VAMPIRO 3
#define DRACULA 4
#define PORTA_ABERTA 5
#define PORTA_FECHADA 6
#define POCAO 7
#define ARMADURA 8
#define ARMA 9
#define MORTO 10

#define TRUE 1
#define FALSE 2

int Vampiros_Movimento() {
        int Move, Randomico;
        srand(time(NULL));
        Randomico = rand()%20;
        Move = rand()%99 + (37 * Randomico);
        if (Move %2 == 0) {
            Move = 1;
        } else if (Move %2 == 1) {
            Move = -1;
        }
        return Move;
}

void Gerar_Itens(int *Item_Ataque, float *Item_Defesa, int *Item_Tipo, float *Item_Life_Steal, int *Item_X, int *Item_Y, int Item_X_Fixo, int Item_Y_Fixo) { /* Por que ter 9 itens se você pode ter infinitos itens? */
    int Randomico, i;
    float Defesa, Life_Steal;
    int Ataque;
    Randomico = rand()%5 + 1;
    if ((*Item_X == 0) && (*Item_Y == 0)) {
        *Item_X = Item_X_Fixo;
        *Item_Y = Item_Y_Fixo;
        *Item_Tipo = rand()% 2;
        if (*Item_Tipo == 0) {
            *Item_Ataque = 0;
            *Item_Life_Steal = 0;
            *Item_Defesa = 0;
        } else if (*Item_Tipo == 1){
            *Item_Ataque = rand()%10 + 7 + Randomico;
            *Item_Life_Steal = ((rand() %5) / 10.0) + (Randomico / 10.0);
            *Item_Defesa = 0;
        } else {
            *Item_Defesa = ((rand() %20) / 10) + (Randomico / 5);
            *Item_Ataque = 0;
            *Item_Life_Steal = 0;
        }
    }

}

void flush_in() {
    int ch;
    do {
        ch = fgetc(stdin);
    } while (ch != EOF && ch != '\n');
}

int fazer_Mapa(int mapa[30][80], int Player_Coluna_Atual, int Player_Linha_Atual){
        int linha,coluna;

        for (linha=0;linha<30;linha++) {
            for (coluna=0;coluna<80;coluna++) {
                if ((linha == Player_Linha_Atual) && (coluna == Player_Coluna_Atual)) {
                    printf("@");
                    continue;
                }
                if (mapa[linha][coluna] == PERSONAGEM)
                    printf("@");
                if (mapa[linha][coluna] == ESPACO)
                    printf(" ");
                if (mapa[linha][coluna] == PAREDE)
                    printf("#");
                if (mapa[linha][coluna] == VAMPIRO)
                    printf("V");
                if (mapa[linha][coluna] == DRACULA)
                    printf("D");
                if (mapa[linha][coluna] == PORTA_ABERTA)
                    printf("-");
                if (mapa[linha][coluna] == PORTA_FECHADA)
                    printf("+");
                if (mapa[linha][coluna] == POCAO)
                    printf("P");
                if (mapa[linha][coluna] == ARMADURA)
                    printf("A");
                if (mapa[linha][coluna] == ARMA)
                    printf("W");
                if (mapa[linha][coluna] == MORTO)
                    printf("M");
        }
        printf("\n");
    }
    return TRUE;
}

float Acerto(){
    float Acerto;
    srand(time(NULL));
    Acerto = rand() %10;
    Acerto = Acerto / 10;
    return Acerto;
}

int Vampiro_1(int *V1_Pocao) {   /* Função para definir a ação do Jogador V1 = Vampiro do jogador */
    char V1_Acao[1];
    printf("\nDigite uma opcao:\nr) Ataque rapido\nf) Ataque forte\nd) Defesa\np) Pocao\ne) Escapar ");
    scanf("%1s%*[^\n]", V1_Acao);
    if ((V1_Acao[0] == 'r') || (V1_Acao[0] =='R')) {
        return 1;
    } else if ((V1_Acao[0] == 'f') || (V1_Acao[0] == 'F')) {
        return 2;
    } else if ((V1_Acao[0] == 'd') || (V1_Acao[0] == 'D')) {
        return 3;
    } else if (((V1_Acao[0] == 'p') || (V1_Acao[0] == 'P')) && (*V1_Pocao >= 1)) {
        return 4;
    } else if ((V1_Acao[0] == 'e') || (V1_Acao[0] == 'E')) {
        return 5;
    } else {
        printf("Voce digitou uma opcao invalida\n");
        return Vampiro_1(V1_Pocao);
    }
}

int Vampiro_2(float *V2_HP, int *V2_Pocao) {
 int V2_Acao;
 srand(time(NULL));
 if((*V2_HP <= 50) && (*V2_Pocao >= 1)) {
    return 4;
 } else if (V2_HP <= 5) {
    return 5;
 } else {
    V2_Acao = rand() %3 + 1;
    return V2_Acao;
 }
}

int Jogar_Novamente() { /* Função que permite que o usuário jogue várias partidas sem ter que reiniciar o programa*/
 char Jogar_de_novo[1];
 printf("Deseja jogar uma partida, 'S' ou 'N' ?");
 scanf(" %1s%*[^\n]", Jogar_de_novo);
 if (Jogar_de_novo[0] == 'S' || Jogar_de_novo[0] == 's'){
    return 1;
 }
 else if (Jogar_de_novo[0] == 'N' || Jogar_de_novo[0] == 'n'){
    return 0;
 }
 else {
	printf("Voce digitou uma opcao invalida\n");
	return Jogar_Novamente();
 }
}

int Combate(float *V1_HP, float *V1_Max_HP, float *V2_HP, float V2_Max_HP, int V1_Atk, int V2_Atk, float V1_Defesa_Inicial, float V2_Defesa_Inicial, float V1_Life_Steal, float V2_Life_Steal, int *V1_Pocao, int *V2_Pocao, float V2_Precisao, int *Escape) {
    const float V1_Precisao = 0.7;
    float V1_Acerto, V2_Acerto, V1_Atordoamento, V2_Atordoamento;
    int V1_Acao = 5, V2_Acao = 5, V1_Defesa, V2_Defesa;
    char Variavel_a_toa; /* Para casos em que o jogador precisa digitar algo para continuar, facilita o entendimento da partida */
    while ((*V1_HP > 0) && (*V2_HP > 0)) {
        V1_Defesa = V1_Defesa_Inicial;
        V2_Defesa = V2_Defesa_Inicial;
        printf("                    *******************************               \n");
        printf("                    *ULTIMATE VAMPIRO CHAMPIONSHIP*               \n");
        printf("                    *******************************               \n");
        printf("\n            Voce                              Vampiro Inimigo   \n");
        printf("     Pontos de Vida: %.1f/%.1f     |     Pontos de Vida: %.1f/%.1f   \n", *V1_HP, *V1_Max_HP, *V2_HP, V2_Max_HP);
        printf("     Pocoes: %d                     |     Pocoes: %d              \n", *V1_Pocao, *V2_Pocao);
        printf("     Ataque: %d                     |     Ataque: %d           \n", V1_Atk, V2_Atk);
        printf("     Life-Steal: %.1f               |     Life_Steal: %.1f        \n", V1_Life_Steal, V2_Life_Steal);
        printf("     Precisao: %.1f                |     Precisao: %.1f           \n\n", V1_Precisao * 100, V2_Precisao * 100);
        if (V1_Acao == 0) {
            V1_Acao = 10;
            printf("       *ATORDOADO*\t");
            printf("Voce esta atordoado, pressione enter para continuar:\n");
            scanf("%c", &Variavel_a_toa);
            flush_in();
        } else {
            if (V2_Acao == 0) {
	    	  printf("                                       *ATORDOADO*\t");
            }
	    V1_Acao = Vampiro_1(V1_Pocao);
        }
        if (V2_Acao == 0) {
            V2_Acao = 10;
        } else {
            V2_Acao = Vampiro_2(V2_HP, V2_Pocao);
        }
        printf("\n");
        if (V1_Acao == 4) {
            *V1_HP = *V1_HP + 30;
            *V1_Pocao = *V1_Pocao - 1;
            V1_Acao = 10;
            if (*V1_HP > *V1_Max_HP) {
                *V1_HP = *V1_Max_HP;
            }
            printf("Comando Vampiro Jogador: Pocao - Restaurou 30 de vida\n");
        }
        if (V2_Acao == 4) {
            V2_HP = V2_HP + 30;
            *V2_Pocao = *V2_Pocao - 1;
            V2_Acao = 10;
            if (*V2_HP > V2_Max_HP) {
                *V2_HP = V2_Max_HP;
            }
            printf("Comando Vampiro Inimigo: Pocao - Restaurou 30 de vida\n");
        }
        if ((V1_Acao == 3) && (V2_Acao == 3)) {
            *V1_HP = *V1_HP + (0.1 * *V1_Max_HP);
            *V2_HP = *V2_HP + (0.1 * V2_Max_HP);
            if (*V2_HP > V2_Max_HP) {
                *V2_HP = V2_Max_HP;
            }
            if (*V1_HP > *V1_Max_HP) {
                *V1_HP = *V1_Max_HP;
            }
        }
        if (V1_Acao == 3) {
            V1_Defesa = 2;
            printf("Comando Vampiro Jogador: Defesa\n");
        }
        if (V2_Acao == 3) {
            V2_Defesa = 2;
        }
        if (V2_Acao == 1) {
            if (V1_Acao == 10) {
                *V1_HP = *V1_HP - (V2_Atk / V1_Defesa);
                *V2_HP = *V2_HP + ((V2_Atk / V1_Defesa) * V2_Life_Steal);
                printf("\nComando Vampiro Inimgo: Ataque rapido - Sucesso\n");
            } else {
                V2_Acerto = Acerto();
                if (V2_Acerto <= V2_Precisao) {
                    *V1_HP = *V1_HP - (V2_Atk / V1_Defesa);
                    *V2_HP = *V2_HP + ((V2_Atk / V1_Defesa) * V2_Life_Steal);
                    printf("\nComando Vampiro Inimgo: Ataque rapido - Sucesso\n");
                } else {
                    printf("\nComando Vampiro Inimgo: Ataque rapido - Falha\n");
                }
            }
            if (V1_Acao == 1) {
                V1_Acerto = Acerto();
                if (V1_Acerto <= V1_Precisao){
                    *V2_HP = *V2_HP - (V1_Atk / V2_Defesa);
                    *V1_HP = *V1_HP + ((V1_Atk / V2_Defesa) * V1_Life_Steal);
                    printf("\nComando Vampiro Jogador: Ataque rapido - Sucesso\n");
                } else {
                    printf("\nComando Vampiro Jogador: Ataque rapido - Falha\n");
                }
            } else if (V1_Acao == 2) {
                V1_Acerto = Acerto();
                if (V1_Acerto <= V1_Precisao) {
                    *V2_HP = *V2_HP - (( 2 * V1_Atk) / V2_Defesa);
                    printf("\nComando Vampiro Jogador: Ataque forte - Sucesso\n");
                } else {
                    printf("\nComando Vampiro Jogador: Ataque forte - Falha\n");
                }
                V1_Atordoamento = Acerto();
                if (V1_Atordoamento <= 0.3) {
                    V1_Acao = 0;
                }
            } else if (V1_Acao == 3) {
                V2_Atordoamento = Acerto();
                if (V2_Atordoamento <= (2 * (1 - V2_Precisao))) {
                    V2_Acao = 0;
                }
                *V2_HP = *V2_HP  - ((V2_Atk / V1_Defesa) * V2_Life_Steal); /*Gambiarra para negar o life steal, que não deve ocorrer ao se defender um ataque rápido*/
            }

        } else if (V2_Acao == 2) {
            if (V1_Acao == 10) {
                *V1_HP = *V1_HP - ((2 * V2_Atk) / V1_Defesa);
                printf("\nComando Vampiro Inimgo: Ataque forte - Sucesso\n");
            } else {
                V2_Acerto = Acerto();
                if (V2_Acerto <= V2_Precisao) {
                    *V1_HP = *V1_HP - ((2 * V2_Atk) / V1_Defesa);
                    printf("\nComando Vampiro Inimgo: Ataque forte - Sucesso\n");
                } else {
                    printf("\nComando Vampiro Inimgo: Ataque forte - Falha\n");
                }
            }
            if (V1_Acao == 1) {
                V1_Acerto = Acerto();
                if (V1_Acerto <= V1_Precisao){
                    *V2_HP = *V2_HP - (V1_Atk / V2_Defesa);
                    *V1_HP = *V1_HP + ((V1_Atk / V2_Defesa) * V1_Life_Steal);
                    printf("\nComando Vampiro Jogador: Ataque rapido - Sucesso\n");
                } else {
                    printf("\nComando Vampiro Jogador: Ataque rapido - Falha\n");
                }
                V2_Atordoamento = Acerto();
                if (V2_Atordoamento <= (1 - V2_Precisao)) {
                    V2_Acao = 0;
                }
            } else if (V1_Acao == 2) {
                V1_Acerto = Acerto();
                if (V1_Acerto <= V1_Precisao){
                    *V2_HP = *V2_HP - ((2 * V1_Atk) / V2_Defesa);
                    printf("\nComando Vampiro Jogador: Ataque forte - Sucesso\n");
                } else {
                    printf("\nComando Vampiro Jogador: Ataque forte - Falha\n");
                }
            } else if (V1_Acao == 3) {
                V2_Acao = 0;
                *V1_HP = *V1_HP + (V2_Atk / V1_Defesa); /*Gambiarra para corrigir o fato de que ao defender um ataque forte, o dano causado deve ser reduzido pela metade*/
            }
        } else if (V2_Acao == 3) {
            if (V1_Acao == 1) {
                V1_Acerto = Acerto();
                if (V1_Acerto <= V1_Precisao){
                    *V2_HP = *V2_HP - (V1_Atk / V2_Defesa);
                    printf("\nComando Vampiro Jogador: Ataque rapido - Sucesso\n");
                } else {
                    printf("\nComando Vampiro Jogador: Ataque rapido - Falha\n");
                }
                V1_Atordoamento = Acerto();
                if (V1_Atordoamento <= 0.6) {
                    V1_Acao = 0;
                    printf("\nComando Vampiro Inimgo: Defesa - Atordoou voce\n");
                } else {
                    printf("\nComando Vampiro Inimgo: Defesa\n");
                }
            } else if (V1_Acao == 2) {
                V1_Acerto = Acerto();
                if (V1_Acerto <= V1_Precisao){
                    *V2_HP = *V2_HP - (V1_Atk / V2_Defesa);
                    printf("\nComando Vampiro Jogador: Ataque forte - Sucesso\n");
                } else {
                    printf("\nComando Vampiro Jogador: Ataque forte - Falha\n");
                }
                V1_Acao = 0;
                printf("\nComando Vampiro Inimgo: Defesa - Atordoou voce\n");
            } else {
                printf("\nComando Vampiro Inimgo: Defesa\n");
            }
        } else if (V2_Acao == 10) {
            if (V1_Acao == 1) {
                *V2_HP = *V2_HP - (V1_Atk / V2_Defesa);
                *V1_HP = *V1_HP + ((V1_Atk / V2_Defesa) * V1_Life_Steal);
                printf("\nComando Vampiro Jogador: Ataque rapido - Sucesso\n");
            } else if (V1_Acao == 2) {
                V2_HP = V2_HP - ((2 * V1_Atk) / V2_Defesa);
                printf("\nComando Vampiro Jogador: Ataque forte - Sucesso\n");
            }
        }

        if (V2_Acao == 5) {
            printf("\nUm verdadeiro vampiro nunca morre\n");
            printf("Parabens, voce venceu o duelo! Pressione enter para continuar:\n");
            scanf("%c", &Variavel_a_toa);
            flush_in();
            V2_HP = 0;
            return 0;
        } else if (V1_Acao == 5) {
            printf("\nVoce perdeu o duelo. Boa sorte na proxima. Pressione enter para continuar: \n");
            scanf("%c", &Variavel_a_toa);
            flush_in();
            *Escape = TRUE;
            V1_HP = 0;
            return 1;
        } else if (*V2_HP <= 0) {      /* Porque em Dark Souls se você matar o chefe e morrer ao mesmo tempo você ganha do mesmo jeito */
            printf("\nParabens, voce venceu o duelo! Pressione enter para continuar: \n");
            scanf("%c", &Variavel_a_toa);
            flush_in();
            return 0;
        } else if (*V1_HP <= 0) {
            printf("\nVoce perdeu o duelo. Boa sorte na proxima. Pressione enter para continuar\n");
            scanf("%c", &Variavel_a_toa);
            flush_in();
            *Escape = FALSE;
            return 1;
        }
        if (*V1_HP > *V1_Max_HP) {
            *V1_HP = *V1_Max_HP;
        }
        if (*V2_HP > V2_Max_HP) {
            *V2_HP = V2_Max_HP;
        }
    }
 }

int main(){
    int SAIR = FALSE;
    int Comparacao = FALSE;
    int Unica_comparacao = TRUE;
    int Desenvolvedor = 0;
    int Achou = FALSE;
    while (Jogar_Novamente() == 1){
        SAIR = FALSE;
        int const M, N, K;
        int mapa[30][80];
        char mapac[30][81];
        const int Player_Precisao = 70;
        char opcao, caractere;
        int i, j, l, Turnos = 10, Atualiza_X, Atualiza_Y, Armazena_X, Armazena_Y, X_ou_Y, X, Y, Turnos_Total = 0, Contador = 0, linha, coluna, Canto_Superior_Direito[2], Canto_Inferior_Esquerdo[2], Posicao_inicial_Y, Posicao_inicial_X, Contador_Portas = 0;
        int Contador_de_mortes = 0, Nivel, XP = 0, Derrota = 0, Escapar = 0, Portas[50], Portas_X[50], Portas_Y[50];
        float V_Life_Steal[21], V_Max_HP[21], V_Precisao[21], V_Def[21], V_HP[21];
        int Item_Ataque[5], Item_X[5],/* coluna*/Item_Y[5]/* linha */, Item_Tipo[3];
        float Player_HP = 100, Player_Max_HP = 100, Player_Life_Steal = 0, Player_Def = 1, Item_Life_Steal[5], Item_Defesa[5], Player_Armadura = 0;
        int V_Pocao[21], V_Atk[21], V_X[21]/* coluna */, V_Y[21] /* linha */, Vampiro[21], Vampiro_Level[21]; /* V = Vampiro lul, o V[10] é o Drácula */
        int Player_Pocao = 0, Player_Atk = 5, Player_XP = 0, Player_Lvl = 0, Player_Arma = 0;
        int Barra_de_XP = 2;
        int Player_Coluna_Atual, Player_Linha_Atual, Player_Coluna_Inicial, Player_Linha_Inicial;
        int Item_X_Fixo[5], Item_Y_Fixo[5];
        Derrota = 0;
        Item_X_Fixo[0] = 50;
        Item_Y_Fixo[0] = 1;
        Item_Ataque[0] = 5;
        Item_Defesa[0] = 0;
        Item_Tipo[0] = 1;
        Item_Life_Steal[0] = 0.3;
        Item_X_Fixo[1] = 21;
        Item_Y_Fixo[1] = 1;
        Item_Tipo[1] = 0;
        Item_Ataque[1] = 0;
        Item_Defesa[1] = 0;
        Item_Life_Steal[1] = 0.0;
        Item_X_Fixo[2] = 21;
        Item_Y_Fixo[2] = 18;
        Item_Tipo[2] = 2;
        Item_Ataque[2] = 0;
        Item_Defesa[2] = 1.0;
        Item_Life_Steal[2] = 0.0;
        Item_X_Fixo[3] = 68;
        Item_Y_Fixo[3] = 14;
        Item_Tipo[3] = 0;
        Item_Ataque[3] = 0;
        Item_Defesa[3] = 0.0;
        Item_Life_Steal[3] = 0.0;
        Item_X_Fixo[4] = 41;
        Item_Y_Fixo[4] = 12;
        Item_Tipo[4] = 0;
        Item_Ataque[4] = 0;
        Item_Defesa[4] = 0.0;
        Item_Life_Steal[4] = 0.0;
        for (i = 0; i < 20; i++) {
            Portas[i] = 0;
        }
        char mover = 'p';
        int *Ptr_Player_HP = &Player_HP;
        int *Ptr_Player_Max_HP = &Player_Max_HP;
        int *Ptr_Player_Pocao = &Player_Pocao;
        int *Ptr_V_Pocao = &V_Pocao;
        int *Ptr_Escapar = &Escapar;
        int *Ptr_Item_Tipo = &Item_Tipo;
        float *Ptr_Item_Life_Steal = &Item_Life_Steal;
        int *Ptr_Item_Ataque = &Item_Ataque;
        float *Ptr_Item_Defesa = &Item_Defesa;
        int *Ptr_Item_X = &Item_X;
        int *Ptr_Item_Y = &Item_Y;
        float *Ptr_V_HP = &V_HP;
        while (opcao != 's' && opcao != 'S' && opcao != 'n' && opcao != 'N') {
            flush_in();
            printf("\nPara iniciar o jogo e necessario acessar arquivos localizados na pasta do mesmo. Voce autoriza a abertura do arquivo mapa.txt e do arquivo jogo.bin S ou N?\n");
            scanf("%c", &opcao);
        }
        if (opcao == 'n' || opcao =='N') {
            return 0;
        } else {
            FILE *mapa_arquivo = fopen("mapa.txt", "r");
            FILE *jogo_arquivo_leitura = fopen("jogo.bin", "rb+");
            if (mapa_arquivo != NULL) {
                fclose(mapa_arquivo);
            }
            if (jogo_arquivo_leitura != NULL) {
                fclose(jogo_arquivo_leitura);
            }
            mapa_arquivo = fopen("mapa.txt", "r");
            jogo_arquivo_leitura = fopen("jogo.bin", "rb+");
            fscanf(mapa_arquivo, "%d %d", &M, &N);
            fgetc(mapa_arquivo);
            for (i = 0; i < N; i++){
                for (j = 0; j < M; j++) {
                    fscanf(mapa_arquivo, "%c", &caractere);
                    mapac[i][j] = caractere;
                }
                fgetc(mapa_arquivo);
            }
            printf("\nEste foi o mapa carregado: \n\n");
            for (linha=0;linha<N;linha++) {
                for (coluna=0;coluna<M;coluna++) {
                    printf("%c", mapac[linha][coluna]);
                    if (mapac[linha][coluna] == '@')
                        mapa[linha][coluna] = PERSONAGEM;
                    if (mapac[linha][coluna] == ' ')
                        mapa[linha][coluna] = ESPACO;
                    if (mapac[linha][coluna] == '#')
                        mapa[linha][coluna] = PAREDE;
                    if (mapac[linha][coluna] == 'V')
                        mapa[linha][coluna] = VAMPIRO;
                    if (mapac[linha][coluna] == 'D')
                        mapa[linha][coluna] = DRACULA;
                    if (mapac[linha][coluna] == '-')
                        mapa[linha][coluna] = PORTA_ABERTA;
                    if (mapac[linha][coluna] == '+')
                        mapa[linha][coluna] = PORTA_FECHADA;
                }
                printf("\n");
            }
            flush_in();
            for (i = 0; i < 30; i++) {
                for (j = M; j < 80; j++) {
                    mapa[i][j] = ESPACO;
                }
            }
            for (i = N; i < 30; i++) {
                for (j = 0; j < 80; j++) {
                    mapa[i][j] = ESPACO;
                }
            }
            printf("\n\nAperte enter para continuar\n");
            scanf("%c", &opcao);

            while (opcao != 's' && opcao != 'S' && opcao != 'n' && opcao != 'N') {
                printf("Deseja comecar o jogo do inicio S ou N?\n");
                scanf("%c", &opcao);
                flush_in();
            }
            if (opcao == 's' || opcao == 'S') {
                jogo_arquivo_leitura = NULL;
            }
            if (jogo_arquivo_leitura == NULL) {
                printf("\nKkk eae men, ou e a sua primeira vez executanto o jogo ou algum problema ocorreu ao abrir o arquivo jogo.bin. De qualquer maneira, bora comecar do 0. Aperte enter para continuar\n");
                scanf("%c", &opcao);
                fscanf(mapa_arquivo, "%d %d", &Player_Coluna_Inicial, &Player_Linha_Inicial);
                fgetc(mapa_arquivo);
                fscanf(mapa_arquivo, "%d", &K);
                fgetc(mapa_arquivo);
                Player_Coluna_Atual = Player_Coluna_Inicial;
                Player_Linha_Atual = Player_Linha_Inicial;
                for (i = 0; i <= K; i++) {
                    fscanf(mapa_arquivo, "%d %d %d", &V_X[i], &V_Y[i], &Vampiro_Level[i]);
                    fgetc(mapa_arquivo);
                }
            } else {
                fread(Portas, sizeof(Portas), 1, jogo_arquivo_leitura);
                fread(V_X, sizeof(V_X), 1, jogo_arquivo_leitura);
                fread(V_Y, sizeof(V_Y), 1, jogo_arquivo_leitura);
                fread(Vampiro_Level, sizeof(Vampiro_Level), 1, jogo_arquivo_leitura);
                fread(&Player_Linha_Atual, sizeof(int), 1, jogo_arquivo_leitura);
                fread(&Player_Coluna_Atual, sizeof(int), 1, jogo_arquivo_leitura);
                fread(&Player_Lvl, sizeof(int), 1, jogo_arquivo_leitura);
                fread(&Player_Armadura, sizeof(float), 1, jogo_arquivo_leitura);
                fread(&Player_Arma, sizeof(int), 1, jogo_arquivo_leitura);
                fread(&Player_Life_Steal, sizeof(float), 1, jogo_arquivo_leitura);
                fread(&Player_Pocao, sizeof(int), 1, jogo_arquivo_leitura);
                fread(&Player_HP, sizeof(float), 1, jogo_arquivo_leitura);
                fread(&Player_XP, sizeof(int), 1, jogo_arquivo_leitura);
                fread(&Barra_de_XP, sizeof(int), 1, jogo_arquivo_leitura);
                fread(&Comparacao, sizeof(int), 1, jogo_arquivo_leitura);
                fread(&Unica_comparacao, sizeof(int), 1, jogo_arquivo_leitura);
                fread(&Contador_Portas, sizeof(int), 1, jogo_arquivo_leitura);
                fread(&Contador_de_mortes, sizeof(int), 1, jogo_arquivo_leitura);
                fread(Portas_X, sizeof(Portas_X), 1, jogo_arquivo_leitura);
                fread(Portas_Y, sizeof(Portas_Y), 1, jogo_arquivo_leitura);
                fread(&Player_Linha_Inicial, sizeof(int), 1, jogo_arquivo_leitura);
                fread(&Player_Coluna_Inicial, sizeof(int), 1, jogo_arquivo_leitura);
                fread(&K, sizeof(int), 1, jogo_arquivo_leitura);
            }
            fclose(jogo_arquivo_leitura);
            fclose(mapa_arquivo);
        }
        for (i = 0; i < 50; i++)  {
            if (Portas[i] == TRUE && Comparacao == FALSE) {
                Armazena_X = Portas_X[i];
                Armazena_Y = Portas_Y[i];
                mapa[Armazena_Y][Armazena_X] = PORTA_ABERTA;
            }
            Achou = FALSE;
        }
        Player_Atk = 5 + Player_Lvl + Player_Arma;
        Player_Def = 1 + (0.1 * Player_Lvl) + Player_Armadura;
        Player_Max_HP = 100 + (25 * Player_Lvl);
        for (i = 0; i <= K; i++) {
            V_Atk[i] = 6 + (2 * Vampiro_Level[i]);
            V_Def[i] = 1.0 + (0.1 * Vampiro_Level[i]);
            V_Life_Steal[i] = 0;
            V_Pocao[i] = 0;
            V_Max_HP[i] = 100 + (25 * Vampiro_Level[i]);
            V_Precisao[i] = 0.6 + (0.01 * Vampiro_Level[i]);
            if (i == 10) {
                V_Life_Steal[10] = 0.7;
                V_Pocao[10] = 3;
            }
        }
        printf("\n\nDeseja iniciar o jogo no modo desenvolvedor?. 1 para sim, 0 para nao\n");
        scanf("%d", &Desenvolvedor);
        if (Desenvolvedor == 1) {
            Player_Atk = 100;
        }
        for (i = 0; i <= K; i++) {
            Vampiro[i] = TRUE;
            V_HP[i] = V_Max_HP[i];
            Armazena_X = V_X[i];
            Armazena_Y = V_Y[i];
            if (mapa[Armazena_Y][Armazena_X] == PAREDE) {
                for (l = Armazena_Y - 1; l <= Armazena_Y + 1; l++){
                    for (j = Armazena_X - 1; j <= Armazena_X + 1; j++) {
                        if (mapa[l][j] == ESPACO) {
                            V_X[i] = Armazena_X;
                            V_Y[i] = Armazena_Y;
                            break;
                        }
                    }
                    if (mapa[l][j] == ESPACO) {                                                    /* Para qualquer mapa, se as entidades que interagem entre si nascem em paredes, é corrigido */
                        break;
                    }
                }
            }
        }
        Armazena_X = V_X[10];
        Armazena_Y = V_Y[10];
        mapa[Armazena_Y][Armazena_X] = DRACULA;
        flush_in();
        for (i = 0; i < 5; i++){
            Item_X[i] = Item_X_Fixo[i];
            Item_Y[i] = Item_Y_Fixo[i];
            Armazena_X = Item_X[i];
            Armazena_Y = Item_Y[i];
            if (mapa[Armazena_Y][Armazena_X] == PAREDE) {
                for (l = Armazena_Y - 1; l <= Armazena_Y + 1; l++){
                    for (j = Armazena_X - 1; j <= Armazena_X + 1; j++) {
                        if (mapa[l][j] == ESPACO) {
                            Item_X[i] = Armazena_X;
                            Item_Y[i] = Armazena_Y;
                            break;
                        }
                    }
                    if (mapa[l][j] == ESPACO) {
                        break;
                    }
                }
            }
        }
        while (Contador_de_mortes < 5) {
            if (SAIR == TRUE) {
                break;
            }
            if (Derrota == 1) {
                Player_Coluna_Atual = Player_Coluna_Inicial;
                Player_Linha_Atual = Player_Linha_Inicial;
            }
            Derrota = 0;
            if (Escapar == TRUE){
                Escapar = 0;
            } else if (Escapar == FALSE) {
                Player_HP = Player_Max_HP;
                Player_Def-= Player_Armadura;
                Player_Armadura = 0;
                Player_Atk-= Player_Arma;
                Player_Arma = 0;
                Player_Life_Steal = 0;
                Player_Pocao = 0;
                Player_XP = Barra_de_XP / 2;
                Contador_de_mortes+= 1;
                for (i = 0; i < 5; i++){
                    Gerar_Itens(Ptr_Item_Ataque, Ptr_Item_Defesa, Ptr_Item_Tipo, Ptr_Item_Life_Steal, Ptr_Item_X, Ptr_Item_Y, Item_X_Fixo[i], Item_Y_Fixo[i]);
                }
                Escapar = 0;
            }
            while (Derrota == 0) {
                Armazena_X = 0;
                Armazena_Y = 0;
                Atualiza_X = 0;
                Atualiza_Y = 0;
                system("clear");
                fazer_Mapa(mapa, Player_Coluna_Atual, Player_Linha_Atual);
                printf("\n\nLEVEL                   %d\n", Player_Lvl);
                printf("EXPERIENCE            %d/%d\n", Player_XP, Barra_de_XP);
                printf("HIT POINTS        %.0f/%.0f\n", Player_HP, Player_Max_HP);
                printf("ATAQUE                  %d\n", Player_Atk);
                printf("DEFESA                 %.1f\n", Player_Def);
                printf("LIFESTEAL             %.1f\n", Player_Life_Steal);
                printf("POCOES                  %d\n", Player_Pocao);
                printf("PRECISAO               %d\n", Player_Precisao);
                printf("\n\n  (w)                            K - se estiver preso, abre as portas da sala atual\n(a)+(d)                          Z - salvar o jogo\n  (s)                            X - salvar o jogo e sair\n\n"); /* Imprime as direcoes */
                while (mover != 'w' && mover != 'a' && mover != 's' && mover != 'd' && mover != 'W' && mover != 'A' && mover != 'S' && mover != 'D' && mover != 'x' && mover != 'X' && mover != 'z' && mover != 'Z' && mover!= 'k' && mover != 'K'){
                    if (scanf("%c", &mover)==0){
                        return FALSE;
                    }
                    /*up*/
                    if ((mover == 'w') || (mover == 'W')) {
                        Player_Linha_Atual = Player_Linha_Atual - 1;
                        flush_in();
                        /* Caso entrar em conflito com a parede, retorna na posição anterior */
                        if ((mapa[Player_Linha_Atual][Player_Coluna_Atual] != ESPACO) && (mapa[Player_Linha_Atual][Player_Coluna_Atual] != PORTA_ABERTA) && (mapa[Player_Linha_Atual][Player_Coluna_Atual] != MORTO) && (mapa[Player_Linha_Atual][Player_Coluna_Atual] != ARMADURA) && (mapa[Player_Linha_Atual][Player_Coluna_Atual] != POCAO) && (mapa[Player_Linha_Atual][Player_Coluna_Atual] != ARMA)) {
                            Player_Linha_Atual = Player_Linha_Atual + 1;
                            mover = 'g';
                            printf("\n\nkkk eae men, pode ir ai nao\n");
                            getchar();
                        }
                    }

                    /*down*/
                    if ((mover == 's') || (mover == 'S')) {
                        Player_Linha_Atual = Player_Linha_Atual + 1;
                        flush_in();
                        /* Caso entrar em conflito com a parede, retorna na posição anterior */
                        if ((mapa[Player_Linha_Atual][Player_Coluna_Atual] != ESPACO) && (mapa[Player_Linha_Atual][Player_Coluna_Atual] != MORTO) && (mapa[Player_Linha_Atual][Player_Coluna_Atual] != PORTA_ABERTA) && (mapa[Player_Linha_Atual][Player_Coluna_Atual] != ARMADURA) && (mapa[Player_Linha_Atual][Player_Coluna_Atual] != POCAO) && (mapa[Player_Linha_Atual][Player_Coluna_Atual] != ARMA)) {
                            Player_Linha_Atual = Player_Linha_Atual - 1;
                            mover = 'g';
                            printf("kkk eae men, pode ir ai nao\n");
                            getchar();
                        }
                    }

                    /*right*/
                    if ((mover == 'd') || (mover == 'D')) {
                        Player_Coluna_Atual = Player_Coluna_Atual + 1;
                        flush_in();
                        /* Caso entrar em conflito com a parede, retorna na posição anterior */
                        if ((mapa[Player_Linha_Atual][Player_Coluna_Atual] != ESPACO) && (mapa[Player_Linha_Atual][Player_Coluna_Atual] != MORTO) &&  (mapa[Player_Linha_Atual][Player_Coluna_Atual] != PORTA_ABERTA) && (mapa[Player_Linha_Atual][Player_Coluna_Atual] != ARMADURA) && (mapa[Player_Linha_Atual][Player_Coluna_Atual] != POCAO) && (mapa[Player_Linha_Atual][Player_Coluna_Atual] != ARMA)) {
                            Player_Coluna_Atual = Player_Coluna_Atual - 1;
                            mover = 'g';
                            printf("\n\nkkk eae men, pode ir ai nao\n");
                            getchar();
                        }
                    }

                    /* left */
                    if ((mover == 'a') || (mover == 'A')) {
                        Player_Coluna_Atual = Player_Coluna_Atual - 1;
                        flush_in();
                        /* Caso entrar em conflito com a parede, retorna na posição anterior */
                        if ((mapa[Player_Linha_Atual][Player_Coluna_Atual] != ESPACO) && (mapa[Player_Linha_Atual][Player_Coluna_Atual] != MORTO) && (mapa[Player_Linha_Atual][Player_Coluna_Atual] != PORTA_ABERTA) && (mapa[Player_Linha_Atual][Player_Coluna_Atual] != ARMADURA) && (mapa[Player_Linha_Atual][Player_Coluna_Atual] != POCAO) && (mapa[Player_Linha_Atual][Player_Coluna_Atual] != ARMA)) {
                            Player_Coluna_Atual = Player_Coluna_Atual + 1;
                            mover = 'g';
                            printf("\n\nkkk eae men, pode ir ai nao\n");
                            getchar();
                        }
                    }
                    if (mover == 'z' || mover == 'Z') {
                        opcao = 'a';
                        while (opcao != 's' && opcao != 'S' && opcao != 'n' && opcao != 'N') {
                            flush_in();
                            printf("Para salvar o arquivo do jogo, e necessario acessar o arquivo jogo.bin. Autoriza a abertura do mesmo? S ou N\n");
                            scanf("%c", &opcao);
                        }
                        FILE *jogo_arquivo_escrita = fopen("jogo.bin", "wb+");
                        if (jogo_arquivo_escrita != NULL) {
                            fclose(jogo_arquivo_escrita);
                        }
                        if (opcao == 'S' || opcao == 's') {
                            jogo_arquivo_escrita = fopen("jogo.bin", "wb+");
                            if (!jogo_arquivo_escrita) {
                                printf("Ocorreu um erro ao tentar abrir o arquivo jogo.bin. Pressione enter para continuar:");
                                scanf("%c", &opcao);
                                fclose(jogo_arquivo_escrita);
                            } else {
                                fwrite(Portas, sizeof(Portas), 1, jogo_arquivo_escrita);
                                fwrite(V_X, sizeof(V_X), 1, jogo_arquivo_escrita);
                                fwrite(V_Y, sizeof(V_Y), 1, jogo_arquivo_escrita);
                                fwrite(Vampiro_Level, sizeof(Vampiro_Level), 1, jogo_arquivo_escrita);
                                fwrite(&Player_Linha_Atual, sizeof(int), 1, jogo_arquivo_escrita);
                                fwrite(&Player_Coluna_Atual, sizeof(int), 1, jogo_arquivo_escrita);
                                fwrite(&Player_Lvl, sizeof(int), 1, jogo_arquivo_escrita);
                                fwrite(&Player_Armadura, sizeof(float), 1, jogo_arquivo_escrita);
                                fwrite(&Player_Arma, sizeof(int), 1, jogo_arquivo_escrita);
                                fwrite(&Player_Life_Steal, sizeof(float), 1, jogo_arquivo_escrita);
                                fwrite(&Player_Pocao, sizeof(int), 1, jogo_arquivo_escrita);
                                fwrite(&Player_HP, sizeof(float), 1, jogo_arquivo_escrita);
                                fwrite(&Player_XP, sizeof(int), 1, jogo_arquivo_escrita);
                                fwrite(&Barra_de_XP, sizeof(int), 1, jogo_arquivo_escrita);
                                fwrite(&Comparacao, sizeof(int), 1, jogo_arquivo_escrita);
                                fwrite(&Unica_comparacao, sizeof(int), 1, jogo_arquivo_escrita);
                                fwrite(&Contador_Portas, sizeof(int), 1, jogo_arquivo_escrita);
                                fwrite(&Contador_de_mortes, sizeof(int), 1, jogo_arquivo_escrita);
                                fwrite(Portas_X, sizeof(Portas_X), 1, jogo_arquivo_escrita);
                                fwrite(Portas_Y, sizeof(Portas_Y), 1, jogo_arquivo_escrita);
                                fwrite(&Player_Linha_Inicial, sizeof(int), 1, jogo_arquivo_escrita);
                                fwrite(&Player_Coluna_Inicial, sizeof(int), 1, jogo_arquivo_escrita);
                                fwrite(&K, sizeof(int), 1, jogo_arquivo_escrita);
                                fclose(jogo_arquivo_escrita);
                            }
                        }
                    }
                    if (mover == 'x' || mover == 'X') {
                        opcao = 'a';
                        while (opcao != 's' && opcao != 'S' && opcao != 'n' && opcao != 'N') {
                            flush_in();
                            printf("Para salvar o arquivo do jogo, e necessario acessar o arquivo jogo.bin. Autoriza a abertura do mesmo? S ou N\n");
                            scanf("%c", &opcao);
                        }
                        FILE *jogo_arquivo_escrita = fopen("jogo.bin", "wb+");
                        if (jogo_arquivo_escrita != NULL) {
                            fclose(jogo_arquivo_escrita);
                        }
                        if (opcao == 'S' || opcao == 's') {
                            jogo_arquivo_escrita = fopen("jogo.bin", "wb+");
                            if (!jogo_arquivo_escrita) {
                                printf("Ocorreu um erro ao tentar abrir o arquivo jogo.bin. Pressione enter para continuar:");
                                scanf("%c", &opcao);
                                fclose(jogo_arquivo_escrita);
                            } else {
                                fwrite(Portas, sizeof(Portas), 1, jogo_arquivo_escrita);
                                fwrite(V_X, sizeof(V_X), 1, jogo_arquivo_escrita);
                                fwrite(V_Y, sizeof(V_Y), 1, jogo_arquivo_escrita);
                                fwrite(Vampiro_Level, sizeof(Vampiro_Level), 1, jogo_arquivo_escrita);
                                fwrite(&Player_Linha_Atual, sizeof(int), 1, jogo_arquivo_escrita);
                                fwrite(&Player_Coluna_Atual, sizeof(int), 1, jogo_arquivo_escrita);
                                fwrite(&Player_Lvl, sizeof(int), 1, jogo_arquivo_escrita);
                                fwrite(&Player_Armadura, sizeof(float), 1, jogo_arquivo_escrita);
                                fwrite(&Player_Arma, sizeof(int), 1, jogo_arquivo_escrita);
                                fwrite(&Player_Life_Steal, sizeof(float), 1, jogo_arquivo_escrita);
                                fwrite(&Player_Pocao, sizeof(int), 1, jogo_arquivo_escrita);
                                fwrite(&Player_HP, sizeof(float), 1, jogo_arquivo_escrita);
                                fwrite(&Player_XP, sizeof(int), 1, jogo_arquivo_escrita);
                                fwrite(&Barra_de_XP, sizeof(int), 1, jogo_arquivo_escrita);
                                fwrite(&Comparacao, sizeof(int), 1, jogo_arquivo_escrita);
                                fwrite(&Unica_comparacao, sizeof(int), 1, jogo_arquivo_escrita);
                                fwrite(&Contador_Portas, sizeof(int), 1, jogo_arquivo_escrita);
                                fwrite(&Contador_de_mortes, sizeof(int), 1, jogo_arquivo_escrita);
                                fwrite(Portas_X, sizeof(Portas_X), 1, jogo_arquivo_escrita);
                                fwrite(Portas_Y, sizeof(Portas_Y), 1, jogo_arquivo_escrita);
                                fwrite(&Player_Linha_Inicial, sizeof(int), 1, jogo_arquivo_escrita);
                                fwrite(&Player_Coluna_Inicial, sizeof(int), 1, jogo_arquivo_escrita);
                                fwrite(&K, sizeof(int), 1, jogo_arquivo_escrita);
                                fclose(jogo_arquivo_escrita);
                            }
                            SAIR = TRUE;
                        }
                    }
                    if (mover == 'k' || mover == 'K') {
                        Armazena_X = Player_Coluna_Atual;           /*Verifica a área da sala e abre as portas*/
                        Armazena_Y = Player_Linha_Atual;
                        Posicao_inicial_Y = Armazena_Y;
                        Posicao_inicial_X = Armazena_X;
                        while (mapa[Armazena_Y - 1][Armazena_X] != PAREDE && mapa[Armazena_Y - 1][Armazena_X] != PORTA_ABERTA && mapa[Armazena_Y - 1][Armazena_X] != PORTA_FECHADA){
                            Armazena_Y-= 1;
                        }
                        while (mapa[Armazena_Y][Armazena_X + 1] != PAREDE && mapa[Armazena_Y][Armazena_X + 1] != PORTA_ABERTA && mapa[Armazena_Y][Armazena_X + 1] != PORTA_FECHADA) {
                            Armazena_X += 1;
                        }
                        Canto_Superior_Direito[0] = Armazena_X + 1;
                        Canto_Superior_Direito[1] = Armazena_Y - 1;
                        Armazena_X = Posicao_inicial_X;
                        Armazena_Y = Posicao_inicial_Y;
                        while (mapa[Armazena_Y + 1][Armazena_X] != PAREDE && mapa[Armazena_Y + 1][Armazena_X] != PORTA_ABERTA && mapa[Armazena_Y + 1][Armazena_X] != PORTA_FECHADA){
                            Armazena_Y+= 1;
                        }
                        while (mapa[Armazena_Y][Armazena_X - 1] != PAREDE && mapa[Armazena_Y][Armazena_X - 1] != PORTA_ABERTA && mapa[Armazena_Y][Armazena_X - 1] != PORTA_FECHADA) {
                            Armazena_X-= 1;
                        }
                        Canto_Inferior_Esquerdo[0] = Armazena_X - 1;
                        Canto_Inferior_Esquerdo[1] = Armazena_Y + 1;
                        for (j = Canto_Superior_Direito[1]; j <= Canto_Inferior_Esquerdo[1]; j++) {
                            for (l = Canto_Inferior_Esquerdo[0]; l <= Canto_Superior_Direito[0]; l++)  {
                                if (mapa[j][l] == PORTA_FECHADA) {
                                    mapa[j][l] = PORTA_ABERTA;
                                    Portas_X[Contador_Portas] = l;
                                    Portas_Y[Contador_Portas] = j;
                                    Portas[Contador_Portas] = TRUE;
                                    Contador_Portas+= 1;
                                }
                            }
                        }
                    }
                }
                if (SAIR == TRUE){
                    break;
                }
                for (i = 0; i < 5; i++){
                    if ((Player_Coluna_Atual == Item_X[i]) && (Player_Linha_Atual == Item_Y[i])){
                        if (Item_Tipo[i] == 0) {
                            Player_Pocao+= 1;
                        } else if (Item_Tipo[i] == 1) {
                            Player_Atk-= Player_Arma;
                            Player_Arma = Item_Ataque[i];
                            Player_Life_Steal = Item_Life_Steal[i];
                            Player_Atk+= Player_Arma;

                        } else if (Item_Tipo[i] == 2){
                            Player_Def-= Player_Armadura;
                            Player_Armadura = Item_Defesa[i];
                            Player_Def+= Player_Armadura;
                        }
                        Armazena_X = Item_X[i];
                        Armazena_Y = Item_Y[i];
                        mapa[Armazena_Y][Armazena_X] = ESPACO;
                        Item_X[i] = 0;
                        Item_Y[i] = 0;
                        Turnos = 0; /* Por algum motivo, se o jogador pegasse um item no momento em que os itens seriam gerados de novo, o jogo crashava, isso resolve o problema*/
                    }
                }
                i = 0;
                for (i = 0; i <= K; i++){
                    if (Vampiro[i] == TRUE) {
                        Armazena_X = V_X[i];
                        Armazena_Y = V_Y[i];
                        Ptr_V_HP = &V_HP[i];
                        Ptr_V_Pocao = &V_Pocao[i];
                        /* não leia tudo isso, só confia que essa é a verificação para as condições necessárias para o início de um combate kkk */
                        if (((Player_Linha_Atual + 2 == Armazena_Y) && (Player_Coluna_Atual == Armazena_X) && mapa[Player_Linha_Atual + 1][Player_Coluna_Atual] != PAREDE) || ((Player_Coluna_Atual + 2 == Armazena_X) && (Player_Linha_Atual == Armazena_Y) && mapa[Player_Linha_Atual + 1][Player_Coluna_Atual] != PAREDE) || ((Player_Linha_Atual - 2 == Armazena_Y) && (Player_Coluna_Atual == Armazena_X) && mapa[Player_Linha_Atual - 1][Player_Coluna_Atual] != PAREDE) || ((Player_Coluna_Atual - 2 == Armazena_X) && (Player_Linha_Atual == Armazena_Y) && mapa[Player_Linha_Atual - 1][Player_Coluna_Atual] != PAREDE) || ((Player_Linha_Atual + 1 == Armazena_Y) && (Player_Coluna_Atual == Armazena_X)) || ((Player_Coluna_Atual + 1 == Armazena_X) && (Player_Linha_Atual == Armazena_Y)) || ((Player_Linha_Atual - 1 == Armazena_Y) && (Player_Coluna_Atual == Armazena_X)) || ((Player_Coluna_Atual - 1 == Armazena_X) && (Player_Linha_Atual == Armazena_Y))) {
                            Derrota = Combate(Ptr_Player_HP, Ptr_Player_Max_HP, Ptr_V_HP, V_Max_HP[i], Player_Atk, V_Atk[i], Player_Def, V_Def[i], Player_Life_Steal, V_Life_Steal[i], Ptr_Player_Pocao, Ptr_V_Pocao, V_Precisao[i], Ptr_Escapar);
                            if (Derrota == 0) {
                                Turnos = 0;
                                mapa[Armazena_Y][Armazena_X] = MORTO;
                                Vampiro[i] = FALSE;
                                XP = (60 * i) + 60;
                                if (i >= 5) {
                                    XP+= (100 * i) + 150;
                                } else if (i >= 8){
                                    XP+= (1000 * i) + 200;
                                }
                                Player_XP = XP;
                                while (Player_XP > Barra_de_XP) {
                                    Barra_de_XP*= 2;
                                    Player_Lvl+= 1;
                                    Player_Atk+= 1;
                                    Player_Def+= 0.1;
                                    Player_Max_HP+= 25;
                                    Player_HP+= 25;
                                }
                                break;
                            } else if (Derrota == 1) {
                                Contador_de_mortes+= 1;
                                for (j = 0; j <= K; j++){
                                    Player_Coluna_Atual = Player_Coluna_Inicial;
                                    Player_Linha_Atual = Player_Linha_Inicial;
                                    V_Max_HP[j]+= 25;
                                    V_Atk[j]+= 2;
                                    V_Def[j]+= 0.1;
                                    V_HP[j]+= 25;
                                    V_Precisao[j]+= 0.01;
                                    Vampiro_Level[j]+= 1;
                                    break;
                                }
                                break;
                            }
                        }
                        if (i != K) {
                            srand(time(NULL));
                            Armazena_X = V_X[i];
                            Armazena_Y = V_Y[i];
                            Atualiza_X = V_X[i];
                            Atualiza_Y = V_Y[i];
                            X_ou_Y = rand()%99 + 1;
                            if (X_ou_Y %2 == 0) {
                                X = Vampiros_Movimento();
                                Atualiza_X = Atualiza_X + (-1 * X);
                                if (((mapa[Atualiza_Y][Atualiza_X] == ESPACO) || (mapa[Atualiza_Y][Atualiza_X] == ARMADURA) || (mapa[Atualiza_Y][Atualiza_X == POCAO]) || (mapa[Atualiza_Y][Atualiza_X] == ARMA)) && (mapa[Atualiza_Y][Atualiza_X] != PAREDE) && (mapa[Atualiza_Y][Atualiza_X] != PORTA_ABERTA) && (mapa[Atualiza_Y][Atualiza_X] != PORTA_FECHADA)) {
                                    mapa[Atualiza_Y][Atualiza_X] = VAMPIRO;
                                    mapa[Armazena_Y][Armazena_X] = ESPACO;
                                    V_X[i] = Atualiza_X;
                                } else {
                                    V_X[i] = Armazena_X;
                                    Atualiza_X = Armazena_X;            /* Essa parte do código "decide" qual variável do vampiro será alterada: linha ou coluna; e verifica se a posição nova é válida, senão, retorna à posição anterior */
                                }
                            } else if (X_ou_Y %2 == 1){
                                Y = Vampiros_Movimento();
                                Atualiza_Y = Atualiza_Y + (-1 * Y);
                                if (((mapa[Atualiza_Y][Atualiza_X] == ESPACO) || (mapa[Atualiza_Y][Atualiza_X] == ARMADURA) || (mapa[Atualiza_Y][Atualiza_X == POCAO]) || (mapa[Atualiza_Y][Atualiza_X] == ARMA)) && (mapa[Atualiza_Y][Atualiza_X] != PAREDE) && (mapa[Atualiza_Y][Atualiza_X] != PORTA_ABERTA) && (mapa[Atualiza_Y][Atualiza_X] != PORTA_FECHADA)) {
                                    mapa[Atualiza_Y][Atualiza_X] = VAMPIRO;
                                    mapa[Armazena_Y][Armazena_X] = ESPACO;
                                    V_Y[i] = Atualiza_Y;
                                } else {
                                    V_Y[i] = Armazena_Y;
                                    Atualiza_Y = Armazena_Y;
                                }
                            }
                        }
                    }
                }
                if (mapa[Player_Linha_Atual - 1][Player_Coluna_Atual] == DRACULA){
                    Ptr_V_HP = &V_HP[K];
                    Ptr_V_Pocao = &V_Pocao[K];
                    Derrota = Combate(Ptr_Player_HP, Ptr_Player_Max_HP, Ptr_V_HP, V_Max_HP[10], Player_Atk, V_Atk[10], Player_Def, V_Def[10], Player_Life_Steal, V_Life_Steal[10], Ptr_Player_Pocao, Ptr_V_Pocao, V_Precisao[10], Ptr_Escapar);
                }
                if (Unica_comparacao = TRUE){
                    if (mapa[9][70] == PORTA_ABERTA) {
                        Comparacao = TRUE;
                    }
                    Unica_comparacao = FALSE;
                }
                if (Comparacao == TRUE) { /* tentei de outros métodos mas meu mapa é irregular e precisei fazer essa gambiarra */
                    if (Vampiro[0] == FALSE || Portas[0] == TRUE) {
                        mapa[8][11] = PORTA_ABERTA;
                        Portas[0] = TRUE;
                    }
                    if (Vampiro[1] == FALSE || (Portas[1] == TRUE && Portas[2] == TRUE)) {
                        mapa[1][11] = PORTA_ABERTA;
                        mapa[5][30] = PORTA_ABERTA;
                        Portas[1] = TRUE;
                        Portas[2] = TRUE;
                    }
                    if (Vampiro[2] == FALSE || Portas[3] == TRUE) {
                        mapa[9][25] = PORTA_ABERTA;
                        Portas[3] = TRUE;
                    }
                    if (Vampiro[3] == FALSE || Portas[4] == TRUE) {
                        mapa[11][11] = PORTA_ABERTA;
                        Portas[4] = TRUE;
                    }
                    if (Vampiro[4] == FALSE || Portas[5] == TRUE) {
                        mapa[16][11] = PORTA_ABERTA;
                        Portas[5] = TRUE;
                    }
                    if (Vampiro[5] == FALSE || (Portas[6] == TRUE && Portas[7] == TRUE)) {
                        mapa[15][36] = PORTA_ABERTA;
                        mapa[14][48] = PORTA_ABERTA;
                        Portas[6] = TRUE;
                        Portas[7] = TRUE;
                    }
                    if (Vampiro[6] == FALSE || Portas[8] == TRUE) {
                        mapa[12][57] = PORTA_ABERTA;
                    }
                    if (Vampiro[7] == FALSE || (Portas[9] == TRUE && Portas[10] == TRUE)) {
                        mapa[13][60] = PORTA_ABERTA;
                        mapa[7][54] = PORTA_ABERTA;
                        Portas[9] = TRUE;
                        Portas[10] = TRUE;
                    }
                    if (Vampiro[8] == FALSE || Portas[11] == TRUE) {
                        mapa[18][10] = PORTA_ABERTA;
                        Portas[11] = TRUE;
                    }
                    if (Vampiro[9] == FALSE || (Portas[12] == TRUE && Portas[13] == TRUE && Portas[14] == TRUE)) {
                        mapa[22][62] = PORTA_ABERTA;
                        mapa[18][68] = PORTA_ABERTA;
                        mapa[23][25] = PORTA_ABERTA;
                        Portas[13] = TRUE;
                        Portas[14] = TRUE;
                        Portas[15] = TRUE;
                    }
                } else {
                    for (i = 0; i < K; i++) {
                        if (Vampiro[i] == FALSE) {
                            Armazena_X = V_X[i];           /*Verifica a área da sala e abre as portas*/
                            Armazena_Y = V_Y[i];
                            Posicao_inicial_Y = Armazena_Y;
                            Posicao_inicial_X = Armazena_X;
                            while (mapa[Armazena_Y - 1][Armazena_X] != PAREDE && mapa[Armazena_Y - 1][Armazena_X] != PORTA_ABERTA && mapa[Armazena_Y - 1][Armazena_X] != PORTA_FECHADA){
                                Armazena_Y-= 1;
                            }
                            while (mapa[Armazena_Y][Armazena_X + 1] != PAREDE && mapa[Armazena_Y][Armazena_X + 1] != PORTA_ABERTA && mapa[Armazena_Y][Armazena_X + 1] != PORTA_FECHADA) {
                                Armazena_X += 1;
                            }
                            Canto_Superior_Direito[0] = Armazena_X + 1;
                            Canto_Superior_Direito[1] = Armazena_Y - 1;
                            Armazena_X = Posicao_inicial_X;
                            Armazena_Y = Posicao_inicial_Y;
                            while (mapa[Armazena_Y + 1][Armazena_X] != PAREDE && mapa[Armazena_Y + 1][Armazena_X] != PORTA_ABERTA && mapa[Armazena_Y + 1][Armazena_X] != PORTA_FECHADA){
                                Armazena_Y+= 1;
                            }
                            while (mapa[Armazena_Y][Armazena_X - 1] != PAREDE && mapa[Armazena_Y][Armazena_X - 1] != PORTA_ABERTA && mapa[Armazena_Y][Armazena_X - 1] != PORTA_FECHADA) {
                                Armazena_X-= 1;
                            }
                            Canto_Inferior_Esquerdo[0] = Armazena_X - 1;
                            Canto_Inferior_Esquerdo[1] = Armazena_Y + 1;
                            for (j = Canto_Superior_Direito[1]; j <= Canto_Inferior_Esquerdo[1]; j++) {
                                for (l = Canto_Inferior_Esquerdo[0]; l <= Canto_Superior_Direito[0]; l++)  {
                                    if (mapa[j][l] == PORTA_FECHADA) {
                                        mapa[j][l] = PORTA_ABERTA;
                                        Portas_X[Contador_Portas] = l;
                                        Portas_Y[Contador_Portas] = j;
                                        Portas[Contador_Portas] = TRUE;
                                        Contador_Portas+= 1;
                                    }
                                }
                            }
                        }
                    }
                }
                mover = 'p';
                if (Turnos == 10){
                    Turnos = 0;
                    for (i = 0; i < 5; i++) {
                        Gerar_Itens(Ptr_Item_Ataque, Ptr_Item_Defesa, Ptr_Item_Tipo, Ptr_Item_Life_Steal, Ptr_Item_X, Ptr_Item_Y, Item_X_Fixo[i], Item_Y_Fixo[i]);
                        Ptr_Item_Ataque+= 1;
                        Ptr_Item_Defesa+= 1;
                        Ptr_Item_Tipo+= 1;
                        Ptr_Item_Life_Steal+= 1;
                        Ptr_Item_X+= 1;
                        Ptr_Item_Y+= 1;
                    }
                    Ptr_Item_Ataque-= 5;
                    Ptr_Item_Defesa-= 5;
                    Ptr_Item_Tipo-= 5;
                    Ptr_Item_Life_Steal-= 5;
                    Ptr_Item_X-= 5;
                    Ptr_Item_Y-= 5;
                    for (i = 0; i < 5; i++){
                        Armazena_X = Item_X[i];
                        Armazena_Y = Item_Y[i];
                        if (Item_Tipo[i] == 0){
                            mapa[Armazena_Y][Armazena_X] = POCAO;
                        } else if (Item_Tipo[i] == 1){
                            mapa[Armazena_Y][Armazena_X] = ARMA;
                        } else if (Item_Tipo[i] == 2) {
                            mapa[Armazena_Y][Armazena_X] = ARMADURA;
                        }
                    }
                    for (i = 0; i <= K; i++) {
                        if (Vampiro[i] == FALSE) {
                            Vampiro[i] = TRUE;
                            Atualiza_X = V_X[i];
                            Atualiza_Y = V_Y[i];
                            mapa[Atualiza_Y][Atualiza_X] = VAMPIRO;
                            V_HP[i] = V_Max_HP[i];
                        }
                    }
                }
                Turnos = Turnos + 1;
                Turnos_Total+= 1;
                if (Player_HP <= 0) {
                    Derrota = 1;
                    Escapar = FALSE;
                }
                if (Vampiro[K + 1] == FALSE){
                    printf("\n                               *******************************************************************\n");
                    printf("                               *PARABENS, VOCE CONSEGUIU MATAR O DRACULA E SALVAR O MUNDO DO CAOS*\n");
                    printf("                               *******************************************************************\n");
                    Derrota = 1;
                    Contador_de_mortes = 5;
                }
            }
        }
        if (Vampiro[K + 1] == TRUE && SAIR != TRUE){
            printf("\n                               **********************************************************\n");
            printf("                               *O JOGO TERMINOU DEFINITIVAMENTE COM A VITORIA DO MAL KKK*\n");
            printf("                               **********************************************************\n");
            Derrota = 1;
            Contador_de_mortes = 5;
        }
        if (SAIR != TRUE) {
            printf("\n\n             O jogo durou %d turnos\n", Turnos_Total);
            printf("\n\n                               ***********\n");
            printf("                               *GAME OVER*\n");
            printf("                               ***********\n");
        }
    }
    return 0;
}
