struct jogador {
    char cartas[3];
    int naipes[3];
    int rounds_ganhos;
};

//Inicializa o baralho, colocando as cartas em ordem crescente
void inicializar_baralho(char cartas[], int naipes[]) {
    char valores[] = {'4','5','6','7','Q','J','K','A','2','3'};
    int indice_cartas = 0;
    
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 4; j++) {
            cartas[indice_cartas] = valores[i];
            naipes[indice_cartas] = j;
            indice_cartas++;
        }
    }
}

//Embaralha as cartas utilizando uma variavel auxiliar e o rand()
void embaralhar(char cartas[], int naipes[]) {
    for (int i = 39; i > 0; i--) {

        //Pega as cartas que a carta atual e troca de lugar com a carta atual
        int j = rand() % (i + 1); 
        
        char temp_carta = cartas[i];
        cartas[i] = cartas[j];
        cartas[j] = temp_carta;
        
        int temp_naipe = naipes[i];
        naipes[i] = naipes[j];
        naipes[j] = temp_naipe;
    }
}


//Pega as cartas ja embaralhadas e as distribui
//considerando o topo do baralho com o indice 0
void distribuir_cartas(struct jogador *jogador, char baralho[], int naipes_baralho[], int *topo) {

    for (int i = 0; i < 3; i++) {
        jogador->cartas[i] = baralho[*topo];
        jogador->naipes[i] = naipes_baralho[*topo];
        (*topo)++;
    }
}

//Atribui a força de cada carta
int calcular_forca(char carta, int naipe, char vira) {

    int valor = -1;
    int valor_vira = -1;

    char valores[] = {'4','5','6','7','Q','J','K','A','2','3'};

    int cont = 0;
    while(valor == -1){
        if (valores[cont] == carta)
            valor = cont;
        cont++;
    }

    cont = 0;
    while(valor_vira == -1){
        if (valores[cont] == vira)
            valor_vira = cont;
        cont++;
    }

    //Se o vira for o 3, ele volta na sequencia e
    //atribui ao 4 o valor de carta mais forte
    int proxima_carta;
    if (valor_vira == 9) 
        proxima_carta = 0;
    else
        valor_vira + 1;
    
    //Se a carta for manilha (a mais forte) ocorre uma comparacao pelo naipe
    if (valor == proxima_carta) {
        return 10 + naipe;
    }
    
    return valor;
}