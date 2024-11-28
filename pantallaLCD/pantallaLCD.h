//12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567


/**
  * @file pantallaLCD.h
  * @author EII
  *
  * @brief Recursos de manejo de la pantalla LCD de la placa STM32F429I-DISC1.
  *
  * Biblioteca de estructuras de datos y funciones en lenguaje C para el manejo de la pantalla LCD de
  * una placa STM32F429I-DISC1. Es una pantalla en color de 240x320 puntos con interfaz táctil resistiva.
  *
  *
  * @mainpage
  *
  *
  * @section S1 Introducción
  *
  * Biblioteca de estructuras de datos y funciones codificadas en lenguaje C para el manejo de la pantalla
  * LCD de una placa STM32F429I-DISC1. Es una pantalla táctil en color de 240x320 puntos.
  *
  *
  * @section S2 Orientación
  *
  * Se puede utilizar la pantalla en orientación vertical u horizontal.
  *
  * En vertical tiene una resolución de 240 puntos en el eje X y de 320 puntos en el eje Y.
  * En horizontal tiene una resolución de 320 puntos en el eje X y de 240 puntos en el eje Y.
  *
  * El origen de coordenadas en ambos casos corresponde a la esquina superior izquierda.
  *
  * @see LCD_inicializa2Buffers(), LCD_ancho(), LCD_alto()
  *
  *
  * @section S3 Frame Buffers
  *
  * Se dispone de dos zonas de memoria en una RAM externa para implantar dos frame buffers.
  * Cada uno de ellos tiene un tamaño de 4 * 240 * 320 = 30720 bytes para representar el color de cada uno
  * de los puntos de la pantalla. En cada frame buffer se guarda el color de todos los puntos de la pantalla,
  * desde los puntos de la fila superior a los puntos de la inferior y dentro de cada fila desde el punto
  * situado en el extremo izquierdo al situado en el extremo derecho.
  *
  * El color de cada punto de la pantalla se representa en 4 bytes en formato ARGB, cada byte (con valores de
  * 0 a 255) representa un canal:
  * - Canal A = Alpha = grado de transparencia, desde totalmente transparente (valor 0) a totalmente opaco
  *   (valor 255).
  * - Canal R = Red = componente roja del color, de 0 a 255.
  * - Canal G = Green = componente verde del color, de 0 a 255.
  * - Canal B = Blue = componente azul del color, de 0 a 255.
  *
  * Hay que tener en cuenta que cada color se indica en formato ARGB en 32 bits y que este microcontrolador
  * está configurado en modo little-endian, por lo que los 4 bytes de cada color se guarda en memoria desde
  * el byte menos significativo al más significativo. Por ejemplo, el color 0x39d0a807 (A=0x39, R=0xd0,
  * G=0xa8, B=0x07) se guarda en memoria con la secuencia de bytes 0x07, 0xa8, 0xd0 y 0x39.
  *
  * Los colores guardados en los frame buffers serán opacos (es decir, con canal Alpha 255) para que se
  * muestren tal cual en la pantalla.
  * Los colores de imágenes adicionales que se van a visualizar en pantalla pueden tener un canal Alpha
  * menor que 255 para que se puedan mezclar con el fondo.
  * Se utiliza la técnica de doble buffer para hacer que las transiciones entre dos escenas sea fluida
  * y sin parpadeos. Uno de los dos frame buffers está oculto mientras que el contenido del otro buffer
  * se muestra en pantalla. La aplicación dibuja en el buffer oculto lo que se desea mostrar a continuación.
  * Cuando ha finalizado el dibujado, la aplicación llama a la función LCD_intercambiaBuffers() para que el
  * buffer oculto pase a ser visible y el visible pase a ser oculto. Este intercambio es inmediato y el
  * usuario ve que se ha modificado la información en la pantalla y sin parpadeos.
  *
  * Se pueden utilizar las funciones LCD_setFondoColor() o LCD_setFondoImagen() para rellenar todo el
  * frame buffer oculto con un color o con una imagen.
  *
  * @see LCD_inicializa2Buffers(), LCD_intercambiaBuffers(), LCD_ancho(), LCD_alto(), LCD_setFondoColor(),
  *      LCD_setFondoImagen(), LCD_dibujaPunto(), LCD_dibujaPuntoOpaco().
  *
  *
  * @section S4 Interfaz táctil
  *
  * La pantalla dispone de una interfaz táctil resistiva. El origen de coordenadas corresponde a la esquina
  * superior izquierda de la pantalla.
  *
  * Para poder utilizar la interfaz táctil, hay que llamar a LCD_actualizaPulsacion() y a continuación se
  * puede llamar a LCD_pulsando() para saber si el usuario está pulsando sobre la pantalla y a
  * LCD_xPulsacion() y LCD_yPunsacion() para obtener las coordenadas del punto sobre el que se está pulsando.
  *
  * Cuando en el programa se ha tratado la pulsación sobre la pantalla, se llama a LCD_resetPulsacion()
  * para que secciones posteriores del programa reciban un valor falso cuando se llama a LCD_pulsando(),
  * hasta que se vuelva a comprobar la interfaz táctil con una llamada a LCD_actualizaPulsacion().
  *
  * @see LCD_actualizaPulsacion(), LCD_pulsando(), LCD_xPulsacion(), LCD_yPulsacion(), LCD_resetPulsacion()
  *
  *
  * @section S5 Elementos geométricos
  *
  * En la pantalla se puede visualizar:
  *
  * Un punto utilizando las funciones LCD_dibujaPunto() o LCD_dibujaPuntoOpaco().
  *
  * Un segmento de línea entre dos puntos con LCD_dibujaLinea().
  *
  * Una circunferencia con LCD_dibujaCircunferencia().
  *
  * Un círculo relleno de color con LCD_dibujaCirculo().
  *
  * El borde de un rectángulo con LCD_dibujaRectangulo()
  *
  * Un rectángulo relleno con un color con LCD_dibujaRectanguloRelleno() o LCD_dibujaRectanguloRellenoOpaco().
  *
  * @see LCD_dibujaPunto(), LCD_dibujaPuntoOpaco(), LCD_dibujaLinea(), LCD_dibujaCircunferencia(),
  * LCD_dibujaCirculo(), LCD_dibujaRectangulo(), LCD_dibujaRectanguloRelleno(),
  * LCD_dibujaRectanguloRellenoOpaco().
  *
  *
  * @section S6 Imágenes
  *
  * Cada imagen a representar en pantalla hay que representarla en una matriz de bytes donde
  * se indican los colores de todos sus puntos, desde la fila superior a la inferior y dentro de cada
  * fila yendo de izquierda a derecha.
  *
  * Para generar una matriz de bytes con todos los colores de una imagen disponible en un archivo en formato
  * JPG, PNG, etc, se puede utilizar un conversor como el disponible en https://lvgl.io/tools/imageconverter.
  * En el formulario de esa página web hay que indicar ARGB8888 en `Color format`. Pulsando el botón
  * `Convert` se genera y descarga un archivo del que hay que aprovechar los valores numéricos que describen
  * los colores de los puntos de la imagen, para incorporarlos en una matriz de bytes.
  *
  * Por cada imagen se puede crear en el proyecto un módulo con un archivo de declaraciones con extensión .h
  * donde indicar la declaración de la matriz. Ejemplo:
  * @code{.c}
  * extern const uint8_t im[];
  * // Declaración de la variable im con clase de almacenamiento externa consistente en una matriz de
  * // bytes donde se describe el color de cada punto de una imagen.
  * @endcode
  *
  * Y un archivo con extensión .c donde se define la matriz. Ejemplo:
  * @code{.c}
  * const uint8_t im[] = {0x07, 0xa8, 0xd0, 0x39};
  * // Matriz de bytes donde se describe el color de cada punto. En este ejemplo se describe un único punto
  * // de color 0x39d0a807, con A=0x39, R=0xd0, G=a8, B=0x07, pero en una imagen normalmente habría varios
  * // puntos.
  * @endcode
  *
  * De esta forma cada imagen se guarda en una matriz que actúa como una variable global en el programa.
  * Se indica const para que los valores de la matriz sean constantes, con lo cual el sistema de desarrollo
  * la preparará para que se almacene en la memoria FLASH del microcontrolador.
  *
  * @see LCD_dibujaImagen(), LCD_dibujaImagenOpaca(), LCD_setFondoImagen()
  *
  *
  * @section S7 Texto utilizando el canal Alpha
  *
  * Se puede mostrar texto en juegos de caracteres de longitud variable (los caracteres pueden tener
  * diferente ancho en puntos, pero el mismo alto), pudiendo utilizar cualquier color y definiéndolos
  * espedificando para cada carácter el canal Alpha de sus puntos, que tendrá un valor de 0 para puntos
  * transparentes, hasta un valor de 255 para puntos opacos, pasando por valores intermedios para puntos
  * semitransparentes.
  *
  * En la pantalla se pueden visualizar los caracteres de la tabla ASCII, desde el código 32 hasta el
  * código 126. Corresponden a letras minúsculas y mayúsculas del idioma inglés, dígitos numéricos y
  * signos de puntuación.
  *
  * También se pueden visualizar letras acentuadas, eñes y algunos símbolos adicionales, utilizando los
  * siguientes códigos:
  * - á = '\x7f'
  * - Á = '\x80'
  * - é = '\x81'
  * - É = '\x82'
  * - í = '\x83'
  * - Í = '\x84'
  * - ó = '\x85'
  * - Ó = '\x86'
  * - ú = '\x87'
  * - Ú = '\x88'
  * - ñ = '\x89'
  * - Ñ = '\x8a'
  * - ¿ = '\x8b'
  * - ¡ = '\x8c'
  * - € = '\x8d'
  * - º = '\x8e'
  *
  * Se dispone de varios juegos de caracteres, cada uno de ellos definido en un módulo con archivo de
  * declaraciones JuegoAlphaxx.h y archivo de definición JuegoAlphaxx.c, donde xx puede ser 13, 15, 17,
  * 19 o 22, para poder utilizar juegos de caracteres de 13 puntos de alto hasta 22 puntos de alto.
  * Cada juego de caracteres se describe en una estructura de tipo LCD_JuegoCaracteresAlpha.
  *
  * Ejemplo:
  * @code{.c}
  * #include "JuegoAlpha15.h"
  * // Incluye la declaración de la estructura de datos JuegoAlpha15, de tipo LCD_JuegoCaracteresAlpha, para
  * // utilizar un juego de caracteres de 15 puntos de alto
  *
  * LCD_dibujaCadenaCaracteresAlpha(10, 70, "Posici\x85n ma\x89ana", 0xFF00FF00, 0, &JuegoAlpha15, 0, 100);
  * // Dibuja desde el punto de coordenadas (10, 70) la cadena de caracteres "Posición mañana" en verde
  * // utilizando un juego de caracteres de 15 puntos de alto.
  * @endcode
  *
  * @see LCD_JuegoCaracteresAlpha, LCD_dibujaCaracterAlpha(), LCD_dibujaCadenaCaracteresAlpha()
  *
  *
  * @section S8 Instalación
  *
  * Para utilizar esta biblioteca de funciones en un proyecto de tipo STM32 en STM32CubeIde, hay que añadir
  * los archivos pantallaLCD.h y pantallaLCD.c.
  *
  * En el caso de utilizar funciones de visualización de texto, hay que añadir también los archivos con los
  * juegos de caracteres que se vayan a emplear. Son archivos JuegoAlphaxx.c y JuegoAlphaxx.h, donde xx
  * puede ser 13, 15, 17, 19 o 22, para juegos de caracteres donde cada carácter tiene ancho variable y
  * alto desde 13 a 22 puntos.
  *
  */


#ifndef INC_PANTALLA_H_
#define INC_PANTALLA_H_
// Para evitar inclusión múltiple de este archivo

#include <stdint.h>  // Para utilizar uint8_t, uint16_t y uint32_t
#include <string.h>  // Para utilizar strlen()
#include "stm32f429i_discovery.h"  // Para varios recursos de la placa
#include "stm32f429i_discovery_lcd.h"  // Recursos de manejo de la pantalla
#include "stm32f429i_discovery_ts.h"  // Recursos de manejo de la interfaz táctil


/** Dirección de memoria del frame buffer 0 */
#define LCD_FRAME_BUFFER_LAYER_0 (LCD_FRAME_BUFFER)

/** Dirección de memoria del frame buffer 1 */
#define LCD_FRAME_BUFFER_LAYER_1 (LCD_FRAME_BUFFER+0x130000)


/** Para definir la orientacion de la pantalla en vertical **/
#define VERTICAL 0

/** Para definir la orientacion de la pantalla en horizontal **/
#define HORIZONTAL 1

/** Para Definir la posicion normal de la pantalla **/
#define NORMAL 0

/** Para Definir la posicion invertida de la pantalla **/
#define INVERTIDA 1

/**
 * @brief Inicializa la pantalla y establece su orientación.
 *
 * Inicializa la pantalla LCD para trabajar con dos frame buffers de manera que se pueda utilizar en
 * horizontal (resolución 320x240 puntos) o en vertical (resolución 240x320 puntos).
 *
 * Es necesario llamar a esta función una única vez al comienzo de la ejecución del programa antes de
 * intentar visualizar algo en la pantalla.
 *
 * @param enHorizontal Buleano cierto si la pantalla se va a utilizar en horizontal y falso si se va a
 *    utilizar en vertical.
 * @param invertida Buleano que indica si la pantalla debe estar invertida (1) o no (0).
 *
 * @see LCD_intercambiaBuffers(), LCD_setFondoColor(), LCD_setFondoImagen()
 *
 */
void LCD_inicializa2Buffers(int enHorizontal, int invertida);



/**
 * @brief Comprueba si la pantalla está pulsada.
 *
 * Comprueba si se está pulsando la pantalla y determina en qué punto de la pantalla se está pulsando.
 * Es necesario llamar a esta función antes de utilizar otras funciones que utilizan la interfaz táctil.
 *
 * Ejemplo:
 * @code{.c}
 *
 * while(1) {  // Repite indefinidamente ...
 *
 *     LCD_actualizaPulsacion();  // Actualiza el estado de la pulsación en la interfaz táctil
 *
 *     if (LCD_pulsando()) { // Si se está pulsando la pantalla ...
 *
 *         LCD_dibujaCirculo(LCD_XPulsacion(), LCD_YPulsacion(), 10, 0xFFFFFFFF, 0, 100);
 *         // Dibuja un círculo blanco de radio 10 con centro en el punto donde se pulsó
 *
 *         LCD_resetPulsacion();  // Para indicar que ya se trató la pulsación
 *     }
 * }
 * @endcode
 *
 * * @see LCD_pulsando(), LCD_XPulsacion(), LCD_YPulsacion(), LCD_resetPulsacion()
 */
void LCD_actualizaPulsacion();


/**
 * @brief Comprueba si la pantalla está pulsada.
 *
 * Determina si se está pulsando la pantalla en el momento de la llamada a esta función.
 *
 * @return Buleano cierto si la pantalla se está pulsando.
 *
 * @see LCD_actualizaPulsacion(), LCD_xPulsacion(), LCD_yPulsacion(), LCD_resetPulsacion()
 */
int LCD_pulsando();


/**
 * @brief Resetea estado de pulsación
 *
 * Indica que ya se ha tratado la pulsación sobre la pantalla. Hay que llamar a esta función desde las
 * funciones de atención a componentes (botones, interruptores, teclado, etc) para que una vez un componente
 * ha tratado una pulsación sobre la pantalla, los demás componentes no lo hagan.
 *
 * @see LCD_actualizaPulsacion(), LCD_xPulsacion(), LCD_yPulsacion, LCD_pulsando()
 */
void LCD_resetPulsacion();


/**
 * @brief Coordenada X de última pulsación.
 *
 * Determina la columna (0 a 239 si la pantalla está en vertical y 0 a 319 si está en horizontal) del
 * punto donde se ha pulsado por última vez la pantalla.
 *
 * @return Coordenada X de la última pulsación.
 *
 * @see LCD_actualizaPulsacion(), LCD_pulsando(), LCD_yPulsacion(), LCD_resetPulsacion()
 */
uint16_t LCD_xPulsacion();


/**
 * @brief Coordenada Y de última pulsación.
 *
 * Determina la fila (0 a 319 si la pantalla está en vertical y 0 a 239 si está en horizontal) del punto
 * donde se ha pulsado por última vez la pantalla.
 *
 * @return Coordenada Y de la última pulsación.
 *
 * @see LCD_actualizaPulsacion(), LCD_pulsando(), LCD_xPulsacion(), LCD_resetPulsacion()
 */
uint16_t LCD_yPulsacion();


/**
 * @brief Devuelve el ancho de la pantalla en puntos.
 * @return Número de puntos en cada fila de la pantalla.
 *
 * @see LCD_alto().
*/
uint16_t LCD_ancho();


/**
 * @brief Devuelve el alto de la pantalla en puntos.
 * @return Número de puntos en cada columna de la pantalla.
 *
 * @see LCD_ancho().
*/
uint16_t LCD_alto();


/**
 * @brief Intercambia en qué frame buffer se dibuja y qué frame buffer se visualiza.
 *
 * Se utilizan los frame buffers número 0 y número 1. Uno de ellos es el fame buffer visible, cuyo contenido
 * se muestra en la pantalla. El otro es el frame buffer oculto don de se dibuja nueva información a mostrar.
 * Con esta función el frame buffer oculto pasa a ser visible y el visible pasa a ser oculto.
 *
 * Normalmente debería de existir un hilo dedicado a la visualización de información en pantalla. Este hilo
 * debería de consistir en un bucle contínuo que actualiza la información a visualizar y al final del bucle
 * llamaría a esta función para que la información actualizada aparezca en pantalla.
 *
 * Ejemplo:
 * @code
 * int radio = 10;  // Radio de una circunferencia
 *
 * LCD_inicializa2Buffers(0); // Inicializa la pantalla en vertical
 *
 * while(1) {  // Repite indefinidamente ...
 *
 *     LCD_setFondoColor(0xFF000000);
 *     // Borra la pantalla, poniendo todos sus puntos a color negro
 *
 *     LCD_dibujaCircunferencia(100, 100, radio, 0xFFFF0000, 0, 100);
 *     // Circunferencia en (100, 100), radio variable y en color rojo
 *
 *     radio ++;  // Incrementa el radio
 *     if (radio == 100)  // con un límite superior
 *         radio = 10;
 *
 *     LCD_intercambiaBuffers();
 *     // La circunferencia se dibujó en el buffer oculto, al intercambiar los buffers se hace visible.
 *  }
 * @endcode
 *
 * @see LCD_inicializa2Buffers().
 */
void LCD_intercambiaBuffers();


/**
 * @brief Establece color de fondo.
 *
 * Borra todo el frame buffer oculto y pone todos sus puntos al color indicado
 *
 * Ejemplo:
 * @code{.c}
 * LCD_setFondoColor(0xFF000000); // Establece como fondo el color negro
 * @endcode
 *
 * @param color Color de fondo utilizado, expresado en formato ARGB en 32 bits.
 *
 * @see LCD_inicaliza2Buffers(), LCD_setFondoImagen().
 */
void LCD_setFondoColor(uint32_t color);


/**
 * @brief Establece imagen de fondo.
 *
 * Borra toda el frame buffer oculto y dibuja la imagen indicada.
 *
 * Ejemplo:
 * @code{.c}
 * const uint8_t imagen[] = { ... };
 * //  Imagen utilizada como fondo, en los puntos suspensivos irían los colores.
 *
 * LCD_setFondoImagen(imagen);  // Establece la imagen como fondo
 * @endcode
 *
 * @param imagen Puntero a la zona de memoria donde se guarda la imagen de fondo a establecer, que tiene que
 *     tener una resolución de 240x320 puntos si la pantalla se utiliza en vertical y 320x240 puntos si
 *     se utiliza en horizontal. Cada punto expresado en formato ARGB en 32 bits, con canal
 *     Alpha 255, es decir, con colores opacos.
 *
 * @see LCD_inicializa2Buffers(), LCD_setFondoColor().
 */
void LCD_setFondoImagen(const uint8_t * imagen);


/**
 * @brief Dibuja un punto en el frame buffer oculto.
 *
 * Establece el color de un punto de la pantalla, utilizando el canal Alpha para realizar mezclas de colores,
 * pudiendo convertirlo a niveles de gris y pudiendo aplicarle un cierto grado de transparencia.
 *
 * Ejemplo:
 * @code{.c}
 * LCD_dibujaPunto(10, 10, 0xFFFF0000, 0, 50);
 * // Dibuja un punto en rojo en la posición (10, 10) reduciendo su transparencia a un 50%.
 * @endcode
 *
 * @param x Coordenada horizontal del punto, de 0 a 239 para pantalla en vertical y 0 a 319 para pantalla
 *     en horizontal.
 * @param y Coordenada vertical del punto, de 0 a 319 para pantalla en vertical y 0 a 239 para pantalla
 *     en horizontal.
 * @param color Color a establecer en el punto, expresado en formato ARGB en 32 bits.
 * @param enBlancoYNegro Buleano que indica si hay que mostrar el punto en niveles de gris.
 * @param transparencia Grado de transparencia aplicado al parámetro \p color, desde 0 (completamente
 *     transparente) hasta 100 (color original).
 *
 * @see LCD_dibujaPuntoOpaco().
 */
void LCD_dibujaPunto(uint16_t x, uint16_t y, uint32_t color, int enBlancoYNegro, int transparencia);


/**
 * @brief Dibuja un punto con color opaco en el frame buffer oculto.
 *
 * Dibuja un punto en una posición de la pantalla, pero de forma más rápida que la función
 * LCD_dibujaPunto() debido a que no se tiene en cuenta una posible mezcla de colores, ya que el color
 * utilizado es completamente opaco, con canal Alpha 255.
 *
 * Ejemplo:
 * @code{.c}
 * LCD_dibujaPuntoOpaco(10, 10, 0xFFFF0000);
 * // Dibuja un punto opaco en rojo en la posición (10, 10)
 * @endcode
 *
 * @param x Coordenada horizontal del punto, de 0 a 239.
 * @param y Coordenada vertical del punto, de 0 a 319.
 * @param color Color utilizado, expresado en formato ARGB de 32 bits, con canal Alpha 255, totalmente
 * opaco.
 *
 * @see LCD_dibujaPunto().
 */
void LCD_dibujaPuntoOpaco(uint16_t x, uint16_t y, uint32_t color);


/**
 * @brief Dibuja una línea en el frame buffer oculto.
 *
 * Dibuja una línea con un color que une dos puntos en pantalla utilizando el
 * <a href="https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm">algoritmo de Bressenham</a>. Puede
 * convertir el color a niveles de gris y puede aplicarle un grado de transparencia.
 *
 * Ejemplo:
 * @code{.c}
 * LCD_dibujaLinea(10, 10, 20, 20, 0x7FFF0000, 0, 100);
 * // Dibuja una línea en rojo (canal R=0xFF) semitransparente (canal Alpha=0x7F) desde el punto (10, 10)
 * // al punto (20, 20) mezclando su color con los colores de los puntos que hay en las mismas posiciones
 * // en el fondo de la pantalla.
 * @endcode
 *
 * @param x0 Coordenada horizontal del punto inicial.
 * @param y0 Coordenada vertical del punto inicial.
 * @param x1 Coordenada horizontal del punto final.
 * @param y1 Coordenada vertical del punto final.
 * @param color Color utilizado para dibujar la línea, expresado en formato ARGB en 32 bits.
 * @param enBlancoYNegro Buleano que indica si hay que mostrar la línea en niveles de gris.
 * @param transparencia Grado de transparencia aplicado al parámetro \p color, desde 0 (completamente
 *     transparente) hasta 100 (color original).
 */
void LCD_dibujaLinea(int x0, int y0, int x1, int y1, uint32_t color, int enBlancoYNegro, int transparencia);


/**
 * @brief Dibuja una circunferencia en el frame buffer oculto.
 *
 * Dibuja una circunferencia expresada con un centro y un radio. Dibuja el borde con un ancho de un punto en
 * un determinado color. Puede convertir el color a niveles de gris y puede aplicarle un grado de
 * transparencia.
 *
 * Ejemplo:
 *
 * @code{.c}
 * LCD_dibujaCircunferencia(100, 100, 30, 0xFFFF0000, 0, 100);
 * // Dibuja sobre el fondo de la pantalla una circunferencia de centro (100, 100), radio 30 y color rojo
 * @endcode
 *
 * @param xc Coordenada horizontal del centro.
 * @param yc Coordenada vertical del centro.
 * @param r Radio de la circunferencia.
 * @param color Color utilizado para dibujar la circunferencia, expresado en formato ARGB en 32 bits.
 * @param enBlancoYNegro Buleano que indica si hay que mostrar la circunferencia en niveles de gris.
 * @param transparencia Grado de transparencia aplicado al parámetro \p color, desde 0 (completamente
 *     transparente) hasta 100 (color original).
 *
 * @see LCD_dibujaCirculo()
 *
 */
void LCD_dibujaCircunferencia(int xc, int yc, int r, uint32_t color, int enBlancoYNegro, int transparencia);

/**
 * @brief Dibuja un círculo en el frame buffer oculto.
 *
 * Dibuja una círculo expresado con un centro y un radio, rellenándolo con
 * un determinado color. Puede convertir el color a niveles de gris y puede aplicarle un grado de
 * transparencia.
 *
 * Ejemplo:
 *
 * @code{.c}
 * LCD_dibujaCircunferencia(100, 100, 30, 0xFFFF0000, 0, 100);
 * // Dibuja sobre el fondo de la pantalla unun círculo de centro (100, 100), radio 30 y color rojo
 * @endcode
 *
 * @param xc Coordenada horizontal del centro.
 * @param yc Coordenada vertical del centro.
 * @param r Radio del círculo.
 * @param color Color utilizado para dibujar el círculo, expresado en formato ARGB en 32 bits.
 * @param enBlancoYNegro Buleano que indica si hay que mostrar el círculo en niveles de gris.
 * @param transparencia Grado de transparencia aplicado al parámetro \p color, desde 0 (completamente
 *     transparente) hasta 100 (color original).
 *
 * @see LCD_dibujaCircunferencia()
 *
 */
void LCD_dibujaCirculo(int xc, int yc, int r, uint32_t color, int enBlancoYNegro, int transparencia);


/**
 * @brief Dibuja el borde de un rectángulo en el frame buffer oculto.
 *
 * Dibuja el borde de un rectángulo con un determinado color, situado en una posición y con unas dimensiones.
 * Puede convertir el color a niveles de gris y puede aplicarle un grado de transparencia.
 *
 * Ejemplo:
 * @code{.c}
 * LCD_dibujaRectangulo(10, 10, 100, 50, 0xFF00FF00, 0, 100);
 * // Dibuja un rectángulo con esquina superior izquierda en (10, 10), ancho 100, alto 50, en verde.
 * @endcode
 *
 * @param xInicial Coordenada X de la esquina superior izquierda.
 * @param yInicial Coordenada Y de la esquina superior izquierda.
 * @param ancho Dimensión horizontal del rectángulo en puntos.
 * @param alto Dimensión vertical del rectángulo en puntos.
 * @param color Color utilizado para dibujar el rectángulo, expresado en formato ARGB en 32 bits.
 * @param enBlancoYNegro Buleano que indica si hay que mostrar el rectángulo en niveles de gris.
 * @param transparencia Grado de transparencia aplicado al parámetro \p color, desde 0 (completamente
 *     transparente) hasta 100 (color original).
 *
 * @see LCD_dibujaRectanguloRelleno(), LCDdibujaRectanguloRellenoOpaco().
 */
void LCD_dibujaRectangulo(uint16_t xInicial, uint16_t yInicial, uint16_t ancho, uint16_t alto,
	uint32_t color, int enBlancoYNegro, int transparencia);


/**
 * @brief Dibuja un rectángulo relleno en el frame buffer oculto.
 *
 * Dibuja un rectángulo relleno de un determinado color, situado en una posición y con unas dimensiones.
 * Puede convertir el color a niveles de gris y puede aplicarle un grado de transparencia.
 *
 * Ejemplo:
 * @code{.c}
 * LCD_dibujaRectanguloRelleno(10, 10, 100, 50, 0xFF0000FF, 0, 100);
 * // Dibuja un rectángulo con esquina superior izquierda en (10, 10), ancho 100, alto 50, relleno
 * // en azul.
 * @endcode
 *
 * @param xInicial Coordenada X de la esquina superior izquierda.
 * @param yInicial Coordenada Y de la esquina superior izquierda.
 * @param ancho Dimensión horizontal del rectángulo en puntos.
 * @param alto Dimensión vertical del rectángulo en puntos.
 * @param color Color utilizado para rellenar el rectángulo, expresado en formato ARGB en 32 bits.
 * @param enBlancoYNegro Buleano que indica si hay que mostrar el rectángulo en niveles de gris.
 * @param transparencia Grado de transparencia aplicado al parámetro \p color, desde 0 (completamente
 *     transparente) hasta 100 (color original).
 *
 * @see LCD_dibujaRectangulo(), LCD_dibujaRectanguloRellenoOpaco().
 */
void LCD_dibujaRectanguloRelleno(uint16_t xInicial, uint16_t yInicial, uint16_t ancho, uint16_t alto,
	uint32_t color, int enBlancoYNegro, int transparencia);


/**
 * @brief Dibuja un rectángulo relleno de un color opaco en el frame buffer oculto.
 *
 * Dibuja un rectángulo opaco (color con Alpha 255) en una posición y con unas dimensiones. No se utiliza el
 * canal Alpha para realizar mezclas de colores, por lo que el dibujado es más rápido que utilizando la
 * función LCD_dibujaRectanguloRelleno().
 *
 * Ejemplo:
 * @code{.c}
 * LCD_dibujaRectanguloRellenoOpaco(10, 10, 100, 50, 0xFF0000FF);
 * // Dibuja un rectángulo con esquina superior izquierda en (10, 10), ancho 100, alto 50, relleno
 * // en azul.
 * @endcode
 *
 * @param xInicial Coordenada X de la esquina superior izquierda.
 * @param yInicial Coordenada Y de la esquina superior izquierda.
 * @param ancho Dimensión horizontal del rectángulo en puntos.
 * @param alto Dimensión vertical del rectángulo en puntos.
 * @param color Color utilizado para rellenar el rectángulo, expresado en formato ARGB en 32 bits. El
 *    canal Alpha tiene que ser 255 (color totalmente opaco).
 *
 * @see LCD_dibujaRectangulo(), LCD_dibujaRectanguloRelleno().
 */
void LCD_dibujaRectanguloRellenoOpaco(uint16_t xInicial, uint16_t yInicial, uint16_t ancho, uint16_t alto,
    uint32_t color);


/**
 * @brief Dibuja una imagen en el frame buffer oculto.
 *
 * Dibuja una imagen en una posición determinada. Puede convertir el color a niveles de gris y
 * puede aplicarle un grado de transparencia.
 *
 * Ejemplo:
 * @code{.c}
 * const uint8_t im[] = { ... };  // En ... se indicarían los colores de los puntos de la imagen
 *
 * LCD_dibujaImagen(10, 10, 60, 100, im, 0, 50);
 * // Dibuja la imagen descrita en la matriz im situando su esquina superior izquierda en (10, 10),
 * // con ancho 60, alto 100, haciendo que sus colores sean 50% transparentes con respecto a los colores
 * // originales de la imagen y mezclándolos con los colores de los mismos puntos del fondo de la pantalla.
 * @endcode
 *
 * @param xImagen Coordenada x de la esquina superior izquierda.
 * @param yImagen Coordenada y de la esquina superior izquierda.
 * @param ancho Ancho de la imagen en puntos.
 * @param alto Alto de la imagen en puntos.
 * @param imagen Puntero a la zona de memoria donde se describe el color de cada punto de la imagen en
 *     formato ARGB en 32 bits.
 * @param enBlancoYNegro Buleano que indica si hay que mostrar la imagen en niveles de gris.
 * @param transparencia Grado de transparencia aplicado al color de cada punto de la imagen, desde 0
 *     (completamente transparente) hasta 100 (color original).
 *
 * @see LCD_dibujaImagenOpaca(), LCD_setFondoImagen()
 */
void LCD_dibujaImagen(uint16_t xImagen, uint16_t yImagen, uint16_t ancho, uint16_t alto,
	const uint8_t * imagen, int enBlancoYNegro, int transparencia);

/**
 * @brief Dibuja una imagen opaca en el frame buffer oculto.
 *
 * Dibuja una imagen en una posición con unas dimensiones. Los colores de la imagen son opacos (canal A=255),
 * por lo que no hay que realizar mezclas de colores, agilizando así el dibujado.
 *
 * Ejemplo:
 * @code{.c}
 * const uint8_t im[] = { ... };  // En ... se indicarían los colores de los puntos de la imagen
 *
 * LCD_dibujaImagen(10, 10, 60, 100, im);
 * // Dibuja la imagen descrita en la matriz im situando su esquina superior izquierda en (10, 10).
 * // La imagen tiene un ancho 60 y alto 100.
 * @endcode
 *
 * @param xImagen Coordenada x de la esquina superior izquierda, de 0 a 239.
 * @param yImagen Coordenada y de la esquina superior izquierda, de 0 a 319.
 * @param ancho Ancho de la imagen en puntos.
 * @param alto Alto de la imagen en puntos.
 * @param imagen Puntero a la zona de memoria donde se describe el color de cada punto de la imagen en
 *     formato ARGB en 32 bits.
 *
 * @see LCD_dibujaImagen(), LCD_setFondoImagen()
 */
void LCD_dibujaImagenOpaca(uint16_t xImagen, uint16_t yImagen, uint16_t ancho, uint16_t alto,
	const uint8_t * imagen);


/**
 * @brief Estructura para representar a un juego de caracteres.
 *
 * Una estructura de este tipo representa a un juego de caracteres definido mediante el canal Alpha de
 * cada punto de cada carácter de la tabla ASCII (más algunos caracteres adicionales mencionados más
 * abajo) para indicar cómo se dibuja cada punto mezclando su color con el color del punto sobre el que se
 * superpone.
 *
 * En este juego de caracteres se indica sólo el canal Alpha, es decir, por cada punto del caracter se
 * indica un único byte. Esto permite dibujar texto en cualquier color (especificando las componentes
 * R, G y B) cuando se va a visualizar un determinado texto.
 *
 * Todos los caracteres tienen el mismo alto en puntos, pero pueden tener ancho diferente.
 *
 */
typedef struct {
    /** @brief Alto de cada carácter en puntos, todos los caracteres tienen el mismo alto */
	uint8_t alto;
    /** @brief Matriz con enteros que indican la posición de cada carácter en la matriz `caracteres` */
	uint32_t posiciones[142-32+1];
	/** @brief Matriz de bytes donde se describe por cada carácter
	 * - el número de puntos de ancho, expresado en un byte
	 * - un byte por cada punto del carácter para indicar su componente alpha
	 */
	uint8_t caracteres[];
} LCD_JuegoCaracteresAlpha;


/**
 * @brief Dibuja un carácter.
 *
 * Dibuja un carácter con un color, situándolo en una posición, utilizando un juego de caracteres,
 * pudiendo visualizarlo en niveles de gris, pudiendo aplicarle un cierto grado de transparencia.
 *
 * Se pueden visualizar los caracteres de la tabla ASCII desde el código 32 hasta el 126.
 *
 * Además se pueden utilizar las vocales acentuadas y eñe, en minúsculas y mayúsculas, y algunos símbolos,
 * con los siguientes códigos:
 * - á = '\\x7f'
 * - Á = '\\x80'
 * - é = '\\x81'
 * - É = '\\x82'
 * - í = '\\x83'
 * - Í = '\\x84'
 * - ó = '\\x85'
 * - Ó = '\\x86'
 * - ú = '\\x87'
 * - Ú = '\\x88'
 * - ñ = '\\x89'
 * - Ñ = '\\x8a'
 * - ¿ = '\\x8b'
 * - ¡ = '\\x8c'
 * - € = '\\x8d'
 * - º = '\\x8e'
 *
 * Ejemplo:
 * @code{.c}
 * #include <JuegoAlpha15.h>
 * // Incluye la declaración de la estructura juegoAlpha15 de tipo LCD_JuegoCaracteresAlpha para
 * // manejar un juego de caracteres de 15 puntos de alto. Los datos que describen el juego de
 * // caracteres se indican en JuegoAlpha15.c.
 *
 * LCD_dibujaCaracterAlpha(10, 10, 'A', 0xFFFF0000, & juegoAlpha15, 0, 100);
 * // Dibuja el carácter 'A' en rojo situando su esquina superior izquierda en (10, 10), utilizando el juego
 * // de caracteres juegoAlpha15.
 * @endcode
 *
 * @see LCD_JuegoCaracteresAlpha, LCD_dibujaCadenaCaracteresAlpha(), LCD_anchoCaracterAlpha(),
 *     LCD_anchoCadenaCaracteresAlpha().
 */
void LCD_dibujaCaracterAlpha(uint16_t x, uint16_t y, char caracter, uint32_t color,
		const LCD_JuegoCaracteresAlpha * pJuegoCaracteres,
		int enBlancoYNegro, int transparencia);

/**
 * @brief Ancho de un carácter expresado en puntos.
 *
 * Devuelve el número de puntos de ancho necesarios para representar un carácter utilizando
 * un juego de caracteres determinado.
 *
 * @param caracter Carácter del cual se desea obtener su ancho
 * @param pJuegoCaracteres Puntero a la estructura donde se describe el juego de caracteres utilizado.
 * @return Devuelve el número de puntos que ocupa el carácter en horizontal.
 *
 * @see LCD_JuegoCaracteresAlpha, LCD_dibujaCaracterAlpha(), LCD_dibujaCadenaCaracteresAlpha(),
 *     LCD_anchoCadenaCaracteresAlpha().
 */
uint16_t LCD_anchoCaracterAlpha(char caracter, const LCD_JuegoCaracteresAlpha * pJuegoCaracteres);


/**
 * @brief Ancho de una cadena de caracteres expresado en puntos.
 *
 * Devuelve el número de puntos de ancho necesarios para representar una cadena de caracteres utilizando
 * un juego de caracteres determinado y con una cierta separación entre caracteres consecutivos.
 *
 * @param cadena Cadena de caracteres de la cual se desea obtener su ancho
 * @param pJuegoCaracteres Puntero a la estructura donde se describe el juego de caracteres utilizado.
 * @param separacion Separación en puntos entre dos caracteres consecutivos.
 *
 * @return Devuelve el número de puntos que ocupa la cadena en horizontal.
 *
 * @see LCD_JuegoCaracteresAlpha, LCD_anchoCaracterAlpha(), LCD_dibujaCaracterAlpha(),
 *     LCD_dibujaCadenaCaracteresAlpha().
 */
uint16_t LCD_anchoCadenaCaracteresAlpha(const char* cadena,
	const LCD_JuegoCaracteresAlpha * pJuegoCaracteres, uint16_t separacion);


/**
 * @brief Dibuja una cadena de caracteres.
 *
 * Dibuja la cadena de caracteres `cadena` situándola en una posición, utilizando un juego de caracteres,
 * en un color, pudiendo visualizarla en niveles de gris, aplicarle transparencia y mezclarla sobre lo
 * que ya hay en el frame buffer oculto o sobre el fondo de la pantalla.
 *
 * Se pueden visualizar los caracteres de la tabla ASCII desde la posición 32 hasta la 126.
 *
 * Además se han incluido las letras acentuadas y eñe, en minúsculas y mayúsculas, y algunos signoscon los
 * siguientes códigos:
 * - á = '\\x7f'
 * - Á = '\\x80'
 * - é = '\\x81'
 * - É = '\\x82'
 * - í = '\\x83'
 * - Í = '\\x84'
 * - ó = '\\x85'
 * - Ó = '\\x86'
 * - ú = '\\x87'
 * - Ú = '\\x88'
 * - ñ = '\\x89'
 * - Ñ = '\\x8a'
 * - ¿ = '\\x8b'
 * - ¡ = '\\x8c'
 * - € = '\\x8d'
 * - º = '\\x8e'
 *
 * Ejemplo:
 * @code{.c}
 * #include <JuegoAlpha15.h>
 * // Incluye la declaración de la estructura juegoAlpha15 de tipo LCD_JuegoCaracteresAlpha para
 * // manejar un juego de caracteres de 15 puntos de alto. Los datos que describen el juego de
 * // caracteres se indican en JuegoAlpha15.c.
 *
 * LCD_dibujaCadenaCaracteresAlpha(10, 10, "Hola", 0xFF00FF00, & juegoAlpha15, 0, 100);
 * // Dibuja la cadena "Hola" en verde, situando su esquina superior izquierda en (10, 10), utilizando el
 * // juego de caracteres juegoAlpha15.
 *
 * LCD_dibujaCadenaCaracteresAlpha(10, 40, "Ca\x89\x85n: 7\x8d", 0xFFFF0000, & juegoAlpha15, 0, 100);
 * // Dibuja la cadena "Cañón: 7€" en rojo, situando su esquina superior izquierda en (10, 40), utilizando
 * // el juego de caracteres juegoAlpha15.
 * @endcode
 *
 * @see LCD_JuegoCaracteresAlpha(), LCD_anchoCaracterAlpha(), LCD_anchoCadenaCaracteresAlpha(),
 *     LCD_dibujaCaracterAlpha().
 */
void LCD_dibujaCadenaCaracteresAlpha(uint16_t xCadena, uint16_t yCadena, const char* cadena, uint32_t color,
	uint16_t separacion,
	const LCD_JuegoCaracteresAlpha * pJuegoCaracteres, int enBlancoYNegro, int transparencia);


void LCD_dibujaCuadrado(uint16_t xInicial, uint16_t yInicial, uint16_t lado,
    uint32_t color, int enBlancoYNegro, int transparencia);

void LCD_dibujaCaracterAlphaRotado(uint16_t x, uint16_t y, char caracter, uint32_t color,
	const LCD_JuegoCaracteresAlpha * pJuegoCaracteres, int enBlancoYNegro, int transparencia);

void LCD_dibujaCadenaCaracteresAlphaVertical(uint16_t xCadena, uint16_t yCadena,
		const char* cadena, uint32_t color, uint16_t separacion,
    const LCD_JuegoCaracteresAlpha * pJuegoCaracteres, int enBlancoYNegro, int transparencia);

#endif

