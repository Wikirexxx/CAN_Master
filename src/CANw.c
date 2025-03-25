/**
 * @file CANw.c
 * @author Wiki
 * @brief Este archivo contiene la implementación de funciones para la configuración y manejo del módulo CAN0.
 */
#include "CANw.h"
#include "tm4c123gh6pm.h"

/**
 * @brief Inicializa el módulo CAN0 del microcontrolador.
 * @details Esta función configura el módulo CAN0 para transmitir y recibir información por los pines PB5 y PB4, además de configurar el reloj del sistema
 * para que el módulo CAN0 pueda operar correctamente a una frecuencia de 1Mbps considerando 80MHz de frecuencia de reloj.
 */

void CAN0_Init(void)
{
    SYSCTL_RCGC0_R |= 0x01000000U;                      // Bit 24: habilita módulo CAN0
    SYSCTL_RCGC2_R |= 0x00000002U;                      // Bit 1: habilita reloj para GPIO Puerto B
    (void)SYSCTL_RCGC2_R;                               // Permite que el reloj del sistema se estabilice

    // 3. Configurar PB4 y PB5 para CAN0 (CAN0RX, CAN0TX)
    GPIO_PORTB_AFSEL_R |= 0x00000030U;                  // PB4, PB5 en función alternativa DS. pag. 671
    GPIO_PORTB_PCTL_R &= (uint32_t)(~0x00FF0000U);      // Limpiar campos de control (PCTL) para PB4, PB5
    GPIO_PORTB_PCTL_R |= 0x00880000U;                   // Asignar función CAN0: valor 0x8 para PB4 (CAN0Rx) y PB5 (CAN0Tx) DS. Table 10-2
    GPIO_PORTB_AMSEL_R &= (uint32_t)(~0x00000030U);     // Deshabilitar modo analógico en PB4, PB5
    GPIO_PORTB_DEN_R   |= 0x00000030U;                  // Habilitar función digital en PB4, PB5
    GPIO_PORTB_DIR_R   |= 0x00000020U;                  // PB5 como salida (CAN TX)
    GPIO_PORTB_DIR_R   &= (uint32_t)(~0x00000010U);     // PB4 como entrada (CAN RX)

    // 4. Inicializar el módulo CAN0
    CAN0_CTL_R |= 0x00000041U;                          // Establecer INIT (bit0) y CCE (bit6) para entrar a modo init/config
    // Configurar velocidad: BRP = 9, TSEG1 = 6, TSEG2 = 1, SJW = 1
    // CAN_BIT_R: TSEG2 [14:12], TSEG1 [11:8], SJW [7:6], BRP [5:0]
    CAN0_BIT_R = 0x00000509U;                           // Valores programados: TSEG2 = 1-1 = 0, TSEG1 = 6-1 = 5, SJW = 1-1 = 0, BRP = 9
    CAN0_BRPE_R = 0x00000000U;                          // Extensión de prescaler (no se usa, BRP cabe en 6 bits)
    CAN0_CTL_R &= (uint32_t)(~0x00000041U);             // Salir de modo init (borra INIT y CCE, habilita CAN)
    // (En este punto, CAN0 está en modo normal funcionando a 1 Mbps)
}
/**
 * @brief Configura un objeto de mensaje para transmisión o recepción.
 * @details Esta función configura un objeto
 * @param id_loc Identificador del mensaje a configurar.
 * @param object Número de objeto de mensaje a configurar.
 */
void CAN0_Setmsg(uint32_t id_loc, uint32_t object)
{
    // Configurar Objeto de Mensaje 1 para Transmisión (TX)
    // - ID estándar = ID_Loc, DLC = 8 bytes, objeto válido.
    // - No se usa máscara (no aplica a TX), no iniciar transmisión aún (TXRQST=0).
    while(CAN0_IF1CRQ_R & 0x00008000U); // Esperar si IF1 está ocupado
    CAN0_IF1ARB1_R = 0x00000000U;                       // ID bits [15:0] (no usado en 11-bit std)
    CAN0_IF1ARB2_R = (id_loc << 2U) | 0x0000A000U;      // Colocar ID en bits [12:2], XTD=0, DIR=1 (TX), MSGVAL=1
    CAN0_IF1MCTL_R = (0x8U & 0xFU) | 0x00000080U;       // DLC=8 (bits3-0 = 1000b) y EOB=1 (bit7). (TXRQST=0, UMASK=0)
    CAN0_IF1CMSK_R = 0x000000B0U;                       // WRNRD=1|ARB|CONTROL (0x80+0x20+0x10 = 0xB0), sin transferir máscara ni datos ahora
    CAN0_IF1CRQ_R  = object;                            // Ejecutar transferencia a objeto 1
    while(CAN0_IF1CRQ_R & 0x00008000U);                 // Esperar a que finalice
}
/**
 * @brief Configura un objeto de mensaje para transmisión o recepción.
 * @details Esta función configura un objeto
 * @param id_loc Identificador del mensaje a configurar.
 * @param object Número de objeto de mensaje a configurar.
 */
void CAN0_Setrecept(uint32_t id_loc, uint32_t object)
{
    // Configurar Objeto de Mensaje 2 para Recepción (RX)
    // - ID estándar = ID_Loc, filtro exacto a ese ID (usar máscara completa de 11 bits).
    // - DLC esperado = 8, habilitar máscara (UMASK), objeto válido.
    while(CAN0_IF2CRQ_R & 0x00008000U); // Esperar si IF2 está ocupado
    // Configurar máscara de aceptación: 11 bits deben coincidir con ID (ID exacto)
    CAN0_IF2MSK1_R = 0x0000U;                       // Mask bits [15:0] (no usado para 11-bit ID)
    CAN0_IF2MSK2_R = 0x9FFCU;                       // Mask bits [12:0] = 0x7FF<<2 (11 bits en 1 desde bit2 a bit12) + MXTD=1 (bit15). MDIR=0.
    CAN0_IF2ARB1_R = 0x0000U;                       // ID extended bits [15:0] (no usado en estándar)
    CAN0_IF2ARB2_R = (id_loc << 2U) | 0x8000U;       // Colocar ID en bits [12:2], XTD=0, DIR=0 (RX), MSGVAL=1
    CAN0_IF2MCTL_R = 0x1088U;                       // DLC=8, EOB=1, UMASK=1 (bit12) para filtrar ID, sin INTs
    CAN0_IF2CMSK_R = 0xF0U;                         // WRNRD=1|MASK|ARB|CONTROL (0x80+0x40+0x20+0x10 = 0xF0), sin transferir datos
    CAN0_IF2CRQ_R  = object;                            // Transferir configuración a objeto 2
    while(CAN0_IF2CRQ_R & 0x00008000U);             // Esperar finalización
}
/**
 * @brief Transmite un mensaje a través del módulo CAN0.
 * @details Esta función transmite un mensaje a través del módulo CAN0 utilizando el objeto de mensaje configurado previamente.
 * @param Txvec Arreglo de 8 bytes que contiene los datos a transmitir.
 * @param repeticiones Número de veces que se transmitirá el mensaje.
 * @param object Número de objeto de mensaje a utilizar.
 */
void CAN0_Transmit(const uint8_t Txvec[], uint8_t repeticiones, uint32_t object)
{
    uint8_t i;
    uint32_t maskobj = 0x00U;
    if (object >= 32U || object == 0U)
    {
        while(1U)
        {}
    }
    maskobj = 0x01U << object-1U;                       // Desplazar 1 a la izquierda object veces
    for(i = 0U; i < repeticiones; i++)
    {
        // Cargar los 8 bytes de TxData[] en los registros de interfaz IF1
        while(CAN0_IF1CRQ_R & 0x00008000U);             // Esperar IF1 libre
        // Escribir los datos en los registros de datos de IF1 (dos bytes por registro)
        CAN0_IF1DA1_R = (Txvec[1U] << 8U) | Txvec[0U];  // Bytes 0-1
        CAN0_IF1DA2_R = (Txvec[3U] << 8U) | Txvec[2U];  // Bytes 2-3
        CAN0_IF1DB1_R = (Txvec[5U] << 8U) | Txvec[4U];  // Bytes 4-5
        CAN0_IF1DB2_R = (Txvec[7U] << 8U) | Txvec[6U];  // Bytes 6-7
        // Solicitar la transmisión (TXRQST) transfiriendo datos al objeto 1
        CAN0_IF1CMSK_R = 0x87U;                         // WRNRD=1, DATAA, DATAB, TXRQST (0x80+0x04+0x02+0x01 = 0x87)
        CAN0_IF1CRQ_R  = object;                        // Iniciar transferencia a objeto 1 (envía el mensaje)
        // Esperar a que el mensaje se haya transmitido (TXRQST se borra cuando se envía correctamente)
        while(CAN0_TXRQ1_R & maskobj)                   // Espera activa hasta que el bit TXRQST del objeto 1 sea 0 (requiere ACK en el bus CAN)
        {}
    }
}
/**
 * @brief Recibe un mensaje a través del módulo CAN0.
 * @details Esta función recibe un mensaje a través del módulo CAN0 utilizando el objeto de mensaje configurado previamente.
 * @param vec Arreglo de 8 bytes donde se almacenarán los datos recibidos.
 * @param object Número de objeto de mensaje a utilizar.
 */
void CAN0_Reception(uint8_t vec[],uint32_t object)
{
    uint32_t maskobj = 0x00U;
    if (object >= 32U || object == 0U)
    {
        while(1U)
        {}
    }
    maskobj = 0x01U << object-1U;                   // Desplazar 1 a la izquierda object veces
    while((CAN0_NWDA1_R & maskobj) == 0U)           // Espera activa hasta que el objeto x indique datos nuevos (NEW DATA)
    {}
    // Cuando NEWDAT esté activo para el objeto x, significa que se recibió un mensaje
    // Leer los datos recibidos del objeto 2 y almacenarlos en RxData[]
    while(CAN0_IF2CRQ_R & 0x00008000U);             // Esperar IF2 libre
    CAN0_IF2CMSK_R = 0x17U;                         // WRNRD=0 (lectura), DATAA, DATAB, CONTROL, NEWDAT (0x10+0x4+0x2+0x1 = 0x17)
    CAN0_IF2CRQ_R  = object;                        // Solicitar transferencia desde objeto 2 a IF2
    while(CAN0_IF2CRQ_R & 0x00008000U);             // Esperar a que IF2 termine la lectura
    // Extraer los 8 bytes desde los registros de datos IF2 hacia el arreglo RxData
    uint32_t temp;
    temp = CAN0_IF2DA1_R;
    vec[0] = temp & 0xFFU;
    vec[1] = (temp >> 8U) & 0xFFU;
    temp = CAN0_IF2DA2_R;
    vec[2] = temp & 0xFFU;
    vec[3] = (temp >> 8U) & 0xFFU;
    temp = CAN0_IF2DB1_R;
    vec[4] = temp & 0xFFU;
    vec[5] = (temp >> 8U) & 0xFFU;
    temp = CAN0_IF2DB2_R;
    vec[6] = temp & 0xFFU;
    vec[7] = (temp >> 8U) & 0xFFU;
    // (En este punto RxData[] contiene los 8 bytes recibidos del mensaje CAN)
}
/**
 * @brief Rellena un arreglo de bytes con un valor específico.
 * @details Esta función rellena un arreglo de bytes con un valor específico.
 * @param vec Arreglo de bytes a rellenar.
 * @param inicio Índice de inicio del relleno.
 * @param fin Índice de fin del relleno.
 */
void CAN0_padding(uint8_t vec[], uint8_t inicio, uint8_t fin)
{
    uint8_t i = 0U;
    for(i = inicio; i < fin; i++)
    {
        vec[i] = 0xFFU;
    }
}
