#include <stdint.h>
#include "uart.h"

// Fonction pour convertir un nombre en chaîne
void uint32_to_string(uint32_t num, uint8_t* str)
{
    uint8_t temp[12];
    int i = 0;
    
    if(num == 0)
    {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    
    while(num > 0)
    {
        temp[i++] = (num % 10) + '0';
        num /= 10;
    }
    
    // Inverser la chaîne
    for(int j = 0; j < i; j++)
    {
        str[j] = temp[i - 1 - j];
    }
    str[i] = '\0';
}

// Fonction pour lire le compteur de cycles RISC-V (mcycle) 32-bit
static inline uint32_t get_cycle_count(void)
{
    uint32_t cycles;
    _asm_ volatile ("rdcycle %0" : "=r" (cycles));
    return cycles;
}


void init_cycle_counter(void)
{
    
}

// calcul de la somme des carrés
uint32_t calculer_somme_carres(uint32_t n)
{
    uint32_t somme = 0;
    for(uint32_t i = 1; i <= n; i++)
    {
        somme += i * i;
    }
    return somme;
}

// tri à bulles
void tri_bulles(uint32_t* tableau, uint32_t taille)
{
    for(uint32_t i = 0; i < taille - 1; i++)
    {
        for(uint32_t j = 0; j < taille - i - 1; j++)
        {
            if(tableau[j] > tableau[j + 1])
            {
                uint32_t temp = tableau[j];
                tableau[j] = tableau[j + 1];
                tableau[j + 1] = temp;
            }
        }
    }
}

int main(void)
{
    uint32_t start_cycles, end_cycles, execution_cycles;
    uint8_t time_str[12];
    
    // Initialisation UART
    UART_init(&g_uart_0,
              UART_115200_BAUD,
              UART_DATA_8_BITS | UART_NO_PARITY | UART_ONE_STOP_BIT);
    
    init_cycle_counter();
    
   
    UART_polled_tx_string(&g_uart_0, (uint8_t*)"\r\n=== Calcul somme des carres ===\r\n");
    while(UART_tx_complete(&g_uart_0) == 0);
    
    start_cycles = get_cycle_count();
    volatile int n = 1000;
    volatile uint32_t resultat = calculer_somme_carres(n);
    
    end_cycles = get_cycle_count();
    
    if(end_cycles >= start_cycles)
        execution_cycles = end_cycles - start_cycles;
    else
        execution_cycles = (0xFFFFFFFF - start_cycles) + end_cycles + 1;
    
    UART_polled_tx_string(&g_uart_0, (uint8_t*)"Resultat: ");
    uint32_to_string(resultat, time_str);
    UART_polled_tx_string(&g_uart_0, time_str);
    UART_polled_tx_string(&g_uart_0, (uint8_t*)"\r\n");
    while(UART_tx_complete(&g_uart_0) == 0);
    
    UART_polled_tx_string(&g_uart_0, (uint8_t*)"Temps: ");
    uint32_to_string(execution_cycles, time_str);
    UART_polled_tx_string(&g_uart_0, time_str);
    UART_polled_tx_string(&g_uart_0, (uint8_t*)" cycles\r\n");
    while(UART_tx_complete(&g_uart_0) == 0);
    
    
    UART_polled_tx_string(&g_uart_0, (uint8_t*)"\r\n=== Tri a bulles ===\r\n");
    while(UART_tx_complete(&g_uart_0) == 0);
    
    uint32_t tableau[50];
    // Remplir le tableau avec des valeurs décroissantes
    for(uint32_t i = 0; i < 50; i++)
    {
        tableau[i] = 50 - i;
    }
    
    start_cycles = get_cycle_count();
    
    tri_bulles(tableau, 50);
    
    end_cycles = get_cycle_count();
    
    if(end_cycles >= start_cycles)
        execution_cycles = end_cycles - start_cycles;
    else
        execution_cycles = (0xFFFFFFFF - start_cycles) + end_cycles + 1;
    
    UART_polled_tx_string(&g_uart_0, (uint8_t*)"Tableau trie!\r\n");
    UART_polled_tx_string(&g_uart_0, (uint8_t*)"Temps: ");
    uint32_to_string(execution_cycles, time_str);
    UART_polled_tx_string(&g_uart_0, time_str);
    UART_polled_tx_string(&g_uart_0, (uint8_t*)" cycles\r\n");
    while(UART_tx_complete(&g_uart_0) == 0);
    
   
    uint32_t time_us = execution_cycles / 50;  
    
    UART_polled_tx_string(&g_uart_0, (uint8_t*)"Soit: ");
    uint32_to_string(time_us, time_str);
    UART_polled_tx_string(&g_uart_0, time_str);
    UART_polled_tx_string(&g_uart_0, (uint8_t*)" us\r\n");
    while(UART_tx_complete(&g_uart_0) == 0);
    
    return 0;
}