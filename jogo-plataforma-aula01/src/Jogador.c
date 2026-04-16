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

static void resolverColisaoJogadorObstaculosX( Jogador *j, Obstaculo *obstaculos, int quantidade );
static void resolverColisaoJogadorObstaculosY( Jogador *j, Obstaculo *obstaculos, int quantidade );
static void resolverColisaoJogadorCenarioX( Jogador *j );
static void resolverColisaoJogadorCenarioY( Jogador *j, float chaoY );

/**
 * @brief Cria uma instância alocada dinamicamente da struct Jogador.
 */
Jogador *createJogador( float x, float y, float w, float h ) {

    Jogador *j = (Jogador*) malloc( sizeof( Jogador ) );

    j->pos.x = x;
    j->pos.y = y;
    j->dim.x = w;
    j->dim.y = h;
    j->vel = (Vector2) { 0 };

    j->cor = BLUE;

    j->velPulo = -300;
    j->velMaxQueda = 500;
    j->noChao = false;

    return j;

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

    if ( IsKeyDown( KEY_LEFT ) ) {
        j->vel.x = -200;
    } else if ( IsKeyDown( KEY_RIGHT ) ) {
        j->vel.x = 200;
    } else {
        j->vel.x = 0;
    }

    if ( IsKeyPressed( KEY_SPACE ) && j->noChao ) {
        j->vel.y = j->velPulo;
    }

    // noChao será setado pelos resolvedores de colisão ao fim do frame.
    // resetar aqui garante que o estado reflita apenas o frame atual.
    j->noChao = false;

}

/**
 * @brief Aplica física e resolve colisões do jogador com o mundo.
 */
void updateJogador( Jogador *j, GameWorld *gw, float delta ) {

    // fase X: move horizontalmente e resolve colisões laterais
    j->pos.x += j->vel.x * delta;
    resolverColisaoJogadorObstaculosX( j, gw->obstaculos, gw->quantidadeObstaculos );
    resolverColisaoJogadorCenarioX( j );

    // fase Y: aplica gravidade, move verticalmente e resolve colisões verticais
    j->vel.y += gw->gravidade * delta;
    if ( j->vel.y > j->velMaxQueda ) {
        j->vel.y = j->velMaxQueda;
    }
    j->pos.y += j->vel.y * delta;
    resolverColisaoJogadorObstaculosY( j, gw->obstaculos, gw->quantidadeObstaculos );
    resolverColisaoJogadorCenarioY( j, gw->chao.y );

}

/**
 * @brief Desenha o jogador.
 */
void drawJogador( Jogador *j ) {
    DrawRectangle( j->pos.x, j->pos.y, j->dim.x, j->dim.y, j->cor );
}

/**
 * @brief Resolve colisões do jogador com os obstáculos no eixo X.
 */
static void resolverColisaoJogadorObstaculosX( Jogador *j, Obstaculo *obstaculos, int quantidade ) {

    for ( int i = 0; i < quantidade; i++ ) {

        Obstaculo *o = &obstaculos[i];
        Rectangle rJ = { j->pos.x, j->pos.y, j->dim.x, j->dim.y };
        Rectangle rO = { o->pos.x, o->pos.y, o->dim.x, o->dim.y };

        if ( CheckCollisionRecs( rJ, rO ) ) {
            if ( j->pos.x + j->dim.x / 2 < o->pos.x + o->dim.x / 2 ) {
                j->pos.x = o->pos.x - j->dim.x;
            } else {
                j->pos.x = o->pos.x + o->dim.x;
            }
            j->vel.x = 0;
        }

    }

}

/**
 * @brief Resolve colisões do jogador com os obstáculos no eixo Y.
 */
static void resolverColisaoJogadorObstaculosY( Jogador *j, Obstaculo *obstaculos, int quantidade ) {

    for ( int i = 0; i < quantidade; i++ ) {

        Obstaculo *o = &obstaculos[i];
        Rectangle rJ = { j->pos.x, j->pos.y, j->dim.x, j->dim.y };
        Rectangle rO = { o->pos.x, o->pos.y, o->dim.x, o->dim.y };

        if ( CheckCollisionRecs( rJ, rO ) ) {
            if ( j->pos.y + j->dim.y / 2 < o->pos.y + o->dim.y / 2 ) {
                j->pos.y = o->pos.y - j->dim.y;
                j->noChao = true;
            } else {
                j->pos.y = o->pos.y + o->dim.y;
            }
            j->vel.y = 0;
        }

    }

}

/**
 * @brief Resolve colisões do jogador com os limites do cenário no eixo X.
 */
static void resolverColisaoJogadorCenarioX( Jogador *j ) {

    if ( j->pos.x + j->dim.x > GetScreenWidth() ) {
        j->pos.x = GetScreenWidth() - j->dim.x;
    } else if ( j->pos.x < 0 ) {
        j->pos.x = 0;
    }

}

/**
 * @brief Resolve colisões do jogador com os limites do cenário no eixo Y.
 */
static void resolverColisaoJogadorCenarioY( Jogador *j, float chaoY ) {

    if ( j->pos.y + j->dim.y > chaoY ) {
        j->pos.y = chaoY - j->dim.y;
        j->vel.y = 0;
        j->noChao = true;
    }

}