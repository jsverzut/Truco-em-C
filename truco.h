//Funcao que responde ao pedido de truco do jogador
int truco_bot(struct jogador bot, int vira){
    
    int soma_forcas = 0;
    int cont = 0;
    for(int i = 0; i < 3; i++){
        if (bot.cartas[i] != ' '){
            soma_forcas += calcular_forca(bot.cartas[i], bot.naipes[i], vira);
            cont++;
        }
    }

    //Realiza uma progressao aritmetica e verifica e se as cartas do bot
    int cartas_boas = ((4 + (4 + (cont-1)*1))*cont)/2;  //(Se possui cartas com força maior que 4,5,6 , ou 4,5 ...)
    int cartas_ruins = ((3 + (3 + (cont-1)*1))*cont)/2; //(Se possui cartas com força menor que 3,4,5 , ou 3,4 ...)
    int cartas_otimas = ((8 + (8 + (cont-1)*1))*cont)/2;//(Se possui cartas com força maior que 8,9,10 , ou 8,9 ...)

    if (soma_forcas >= cartas_boas && soma_forcas < cartas_otimas)
        return 1;
    if (soma_forcas <= cartas_ruins)
        return 2;
    if (soma_forcas >= cartas_otimas)
        return 3;    
}

//Funcao que retorna o valor_truco ao jogador
int truco_jogador(struct jogador bot, int vira){

    int valor_truco;

    switch(truco_bot(bot, vira)){
        //Bot aceitou o truco
        case 1:
            printf("\nO bot ACEITOU o truco!\n");
            valor_truco = 3;
            return valor_truco;
            break;
        //Bot recusou o truco
        case 2:
            printf("\nO bot RECUSOU o truco!\n");
            valor_truco = 1;
            return valor_truco;
            break;
        //Bot pediu 6
        case 3:
            printf("\nO bot pediu 6!\n");
            valor_truco = 6;
            return valor_truco;
            break;
    }     
}
