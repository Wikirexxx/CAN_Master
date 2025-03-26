/**
 * @file GPIOw.h
 * @author Wiki
 * @brief Este archivo contiene la definición de funciones para la configuración de pines GPIO.
 * 
 */

#ifndef GPIOW_H
#define GPIOW_H

#include <stdint.h>

/**
 * @brief Define usado para hacer referencia a led rojo PF1
 */
#define     Led_rojo    0x02U
/**
 * @brief Define usado para hacer referencia a led azul PF2
 */
#define     Led_azul    0x04U
/**
 * @brief Define usado para hacer referencia a led verde PF3
 */
#define     Led_verde   0x08U


#define     write_portf(x) \
            GPIO_PORTF_DATA_R = (x);

void GPIOF_Init(void);

#endif // GPIOW_H