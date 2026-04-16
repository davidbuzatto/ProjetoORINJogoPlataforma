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
void drawObstaculo( Obstaculo *o ) {
    DrawRectangle( o->pos.x, o->pos.y, o->dim.x, o->dim.y, o->cor );
}

/**
 * @brief Desenha todos os obstáculos.
 */
void drawObstaculos( Obstaculo *j, int quantidade ) {
    for ( int i = 0; i < quantidade; i++ ) {
        drawObstaculo( &j[i] );
    }
}