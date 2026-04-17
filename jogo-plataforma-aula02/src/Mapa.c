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
#include "Tipos.h"

/**
 * @brief Carrega um mapa a partir de uma arquivo.
 */
Mapa *carregarMapa( const char *caminhoArquivo ) {

    Mapa *novoMapa = (Mapa*) malloc( sizeof( Mapa ) );
    novoMapa->itens = NULL;
    novoMapa->quantidadeItens = 0;

    char *dadosMapa = LoadFileText( caminhoArquivo );
    char *caractereAtual = dadosMapa;
    int linhaAtual = 0;
    int colunaAtual = 0;

    int tamanhoObstaculo = 50;

    while ( *caractereAtual != '\0' ) {

        if ( *caractereAtual == '\n' ) {
            linhaAtual++;
            colunaAtual = 0;
        } else {

            Color cor = BLACK;
            bool criar = true;

            switch ( *caractereAtual ) {
                case 'x': cor = GREEN;  break;
                case 'r': cor = ORANGE; break;
                default: criar = false; break;
            }

            if ( criar ) {

                ElementoMapa *el = (ElementoMapa*) malloc( sizeof( ElementoMapa ) );

                el->obstaculo = (Obstaculo) {
                    .ret = { 
                        .x = tamanhoObstaculo * colunaAtual, 
                        .y = tamanhoObstaculo * linhaAtual, 
                        .width = tamanhoObstaculo, 
                        .height = tamanhoObstaculo
                    },
                    .cor = cor
                };
                el->proximo = NULL;

                if ( novoMapa->itens == NULL ) {
                    novoMapa->itens = el;
                } else {
                    el->proximo = novoMapa->itens;
                    novoMapa->itens = el;
                }
                novoMapa->quantidadeItens++;

            }

            colunaAtual++;

        }

        caractereAtual++;

    }

    UnloadFileText( dadosMapa );

    return novoMapa;

}

/**
 * @brief Destroi um mapa.
 */
void destroyMapa( Mapa *m ) {

    ElementoMapa *el = m->itens;

    while ( el != NULL ) {
        ElementoMapa *t = el;
        el = el->proximo;
        free( t );
    }

}

/**
 * @brief Desenha um mapa.
 */
void drawMapa( Mapa *m ) {

    ElementoMapa *el = m->itens;

    while ( el != NULL ) {
        Obstaculo *o = &el->obstaculo;
        DrawRectangleRec( o->ret, o->cor );
        DrawRectangleLines( o->ret.x, o->ret.y, o->ret.width, o->ret.height, BLACK );
        el = el->proximo;
    }

}