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
    DrawRectangleRec( o->ret, o->cor );
    DrawRectangleLines( o->ret.x, o->ret.y, o->ret.width, o->ret.height, BLACK );
}

/**
 * @brief Desenha todos os obstáculos.
 */
void drawObstaculos( Obstaculo *obstaculos, int quantidade ) {
    for ( int i = 0; i < quantidade; i++ ) {
        drawObstaculo( &obstaculos[i] );
    }
}