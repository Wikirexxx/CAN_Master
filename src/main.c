/**
 * @file main.c
 * @brief Ejemplo de configuración de modulo CAN0 en modo Loopback y transmisión.
 * @details Este programa configura el módulo CAN0 para transmitir información y recibir por los perifericos 
 * PB5 y PB4.
 * @author Wiki
 * @date 2025
 */
#include <stdint.h>
#include "PLLw.h"
#include "GPIOw.h"
#include "Delayw.h"
#include "tm4c123gh6pm.h"

/**
 * @brief Función principal.
 * @return 0.
 */
int32_t main(void);

/**
 * @brief Configuración inicial del sistema, incluyendo el PLL inicialización de CAN.
 */
void C_general(void);

/**
 * @brief Función principal del programa.
 * @return No retorna en un sistema embebido.
 */
int32_t main(void)
{
    C_general();
    while(1U)
    {
        GPIO_PORTF_DATA_R = 0X02;
        Delay_ms(100);
        GPIO_PORTF_DATA_R = 0X04;
        Delay_ms(100);
        GPIO_PORTF_DATA_R = 0X08;
        Delay_ms(100);
    }
}
/**
 * @brief Inicializa el PLL y configura los pines necesarios del microcontrolador.
 */
void C_general(void)
{
    PLL_Init();
    GPIOF_Init();
}
