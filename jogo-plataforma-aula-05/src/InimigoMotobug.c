/**
 * @file Inimigo.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação do Inimigo (Motobug).
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "raylib/raylib.h"

#include "Animacao.h"
#include "InimigoMotobug.h"
#include "Macros.h"
#include "ResourceManager.h"
#include "Tipos.h"

static void desenharQuadroAnimacaoInimigoMotobug( InimigoMotobug *j, QuadroAnimacao *qa, Vector2 deslocamento, Color tonalidade );
static Animacao *getAnimacaoAtualInimigoMotobug( InimigoMotobug *inimigo );

static const bool MOSTRAR_RETANGULOS = false;

/**
 * @brief Cria um novo Inimigo (motobug).
 */
InimigoMotobug *criarInimigoMotobug( Rectangle ret, Color cor ) {

    InimigoMotobug *novoInimigo = (InimigoMotobug*) malloc( sizeof( InimigoMotobug ) );

    novoInimigo->ret = ret;
    novoInimigo->vel = (Vector2) { 0 };
    novoInimigo->cor = cor;

    novoInimigo->velAndando = 100;
    novoInimigo->velMaxQueda = 600;

    novoInimigo->estado = ESTADO_INIMIGO_MOTOBUG_ANDANDO;
    novoInimigo->ativo = true;
    novoInimigo->olhandoParaDireita = false;

    int quantidadeAnimacoes = 0;

    novoInimigo->animacaoAndando.quantidadeQuadros = 4;
    novoInimigo->animacaoAndando.quadroAtual = 0;
    novoInimigo->animacaoAndando.contadorTempoQuadro = 0.0f;
    novoInimigo->animacaoAndando.pararNoUltimoQuadro = false;
    novoInimigo->animacaoAndando.executarUmaVez = false;
    novoInimigo->animacaoAndando.finalizada = false;
    criarQuadrosAnimacao( &novoInimigo->animacaoAndando, novoInimigo->animacaoAndando.quantidadeQuadros );
    inicializarQuadrosAnimacao( 
        novoInimigo->animacaoAndando.quadros,
        novoInimigo->animacaoAndando.quantidadeQuadros,
        250,             // duração padrão para todos os quadros
        1, 1,            // início
        40, 30,          // dimensões
        1,               // separação
        0, 0,            // deslocamento
        false,           // de trás para frente
        (Rectangle) {    // retângulo de colisão padrão para cada quadro
            2, 2, 68, 58
        }
    );

    novoInimigo->animacaoMorrendo.quantidadeQuadros = 4;
    novoInimigo->animacaoMorrendo.quadroAtual = 0;
    novoInimigo->animacaoMorrendo.contadorTempoQuadro = 0.0f;
    novoInimigo->animacaoMorrendo.pararNoUltimoQuadro = false;
    novoInimigo->animacaoMorrendo.executarUmaVez = false;
    novoInimigo->animacaoMorrendo.finalizada = false;
    criarQuadrosAnimacao( &novoInimigo->animacaoMorrendo, novoInimigo->animacaoMorrendo.quantidadeQuadros );
    inicializarQuadrosAnimacao( 
        novoInimigo->animacaoMorrendo.quadros,
        novoInimigo->animacaoMorrendo.quantidadeQuadros,
        250,             // duração padrão para todos os quadros
        1, 1,            // início
        40, 30,          // dimensões
        1,               // separação
        0, 0,            // deslocamento
        false,           // de trás para frente
        (Rectangle) {    // retângulo de colisão padrão para cada quadro
            2, 2, 68, 58
        }
    );

    novoInimigo->animacoes[ESTADO_INIMIGO_MOTOBUG_ANDANDO] = &novoInimigo->animacaoAndando; quantidadeAnimacoes++;
    novoInimigo->animacoes[ESTADO_INIMIGO_MOTOBUG_MORRENDO] = &novoInimigo->animacaoMorrendo; quantidadeAnimacoes++;
    novoInimigo->quantidadeAnimacoes = quantidadeAnimacoes;

    return novoInimigo;

}

/**
 * @brief Destroi um inimigo (motobug).
 */
void destruirInimigoMotobug( InimigoMotobug *inimigo ) {
    for ( int i = 0; i < inimigo->quantidadeAnimacoes; i++ ) {
        destruirQuadrosAnimacao( inimigo->animacoes[i] );
    }
    free( inimigo );
}

/**
 * @brief Atualiza um inimigo (motobug).
 */
void atualizarInimigoMotobug( InimigoMotobug *inimigo, float delta ) {

    if ( inimigo->ativo ) {

        Animacao *animacaoAtual = getAnimacaoAtualInimigoMotobug( inimigo );
        atualizarAnimacao( animacaoAtual, delta );

        if ( inimigo->estado == ESTADO_INIMIGO_MOTOBUG_ANDANDO ) {
            if ( inimigo->olhandoParaDireita ) {
                inimigo->vel.x = inimigo->velAndando;
            } else {
                inimigo->vel.x = -inimigo->velAndando;
            }
        } else {
            inimigo->vel.x = 0.0f;
        }

        inimigo->ret.x += inimigo->vel.x * delta;

        if ( inimigo->estado == ESTADO_INIMIGO_MOTOBUG_MORRENDO && animacaoAtual->finalizada ) {
            inimigo->ativo = false;
        }

    }

}

/**
 * @brief Desenha um inimigo (motobug).
 */
void desenharInimigoMotobug( InimigoMotobug *inimigo ) {
    if ( inimigo->ativo ) {
        QuadroAnimacao *qa = getQuadroAnimacaoAtualInimigoMotobug( inimigo );
        desenharQuadroAnimacaoInimigoMotobug( inimigo, qa, (Vector2) { 0 }, WHITE );
        if ( MOSTRAR_RETANGULOS ) {
            DrawRectangleRec( inimigo->ret, Fade( inimigo->cor, 0.5f ) );
            DrawRectangleLines( inimigo->ret.x, inimigo->ret.y, inimigo->ret.width, inimigo->ret.height, BLACK );
        }
    }
}

/**
 * @brief Obtém o quadro de animação atual de um inimigo (motobug).
 */
QuadroAnimacao *getQuadroAnimacaoAtualInimigoMotobug( InimigoMotobug *inimigo ) {
    return getQuadroAtualAnimacao( getAnimacaoAtualInimigoMotobug( inimigo ) );
}

static void desenharQuadroAnimacaoInimigoMotobug( InimigoMotobug *j, QuadroAnimacao *qa, Vector2 deslocamento, Color tonalidade ) {

    if ( qa != NULL ) {
        
        DrawTexturePro(
            rm.texturaBadniks,
            qa->fonte,
            (Rectangle) {
                deslocamento.x + j->ret.x + qa->deslocamentoDesenho.x,
                deslocamento.y + j->ret.y + qa->deslocamentoDesenho.y,
                j->ret.width,
                j->ret.height
            },
            (Vector2) { 0 },
            0.0f,
            tonalidade
        );

        if ( MOSTRAR_RETANGULOS ) {
            float xDesenho = j->ret.x + qa->deslocamentoDesenho.x + qa->retColisao.x;
            float yDesenho = j->ret.y + qa->deslocamentoDesenho.y + qa->retColisao.y;
            DrawRectangle( xDesenho, yDesenho, qa->retColisao.width, qa->retColisao.height, Fade( GREEN, 0.5f ) );
        }

    }

}

static Animacao *getAnimacaoAtualInimigoMotobug( InimigoMotobug *inimigo ) {
    return inimigo->animacoes[inimigo->estado];
}