/**
 * @file Obstaculo.h
 * @author Prof. Dr. David Buzatto
 * @brief Declarações das funções do Inimigo.
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "Tipos.h"

/**
 * @brief Cria um novo inimigo.
 */
Inimigo *criarInimigo( TipoInimigo tipo );

/**
 * @brief Destroi um inimigo.
 */
void destruirInimigo( Inimigo *inimigo );

/**
 * @brief Atualiza um inimigo.
 */
void atualizarInimigo( Inimigo *inimigo, float delta );

/**
 * @brief Desenha um inimigo.
 */
void desenharInimigo( Inimigo *inimigo );