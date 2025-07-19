int jogar_cartas (struct jogador jogador){
    int carta_escolhida;
    
    //Determina se a carta escolhida e valida e retira 1 do seu valor
    //para retornar seu valor como indice
    do {
        printf("\nEscolha uma carta (1-3): ");
        scanf("%d", &carta_escolhida); //As cartas ficam armazenadas em indice (vetor de 3 posicoes)
        carta_escolhida--; // Converter para índice (cartas de 1 a 3, indices de 0 a 2)
        
        if (carta_escolhida < 0 || carta_escolhida > 2 || jogador.cartas[carta_escolhida] == ' ') {
            printf("Carta inválida! Tente novamente.\n");
            carta_escolhida = -1;
        }
    } while (carta_escolhida == -1);

    return carta_escolhida;
}

int jogada_bot(struct jogador jogador, int escolha_jogador, struct jogador *bot, char vira, bool jogador_primeiro, int *forca_bot) {
    
    int escolha_bot;

    //Variaveis que representam a forca da carta do jogador (supondo que ele jogou primeiro), 
    //a forca de cada carta do bot, a carta de menor forca dele e a carta de menor forca que
    //ainda vence o jogador
    int forca_jogador, forca_aux, forca_menor, forca_carta;

    //Representam o indice da carta de menor valor do bot e o indice da carta de menor valor
    //que ainda vence o jogador
    int carta_menor, carta_indice = -1;

    //Determina se forca_carta e forca_menor ja foram inicializados
    bool bot_maior_jogador = false, bot_menor_jogador = false;

    if (jogador_primeiro){
        forca_jogador = calcular_forca(jogador.cartas[escolha_jogador], jogador.naipes[escolha_jogador], vira);
        for(int i = 0; i < 3; i++){
            forca_aux = calcular_forca(bot->cartas[i], bot->naipes[i], vira);
            
            //Se a carta existir e se ela for mais forte que a carta do jogador
            //seus valores sao armazenados e comparados para saber qual carta
            //dentre aquelas que vencem o jogador, possuem menor forca 
            if (bot->cartas[i] != ' ') {
                
                if(forca_aux > forca_jogador){
                    if (!(bot_maior_jogador)){
                        forca_carta = forca_aux;
                        carta_indice = i;
                        bot_maior_jogador = true;
                    }
                    else if (forca_aux < forca_carta){
                        forca_carta = forca_aux;
                        carta_indice = i;
                    }
                }
                
                //Se nenhuma das cartas vence o jogador, o bot armazena aquela que tem
                //menor forca
                else{
                    if (!(bot_menor_jogador)){
                        forca_menor = forca_aux;
                        carta_menor = i;
                        bot_menor_jogador = true;
                    }
                    else if (forca_aux < forca_menor){
                        forca_menor = forca_aux;
                        carta_menor = i;
                    }
                }
            }
        }
    }
    else{
        //Se o bot joga primeiro, entao ela joga sua carta de menor valor
        for(int i = 0; i < 3; i++){
            if (bot->cartas[i] != ' '){
                forca_aux = calcular_forca(bot->cartas[i], bot->naipes[i], vira);
                if (i == 0){
                    forca_menor = forca_aux;
                    carta_menor = i;
                }
                else if (forca_aux < forca_menor){
                    forca_menor = forca_aux;
                    carta_menor = i;
                }
            }
        }
    }
    
    //Se uma ou mais cartas que vencem o jogador foram encontradas
    //a escolha_bot recebe o indice da carta de menor forca dentre essas
    //e se nao encontrou, recebe o indice da carta de menor valor
    if (bot_maior_jogador)
        escolha_bot = carta_indice;
    else
        escolha_bot = carta_menor;

    //Atribui a carta_bot e naipe_bot os valores da carta escolhida
    //e depois retira o seu valor, deixando ela sem conteudo
    char carta_bot = bot->cartas[escolha_bot];
    int naipe_bot = bot->naipes[escolha_bot];
    *forca_bot = calcular_forca(bot->cartas[escolha_bot], bot->naipes[escolha_bot], vira);
    bot->cartas[escolha_bot] = ' ';
    
    printf("Bot jogou:\n");
    mostrar_carta(carta_bot, naipe_bot);
}

void jogada_jogador(struct jogador *jogador, struct jogador *bot, bool *optDesistir, int *escolha_jogador, int *valor_truco, int vira, bool *desistir_bot){
    bool opcao_valida = false;
    
    //Enquanto o usuario nao fornecer uma opcao valida
    //ele volta para a tela inicial
    while (!(opcao_valida)){
        printf("Digite uma opcao:\n");
        printf("[1] Truco\n[2] Jogar cartas\n[3] Desistir\n\n> ");

        int opcao;
        scanf("%d", &opcao);

            switch(opcao){
                //Verifica se o truco ja foi selecionado
                case 1:
                    if(*valor_truco >= 3){
                        printf("\nVoce nao pode pedir truco duas vezes seguidas\n\n");
                        opcao_valida = false;
                        break;
                    }
                    else {
                        printf("\nVoce pediu TRUCO!\n");
                        *valor_truco = truco_jogador(*bot, vira); //Armazena se o truco vale 1 (recusa), 3 ou 6
                        opcao_valida = true;

                        //Se o bot rejeita o truco ele perde a rodada
                        if (*valor_truco == 1){
                            jogador->rounds_ganhos = 3;
                            *desistir_bot = true;
                            break;
                        }
                    }
                case 2:
                    *escolha_jogador = jogar_cartas(*jogador);
                    opcao_valida = true;
                    break;
                case 3:
                    printf("Você desistiu\n");
                    *optDesistir = true;
                    bot->rounds_ganhos = 3;
                    opcao_valida = true;
                    break;
                default:
                    printf("Opcao inválida. Tente Novamente\n");
                    break;
            }
    }
}