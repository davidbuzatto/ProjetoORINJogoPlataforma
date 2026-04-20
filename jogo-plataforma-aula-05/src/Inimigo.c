/**
 * @file Inimigo.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação do Inimigo.
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "raylib/raylib.h"

#include "Inimigo.h"
#include "InimigoMotobug.h"
#include "Tipos.h"

/**
 * @brief Cria um novo Inimigo.
 */
Inimigo *criarInimigo( TipoInimigo tipo ) {

    Inimigo *novoInimigo = (Inimigo*) malloc( sizeof( Inimigo ) );
    novoInimigo->objeto = NULL;
    novoInimigo->tipo = tipo;

    return novoInimigo;

}

/**
 * @brief Destroi um inimigo.
 */
void destruirInimigo( Inimigo *inimigo ) {
    if ( inimigo != NULL ) {
        switch ( inimigo->tipo ) {
            case TIPO_INIMIGO_MOTOBUG:
                destruirInimigoMotobug( (InimigoMotobug*) inimigo->objeto );
                break;
            default:
                break;
        }
        free( inimigo );
    }
}

/**
 * @brief Atualiza um inimigo.
 */
void atualizarInimigo( Inimigo *inimigo, float delta ) {

    switch ( inimigo->tipo ) {
        case TIPO_INIMIGO_MOTOBUG:
            atualizarInimigoMotobug( (InimigoMotobug*) inimigo->objeto, delta );
            break;
        default:
            return;
    }

}

/**
 * @brief Desenha um inimigo.
 */
void desenharInimigo( Inimigo *inimigo ) {

    switch ( inimigo->tipo ) {
        case TIPO_INIMIGO_MOTOBUG:
            desenharInimigoMotobug( (InimigoMotobug*) inimigo->objeto );
            break;
        default:
            return;
    }

}