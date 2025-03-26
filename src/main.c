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
#include "CANw.h"
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
    uint8_t cs1 = 0U;
    uint8_t cs2 = 0U;
    C_general();
    can_control_t can_c_local_t=
    {
        .i_CMD = 0U,
        .rxdatos = {0U},
        .txdatos = {0U},
        .tx_id_s1 = 0x101U,
        .tx_id_s2 = 0x102U,
        .rx_id = 0x100U,
        .rx_obj = 3U,
        .tx_s1_obj = 1U,
        .tx_s2_obj = 2U,
        .tx_rep =  1U,
        .ini_padding = 1U
    };

    CAN0_Setmsg(can_c_local_t.tx_id_s1, can_c_local_t.tx_s1_obj);
    CAN0_Setmsg(can_c_local_t.tx_id_s2, can_c_local_t.tx_s2_obj);
    CAN0_Setrecept(can_c_local_t.rx_id, can_c_local_t.rx_obj);

    while(1U)
    {
        if((GPIO_PORTF_DATA_R & 0x01U) == 0U)
        {
            write_portf(Led_azul);
            can_c_local_t.txdatos[0U] = cs1 + 1U;
            CAN0_Transmit(can_c_local_t.txdatos,can_c_local_t.tx_rep, can_c_local_t.tx_s1_obj);
            Delay_ms(10U);
            CAN0_Reception(can_c_local_t.rxdatos,can_c_local_t.rx_obj);
            Delay_ms(500U);
            while((GPIO_PORTF_DATA_R & 0x01U) == 0U) 
            {
                write_portf(Led_rojo);
            }
            Delay_ms(50U);
            cs1++;
            if(cs1 >= 4U)
            {
                cs1 = 0U;
            }
        }
        if((GPIO_PORTF_DATA_R & 0x10U) == 0U)
        {
            write_portf(Led_azul);
            can_c_local_t.txdatos[0U] = cs2 + 1U;
            CAN0_Transmit(can_c_local_t.txdatos,can_c_local_t.tx_rep, can_c_local_t.tx_s2_obj);
            CAN0_Reception(can_c_local_t.rxdatos,can_c_local_t.rx_obj);
            Delay_ms(500U);
            while((GPIO_PORTF_DATA_R & 0x10U) == 0U) 
            {
                write_portf(Led_rojo);
            }
            Delay_ms(50U);
            cs2++;
            if(cs2 >= 4U)
            {
                cs2 = 0U;
            }
        }
    }
}
/**
 * @brief Inicializa el PLL y configura los pines necesarios del microcontrolador.
 */
void C_general(void)
{
    PLL_Init();
    GPIOF_Init();
    CAN0_Init();
}
