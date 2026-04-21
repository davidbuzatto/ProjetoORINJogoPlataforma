/**
 * @file Animacao.h
 * @author Prof. Dr. David Buzatto
 * @brief Declaração das funções para gerencimanto de animações.
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "Tipos.h"

void criarQuadrosAnimacao( Animacao *anim, int quantidadeQuadros );
void inicializarQuadrosAnimacao( 
    QuadroAnimacao *quadros,
    int quantidadeQuadros,
    int duracaoPadrao,
    int xInicioFonte,
    int yInicioFonte,
    int larguraFonte,
    int alturaFonte,
    int espacamentoFonte,
    bool deTrasParaFrente,
    Rectangle retColisaoPadrao
);
void destruirQuadrosAnimacao( Animacao *anim );
void atualizarAnimacao( Animacao *anim, float delta );

int getNumeroQuadroAtualAnimacao( Animacao *anim );
QuadroAnimacao *getQuadroAtualAnimacao( Animacao *anim );
void reiniciarAnimacao( Animacao *anim );

void sincronizarAnimacao( Animacao *destino, Animacao *origem );