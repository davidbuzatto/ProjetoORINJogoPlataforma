# =============================================================================
# Makefile multiplataforma para projetos C/C++ com raylib
# Baseado em:
#   https://makefiletutorial.com/
#   https://spin.atomicobject.com/2016/08/26/makefile-c-projects/
#
# Uso:
#   make               - compila e executa o projeto
#   make compile       - apenas compila o projeto
#   make run           - apenas executa o binário compilado
#   make clean         - remove os arquivos de build
#   make cleanAndCompile - limpa e recompila
#
# Compatível com: Linux, macOS e Windows (MSYS2 e cmd.exe/MinGW nativo)
# Autor: Prof. Dr. David Buzatto
# =============================================================================


# -----------------------------------------------------------------------------
# DETECÇÃO DO SISTEMA OPERACIONAL
#
# A detecção aqui precisa lidar com um detalhe sutil: o MSYS2 filtra algumas
# variáveis de ambiente do Windows (como OS=Windows_NT) antes de repassá-las
# ao make. Portanto $(OS) está disponível no cmd.exe nativo, mas NÃO está
# no MSYS2. Por isso começamos pelo `uname -s`, que funciona em Linux, macOS
# e em qualquer shell Unix-like no Windows (MSYS2, Cygwin, Git Bash). Se
# uname não existir (cmd.exe puro), a string volta vazia e caímos no teste
# de $(OS) para detectar Windows nativo.
#
# Classificamos em quatro ambientes distintos:
#   - Linux          : Linux nativo
#   - macOS          : macOS (Darwin)
#   - Windows_MSYS2  : Windows rodando num shell Unix (MSYS2, Cygwin, Git Bash).
#                      Comandos Unix (mkdir -p, rm -rf) funcionam; binário .exe.
#   - Windows_Native : Windows rodando no cmd.exe sem camada Unix. Precisa de
#                      comandos cmd (if not exist, rmdir /s /q); binário .exe.
#
# IS_WINDOWS é um sinalizador derivado usado mais abaixo para decidir se
# acrescentamos ".exe" ao nome do executável.
# -----------------------------------------------------------------------------
UNAME_S := $(shell uname -s 2>/dev/null)

ifeq ($(UNAME_S), Linux)
    DETECTED_OS := Linux
    IS_WINDOWS  := 0
else ifeq ($(UNAME_S), Darwin)
    DETECTED_OS := macOS
    IS_WINDOWS  := 0
else ifneq ($(filter MINGW% MSYS% CYGWIN%, $(UNAME_S)),)
    # uname devolve coisas como "MINGW64_NT-10.0-19045", "MSYS_NT-..." ou
    # "CYGWIN_NT-..." quando estamos num shell Unix sobre Windows.
    DETECTED_OS := Windows_MSYS2
    IS_WINDOWS  := 1
else ifeq ($(OS), Windows_NT)
    # Sem uname disponível, mas $(OS) chegou ao make: cmd.exe nativo.
    DETECTED_OS := Windows_Native
    IS_WINDOWS  := 1
else
    DETECTED_OS := Unknown
    IS_WINDOWS  := 0
endif

# -----------------------------------------------------------------------------
# FORÇA DO SHELL NO WINDOWS NATIVO
#
# No Windows nativo, se houver um `sh.exe` no PATH (muito comum quando o Git
# está instalado), o GNU Make tende a escolhê-lo automaticamente como shell
# de execução. Isso quebraria as nossas receitas escritas em sintaxe cmd
# (if not exist, rmdir /s /q). Para garantir que o cmd.exe será usado,
# forçamos SHELL e SHELLFLAGS explicitamente neste caso.
# -----------------------------------------------------------------------------
ifeq ($(DETECTED_OS), Windows_Native)
    SHELL       := cmd.exe
    .SHELLFLAGS := /C
endif


# -----------------------------------------------------------------------------
# NOME DO EXECUTÁVEL
#
# $(CURDIR) é uma variável interna do GNU Make com o caminho absoluto do
# diretório atual. É mais portável que `$(shell pwd)` pois não precisa
# invocar o shell.
#
# $(subst \,/,...) normaliza barras invertidas do Windows para barras normais,
# garantindo que $(notdir ...) funcione corretamente em qualquer plataforma.
#
# $(notdir ...) extrai somente o último componente do caminho (o nome do
# diretório do projeto), descartando todo o restante do caminho absoluto.
#
# No Windows, o GCC espera ".exe" no nome do binário de saída.
# -----------------------------------------------------------------------------
TARGET_NAME := $(notdir $(subst \,/,$(CURDIR)))

ifeq ($(IS_WINDOWS), 1)
    TARGET_EXEC := $(TARGET_NAME).exe
else
    TARGET_EXEC := $(TARGET_NAME)
endif


# -----------------------------------------------------------------------------
# DIRETÓRIOS PRINCIPAIS
# -----------------------------------------------------------------------------
BUILD_DIR := build
SRC_DIRS  := src


# -----------------------------------------------------------------------------
# FUNÇÃO AUXILIAR: BUSCA RECURSIVA DE ARQUIVOS (rwildcard)
#
# Substitui o comando `find` do shell Unix, que não existe no Windows nativo.
# É implementada em make puro (sem invocar o shell), funcionando em qualquer
# plataforma.
#
# Sintaxe de uso: $(call rwildcard, DIRETÓRIO_RAIZ, PADRÕES_SEPARADOS_POR_ESPAÇO)
#   Exemplo: $(call rwildcard, src, *.c *.cpp)
#
# Como funciona passo a passo:
#   $(wildcard $(1:=/*))         → lista todos os itens (arquivos e pastas)
#                                   diretamente dentro do diretório $(1).
#                                   O sufixo ":=/*" concatena "/*" ao nome
#                                   do diretório para montar o padrão correto.
#   $(call rwildcard, $d, $2)    → chamada recursiva para cada item $d.
#                                   O make continua descendo nos subdiretórios
#                                   até não haver mais nada para listar.
#   $(filter $(subst *,%,$2),$d) → filtra o item $d atual pelos padrões em $2.
#                                   Como o make usa % onde o shell usa *, a
#                                   função $(subst *,%, ...) faz essa conversão
#                                   antes de chamar $(filter ...).
#   $(foreach d, LISTA, EXPR)    → itera sobre cada item da LISTA, expandindo
#                                   EXPR com $d substituído pelo item atual.
#                                   Concatena todos os resultados com espaços.
# -----------------------------------------------------------------------------
rwildcard = $(foreach d,$(wildcard $(1:=/*)),\
                $(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))


# -----------------------------------------------------------------------------
# ARQUIVOS FONTE
#
# Coleta todos os .c, .cpp e .s dentro de SRC_DIRS, em qualquer profundidade
# de subdiretório, sem depender do comando `find` do shell.
# -----------------------------------------------------------------------------
SRCS := $(call rwildcard,$(SRC_DIRS),*.c *.cpp *.s)


# -----------------------------------------------------------------------------
# ARQUIVOS OBJETO E DE DEPENDÊNCIA
#
# Para cada arquivo fonte, criamos um arquivo objeto (.o) dentro de BUILD_DIR,
# preservando a estrutura de subdiretórios original.
# Exemplo:  src/GameWorld.c  →  build/src/GameWorld.c.o
#
# O operador de substituição $(SRCS:%=$(BUILD_DIR)/%.o) aplica o padrão
# "prefixo BUILD_DIR/ + valor original + sufixo .o" a cada item de SRCS.
#
# Os arquivos .d registram as dependências de cada .o em relação aos headers
# (#include "...") usados no arquivo fonte correspondente. Eles são gerados
# automaticamente pelas flags -MMD e -MP do GCC/Clang durante a compilação.
# Ao incluí-los ao final deste Makefile (via -include $(DEPS)), o Make passa
# a saber que deve recompilar um .c quando qualquer .h que ele inclui mudar.
# -----------------------------------------------------------------------------
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)


# -----------------------------------------------------------------------------
# DIRETÓRIOS DE INCLUDE (headers)
#
# $(call rwildcard, SRC_DIRS, *) retorna TODOS os arquivos sob SRC_DIRS,
# inclusive os .h que estão em subdiretórios como src/include/. O padrão *
# casa com qualquer nome de arquivo.
#
# $(dir ARQUIVO) extrai a parte de diretório de cada caminho retornado.
# Exemplo: src/include/Foo.h  →  src/include/
#
# $(sort LISTA) ordena e remove duplicatas (cada diretório aparece uma vez).
#
# $(addprefix -I, LISTA) adiciona o prefixo "-I" a cada diretório. Esse flag
# é passado ao compilador para que ele encontre os arquivos .h ao compilar.
# -----------------------------------------------------------------------------
INC_DIRS  := $(sort $(dir $(call rwildcard,$(SRC_DIRS),*)))
INC_FLAGS := $(addprefix -I,$(INC_DIRS))


# -----------------------------------------------------------------------------
# FLAGS DO COMPILADOR
#
# $(INC_FLAGS)           : caminhos de include (-Isrc/ -Isrc/include/ ...)
# -MMD -MP               : geram arquivos .d de dependência automática
# -O1                    : otimização básica (bom equilíbrio entre velocidade
#                          de build e desempenho do binário final)
# -Wall                  : ativa o conjunto principal de warnings do compilador
# -Wextra                : ativa warnings adicionais além dos de -Wall
# -Wno-unused-parameter  : suprime o warning de parâmetros não utilizados
#                          (comum em callbacks de raylib e handlers de evento)
# -pedantic-errors       : trata qualquer extensão fora do padrão como erro
# -std=c99               : segue o padrão ISO C99 para arquivos .c
# -std=c++20             : segue o padrão ISO C++20 para arquivos .cpp
# -Wno-missing-braces    : suprime falsos positivos com initializer lists
#                          aninhados em certas versões do GCC
# -----------------------------------------------------------------------------
CFLAGS   := $(INC_FLAGS) -MMD -MP -O1 -Wall -Wextra \
             -Wno-unused-parameter -pedantic-errors \
             -std=c99 -Wno-missing-braces

CPPFLAGS := $(INC_FLAGS) -MMD -MP -O1 -Wall -Wextra \
             -Wno-unused-parameter -pedantic-errors \
             -std=c++20 -Wno-missing-braces


# -----------------------------------------------------------------------------
# FLAGS DO LINKER (dependem do sistema operacional)
#
# Linux         : raylib é instalado no sistema (ex.: via apt/pacman).
#                 Requer libs OpenGL (GL), pthreads, X11 e outras do sistema.
#
# macOS         : raylib instalado no sistema (ex.: via Homebrew).
#                 Usa frameworks nativos da Apple em vez de libs avulsas.
#                 -framework OpenGL      : renderização 3D/2D
#                 -framework Cocoa       : janelas e eventos do macOS
#                 -framework IOKit       : entrada de dispositivos (teclado, mouse)
#                 -framework CoreVideo   : sincronização de vídeo (VSync)
#
# Windows (ambos): raylib fornecida como biblioteca estática em lib/libraylib.a.
#                 -L lib/    : adiciona lib/ como diretório de busca de libs
#                 -lopengl32 : OpenGL no Windows
#                 -lgdi32    : GDI (funções gráficas Win32)
#                 -lwinmm    : Windows Multimedia (áudio)
# -----------------------------------------------------------------------------
ifeq ($(DETECTED_OS), Linux)
    LDFLAGS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
else ifeq ($(DETECTED_OS), macOS)
    LDFLAGS := -lraylib -framework OpenGL -framework Cocoa \
               -framework IOKit -framework CoreVideo -lm
else
    # Windows (MSYS2 ou nativo) — raylib em lib/ local
    LDFLAGS := -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm -lm
endif


# -----------------------------------------------------------------------------
# COMANDOS DE SHELL DEPENDENTES DO SISTEMA
#
# As ferramentas de linha de comando diferem entre o Windows cmd.exe nativo
# e os ambientes Unix (Linux, macOS, MSYS2).
#
# MKDIR é uma função make chamada com $(call MKDIR, DIRETÓRIO):
#   Unix    : mkdir -p <dir>
#               Cria o diretório e todos os pais necessários.
#               Não gera erro se o diretório já existir (-p).
#   Windows : if not exist "<dir>" mkdir "<dir>"
#               "if not exist" evita erro de "diretório já existe".
#               O mkdir do cmd cria diretórios pai automaticamente.
#               $(subst /,\,...) converte barras / para \ (padrão cmd.exe).
#
# RM_ALL remove o diretório de build e todo seu conteúdo:
#   Unix    : rm -rf build
#   Windows : rmdir /s /q build    (/s = inclui subdiretórios, /q = sem confirmação)
#             Verificamos "if exist" antes para evitar erro se build não existir.
#
# RUN executa o binário compilado:
#   Unix    : ./build/TARGET        (caminho relativo Unix com ./)
#   Windows : build\TARGET.exe      (caminho com barra invertida para cmd.exe)
# -----------------------------------------------------------------------------
ifeq ($(DETECTED_OS), Windows_Native)
    MKDIR  = if not exist "$(subst /,\,$(1))" mkdir "$(subst /,\,$(1))"
    RM_ALL = if exist $(BUILD_DIR) rmdir /s /q $(BUILD_DIR)
    RUN    = $(subst /,\,$(BUILD_DIR)/$(TARGET_EXEC))
else
    # Linux, macOS e Windows_MSYS2 — todos têm ferramentas Unix disponíveis
    MKDIR  = mkdir -p $(1)
    RM_ALL = rm -rf $(BUILD_DIR)
    RUN    = ./$(BUILD_DIR)/$(TARGET_EXEC)
endif


# =============================================================================
# ALVOS (targets)
# =============================================================================

# .PHONY declara alvos que NÃO correspondem a arquivos reais no disco.
# Sem essa declaração, se existisse um arquivo chamado "clean" ou "run" no
# diretório, o Make acharia que o alvo já está "atualizado" e não executaria
# nada. Com .PHONY, o Make sempre executa as receitas desses alvos.
.PHONY: all compile run clean cleanAndCompile

# Alvo padrão (executado com apenas `make`): compila e em seguida executa.
all: compile run

# compile: gera o binário final. Delega para a regra de link abaixo.
compile: $(BUILD_DIR)/$(TARGET_EXEC)

# cleanAndCompile: remove artefatos de builds anteriores e recompila tudo.
cleanAndCompile: clean compile


# -----------------------------------------------------------------------------
# REGRA DE LINK FINAL
#
# Esta regra é acionada quando o binário final precisa ser (re)criado, ou
# seja, quando algum arquivo .o foi modificado ou o binário não existe.
#
# Variáveis automáticas usadas:
#   $@  = nome do alvo desta regra = o executável final
#         (ex.: build/jogo-plataforma-aula-05  ou  build/jogo-plataforma-aula-05.exe)
#   $^  = lista completa de pré-requisitos = todos os .o listados em $(OBJS)
#
# Usamos $(CXX) (compilador C++) para linkar, mesmo em projetos puramente C.
# Isso é seguro em projetos mistos C/C++ e funciona igualmente bem para
# projetos 100% C. Se quiser usar somente $(CC) (compilador C), também funciona
# desde que não haja arquivos .cpp no projeto.
# -----------------------------------------------------------------------------
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(call MKDIR,$(BUILD_DIR))
	$(CXX) $^ -o $@ $(LDFLAGS)


# -----------------------------------------------------------------------------
# REGRA DE COMPILAÇÃO PARA ARQUIVOS C
#
# O padrão "$(BUILD_DIR)/%.c.o: %.c" casa com qualquer arquivo .c em SRCS
# e define como gerar o .o correspondente dentro de BUILD_DIR.
#
# Variáveis automáticas usadas:
#   $<   = primeiro pré-requisito = o arquivo .c sendo compilado
#   $@   = nome do alvo = o arquivo .o sendo gerado
#   $(@D) = diretório do alvo (ex.: build/src) — criado antes de compilar
#            para garantir que a pasta de destino exista
# -----------------------------------------------------------------------------
$(BUILD_DIR)/%.c.o: %.c
	$(call MKDIR,$(@D))
	$(CC) $(CFLAGS) -c $< -o $@


# -----------------------------------------------------------------------------
# REGRA DE COMPILAÇÃO PARA ARQUIVOS C++
# (mesma lógica da regra C, mas usa $(CXX) e $(CPPFLAGS))
# -----------------------------------------------------------------------------
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(call MKDIR,$(@D))
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


# -----------------------------------------------------------------------------
# ALVO clean: remove o diretório de build e todos os artefatos gerados
#
# O prefixo @ antes do comando suprime sua exibição no terminal. Sem ele,
# o Make imprimiria o comando antes de executá-lo. Com @, apenas o output
# do comando (se houver) é mostrado.
# -----------------------------------------------------------------------------
clean:
	@$(RM_ALL)


# -----------------------------------------------------------------------------
# ALVO run: executa o binário compilado
# -----------------------------------------------------------------------------
run:
	$(RUN)


# -----------------------------------------------------------------------------
# INCLUSÃO DOS ARQUIVOS DE DEPENDÊNCIA (.d)
#
# O prefixo - (hífen) instrui o Make a ignorar erros caso algum arquivo .d
# não exista. Isso ocorre normalmente na primeira compilação, quando nenhum
# .d foi gerado ainda. Após o primeiro build, os .d passam a existir e são
# incluídos aqui, tornando o Make ciente das dependências header-por-header.
# -----------------------------------------------------------------------------
-include $(DEPS)
