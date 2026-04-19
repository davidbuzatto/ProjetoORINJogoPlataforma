/**
 * @file Jogador.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação do Jogador.
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "raylib/raylib.h"

#include "Animacao.h"
#include "Macros.h"
#include "Jogador.h"
#include "ResourceManager.h"
#include "Tipos.h"

static void drawQuadroAnimacaoJogador( Jogador *j, QuadroAnimacao *qa, Vector2 deslocamento, Color tonalidade );
static QuadroAnimacao *getQuadroAnimacaoAtualJogador( Jogador *j );
static Animacao *getAnimacaoAtualJogador( Jogador *j );

static void resolverColisaoJogadorMapaX( Jogador *j, Mapa *mapa );
static void resolverColisaoJogadorMapaY( Jogador *j, Mapa *mapa );

static const bool MOSTRAR_RETANGULOS = true;

/**
 * @brief Cria uma instância alocada dinamicamente da struct Jogador.
 */
Jogador *createJogador( float x, float y, float w, float h ) {

    Jogador *novoJogador = (Jogador*) malloc( sizeof( Jogador ) );

    novoJogador->ret.x = x;
    novoJogador->ret.y = y;
    novoJogador->ret.width = w;
    novoJogador->ret.height = h;
    novoJogador->vel = (Vector2) { 0 };

    novoJogador->cor = BLUE;

    novoJogador->velAndando = 200;
    novoJogador->velAndandoRapido = 400;
    novoJogador->velCorrendo = 600;
    novoJogador->velPulo = -550;
    novoJogador->velMaxQueda = 600;

    novoJogador->aceleracao = 400;
    novoJogador->desaceleracao = 600;
    novoJogador->frenagem = 1800;

    novoJogador->quantidadePulos = 0;
    novoJogador->quantidadeMaxPulos = 1;

    novoJogador->estado = ESTADO_JOGADOR_PARADO;
    novoJogador->olhandoParaDireita = true;

    int quantidadeAnimacoes = 0;

    novoJogador->animacaoParado.quantidadeQuadros = 1;
    novoJogador->animacaoParado.quadroAtual = 0;
    novoJogador->animacaoParado.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoParado.pararNoUltimoQuadro = false;
    novoJogador->animacaoParado.executarUmaVez = false;
    novoJogador->animacaoParado.finalizada = false;
    createQuadrosAnimacao( &novoJogador->animacaoParado, novoJogador->animacaoParado.quantidadeQuadros );
    initQuadrosAnimacao( 
        novoJogador->animacaoParado.quadros,
        novoJogador->animacaoParado.quantidadeQuadros,
        1000,            // duração padrão para todos os quadros
        24, 251,         // início
        48, 48,          // dimensões
        4,               // separação
        0, 0,            // deslocamento
        false,           // de trás para frente
        (Rectangle) {    // retângulo de colisão padrão para cada quadro
            18, 20, 54, 76
        }
    );

    novoJogador->animacaoAndando.quantidadeQuadros = 8;
    novoJogador->animacaoAndando.quadroAtual = 0;
    novoJogador->animacaoAndando.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoAndando.pararNoUltimoQuadro = false;
    novoJogador->animacaoAndando.executarUmaVez = false;
    novoJogador->animacaoAndando.finalizada = false;
    createQuadrosAnimacao( &novoJogador->animacaoAndando, novoJogador->animacaoAndando.quantidadeQuadros );
    initQuadrosAnimacao( 
        novoJogador->animacaoAndando.quadros,
        novoJogador->animacaoAndando.quantidadeQuadros,
        80,              // duração padrão para cada quadro
        664, 324,        // início
        48, 48,          // dimensões
        4,               // separação
        0, 0,            // deslocamento
        false,           // de trás para frente
        (Rectangle) {    // retângulo de colisão padrão para cada quadro
            18, 20, 54, 76
        }
    );

    novoJogador->animacaoAndandoRapido.quantidadeQuadros = 8;
    novoJogador->animacaoAndandoRapido.quadroAtual = 0;
    novoJogador->animacaoAndandoRapido.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoAndandoRapido.pararNoUltimoQuadro = false;
    novoJogador->animacaoAndandoRapido.executarUmaVez = false;
    novoJogador->animacaoAndandoRapido.finalizada = false;
    createQuadrosAnimacao( &novoJogador->animacaoAndandoRapido, novoJogador->animacaoAndandoRapido.quantidadeQuadros );
    initQuadrosAnimacao( 
        novoJogador->animacaoAndandoRapido.quadros,
        novoJogador->animacaoAndandoRapido.quantidadeQuadros,
        40,              // duração padrão para cada quadro
        664, 324,        // início
        48, 48,          // dimensões
        4,               // separação
        0, 0,            // deslocamento
        false,           // de trás para frente
        (Rectangle) {    // retângulo de colisão padrão para cada quadro
            18, 20, 54, 76
        }
    );

    novoJogador->animacaoCorrendo.quantidadeQuadros = 4;
    novoJogador->animacaoCorrendo.quadroAtual = 0;
    novoJogador->animacaoCorrendo.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoCorrendo.pararNoUltimoQuadro = false;
    novoJogador->animacaoCorrendo.executarUmaVez = false;
    novoJogador->animacaoCorrendo.finalizada = false;
    createQuadrosAnimacao( &novoJogador->animacaoCorrendo, novoJogador->animacaoCorrendo.quantidadeQuadros );
    initQuadrosAnimacao( 
        novoJogador->animacaoCorrendo.quadros,
        novoJogador->animacaoCorrendo.quantidadeQuadros,
        20,              // duração padrão para cada quadro
        24, 397,         // início
        48, 48,          // dimensões
        4,               // separação
        0, 0,            // deslocamento
        false,           // de trás para frente
        (Rectangle) {    // retângulo de colisão padrão para cada quadro
            18, 20, 54, 76
        }
    );

    novoJogador->animacaoPulando.quantidadeQuadros = 4;
    novoJogador->animacaoPulando.quadroAtual = 0;
    novoJogador->animacaoPulando.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoPulando.pararNoUltimoQuadro = false;
    novoJogador->animacaoPulando.executarUmaVez = false;
    novoJogador->animacaoPulando.finalizada = false;
    createQuadrosAnimacao( &novoJogador->animacaoPulando, novoJogador->animacaoPulando.quantidadeQuadros );
    initQuadrosAnimacao( 
        novoJogador->animacaoPulando.quadros,
        novoJogador->animacaoPulando.quantidadeQuadros,
        40,              // duração padrão para cada quadro
        248, 397,        // início
        48, 48,          // dimensões
        4,               // separação
        0, 0,            // deslocamento
        false,           // de trás para frente
        (Rectangle) {    // retângulo de colisão padrão para cada quadro
            18, 36, 60, 60
        }
    );

    novoJogador->animacoes[ESTADO_JOGADOR_PARADO] = &novoJogador->animacaoParado; quantidadeAnimacoes++;
    novoJogador->animacoes[ESTADO_JOGADOR_ANDANDO] = &novoJogador->animacaoAndando; quantidadeAnimacoes++;
    novoJogador->animacoes[ESTADO_JOGADOR_ANDANDO_RAPIDO] = &novoJogador->animacaoAndandoRapido; quantidadeAnimacoes++;
    novoJogador->animacoes[ESTADO_JOGADOR_CORRENDO] = &novoJogador->animacaoCorrendo; quantidadeAnimacoes++;
    novoJogador->animacoes[ESTADO_JOGADOR_PULANDO] = &novoJogador->animacaoPulando; quantidadeAnimacoes++;
    novoJogador->quantidadeAnimacoes = quantidadeAnimacoes;

    return novoJogador;

}

/**
 * @brief Destrói um objeto Jogador e libera seus recursos.
 */
void destroyJogador( Jogador *j ) {
    if ( j != NULL ) {
        free( j );
    }
}

/**
 * @brief Lê a entrada do usuário e atualiza as velocidades do jogador.
 */
void inputJogador( Jogador *j, float delta ) {

    if ( IsKeyDown( KEY_RIGHT ) ) {
        if ( j->vel.x < 0 ) {
            j->vel.x += j->frenagem * delta;
            if ( j->vel.x > 0 ) j->vel.x = 0;
        } else {
            j->vel.x += j->aceleracao * delta;
            if ( j->vel.x > j->velCorrendo ) j->vel.x = j->velCorrendo;
        }
        j->olhandoParaDireita = true;
    } else if ( IsKeyDown( KEY_LEFT ) ) {
        if ( j->vel.x > 0 ) {
            j->vel.x -= j->frenagem * delta;
            if ( j->vel.x < 0 ) j->vel.x = 0;
        } else {
            j->vel.x -= j->aceleracao * delta;
            if ( j->vel.x < -j->velCorrendo ) j->vel.x = -j->velCorrendo;
        }
        j->olhandoParaDireita = false;
    } else {
        if ( j->vel.x > 0 ) {
            j->vel.x -= j->desaceleracao * delta;
            if ( j->vel.x < 0 ) {
                j->vel.x = 0;
            }
        } else if ( j->vel.x < 0 ) {
            j->vel.x += j->desaceleracao * delta;
            if ( j->vel.x > 0 ) {
                j->vel.x = 0;
            }
        }
    }

    float absVelX = fabsf( j->vel.x );
    if ( j->quantidadePulos > 0 ) {
        j->estado = ESTADO_JOGADOR_PULANDO;
    } else if ( absVelX < 1.0f ) {
        j->estado = ESTADO_JOGADOR_PARADO;
    } else if ( absVelX <= j->velAndando ) {
        j->estado = ESTADO_JOGADOR_ANDANDO;
    } else if ( absVelX <= j->velAndandoRapido ) {
        j->estado = ESTADO_JOGADOR_ANDANDO_RAPIDO;
    } else {
        j->estado = ESTADO_JOGADOR_CORRENDO;
    }

    if ( IsKeyPressed( KEY_SPACE ) && j->quantidadePulos < j->quantidadeMaxPulos ) {
        j->vel.y = j->velPulo;
        j->quantidadePulos++;
    }

}

/**
 * @brief Aplica física e resolve colisões do jogador com o mundo.
 */
void updateJogador( Jogador *j, GameWorld *gw, float delta ) {

    Animacao *animacaoAtual = getAnimacaoAtualJogador( j );
    updateAnimacao( animacaoAtual, delta );

    // fase X: move horizontalmente e resolve colisões laterais
    j->ret.x += j->vel.x * delta;
    resolverColisaoJogadorMapaX( j, gw->mapa );

    // fase Y: aplica gravidade, move verticalmente e resolve colisões verticais
    j->vel.y += gw->gravidade * delta;
    if ( j->vel.y > j->velMaxQueda ) {
        j->vel.y = j->velMaxQueda;
    }
    j->ret.y += j->vel.y * delta;
    resolverColisaoJogadorMapaY( j, gw->mapa );

}

/**
 * @brief Desenha o jogador.
 */
void drawJogador( Jogador *j ) {
    QuadroAnimacao *qa = getQuadroAnimacaoAtualJogador( j );
    drawQuadroAnimacaoJogador( j, qa, (Vector2) { 0 }, WHITE );
    if ( MOSTRAR_RETANGULOS ) {
        DrawRectangleRec( j->ret, Fade( j->cor, 0.5f ) );
        DrawRectangleLines( j->ret.x, j->ret.y, j->ret.width, j->ret.height, BLACK );
    }
}

/**
 * @brief Reinicia todas as animações do jogador.
 */
void reiniciarAnimacoesJogador( Jogador *j ) {
    for ( int i = 0; i < j->quantidadeAnimacoes; i++ ) {
        reiniciarAnimacao( j->animacoes[i] );
    }
}

static void drawQuadroAnimacaoJogador( Jogador *j, QuadroAnimacao *qa, Vector2 deslocamento, Color tonalidade ) {

    if ( qa != NULL ) {

        DrawTexturePro(
            rm.texturaJogador,
            (Rectangle) {
                qa->fonte.x,
                qa->fonte.y,
                j->olhandoParaDireita ? qa->fonte.width : -qa->fonte.width,
                qa->fonte.height
            },
            (Rectangle) {
                deslocamento.x + j->ret.x + ( j->olhandoParaDireita ? qa->deslocamentoDesenho.x : -qa->deslocamentoDesenho.x ),
                deslocamento.y + j->ret.y + qa->deslocamentoDesenho.y,
                j->ret.width,
                j->ret.height
            },
            (Vector2) { 0 },
            0.0f,
            tonalidade
        );

        if ( MOSTRAR_RETANGULOS ) {
            float rcDrawX = j->olhandoParaDireita
                ? j->ret.x + qa->deslocamentoDesenho.x + qa->retColisao.x
                : j->ret.x - qa->deslocamentoDesenho.x + j->ret.width - qa->retColisao.x - qa->retColisao.width;
            float rcDrawY = j->ret.y + qa->deslocamentoDesenho.y + qa->retColisao.y;
            DrawRectangle( rcDrawX, rcDrawY, qa->retColisao.width, qa->retColisao.height, Fade( GREEN, 0.5f ) );
        }

    }

}

static QuadroAnimacao *getQuadroAnimacaoAtualJogador( Jogador *j ) {
    return getQuadroAtualAnimacao( getAnimacaoAtualJogador( j ) );
}

static Animacao *getAnimacaoAtualJogador( Jogador *j ) {
    switch ( j->estado ) {
        default:
            return j->animacoes[j->estado];
    }
}

/**
 * @brief Resolve colisões do jogador com o mapa no eixo X.
 */
static void resolverColisaoJogadorMapaX( Jogador *j, Mapa *mapa ) {

    ElementoMapa *el = mapa->elementos;

    while ( el != NULL ) {

        Obstaculo *o = &el->obstaculo;
        QuadroAnimacao *qa = getQuadroAnimacaoAtualJogador( j );

        float rcOffsetX = j->olhandoParaDireita
            ? qa->deslocamentoDesenho.x + qa->retColisao.x
            : -qa->deslocamentoDesenho.x + j->ret.width - qa->retColisao.x - qa->retColisao.width;
        float rcOffsetY = qa->deslocamentoDesenho.y + qa->retColisao.y;

        Rectangle rJ = {
            j->ret.x + rcOffsetX,
            j->ret.y + rcOffsetY,
            qa->retColisao.width,
            qa->retColisao.height
        };

        if ( CheckCollisionRecs( rJ, o->ret ) ) {
            if ( rJ.x + rJ.width / 2 < o->ret.x + o->ret.width / 2 ) {
                j->ret.x = o->ret.x - qa->retColisao.width - rcOffsetX;
            } else {
                j->ret.x = o->ret.x + o->ret.width - rcOffsetX;
            }
            j->vel.x = 0;
        }

        el = el->proximo;

    }

}

/**
 * @brief Resolve colisões do jogador com o mapa no eixo Y.
 */
static void resolverColisaoJogadorMapaY( Jogador *j, Mapa *mapa ) {

    ElementoMapa *el = mapa->elementos;

    while ( el != NULL ) {

        Obstaculo *o = &el->obstaculo;
        QuadroAnimacao *qa = getQuadroAnimacaoAtualJogador( j );

        float rcOffsetX = j->olhandoParaDireita
            ? qa->deslocamentoDesenho.x + qa->retColisao.x
            : -qa->deslocamentoDesenho.x + j->ret.width - qa->retColisao.x - qa->retColisao.width;
        float rcOffsetY = qa->deslocamentoDesenho.y + qa->retColisao.y;

        Rectangle rJ = {
            j->ret.x + rcOffsetX,
            j->ret.y + rcOffsetY,
            qa->retColisao.width,
            qa->retColisao.height
        };

        if ( CheckCollisionRecs( rJ, o->ret ) ) {
            if ( rJ.y + rJ.height / 2 < o->ret.y + o->ret.height / 2 ) {
                j->ret.y = o->ret.y - qa->retColisao.height - rcOffsetY;
                j->quantidadePulos = 0;
            } else {
                j->ret.y = o->ret.y + o->ret.height - rcOffsetY;
            }
            j->vel.y = 0;
        }

        el = el->proximo;

    }

}