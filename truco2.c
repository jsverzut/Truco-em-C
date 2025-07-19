/*  Nomes: João Victor dos Santos Sverzut
           Matheus Vilhena Magri
           Mateus Alves da Silva Dias
           Miguel Leal Landi

Descricao: Jogo de truco paulista, com implementações de um bot com noções de jogadas mais avançadas
Requer: A entrada do usuario com valores para escolher opcoes (1-3) e escolher cartas (1-3)
Assegura: O funcionamento de comparar as forcas das cartas, trucar, e desistir para chegar ao vencedor e também o funcionamento do bot com as descricoes acima
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>  //Usada na embaralhamento e de cartas.
#include <time.h>    //Usada na funcao srand. Assim no embaralhamento as cartas vao para diferentes posicoes.
#include <unistd.h>  //Usada com a funcao sleep().
#include "inicial.h" //Possui as funcoes para inicializar o baralho, embaralhar, distribuir as cartas e calcular a forca delas.
#include "mostrar.h" //Possui as funcoes para mostrar as cartas do jogador e do adversario.
#include "truco.h"   //Implementa o truco no programa
#include "jogadas.h" //Implementa as jogadas do jogador e do bot.


int main() {

    //srand() usa a a quantidade de segundos como seed
    srand(time(NULL));
    
    //Declara e inicializa as estruturas que armazenarao
    //as cartas, naipes e rounds ganhos de cada jogador
    struct jogador jogador = {0};
    struct jogador bot = {0};

    //Declara dois vetores (baralho e naipes_baralho) que
    //serao importantes no embaralhamento
    char baralho[40];
    int naipes_baralho[40];
    int pontos_jogador = 0, pontos_bot = 0;
    int rodada = 1;
    
    printf("=== TRUCO PAULISTA ===\n");
    printf("Primeiro a fazer 12 pontos ganha!\n");
    
    //Enquanto os pontos de quaisquer jogadores forem menores que 12, o jogo continua.
    while (pontos_jogador < 12 && pontos_bot < 12) {
        
        //Prepara um novo turno
        inicializar_baralho(baralho, naipes_baralho);
        embaralhar(baralho, naipes_baralho);
        
        //A variavel topo faz referencia ao topo do baralho na vida real
        int topo = 0;
        distribuir_cartas(&jogador, baralho, naipes_baralho, &topo);
        distribuir_cartas(&bot, baralho, naipes_baralho, &topo);
        
        //Depois de distribuir as cartas do baralho tirando-as do topo
        //, o vira e escolhido como sendo a proxima carta depois
        //da distribuicao.
        char vira = baralho[topo];
        
        jogador.rounds_ganhos = 0;
        bot.rounds_ganhos = 0;

        //Declara a escolha de carta do jogador
        int escolha;

        //Declara duas variaveis que guardam se algum dos dois desistiu
        //(no caso do bot ele desiste se em um truco, nao vale a pena jogar suas cartas) 
        bool desistir_jogador = false, desistir_bot = false;

        //Declara as variaveis que guardarao a forca
        //das cartas dos jogadores e o quanto pontos
        //vale aquela rodada (no inicio, sem o truco, vale 1 ponto)
        int forca_jogador = 0, forca_bot = 0;
        int valor_truco = 1;
        
        printf("\n--- RODADA %d ---\n", rodada);
        
        //Variavel que guarda se o jogador joga primeiro (V), ou se o bot joga primeiro (F)
        bool jogador_primeiro;
        
        //Realiza um loop ate que todas as tres cartas sao jogadas
        for (int turno = 0; turno < 3; turno++) {
            
            //Se o numero de rounds ganhos pelo bot ou pelo jogador
            //passar ou for igual a dois entao o loop termina e adicionado um ponto
            if (jogador.rounds_ganhos >= 2 || bot.rounds_ganhos >= 2) 
                break;
            
            printf("\n** Turno %d **\n", turno + 1);
            mostrar_mao(&jogador, vira);

            //Se esta no primeiro turno, quem joga primeiro e o jogador
            if (turno == 0)
                jogador_primeiro = true;
            
            //Se o jogador jogar primeiro, entao ele faz a sua jogada e verifica
            //se ele desistiu ou se o bot desistiu (no caso de um truco)
            if(jogador_primeiro){
                jogada_jogador(&jogador, &bot, &desistir_jogador, &escolha, &valor_truco, vira, &desistir_bot);
                if (!(desistir_jogador) && !(desistir_bot))
                    jogada_bot(jogador, escolha, &bot, vira, jogador_primeiro, &forca_bot);
            }
            else{
                sleep(2); //Separa o tempo de mostrar a mao do jogador e de mostrar a carta jogada pelo bot
                jogada_bot(jogador, escolha, &bot, vira, jogador_primeiro, &forca_bot);                
                jogada_jogador(&jogador, &bot, &desistir_jogador, &escolha, &valor_truco, vira, &desistir_bot);
                
            }            

            //Se nenhum dos jogadores desistiu
            if (!(desistir_jogador) && !(desistir_bot)){
                char carta_jogador = jogador.cartas[escolha];
                int naipe_jogador = jogador.naipes[escolha]; 
                jogador.cartas[escolha] = ' ';   //Marca como ja usada a carta com o indice escolha
                
                //Mostra cartas jogadas
                printf("\nVocê jogou:\n");
                mostrar_carta(carta_jogador, naipe_jogador);

                //Determina vencedor do turno
                forca_jogador = calcular_forca(carta_jogador, naipe_jogador, vira);
                      
                //Compara a forca das cartas e determina quem ganhou a rodada
                if (forca_jogador > forca_bot) {
                    printf("Você ganhou este turno!\n");
                    jogador.rounds_ganhos++;
                    jogador_primeiro = true;
                } 
                else if (forca_bot > forca_jogador) {
                    printf("Bot ganhou este turno!\n");
                    bot.rounds_ganhos++;
                    jogador_primeiro = false;
                } 
                else {
                    printf("Empate neste turno! Vantagem do que jogou primeiro\n");
                    //Vantagem e do primeiro que jogou;
                    if (jogador_primeiro && turno == 0)
                        jogador.rounds_ganhos++;
                    else if (!(jogador_primeiro) && turno == 0)
                        bot.rounds_ganhos++;
                }
            }
            sleep(2);
        }
        
        // Determinar vencedor da rodada
        if (jogador.rounds_ganhos > bot.rounds_ganhos) {
            printf("\033[32m\nVocê venceu a rodada!\033[0m\n");
            pontos_jogador += valor_truco;
        } else {
            printf("\033[31m\nBot venceu a rodada!\033[0m\n");
            pontos_bot += valor_truco;
        }
        
        printf("\n=== PLACAR ===\n");
        printf("Você: %d | Bot: %d\n", pontos_jogador, pontos_bot);
        
        rodada++;
        sleep(2);
    }
    
    //Mostra o resultado final
    if (pontos_jogador >= 12) {
        printf("\nPARABÉNS! VOCÊ VENCEU O JOGO!\n");
    } else {
        printf("\nVOCÊ PERDEU! O BOT VENCEU!\n");
    }
    
    return 0;
}
