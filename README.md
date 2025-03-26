## 🚀  [TM4C123GH6PM] - [CAN0 - Configuración para comunicación CAN ]
Este proyecto tiene como objetivo configurar el microcontrolador TM4C123GH6PM para comunicación CAN como maestro, se configura el microcontrolador para enviar mensajes CAN y se envía un mensaje de respuesta. Además se agregan dos botones para la selcción del esclavo al que se le enviará mensaje.

## Consideraciones de firmware.
En este proyecto considera el ID del mensaje CAN como 0x101 y 0x102 para cada uno de los esclavos en tx_id_s1 y tx_id_s2, solo se requiere de configurar rx_id para la recepción de los mensajes de ambos esclavos como 0x100.
Tras presionar alguno de los botones se incia la transmisión con uno de los esclavos, el maestro quedará esperando la respuesta, no realiza acciones con la captura.

---

## 📂 Estructura del Proyecto
📁 Root<br>
&emsp;│── 📁build<br>
&emsp;&emsp;&emsp;&emsp;│── 📁output<br>
&emsp;│── 📁docs<br>
&emsp;&emsp;&emsp;&emsp;│── 📁Doxyfolder<br>
&emsp;&emsp;&emsp;&emsp;│── 📁Modulos<br>
&emsp;│── 📁Inc<br>
&emsp;│── 📁drivers<br>
&emsp;│── 📁src<br>
&emsp;│── 📁startup<br>
&emsp;│── 📁test<br>
&emsp;│── 📁.vscode<br>

---

## 🔧 **Requisitos**
- **Microcontrolador:** `TM4C123GH6PM`   
- **Compilador ó IDE:** `VSCode`  
- **Flasheo:** `LM Flash Programmer`  
- **Herramientas opcionales:** Ozone para depuración, J-scope para visualización de datos, j-link.  
- **Terminal:** `git bash` para manejo de comandos.

---

## 🔨 Estructura de datos implementada

| Campo          | Tipo             | Descripción                                                              |
|----------------|------------------|---------------------------------------------------------------------------|
| `i_CMD`        | `uint8_t`        | Identificador de comando CAN.                                            |
| `rxdatos[]`    | `uint8_t[]`      | Buffer de recepción de datos CAN.                                        |
| `txdatos[]`    | `uint8_t[]`      | Buffer de transmisión de datos CAN.                                      |
| `tx_id_s1`     | `const uint32_t` | ID estándar de transmisión para esclavo 1.                                |
| `tx_id_s2`     | `const uint32_t` | ID estándar de transmisión para esclavo 2.                                |
| `rx_id`        | `const uint32_t` | ID estándar de recepción.                                                |
| `rx_obj`       | `uint32_t`       | Número de objeto de mensaje usado para recepción.                        |
| `tx_s1_obj`    | `uint32_t`       | Objeto de mensaje usado para enviar datos desde esclavo 1.                |
| `tx_s2_obj`    | `uint32_t`       | Objeto de mensaje usado para enviar datos desde esclavo 2.                |
| `tx_rep`       | `uint32_t`       | Número de repeticiones para envío (útil para pruebas o robustez).        |
| `ini_padding`  | `uint8_t`        | inicio dePadding opcional para relleno.                         |
