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

    j->ret.x = x;
    j->ret.y = y;
    j->ret.width = w;
    j->ret.height = h;
    j->vel = (Vector2) { 0 };

    j->cor = BLUE;

    j->velAndando = 200;
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
        j->vel.x = -j->velAndando;
    } else if ( IsKeyDown( KEY_RIGHT ) ) {
        j->vel.x = j->velAndando;
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
    j->ret.x += j->vel.x * delta;
    resolverColisaoJogadorObstaculosX( j, gw->obstaculos, gw->quantidadeObstaculos );
    resolverColisaoJogadorCenarioX( j );

    // fase Y: aplica gravidade, move verticalmente e resolve colisões verticais
    j->vel.y += gw->gravidade * delta;
    if ( j->vel.y > j->velMaxQueda ) {
        j->vel.y = j->velMaxQueda;
    }
    j->ret.y += j->vel.y * delta;
    resolverColisaoJogadorObstaculosY( j, gw->obstaculos, gw->quantidadeObstaculos );
    resolverColisaoJogadorCenarioY( j, gw->chao.y );

}

/**
 * @brief Desenha o jogador.
 */
void drawJogador( Jogador *j ) {
    DrawRectangleRec( j->ret, j->cor );
}

/**
 * @brief Resolve colisões do jogador com os obstáculos no eixo X.
 */
static void resolverColisaoJogadorObstaculosX( Jogador *j, Obstaculo *obstaculos, int quantidade ) {

    for ( int i = 0; i < quantidade; i++ ) {

        Obstaculo *o = &obstaculos[i];

        if ( CheckCollisionRecs( j->ret, o->ret ) ) {
            if ( j->ret.x + j->ret.width / 2 < o->ret.x + o->ret.width / 2 ) {
                j->ret.x = o->ret.x - j->ret.width;
            } else {
                j->ret.x = o->ret.x + o->ret.width;
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

        if ( CheckCollisionRecs( j->ret, o->ret ) ) {
            if ( j->ret.y + j->ret.width / 2 < o->ret.y + o->ret.height / 2 ) {
                j->ret.y = o->ret.y - j->ret.height;
                j->noChao = true;
            } else {
                j->ret.y = o->ret.y + o->ret.height;
            }
            j->vel.y = 0;
        }

    }

}

/**
 * @brief Resolve colisões do jogador com os limites do cenário no eixo X.
 */
static void resolverColisaoJogadorCenarioX( Jogador *j ) {

    if ( j->ret.x + j->ret.width > GetScreenWidth() ) {
        j->ret.x = GetScreenWidth() - j->ret.width;
    } else if ( j->ret.x < 0 ) {
        j->ret.x = 0;
    }

}

/**
 * @brief Resolve colisões do jogador com os limites do cenário no eixo Y.
 */
static void resolverColisaoJogadorCenarioY( Jogador *j, float chaoY ) {

    if ( j->ret.y + j->ret.height > chaoY ) {
        j->ret.y = chaoY - j->ret.height;
        j->vel.y = 0;
        j->noChao = true;
    }

}