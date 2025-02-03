#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "interrupcoes_rp2040.pio.h"

#define LED_PINS 7
#define NUM_PINS 25
#define BTN_A 5
#define BTN_B 6
#define LED_RED 13
#define LED_GREEN 12
#define LED_BLUE 11
#define DEBOUNCE_TIME 250000 // 250ms
#define time_rgb 100         // 100ms

volatile int num_matriz = 0;         // variável que armazena o número da matriz
volatile uint32_t last_time_a = 0;   // variável que armazena o tempo do último clique do botão A
volatile uint32_t last_time_b = 0;   // variável que armazena o tempo do último clique do botão B
volatile uint32_t last_time_rgb = 0; // variável que armazena o tempo do último clique do botão RGB

PIO pio; // variável que armazena o endereço do pio
uint sm; // variável que armazena o endereço do state machine
// matriz de números
const int matriz_numeros[10][25] = {
    {1, 1, 1, 1, 1,
     1, 0, 0, 0, 1,
     1, 0, 0, 0, 1,
     1, 0, 0, 0, 1,
     1, 1, 1, 1, 1},

    {0, 0, 0, 0, 1,
     0, 0, 0, 0, 1,
     0, 0, 0, 0, 1,
     0, 0, 0, 0, 1,
     0, 0, 0, 0, 1},

    {1, 1, 1, 1, 1,
     0, 0, 0, 0, 1,
     1, 1, 1, 1, 1,
     1, 0, 0, 0, 0,
     1, 1, 1, 1, 1},

    {1, 1, 1, 1, 1,
     0, 0, 0, 0, 1,
     1, 1, 1, 1, 1,
     0, 0, 0, 0, 1,
     1, 1, 1, 1, 1},

    {1, 0, 0, 0, 1,
     1, 0, 0, 0, 1,
     1, 1, 1, 1, 1,
     0, 0, 0, 0, 1,
     0, 0, 0, 0, 1},

    {1, 1, 1, 1, 1,
     1, 0, 0, 0, 0,
     1, 1, 1, 1, 1,
     0, 0, 0, 0, 1,
     1, 1, 1, 1, 1},

    {1, 1, 1, 1, 1,
     1, 0, 0, 0, 0,
     1, 1, 1, 1, 1,
     1, 0, 0, 0, 1,
     1, 1, 1, 1, 1},

    {1, 1, 1, 1, 1,
     0, 0, 0, 0, 1,
     0, 0, 1, 1, 1,
     0, 0, 0, 0, 1,
     0, 0, 0, 0, 1},

    {1, 1, 1, 1, 1,
     1, 0, 0, 0, 1,
     1, 1, 1, 1, 1,
     1, 0, 0, 0, 1,
     1, 1, 1, 1, 1},

    {1, 1, 1, 1, 1,
     1, 0, 0, 0, 1,
     1, 1, 1, 1, 1,
     0, 0, 0, 0, 1,
     1, 1, 1, 1, 1}};
// função que retorna o valor RGB
uint32_t retorno_rgb(double r, double g, double b)
{
    unsigned char R = r * 255;
    unsigned char G = g * 255;
    unsigned char B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}
// função que anima a matriz
void animacao(PIO pio, uint sm, int num)
{
    uint32_t buffer[NUM_PINS];
    int acc = 0;
    for (int i = 24; i >= 20; i--)
    {
        if (matriz_numeros[num][i] == 1)
        {
            buffer[acc] = retorno_rgb(1, 0, 0);
            acc++;
        }
        else
        {
            buffer[acc] = retorno_rgb(0, 0, 0);
            acc++;
        }
    }
    for (int i = 15; i <= 19; i++)
    {
        if (matriz_numeros[num][i] == 1)
        {
            buffer[acc] = retorno_rgb(1, 0, 0);
            acc++;
        }
        else
        {
            buffer[acc] = retorno_rgb(0, 0, 0);
            acc++;
        }
    }
    for (int i = 14; i >= 10; i--)
    {
        if (matriz_numeros[num][i] == 1)
        {
            buffer[acc] = retorno_rgb(1, 0, 0);
            acc++;
        }
        else
        {
            buffer[acc] = retorno_rgb(0, 0, 0);
            acc++;
        }
    }
    for (int i = 5; i <= 9; i++)
    {
        if (matriz_numeros[num][i] == 1)
        {
            buffer[acc] = retorno_rgb(1, 0, 0);
            acc++;
        }
        else
        {
            buffer[acc] = retorno_rgb(0, 0, 0);
            acc++;
        }
    }
    for (int i = 4; i >= 0; i--)
    {
        if (matriz_numeros[num][i] == 1)
        {
            buffer[acc] = retorno_rgb(1, 0, 0);
            acc++;
        }
        else
        {
            buffer[acc] = retorno_rgb(0, 0, 0);
            acc++;
        }
    }
    for (int i = 0; i < NUM_PINS; i++)
    {
        pio_sm_put_blocking(pio, sm, buffer[i]);
    }
}
// função que trata a interrupção dos botões
void interrupcao_btn(uint gpio, uint32_t events)
{
    uint32_t timestamp = to_us_since_boot(get_absolute_time());
    if (gpio == BTN_A && timestamp - last_time_a > DEBOUNCE_TIME)
    {
        last_time_a = timestamp;
        num_matriz + 1 > 9 ? num_matriz = 9 : num_matriz++;
        printf("Botão A incrementando número da matriz, valor atual: %d\n", num_matriz);
    }
    else if (gpio == BTN_B && timestamp - last_time_b > DEBOUNCE_TIME)
    {
        last_time_b = timestamp;
        num_matriz - 1 < 0 ? num_matriz = 0 : num_matriz--;
        printf("Botão B decrementando número da matriz, valor atual: %d\n", num_matriz);
    }
    animacao(pio, sm, num_matriz);
}
// função principal
int main()
{
    stdio_init_all();
    pio = pio0;
    uint offset = pio_add_program(pio, &interrupcoes_rp2040_program);
    sm = pio_claim_unused_sm(pio, true);
    interrupcoes_rp2040_program_init(pio, sm, offset, LED_PINS);
    gpio_init(LED_RED);
    gpio_init(LED_GREEN);
    gpio_init(LED_BLUE);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_set_dir(LED_BLUE, GPIO_OUT);
    gpio_init(BTN_A);
    gpio_init(BTN_B);
    gpio_set_dir(BTN_A, GPIO_IN);
    gpio_set_dir(BTN_B, GPIO_IN);
    gpio_pull_up(BTN_A);
    gpio_pull_up(BTN_B);
    gpio_set_irq_enabled_with_callback(BTN_A, GPIO_IRQ_EDGE_FALL, true, &interrupcao_btn);
    gpio_set_irq_enabled_with_callback(BTN_B, GPIO_IRQ_EDGE_FALL, true, &interrupcao_btn);
    animacao(pio, sm, 0);
    while (true)
    {
        gpio_put(LED_RED, !gpio_get(LED_RED));
        sleep_ms(time_rgb);
    }
}