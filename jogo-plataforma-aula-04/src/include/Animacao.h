/**
 * @file Animacao.h
 * @author Prof. Dr. David Buzatto
 * @brief Declaração das funções para gerencimanto de animações.
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "Tipos.h"

void createQuadrosAnimacao( Animacao *anim, int quantidadeQuadros );
void initQuadrosAnimacao( 
    QuadroAnimacao *quadros,
    int quantidadeQuadros,
    int duracaoPadrao,
    int xInicioFonte,
    int yInicioFonte,
    int larguraFonte,
    int alturaFonte,
    int espacamentoFonte,
    int deslocamentoX,
    int deslocamentoY,
    bool deTrasParaFrente,
    Rectangle retColisaoPadrao
);
void destroyQuadrosAnimacao( Animacao *anim );
void updateAnimacao( Animacao *anim, float delta );

int getNumeroQuadroAtualAnimacao( Animacao *anim );
QuadroAnimacao *getQuadroAtualAnimacao( Animacao *anim );
void reiniciarAnimacao( Animacao *anim );