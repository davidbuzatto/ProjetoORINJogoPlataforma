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
#include "Mapa.h"
#include "Obstaculo.h"
#include "Tipos.h"
#include "ResourceManager.h"

#include "raylib/raylib.h"
//#include "raylib/raymath.h"
//#define RAYGUI_IMPLEMENTATION    // to use raygui, comment these three lines.
//#include "raylib/raygui.h"       // other compilation units must only include
//#undef RAYGUI_IMPLEMENTATION     // raygui.h

static void updateCamera( GameWorld *gw );

/**
 * @brief Cria uma instância alocada dinamicamente da struct GameWorld.
 */
GameWorld *createGameWorld( void ) {

    GameWorld *gw = (GameWorld*) malloc( sizeof( GameWorld ) );

    //gw->mapa = carregarMapa( "resources/mapas/mapaTeste.txt" );
    gw->mapa = carregarMapa( "resources/mapas/mapa01.txt" );
    gw->jogador = criarJogador( GetScreenWidth() / 2, calcularAlturaMapa( gw->mapa ) - 150, 50, 50 );

    gw->camera = (Camera2D) {
        .offset = { 0 },    // deslocamento relativo da câmera em relação ao alvo
        .target = { 0 },    // o alvo da câmera, ou seja, a coordenada em que ela está centralizada
        .rotation = 0.0f,   // rotação da câmera em graus. o pivô é o alvo.
        .zoom = 1.0f        // zoom da câmera. 1.0f significa sem escala
    };

    gw->gravidade = 500;

    return gw;

}

/**
 * @brief Destrói um objeto GameWorld e suas dependências.
 */
void destroyGameWorld( GameWorld *gw ) {
    destruirMapa( gw->mapa );
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

    updateCamera( gw );

}

/**
 * @brief Desenha o estado do jogo.
 */
void drawGameWorld( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( WHITE );

    BeginMode2D( gw->camera );
    desenharMapa( gw->mapa );
    desenharJogador( gw->jogador );
    EndMode2D();

    DrawFPS( 10, 10 );

    EndDrawing();

}

static void updateCamera( GameWorld *gw ) {

    Jogador *j = gw->jogador;
    Camera2D *c = &gw->camera;

    c->offset.x = GetScreenWidth() / 2;
    c->offset.y = GetScreenHeight() / 2;

    c->target.x = j->ret.x + j->ret.width / 2;
    c->target.y = j->ret.y + j->ret.height / 2;

    int minX = GetScreenWidth() / 2;
    int maxX = calcularLarguraMapa( gw->mapa ) - GetScreenWidth() / 2;
    int maxY = calcularAlturaMapa( gw->mapa ) - GetScreenHeight() / 2;

    if ( c->target.x < minX ) {
        c->target.x = minX;
    } else if ( c->target.x > maxX ) {
        c->target.x = maxX;
    }

    if ( c->target.y > maxY ) {
        c->target.y = maxY;
    }

}
