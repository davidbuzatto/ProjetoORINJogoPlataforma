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
 * @brief Representa o estado do jogador.
 */
typedef enum EstadoJogador {
    ESTADO_JOGADOR_PARADO,
    ESTADO_JOGADOR_ANDANDO,
    ESTADO_JOGADOR_ANDANDO_RAPIDO,
    ESTADO_JOGADOR_CORRENDO,
    ESTADO_JOGADOR_PULANDO,
    ESTADO_JOGADOR_PULANDO_RAPIDO,
    ESTADO_JOGADOR_PULANDO_CORRENDO,
} EstadoJogador;

typedef enum TipoElementoMapa {
    TIPO_ELEMENTO_MAPA_OBSTACULO,
    TIPO_ELEMENTO_MAPA_ITEM,
    TIPO_ELEMENTO_MAPA_INIMIGO,
} TipoElementoMapa;

/**
 * @brief Representa um quadro de animação com imagem.
 */
typedef struct QuadroAnimacao {
    Rectangle fonte;
    int duracao;
    Rectangle retColisao; // deve ser interpretado como posicionamento relativo
    Vector2 deslocamentoDesenho;
} QuadroAnimacao;

/**
 * @brief Representa uma animação.
 */
typedef struct Animacao {
    QuadroAnimacao *quadros;
    int quantidadeQuadros;
    int quadroAtual;
    int contadorTempoQuadro;
    bool pararNoUltimoQuadro;
    bool executarUmaVez;
    bool finalizada;
} Animacao;

/**
 * @brief Representa o jogador controlado pelo usuário.
 */
typedef struct Jogador {

    Rectangle ret;
    Vector2 vel;
    Color cor;

    float velAndando;
    float velAndandoRapido;
    float velCorrendo;
    float velPulo;
    float velMaxQueda;

    float aceleracao;
    float desaceleracao;
    float frenagem;

    int quantidadePulos;
    int quantidadeMaxPulos;

    EstadoJogador estado;
    bool olhandoParaDireita;

    Animacao *animacoes[20];
    int quantidadeAnimacoes;

    Animacao animacaoParado;
    Animacao animacaoAndando;
    Animacao animacaoAndandoRapido;
    Animacao animacaoCorrendo;
    Animacao animacaoPulando;
    Animacao animacaoPulandoRapido;
    Animacao animacaoPulandoCorrendo;

} Jogador;

/**
 * @brief Representa um item estático do cenário.
 */
typedef struct Item {
    Rectangle ret;
    Color cor;
    Rectangle fonte;
    Texture2D *textura;
} Item;

/**
 * @brief Representa um obstáculo estático do cenário.
 */
typedef struct Obstaculo {
    Rectangle ret;
    Color cor;
    Rectangle fonte;
    Texture2D *textura;
} Obstaculo;

/**
 * @brief Representa um elemento do mapa.
 */
typedef struct ElementoMapa ElementoMapa;
struct ElementoMapa {
    void *endereco;
    TipoElementoMapa tipo;
    ElementoMapa *proximo;
};

/**
 * @brief Representa um mapa de fase do jogo.
 */
typedef struct Mapa {

    // lista ligada de itens
    ElementoMapa *elementos; // fim da lista
    int quantidadeElementos;

    float tamanhoElemento;
    int linhas;
    int colunas;

} Mapa;

/**
 * @brief Representa o mundo do jogo e seus elementos.
 */
typedef struct GameWorld {

    Mapa *mapa;
    Jogador *jogador;

    Camera2D camera;

    float gravidade;

} GameWorld;