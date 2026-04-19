/**
 * @file Jogador.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação do Jogador.
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "raylib/raylib.h"

#include "Jogador.h"
#include "Tipos.h"

static void resolverColisaoJogadorMapaX( Jogador *j, Mapa *mapa );
static void resolverColisaoJogadorMapaY( Jogador *j, Mapa *mapa );

/**
 * @brief Cria uma instância alocada dinamicamente da struct Jogador.
 */
Jogador *createJogador( float x, float y, float w, float h ) {

    Jogador *novoJogador = (Jogador*) malloc( sizeof( Jogador ) );

    novoJogador->ret.x = x;
    novoJogador->ret.y = y;
    novoJogador->ret.width = w;
    novoJogador->ret.height = h;
    novoJogador->vel = (Vector2) { 0 };

    novoJogador->cor = BLUE;

    novoJogador->velAndando = 200;
    novoJogador->velCorrendo = 350;
    novoJogador->velPulo = -550;
    novoJogador->velMaxQueda = 600;

    novoJogador->quantidadePulos = 0;
    novoJogador->quantidadeMaxPulos = 2;

    return novoJogador;

}

/**
 * @brief Destrói um objeto Jogador e libera seus recursos.
 */
void destroyJogador( Jogador *j ) {
    if ( j != NULL ) {
        free( j );
    }
}

/**
 * @brief Lê a entrada do usuário e atualiza as velocidades do jogador.
 */
void inputJogador( Jogador *j ) {

    bool controlDown = IsKeyDown( KEY_LEFT_CONTROL );

    if ( IsKeyDown( KEY_LEFT ) ) {
        j->vel.x = controlDown ? -j->velCorrendo : -j->velAndando;
    } else if ( IsKeyDown( KEY_RIGHT ) ) {
        j->vel.x = controlDown ?  j->velCorrendo :  j->velAndando;
    } else {
        j->vel.x = 0;
    }

    if ( IsKeyPressed( KEY_SPACE ) && j->quantidadePulos < j->quantidadeMaxPulos ) {
        j->vel.y = j->velPulo;
        j->quantidadePulos++;
    }

}

/**
 * @brief Aplica física e resolve colisões do jogador com o mundo.
 */
void updateJogador( Jogador *j, GameWorld *gw, float delta ) {

    // fase X: move horizontalmente e resolve colisões laterais
    j->ret.x += j->vel.x * delta;
    resolverColisaoJogadorMapaX( j, gw->mapa );

    // fase Y: aplica gravidade, move verticalmente e resolve colisões verticais
    j->vel.y += gw->gravidade * delta;
    if ( j->vel.y > j->velMaxQueda ) {
        j->vel.y = j->velMaxQueda;
    }
    j->ret.y += j->vel.y * delta;
    resolverColisaoJogadorMapaY( j, gw->mapa );

}

/**
 * @brief Desenha o jogador.
 */
void drawJogador( Jogador *j ) {
    DrawRectangleRec( j->ret, j->cor );
    DrawRectangleLines( j->ret.x, j->ret.y, j->ret.width, j->ret.height, BLACK );
}

/**
 * @brief Resolve colisões do jogador com o mapa no eixo X.
 */
static void resolverColisaoJogadorMapaX( Jogador *j, Mapa *mapa ) {

    ElementoMapa *el = mapa->elementos;

    while ( el != NULL ) {

        Obstaculo *o = &el->obstaculo;

        if ( CheckCollisionRecs( j->ret, o->ret ) ) {
            if ( j->ret.x + j->ret.width / 2 < o->ret.x + o->ret.width / 2 ) {
                j->ret.x = o->ret.x - j->ret.width;
            } else {
                j->ret.x = o->ret.x + o->ret.width;
            }
            j->vel.x = 0;
        }

        el = el->proximo;

    }

}

/**
 * @brief Resolve colisões do jogador com o mapa no eixo Y.
 */
static void resolverColisaoJogadorMapaY( Jogador *j, Mapa *mapa ) {

    ElementoMapa *el = mapa->elementos;

    while ( el != NULL ) {

        Obstaculo *o = &el->obstaculo;

        if ( CheckCollisionRecs( j->ret, o->ret ) ) {
            if ( j->ret.y + j->ret.height / 2 < o->ret.y + o->ret.height / 2 ) {
                j->ret.y = o->ret.y - j->ret.height;
                j->quantidadePulos = 0;
            } else {
                j->ret.y = o->ret.y + o->ret.height;
            }
            j->vel.y = 0;
        }

        el = el->proximo;

    }

}