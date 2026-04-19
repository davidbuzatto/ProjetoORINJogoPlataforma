/**
 * @file Obstaculo.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação do Obstáculo.
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "raylib/raylib.h"

#include "Obstaculo.h"
#include "Tipos.h"

/**
 * @brief Desenha um obstáculo.
 */
void desenharObstaculo( Obstaculo *o ) {

    if ( o->textura == NULL ) {
        DrawRectangleRec( o->ret, o->cor );
        DrawRectangleLines( o->ret.x, o->ret.y, o->ret.width, o->ret.height, BLACK );
        return;
    }

    DrawTexturePro(
        *o->textura, 
        o->fonte,
        o->ret,
        (Vector2) { 0 },
        0.0f,
        WHITE
    );

}