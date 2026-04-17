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

/**
 * @brief Cria uma instância alocada dinamicamente da struct GameWorld.
 */
GameWorld* createGameWorld( void ) {

    GameWorld *gw = (GameWorld*) malloc( sizeof( GameWorld ) );

    gw->jogador = createJogador( GetScreenWidth() / 2, GetScreenHeight() / 2, 50, 50 );
    gw->mapa = carregarMapa( "resources/mapas/mapa01.txt" );
    gw->gravidade = 500;

    return gw;

}

/**
 * @brief Destrói um objeto GameWorld e suas dependências.
 */
void destroyGameWorld( GameWorld *gw ) {
    destroyMapa( gw->mapa );
    destroyJogador( gw->jogador );
    free( gw );
}

/**
 * @brief Lê a entrada do usuário e atualiza o estado do jogo.
 */
void updateGameWorld( GameWorld *gw, float delta ) {
    Jogador *j = gw->jogador;
    inputJogador( j );
    updateJogador( j, gw, delta );
}

/**
 * @brief Desenha o estado do jogo.
 */
void drawGameWorld( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( WHITE );

    drawMapa( gw->mapa );
    drawJogador( gw->jogador );

    DrawFPS( 10, 10 );

    EndDrawing();

}
