/**
 * @file Mapa.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação do Mapa.
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "raylib/raylib.h"

#include "Mapa.h"
#include "Obstaculo.h"
#include "Tipos.h"

/**
 * @brief Carrega um mapa a partir de uma arquivo.
 */
Mapa *carregarMapa( const char *caminhoArquivo ) {

    // aloca um novo mapa
    Mapa *novoMapa = (Mapa*) malloc( sizeof( Mapa ) );
    novoMapa->elementos = NULL;
    novoMapa->quantidadeElementos = 0;
    novoMapa->tamanhoElemento = 50;
    novoMapa->linhas = 0;
    novoMapa->colunas = 0;
    
    // carrega dados do arquivo de texto
    char *dadosMapa = LoadFileText( caminhoArquivo );

    // marcadores para processamento do mapa
    char *caractereAtual = dadosMapa;
    int linhaAtual = 0;
    int colunaAtual = 0;

    // caractere atual marca inicialmente a primeira posição de dadosMapa
    // C-strings terminam em '\0', sendo assim, caminhamos caractere por 
    // caractere até o fim
    while ( *caractereAtual != '\0' ) {

        // fim de linha?
        if ( *caractereAtual == '\n' ) {

            linhaAtual++;
            colunaAtual = 0;

            novoMapa->linhas = linhaAtual;

        } else {

            // aqui decidimos se vamos ou não criar um novo elemento
            Color cor = BLACK;
            bool criar = true;

            // o caractere corrente dita qual tipo de elemento será criado
            switch ( *caractereAtual ) {
                case 'c': cor = GRAY; break;
                case 'r': cor = ORANGE; break;
                case 'g': cor = MAROON; break;
                case 'x': cor = GREEN;  break;
                default: criar = false; break;
            }

            if ( criar ) {

                ElementoMapa *el = (ElementoMapa*) malloc( sizeof( ElementoMapa ) );

                el->obstaculo = (Obstaculo) {
                    .ret = { 
                        .x = novoMapa->tamanhoElemento * colunaAtual, 
                        .y = novoMapa->tamanhoElemento * linhaAtual, 
                        .width = novoMapa->tamanhoElemento, 
                        .height = novoMapa->tamanhoElemento
                    },
                    .cor = cor
                };
                el->proximo = NULL;

                // inserção na lista
                if ( novoMapa->elementos == NULL ) {
                    novoMapa->elementos = el;
                } else {
                    el->proximo = novoMapa->elementos;
                    novoMapa->elementos = el;
                }
                novoMapa->quantidadeElementos++;

            }

            colunaAtual++;

            if ( novoMapa->colunas < colunaAtual ) {
                novoMapa->colunas = colunaAtual;
            }

        }

        caractereAtual++;

    }

    novoMapa->linhas++;
    
    // descarrega os dados
    UnloadFileText( dadosMapa );

    return novoMapa;

}

/**
 * @brief Destroi um mapa.
 */
void destruirMapa( Mapa *m ) {

    ElementoMapa *el = m->elementos;

    while ( el != NULL ) {
        ElementoMapa *t = el;
        el = el->proximo;
        free( t );
    }

}

/**
 * @brief Desenha um mapa.
 */
void desenharMapa( Mapa *m ) {

    ElementoMapa *el = m->elementos;

    while ( el != NULL ) {
        Obstaculo *o = &el->obstaculo;
        desenharObstaculo( o );
        el = el->proximo;
    }

}

/**
 * @brief Calcula a largura do mapa.
 */
int calcularLarguraMapa( Mapa *m ) {
    return (int) ( m->tamanhoElemento * m->colunas );
}

/**
 * @brief Calcula a altura do mapa.
 */
int calcularAlturaMapa( Mapa *m ) {
    return (int) ( m->tamanhoElemento * m->linhas );
}