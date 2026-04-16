/**
 * @file Tipos.h
 * @author Prof. Dr. David Buzatto
 * @brief Definição dos tipos utilizados no jogo.
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include <stdbool.h>

#include "raylib/raylib.h"

/**
 * @brief Representa o jogador controlado pelo usuário.
 */
typedef struct Jogador {

    Vector2 pos;
    Vector2 dim;
    Vector2 vel;

    Color cor;

    float velPulo;
    float velMaxQueda;
    bool noChao;

} Jogador;

/**
 * @brief Representa um obstáculo estático do cenário.
 */
typedef struct Obstaculo {
    Vector2 pos;
    Vector2 dim;
    Color cor;
} Obstaculo;

/**
 * @brief Representa o mundo do jogo e seus elementos.
 */
typedef struct GameWorld {

    Jogador *jogador;

    Obstaculo *obstaculos;
    int quantidadeObstaculos;

    Rectangle chao;
    float gravidade;

} GameWorld;