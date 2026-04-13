<p align="center"> <img src="https://img.shields.io/badge/C-Language-blue.svg"> <img src="https://img.shields.io/badge/Status-Acadêmico-green"> <img src="https://img.shields.io/badge/Platform-Windows%20%7C%20Linux-lightgrey"> </p>

# Manipulando Arquivos - Projeto em C

Sistema desenvolvido em linguagem C com o objetivo de unificar múltiplos arquivos CSV em um único arquivo consolidado. 
O programa percorre diversos arquivos contendo dados (como registros de tribunais eleitorais), realiza a leitura linha a linha e gera um arquivo unificado, evitando duplicação de cabeçalhos.

---

## Objetivos do projeto

- Aplicar conceitos de manipulação de arquivos em C;
- Trabalhar com leitura e escrita em arquivos CSV;
- Implementar processamento sequencial de múltiplas fontes de dados;
- Evitar duplicação de informações;
- Organizar dados de diferentes estados em um único arquivo;
- Praticar modularização com arquivos `.c` e `.h`.

---

## Funcionalidades

- Leitura de múltiplos arquivos CSV;
- Abertura e validação de arquivos;
- Concatenação de dados em um único arquivo;
- Remoção automática de cabeçalhos duplicados;
- Escrita organizada em arquivo de saída;
- Tratamento básico de erros.

---

## Estrutura de diretórios

```
APE_TJ/
│
├── src/
│   ├── main.c               # Função principal do projeto
│   ├── tj.h                 # Definições e estruturas
│   ├── concatenar.c         # Lógica de concatenação
│   ├── teste_TRE-AC.csv
│   ├── teste_TRE-SP.csv
│   └── ... (demais arquivos CSV)
│
├── arqConcatenado.csv       # Arquivo gerado pelo programa
│
└── README.md
```

---

## Estrutura dos Modulos

### `main.c`
Responsável por iniciar o programa. 
- Chama a função principal `ConcatenarArqvs()`;
- Controla o fluxo inicial da aplicação.

---

### `tj.h`
Arquivo de cabeçalho responsável por:
- Definir a estrutura `cálculos`;
- Declarar funções utilizadas no sistema.

A estrutura armazena dados estatísticos como:
- Suspensões
- Julgamentos
- Distribuições
- Indicadores do ano de 2026.

---

### `tj.c`
Responsável pela lógica principal do sistema.
Funções implementadas:
- Abertura de arquivos CSV;
- Leitura linha a linha;
- Escrita no arquivo final;
- Controle de cabeçalhos.

---

## Funcionamento do sistema 
### Entrada
O sistema utiliza um vetor contendo os caminhos dos arquivos:
```
char *caminhos[] = {
     "src/teste_TRE-AC.csv",
     "src/teste_TRE-SP.csv",
     ...
};
```
Cada arquivo representa dados de um estado brasileiro.

---

## Processamento
O programa executa os seguintes passos:
 1. Cria o arquivo de saída:    
     ```
     arqConcatenado.csv
     ```
 2. Percorre todos os arquivos de entrada
 3. Para cada arquivo:
    - Abre o arquivo;
    - Ignora o cabeçalho (exceto o primeiro);
    - Lê linha por linha;
    - Escreve no arquivo final.

---

### Saída
Um único arquivo consolidado:
```
arqConcatenado.csv
```
Contendo todos os dados unificados.

---

## Tratamento de Erros
O sistema realiza verificações básicas:
- Falha ao abrir arquivos de saída
- Falha ao abrir arquivos de entrada
Exemplo:
```
if (!saída){
    printf("ERRO AO CRIAR ARQUIVO DE SAIDA!");
}
```
---

## Requisitos 
- Compilador C (GCC, Clang ou MSVC)
- Sistema operacional Windows, Linux ou macOS
- CMake (opcional)

---

## Executar no Terminal 
Linux
```
gcc -o ape src/*.c
.ape
```

Windows (MinGW)
```
gcc -o ape.exe src/*.c
ape.exe
```
---

## Compilação com CMake (Opcional)
```bash
mkdir build
cd build
cmake ..
make
./ape
```

Windows
```bash
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
mingw32-make
ape.exe
```

---

## Gerar documentação com Doxygen
Pré-requisitos:
- Doxygen instalado na máquina

### 1. Criar Doxyfile
Vá até o diretório do projeto, e digite:
```bash
doxygen -g
```

### 2. Configurar

No Doxyfile altere:

```
PROJECT_NAME     = "APE_TJ"
INPUT            = ./src
RECURSIVE        = YES
GENERATE_HTML    = YES
GENERATE_LATEX   = YES
OUTPUT_DIRECTORY = docs
```

Para usar o README como página inicial:

```
USE_MDFILE_AS_MAINPAGE = README.md
```

### 3. Gerar documentação

```bash
doxygen Doxyfile
```

---

# Visualizar HTML

```bash
start html/index.html
```

ou

```bash
start docs/html/index.html
```

---

# Gerar PDF da Documentação com LaTeX

## 1. Instalar LaTeX

### Windows (recomendado)
Instalar MiKTeX:

https://miktex.org/download

Marque:

```
Install missing packages automatically
```

### Linux (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install texlive-full
```

### macOS

```bash
brew install mactex
```

---

## 2. Compilar PDF

Depois de gerar o LaTeX com o Doxygen:

```bash
cd latex
```

Use o XeLaTeX (melhor para Unicode):

```bash
xelatex refman.tex
xelatex refman.tex
```

O PDF será criado em:

```
latex/refman.pdf
```
ou 

```
docs/latex/refman.pdf
```
---

## 3. Abrir o PDF

### Windows

```bash
start refman.pdf
```

### Linux

```bash
xdg-open refman.pdf
```

### macOS

```bash
open refman.pdf
```

---

# Compilar pelo CLion

1. Abrir o projeto  
2. Esperar o CMake carregar  
3. `Ctrl + F9` para compilar  
4. `Shift + F10` para executar

---

## Comandos Úteis

Apagar build do CMake:

```bash
rm -rf build
```

Gerar documentação novamente:

```bash
doxygen Doxyfile
```

Recompilar PDF:

```bash
cd latex
xelatex refman.tex
```

---

## Tecnologias Utilizadas
- Linguagem C
- Biblioteca padrão:
  - `<stdio.h>`
  - `<stdlib.h>`
  - `<string.h>`

---

## Considerações Finais 
Este projeto demonstra na prática:
- Manipulação de arquivos em c
- Organização de códigos em módulos
- Processamento de dados estruturados

---

## Autores
- Carlos Eduardo Estrela
- Cesar Amaru
- Gabriel Alves
- Guilherme Romaniuk
- Guilherme Alfeu
- Thyago Oliveira
- Yan Gabriel Pereira

---

## Licença
Projeto desenvolvido para fins acadêmicos, podendo ser utilizado como base para estudos e aprimoramentos em programação na linguagem C.
