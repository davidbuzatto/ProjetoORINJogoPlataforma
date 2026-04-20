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

#include "Macros.h"
#include "Mapa.h"
#include "Item.h"
#include "ItemAnel.h"
#include "Obstaculo.h"
#include "Tipos.h"
#include "ResourceManager.h"

/**
 * @brief Carrega um mapa a partir de uma arquivo.
 */
Mapa *carregarMapa( const char *caminhoArquivo ) {

    // aloca um novo mapa
    Mapa *novoMapa = (Mapa*) malloc( sizeof( Mapa ) );
    novoMapa->elementos = NULL;
    novoMapa->quantidadeElementos = 0;
    novoMapa->tamanhoElemento = 48;
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

        char c = *caractereAtual;

        // fim de linha?
        if ( c == '\n' ) {

            linhaAtual++;
            colunaAtual = 0;

            novoMapa->linhas = linhaAtual;

        } else {

            if ( c != ' ' ) {

                ElementoMapa *el = (ElementoMapa*) malloc( sizeof( ElementoMapa ) );

                if ( c >= 'A' && c <= 'Z' ) {

                    int deslocamento = c - 'A';

                    el->objeto = criarObstaculo( 
                        (Rectangle) { 
                            .x = novoMapa->tamanhoElemento * colunaAtual, 
                            .y = novoMapa->tamanhoElemento * linhaAtual, 
                            .width = novoMapa->tamanhoElemento, 
                            .height = novoMapa->tamanhoElemento
                        },
                        GRAY,
                        (Rectangle) { 
                            1 + ( novoMapa->tamanhoElemento + 1 ) * deslocamento, 
                            1, 
                            novoMapa->tamanhoElemento,
                            novoMapa->tamanhoElemento
                        },
                        &rm.texturaTerreno
                    );

                    el->tipo = TIPO_ELEMENTO_MAPA_OBSTACULO;

                } else if ( c >= 'a' && c <= 'z' ) {

                    Item *item = NULL;

                    switch ( c ) {

                        case 'a':

                            item = criarItem( TIPO_ITEM_ANEL );

                            item->objeto = criarItemAnel( 
                                (Rectangle) { 
                                    .x = novoMapa->tamanhoElemento * colunaAtual + 8, 
                                    .y = novoMapa->tamanhoElemento * linhaAtual + 5, 
                                    .width = 32, 
                                    .height = 32
                                },
                                YELLOW
                            );

                            el->objeto = item;
                            el->tipo = TIPO_ELEMENTO_MAPA_ITEM;

                            break;
                        default:
                            TraceLog( LOG_ERROR, "Tipo de item desconhecido." );
                            break;
                    }

                }

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

        switch ( el->tipo ) {
            case TIPO_ELEMENTO_MAPA_OBSTACULO:
                destruirObstaculo( (Obstaculo*) el->objeto );
                break;
            case TIPO_ELEMENTO_MAPA_ITEM:
                destruirItem( (Item*) el->objeto );
                break;
            case TIPO_ELEMENTO_MAPA_INIMIGO:
                break;
            default:
                break;
        }

        ElementoMapa *t = el;
        el = el->proximo;
        free( t );

    }

}

/**
 * @brief Atualiza um mapa.
 */
void atualizarMapa( Mapa *m, float delta ) {

    ElementoMapa *el = m->elementos;

    while ( el != NULL ) {
        
        switch ( el->tipo ) {
            case TIPO_ELEMENTO_MAPA_ITEM:
                atualizarItem( (Item*) el->objeto, delta );
                break;
            case TIPO_ELEMENTO_MAPA_INIMIGO:
                break;
            default:
                break;
        }
        
        el = el->proximo;

    }

}

/**
 * @brief Desenha um mapa.
 */
void desenharMapa( Mapa *m ) {

    ElementoMapa *el = m->elementos;

    while ( el != NULL ) {
        
        switch ( el->tipo ) {
            case TIPO_ELEMENTO_MAPA_OBSTACULO:
                desenharObstaculo( (Obstaculo*) el->objeto );
                break;
            case TIPO_ELEMENTO_MAPA_ITEM:
                desenharItem( (Item*) el->objeto );
                break;
            case TIPO_ELEMENTO_MAPA_INIMIGO:
                break;
            default:
                break;
        }
        
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