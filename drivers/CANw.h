/**
 * @file CANw.h
 * @author Wiki
 * @brief  Este archivo contiene la definición de funciones para la configuración y manejo del módulo CAN0.
 */
#ifndef CANW_H
#define CANW_H

#include <stdint.h>

/**
 * @brief Se utiliza para cálculos en función de la máxima cantidad de bytes a transmitir
 */
#define max_datos 8
/**
 * @brief Estructura de control para la comunicación por CAN.
 *
 * Esta estructura se utiliza para manejar la configuración y operación de un canal de comunicación CAN,
 * incluyendo transmisión por dos salidas distintas, recepción, objetos de hardware y buffers de datos.
 *
 * | Campo           | Tipo           | Descripción |
 * |----------------|----------------|-------------|
 * | i_CMD          | uint8_t        | Código de comando asociado al mensaje CAN. |
 * | rxdatos        | uint8_t[]      | Arreglo de datos recibidos desde el bus CAN. Tamaño definido por `max_datos`. |
 * | txdatos        | uint8_t[]      | Arreglo de datos que serán transmitidos por el bus CAN. Tamaño definido por `max_datos`. |
 * | tx_id_s1       | const uint32_t | ID de mensaje CAN para transmisión por la salida 1. |
 * | tx_id_s2       | const uint32_t | ID de mensaje CAN para transmisión por la salida 2. |
 * | rx_id          | const uint32_t | ID de mensaje CAN que se espera recibir. |
 * | rx_obj         | uint32_t       | Número de objeto de hardware asignado a la recepción. |
 * | tx_s1_obj      | uint32_t       | Número de objeto de hardware asignado a la transmisión por la salida 1. |
 * | tx_s2_obj      | uint32_t       | Número de objeto de hardware asignado a la transmisión por la salida 2. |
 * | tx_rep         | uint32_t       | Número de repeticiones de transmisión a realizar. |
 * | ini_padding    | uint8_t        | Bandera de inicialización y/o control de padding. Puede utilizarse como marcador de estado. |
 */
typedef struct 
{
    uint8_t i_CMD;
    uint8_t rxdatos[max_datos];
    uint8_t txdatos[max_datos];
    const uint32_t tx_id_s1;
    const uint32_t tx_id_s2;
    const uint32_t rx_id;
    uint32_t rx_obj;
    uint32_t tx_s1_obj;
    uint32_t tx_s2_obj;
    uint32_t tx_rep;
    uint8_t ini_padding;
}can_control_t;

void CAN0_Init(void);
void CAN0_Setmsg(uint32_t id_loc, uint32_t object);
void CAN0_Setrecept(uint32_t id_loc, uint32_t object);
void CAN0_Transmit(const uint8_t Txvec[], uint8_t repeticiones, uint32_t object);
void CAN0_Reception(uint8_t vec[],uint32_t object);
void CAN0_padding(uint8_t vec[], uint8_t inicio, uint8_t fin);

#endif // CANW_H