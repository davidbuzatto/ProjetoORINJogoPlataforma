/**
 * @file Jogador.h
 * @author Prof. Dr. David Buzatto
 * @brief Declarações das funções do Jogador.
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "Tipos.h"

/**
 * @brief Cria uma instância alocada dinamicamente da struct Jogador.
 */
Jogador *createJogador( float x, float y, float w, float h );

/**
 * @brief Destrói um objeto Jogador e libera seus recursos.
 */
void destroyJogador( Jogador *j );

/**
 * @brief Lê a entrada do usuário e atualiza as velocidades do jogador.
 */
void inputJogador( Jogador *j );

/**
 * @brief Aplica física e resolve colisões do jogador com o mundo.
 */
void updateJogador( Jogador *j, GameWorld *gw, float delta );

/**
 * @brief Desenha o jogador.
 */
void drawJogador( Jogador *j );