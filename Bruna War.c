#include "war.h"

int main(void) {
    /* Setup inicial */
    setbuf(stdout, NULL);
    srand((unsigned int) time(NULL));

    const char* missoes[TOTAL_MISSOES] = {
        "Conquistar 3 territorios seguidos",
        "Eliminar todas as tropas da cor vermelha",
        "Controlar 5 territorios com mais de 5 tropas",
        "Dominar toda a regiao norte (indices 0 e 1)",
        "Eliminar o exército vermelho"
    };

    int tamanhoMapa = NUM_TERRITORIOS_DEFAULT;
    Territorio* mapa = alocarMapa(tamanhoMapa);
    if (!mapa) {
        fprintf(stderr, "Erro ao alocar mapa.\n");
        return 1;
    }
    inicializarTerritorios(mapa, tamanhoMapa);

    /* pergunta a cor do jogador (ex: azul) */
    char corJogador[MAX_COR];
    printf("Digite a cor do seu exército (ex: azul): ");
    if (fgets(corJogador, sizeof(corJogador), stdin) == NULL) {
        strcpy(corJogador, "azul");
    }
    trim_newline(corJogador);
    if (strlen(corJogador) == 0) strcpy(corJogador, "azul");

    /* Aloca dinamicamente a string da missao do jogador */
    char* missaoJogador = (char*) malloc(MAX_MISSAO * sizeof(char));
    if (!missaoJogador) {
        fprintf(stderr, "Erro ao alocar missao.\n");
        liberarMemoria(mapa, NULL);
        return 1;
    }
    atribuirMissao(missaoJogador, missoes, TOTAL_MISSOES);
    int idMissao = sortearMissao(); /* id para verificacoes na funcao verificarVitoria */

    printf("\n=== Sua Missao (sera mostrada uma vez) ===\n");
    exibirMissao(missaoJogador);

    /* Loop principal */
    int opcao;
    int venceu = 0;
    do {
        exibirMapa(mapa, tamanhoMapa);
        exibirMenuPrincipal();
        if (scanf("%d", &opcao) != 1) { limparBufferEntrada(); puts("Opcao invalida."); continue; }
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                faseDeAtaque(mapa, tamanhoMapa, corJogador);
                break;
            case 2:
                if (verificarVitoria(idMissao, corJogador, mapa, tamanhoMapa)) {
                    puts("\nPARABENS! Missao cumprida. Voce venceu!");
                    venceu = 1;
                } else {
                    puts("\nMissao nao cumprida ainda. Continue jogando.");
                }
                break;
            case 0:
                puts("Encerrando o jogo. Ate a proxima!");
                break;
            default:
                puts("Opcao invalida.");
                break;
        }

        /* verificação silenciosa ao final de cada rodada */
        if (!venceu && verificarVitoria(idMissao, corJogador, mapa, tamanhoMapa)) {
            puts("\nMissao cumprida! Vencedor declarado (verificacao automatica).");
            venceu = 1;
            break;
        }

    } while (opcao != 0 && !venceu);

    liberarMemoria(mapa, missaoJogador);
    return 0;
}
