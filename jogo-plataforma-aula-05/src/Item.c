/**
 * @file Item.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação do Item.
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "raylib/raylib.h"

#include "Item.h"
#include "Tipos.h"

/**
 * @brief Desenha um item.
 */
void desenharItem( Item *item ) {

    if ( item->textura == NULL ) {
        DrawRectangleRec( item->ret, item->cor );
        DrawRectangleLines( item->ret.x, item->ret.y, item->ret.width, item->ret.height, BLACK );
        return;
    }

    DrawTexturePro(
        *item->textura, 
        item->fonte,
        item->ret,
        (Vector2) { 0 },
        0.0f,
        WHITE
    );

}