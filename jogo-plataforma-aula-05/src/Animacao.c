/**
 * @file Animacao.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação das funções para gerencimanto de animações.
 *
 * @copyright Copyright (c) 2026
 */
#include <stdlib.h>

#include "Animacao.h"
#include "Tipos.h"

void criarQuadrosAnimacao( Animacao *anim, int quantidadeQuadros ) {
    anim->quadros = (QuadroAnimacao*) malloc( sizeof( QuadroAnimacao ) * quantidadeQuadros );
}

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
    Rectangle retColisaoPadrao ) {

    int passo = abs( larguraFonte ) + espacamentoFonte;

    for ( int i = 0; i < quantidadeQuadros; i++ ) {

        int p = !deTrasParaFrente ? i : quantidadeQuadros - i - 1;

        quadros[i] = (QuadroAnimacao) { 
            .fonte = { 
                xInicioFonte + passo * p, 
                yInicioFonte, 
                larguraFonte, 
                alturaFonte
            }, 
            .duracao = duracaoPadrao,
            .retColisao = retColisaoPadrao
        };

    }

}

void destruirQuadrosAnimacao( Animacao *anim ) {
    if ( anim != NULL ) {
        free( anim->quadros );
    }
}

void atualizarAnimacao( Animacao *anim, float delta ) {

    if ( anim->finalizada ) {
        return;
    }

    anim->contadorTempoQuadro += (int) ( delta * 1000 );

    QuadroAnimacao *frame = getQuadroAtualAnimacao( anim );

    int actualDuration = frame->duracao;

    if ( anim->contadorTempoQuadro >= actualDuration ) {

        anim->contadorTempoQuadro = 0;
        anim->quadroAtual++;

        if ( anim->pararNoUltimoQuadro && anim->quadroAtual >= anim->quantidadeQuadros ) {
            anim->quadroAtual = anim->quantidadeQuadros - 1;
            anim->finalizada = true;
            return;
        }

        if ( anim->executarUmaVez && anim->quadroAtual >= anim->quantidadeQuadros ) {
            anim->quadroAtual = 0;
            anim->finalizada = true;
            return;
        }

        anim->quadroAtual %= anim->quantidadeQuadros;

    }

}

int getNumeroQuadroAtualAnimacao( Animacao *anim ) {
    return anim->quadroAtual % anim->quantidadeQuadros;
}

QuadroAnimacao *getQuadroAtualAnimacao( Animacao *anim ) {
    if ( anim != NULL ) {
        return &anim->quadros[getNumeroQuadroAtualAnimacao(anim)];
    }
    return NULL;
}

void reiniciarAnimacao( Animacao *anim ) {
    anim->quadroAtual = 0;
    anim->contadorTempoQuadro = 0;
    anim->finalizada = false;
}

void sincronizarAnimacao( Animacao *destino, Animacao *origem ) {
    if ( destino->quantidadeQuadros != origem->quantidadeQuadros ) {
        TraceLog( LOG_WARNING, "sincronizarAnimacao: animações com quantidade de quadros diferente (%d != %d)",
            destino->quantidadeQuadros, origem->quantidadeQuadros );
        return;
    }
    destino->quadroAtual = origem->quadroAtual;
    destino->contadorTempoQuadro = 0.0f;
}