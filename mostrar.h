//Mostra as cartas e atribui a cada naipe o simbolo correto
void mostrar_carta(char carta, int naipe) {
    printf("┌─────┐\n");
    printf("│ %c   │\n", carta);
    switch(naipe){
        case 0:
            printf("│  ♦  │\n");
            break;
        case 1:
            printf("│  ♠  │\n");
            break;
        case 2:
            printf("│  ♥  │\n");
            break;
        case 3:
            printf("│  ♣  │\n");
            break;
    }
    printf("│   %c │\n", carta);
    printf("└─────┘\n");
}

//Mostra a mao
void mostrar_mao(struct jogador *jogador, char vira) {
    printf("\n=== VIRA: %c ===\n", vira);
    printf("Suas cartas:\n");
    
    //Se houver carta, mostre ela
    for (int i = 0; i < 3; i++) {
        if (jogador->cartas[i] != ' ') {
            printf("[%d]\n", i + 1);
            mostrar_carta(jogador->cartas[i], jogador->naipes[i]);
        }
    }
}