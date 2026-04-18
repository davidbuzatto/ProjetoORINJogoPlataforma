/**
 * @file Mapa.h
 * @author Prof. Dr. David Buzatto
 * @brief Declarações das funções do Mapa.
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "Tipos.h"

/**
 * @brief Carrega um mapa a partir de uma arquivo.
 */
Mapa *carregarMapa( const char *caminhoArquivo );

/**
 * @brief Destroi um mapa.
 */
void destroyMapa( Mapa *m );

/**
 * @brief Desenha um mapa.
 */
void drawMapa( Mapa *m );

/**
 * @brief Calcula a largura do mapa.
 */
int getLarguraMapa( Mapa *m );

/**
 * @brief Calcula a altura do mapa.
 */
int getAlturaMapa( Mapa *m );