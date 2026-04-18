/**
 * @file ResourceManager.c
 * @author Prof. Dr. David Buzatto
 * @brief ResourceManager implementation.
 * 
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "raylib/raylib.h"

#include "ResourceManager.h"
#include "Utils.h"

ResourceManager rm = { 0 };

void loadResourcesResourceManager( void ) {

    rm.texturaJogador = carregarTexturaAlterandoCores( 
        "resources/imagens/sprites/sonic.png",
        (Color[]) {
            { 13, 72, 7, 255 },
        },
        (Color[]) {
            BLANK,
        },
        1
    );

    rm.texturaTerreno = LoadTexture( "resources/imagens/tiles/terreno.png" );
    rm.texturaFundo = LoadTexture( "resources/imagens/fundo/fundo.png" );

    //rm.soundExample = LoadSound( "resources/sfx/powerUp.wav" );
    //rm.musicExample = LoadMusicStream( "resources/musics/overworld1.ogg" );

}

void unloadResourcesResourceManager( void ) {
    UnloadTexture( rm.texturaJogador );
    UnloadTexture( rm.texturaTerreno );
    UnloadTexture( rm.texturaFundo );
    //UnloadSound( rm.soundExample );
    //UnloadMusicStream( rm.musicExample );
}