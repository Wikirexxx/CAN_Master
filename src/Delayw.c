#include <Delayw.h>
#include <tm4c123gh6pm.h>

/**
 * @brief Constante calculada para retardo en ms considerando un reloj de 80 MHz (b)
 */
#define ms_c0 -2.8921f
/**
 * @brief Constante calculada para retardo en ms considerando un reloj de 80 MHz (ax)
 */
#define ms_c1 6.6667e+03f
/**
 * @brief Constante calculada para retardo en us considerando un reloj de 80 MHz (b)
 */
#define us_c0 -4.583848979374437f
/**
 * @brief Constante calculada para retardo en us considerando un reloj de 80 MHz (ax)
 */
#define us_c1 6.667162584302207f

/**
 * @brief Función de retardo en milisegundos.
 * @param ms: Cantidad de
 * milisegundos a esperar.
 */
void Delay_ms(uint32_t ms)
{
    uint32_t ims = 0U;
    uint32_t Multiploms = 0U;
    Multiploms = ms_c0 + (ms_c1 * ms);
    for(ims = 0U ; ims < Multiploms ; ims++){}
}
/**
 * @brief Función de retardo en microsegundos.
 * @param us: Cantidad de microsegundos a esperar.
 */
void Delay_us(uint32_t us)
{
    uint32_t ius = 0U;
    uint32_t Multiplous = 0U;
    Multiplous = us_c0 + (us_c1 * us);
    for(ius = 0U ; ius < Multiplous ; ius++){}
}
