/**
 * @file GameWorld.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação do GameWorld.
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "GameWorld.h"
#include "Jogador.h"
#include "Obstaculo.h"
#include "Tipos.h"
#include "ResourceManager.h"

#include "raylib/raylib.h"
//#include "raylib/raymath.h"
//#define RAYGUI_IMPLEMENTATION    // to use raygui, comment these three lines.
//#include "raylib/raygui.h"       // other compilation units must only include
//#undef RAYGUI_IMPLEMENTATION     // raygui.h

static void initObstaculos( GameWorld *gw );

/**
 * @brief Cria uma instância alocada dinamicamente da struct GameWorld.
 */
GameWorld* createGameWorld( void ) {

    GameWorld *gw = (GameWorld*) malloc( sizeof( GameWorld ) );
    gw->jogador = criarJogador( GetScreenWidth() / 2, GetScreenHeight() / 2, 50, 50 );

    initObstaculos( gw );

    gw->chao = (Rectangle) {
        .x = 0,
        .y = GetScreenHeight() - 50,
        .width = GetScreenWidth(),
        .height = GetScreenHeight()
    };

    gw->gravidade = 500;

    return gw;

}

/**
 * @brief Destrói um objeto GameWorld e suas dependências.
 */
void destroyGameWorld( GameWorld *gw ) {
    destruirJogador( gw->jogador );
    free( gw );
}

/**
 * @brief Lê a entrada do usuário e atualiza o estado do jogo.
 */
void updateGameWorld( GameWorld *gw, float delta ) {

    Jogador *j = gw->jogador;

    entradaJogador( j );
    atualizarJogador( j, gw, delta );

}

/**
 * @brief Desenha o estado do jogo.
 */
void drawGameWorld( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( WHITE );

    DrawRectangleRec( gw->chao, GREEN );
    desenharObstaculos( gw->obstaculos, gw->quantidadeObstaculos );
    desenharJogador( gw->jogador );

    EndDrawing();

}

/**
 * @brief Inicializa os obstáculos.
 */
static void initObstaculos( GameWorld *gw ) {

    gw->quantidadeObstaculos = 10;
    gw->obstaculos = (Obstaculo*) malloc( sizeof( Obstaculo ) * gw->quantidadeObstaculos );
    
    int q = 0;

    for ( int i = 0; i < 5; i++ ) {
        int tam = 50;
        int xIni = GetScreenWidth() / 2 + tam * 2;
        int yIni = GetScreenHeight() / 2 + tam * 2;
        gw->obstaculos[i] = (Obstaculo) {
            .ret = { 
                xIni + tam * i, 
                yIni - tam * i, 
                tam, 
                tam
            },
            .cor = ORANGE
        };
        q++;
    }

    for ( int i = 0; i < 5; i++ ) {
        int tam = 50;
        int xIni = GetScreenWidth() / 2 + tam;
        int yIni = GetScreenHeight() / 2 - tam;
        gw->obstaculos[i+q] = (Obstaculo) {
            .ret = { 
                xIni - tam * i,
                yIni - tam * i,
                tam,
                tam
            },
            .cor = ORANGE
        };
    }

}

