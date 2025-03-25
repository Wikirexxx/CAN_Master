/**
 * @file GPIOw.c
 * @author Wiki
 * @brief Este archivo contiene la implementación de funciones para la configuración de pines GPIO.
 * 
 */
#include "GPIOw.h"
#include "tm4c123gh6pm.h"

/**
 * @brief Inicializa el puerto F del microcontrolador.
 * @details Esta función configura los pines PF0 y PF4 como entradas digitales con resistencias pull-up activas.
 */
void GPIOF_Init(void)
{
    SYSCTL_RCGC2_R |= SYSCTL_RCGCGPIO_R5;                            // Sirve para definir que puerto vas a utilizar
    (void)SYSCTL_RCGC2_R;
    GPIO_PORTF_LOCK_R |= 0X4C4F434BU;                                    // Se desbloquean los pines especificos
    GPIO_PORTF_CR_R |= 0X00000011U;                                      // Se se define que pines se utilizaran
    GPIO_PORTF_DEN_R |= 0X000000FFU;                                     // Se activan todos los pines como digitales
    GPIO_PORTF_DIR_R |= 0X0000000EU;                                     // Se configuran las entradas y salidas con 0 pones a los pines como entrada y con 1 pones a los pines como salida
    GPIO_PORTF_PUR_R |= 0X00000011U;                                     // Se habilitan las resistencias
}

