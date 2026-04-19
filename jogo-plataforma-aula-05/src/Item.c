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
 * @brief Cria um novo Item.
 */
Item *criarItem( Rectangle ret, Color cor, Rectangle fonte, Texture2D *textura ) {

    Item *novoItem = (Item*) malloc( sizeof( Item ) );

    novoItem->ret = ret;
    novoItem->cor = cor;
    novoItem->fonte = fonte;
    novoItem->textura = textura;

    return novoItem;

}

/**
 * @brief Destroi um item.
 */
void destruirItem( Item *item ) {
    free( item );
}

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