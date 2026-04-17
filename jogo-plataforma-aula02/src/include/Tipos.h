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

    Rectangle ret;
    Vector2 vel;
    Color cor;

    float velAndando;
    float velPulo;
    float velMaxQueda;
    bool noChao;

} Jogador;

/**
 * @brief Representa um obstáculo estático do cenário.
 */
typedef struct Obstaculo {
    Rectangle ret;
    Color cor;
} Obstaculo;

/**
 * @brief Representa um elemento do mapa.
 */
typedef struct ElementoMapa ElementoMapa;
struct ElementoMapa {
    Obstaculo obstaculo;
    ElementoMapa *proximo;
};

/**
 * @brief Representa um mapa de fase do jogo.
 */
typedef struct Mapa {
    ElementoMapa *itens;
    int quantidadeItens;
} Mapa;

/**
 * @brief Representa o mundo do jogo e seus elementos.
 */
typedef struct GameWorld {
    Jogador *jogador;
    Mapa *mapa;
    float gravidade;
} GameWorld;