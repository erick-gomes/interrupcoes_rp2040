# Projeto Interrupções RP2040

Este projeto demonstra o uso de interrupções no microcontrolador RP2040 para controlar uma matriz de LEDs e responder a eventos de botões. O código é escrito em C e utiliza a biblioteca Pico SDK.

## Descrição

O projeto consiste em uma matriz de LEDs que exibe números de 0 a 9. Dois botões são usados para incrementar e decrementar o número exibido na matriz. As interrupções são configuradas para detectar os cliques dos botões e atualizar a matriz de LEDs em tempo real.

## Funcionalidades

- Exibição de números de 0 a 9 em uma matriz de LEDs.
- Controle de incremento e decremento dos números através de dois botões.
- Debouncing de botões para evitar múltiplas detecções de um único clique.
- Animação da matriz de LEDs com cores RGB.

## Estrutura do Código

- `main()`: Função principal que inicializa o sistema e configura as interrupções.
- `interrupcao_btn()`: Função de callback que trata as interrupções dos botões.
- `animacao()`: Função que atualiza a matriz de LEDs com o número atual.
- `retorno_rgb()`: Função que retorna o valor RGB para os LEDs.

## Requisitos

- Placa com microcontrolador RP2040 (ex: Raspberry Pi Pico).
- Biblioteca Pico SDK instalada.
- LEDs e botões conectados aos pinos especificados no código.

## Como Executar

1. Clone o repositório.
2. Configure o ambiente de desenvolvimento para o Pico SDK.
3. Compile o código e carregue-o na placa RP2040.
4. Conecte os LEDs e botões conforme especificado no código.
5. Execute o programa e interaja com os botões para ver a matriz de LEDs em ação.


