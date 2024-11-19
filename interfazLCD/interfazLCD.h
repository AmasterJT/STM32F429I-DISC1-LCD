
#ifndef INTERFAZLCD_H_
#define INTERFAZLCD_H_

#include <stdint.h>
#include <pantallaLCD.h>

/**
  * @file interfazLCD.h
  * @author EII
  *
  * @brief Recursos para interfaces de usuario en la pantalla LCD de la placa STM32F429I-DISC1.
  *
  * Biblioteca de estructuras de datos y funciones codificadas en lenguaje C para implantar diferentes
  * elementos de interfaz de usuario para aplicaciones que utilicen la pantalla
  * LCD de una placa STM32F429I-DISC1. Es una pantalla táctil en color de 240x320 puntos.
  *
  *
  * @mainpage
  *
  *
  * @section S1 Introducción
  *
  * Biblioteca de estructuras de datos y funciones codificadas en lenguaje C para implantar diferentes
  * elementos de interfaz de usuario para aplicaciones que utilicen la pantalla
  * LCD de una placa STM32F429I-DISC1. Es una pantalla táctil en color de 240x320 puntos.
  *
  * Utiliza la biblioteca pantallaLCD para la visualización en pantalla. Los colores e imágenes se
  * manejan según se explica en la documentación de esa biblioteca.
  *
  *
  * @section S2 Etiquetas de texto
  *
  * Componente que permite visualizar una línea de texto horizontal. El texto tiene un color y se muestra
  * sobre un fondo de otro color. Se utiliza un juego de caracteres de tipo LCD_JuegoCaracteresAlpha.
  * Existe un márgen entre el texto y los bordes de la etiqueta. El texto se puede mostrar alineado a la
  * izquierda, centrado o a la derecha. La etiqueta se puede deshabilitar, en cuyo caso se muestra con niveles
  * de gris. Se puede hacer visible o invisible.
  *
  * Ejemplo:
  * @code{.c}
  * LCD_inicializa2Buffers(1);  // Inicializa la pantalla para utilizar doble frame buffer
  *
  * LCD_Etiqueta e;  // Estructura para manejar una etiqueta
  *
  * LCD_inicializaEtiqueta("Contador:", 10, 10, &juegoAlpha15, 1, 150, 20, 3, 3, LCD_ALINEACION_IZQUIERDA,
  *      0xFFFFFF00, 0xFF990000, 1, 1, &e);
  * // Inicializa la etiqueta representada por la estructura e. Muestra inicialmente el texto "Contador:".
  * // La esquina superior izquierda se situa en (10, 10). Se utiliza el juego de caracteres juegoAlpha15 con
  * // 1 punto de separación entre caracteres. La etiqueta tiene 150 puntos de ancho y 20 puntos de alto.
  * // Hay un márgen interno de 3 puntos entre los bordes de la etiqueta y el texto, que se alinea a la izquierda.
  * // El texto se muestra en color 0xFFFFFF00 y el fondo en el color 0xFF990000.
  *
  * char cadena[30];  // Cadena para montar mensajes
  * int n = 1;  // Contador
  *
  * while(1) {
  *     osDelay(100);  // Bloquea durante 100 ms
  *     sprintf(cadena, "Contador: %d", n);  // Formatea el valor del contador en una cadena
  *     n++;  // Incrementa el contador
  *     LCD_setTextoEtiqueta(cadena, &e);  // para establecer el texto a mostrar en la etiqueta
  *     LCD_setFondoColor(0xFF0000FF);  // Borra el frame buffer oculto con el color 0xFF0000FF
  *     LCD_atiendeEtiqueta(&e);  // Dibuja la etiqueta en el frame buffer oculto
  *     LCD_intercambiaBuffers();  // Hace que el frame buffer oculto sea visible y el visible sea oculto
  * }
  * @endcode
  *
  * @see LCD_Etiqueta, LCD_inicializaEtiqueta(), LCD_setTextoEtiqueta(), LCD_setColorEtiqueta(),
  *     LCD_setColorFondoEtiqueta(), LCD_setAlineacionEtiqueta(), LCD_atiendeEtiqueta(),
  *     LCD_setHabilitacionEtiqueta(), LCD_setVisibilidadEtiqueta(), LCD_setPosicionEtiqueta()
  *
  *
  * @section S3 Imágenes
  *
  * Componente para visualización de imágenes. Cada imagen se muestra con sus dimensiones originales.
  * Se puede establecer su posición (esquina superior izquierda), se puede modificar la imagen a visualizar,
  * se puede deshabilitar (en cuyo caso se muestra en niveles de gris y semitransparente) y se puede hacer
  * visible o invisible.
  *
  * Cada imagen se indica en una matriz de bytes donde se guarda el color de cada punto en formato ARGB
  * de 32 bits, por filas desde la superior a la inferior y en cada fila de izquierda a derecha.
  *
  * Ejemplo:
  * @code{.c}
  *  LCD_inicializa2Buffers(1);  // Inicializa la pantalla para utilizar doble frame buffer
  *
  *  LCD_Imagen imagen;  // Estructura para manejar una imagen
  *
  *  LCD_inicializaImagen(10, 10, 60, 60, alarma4_60x60, 1, 1, &imagen);
  *  // Inicializa la imagen representada por la estructura 'imagen'. Muestra la imagen descrita en la matriz
  *  // de bytes 'alarma_60x60'. La esquina superior izquierda se situa en (10, 10) y la imagen tiene
  *  // 60 puntos de ancho por 60 puntos de alto. Inicialmente se muestra visible y en color original.
  *
  *  uint32_t numTicks = osKernelGetTickCount();  // Obtiene el contador de ticks de FreeRTOS
  *  int visible = 1;  // Buleano para controlar la visibilidad de la imagen
  *  while(1) {
  *      numTicks += 500U;  // Incrementa el contador de ticks para esperar 0.5 segundos
  *      osDelayUntil(numTicks);  // Bloquea hasta que el contador de ticks llegue a ese valor
  *      LCD_setFondoColor(0xFF0000FF);  // Borra el frame buffer oculto con el color 0xFF0000FF
  *      LCD_setVisibilidadImagen(visible, &imagen);  // Establece la visibilidad de la imagen
  *      visible = ! visible;  // Niega este buleano para hacer parpadear la imagen
  *      LCD_atiendeImagen(&imagen);  // Dibuja la imagen en el frame buffer oculto
  *      LCD_intercambiaBuffers();  // Hace que el frame buffer oculto sea visible y el visible sea oculto
  *  }
  * @endcode
  *
  * @see LCD_Imagen, LCD_inicializaImagen, LCD_setImagen(), LCD_setPosicionImagen(),
  * LCD_setVisibilidadImagen(), LCD_setHabilitacionImagen(), LCD_atiendeImagen()
  *
  *
  * @section S4 Botones
  *
  * Cada botón se muestra utilizando una imagen.
  * Se le asocia una función que se ejecuta cuando se pulsa el botón en la pantalla.
  * Se puede hacer visible o invisible. Se puede habilitar o deshabilitar (en cuyo caso se muestra
  * semitransparente y en niveles de gris y no ejecuta la función si se pulsa). Se puede mostrar un texto
  * sobre el botón. Se puede modificar el texto, su color y la imagen del botón.
  *
  * Ejemplo:
  * @code{.c}
  * int contador = 0;  // Entero para contaje
  *
  * void fBoton() {  // Función que atiende la pulsación sobre el botón
  *     contador = 0;  // Resetea el contador
  * }
  *
  * void StartDefaultTask(void *argument) {
  *
  *     char cadena[20];  // Cadena de caracteres para mostrar el contador como texto
  *
  *     LCD_inicializa2Buffers(1);  // Inicializa la pantalla para utilizar doble frame buffer
  *
  *     LCD_Boton boton;  // Estructura para manejar un botón
  *
  *     LCD_inicializaBoton(10, 10, 80, 29, imagenBoton, "Reset", &juegoAlpha15, 1, 0xFF000000,
  *         fBoton, 1, 1, &boton);
  *     // Inicializa un botón representado por la estructura 'boton'. Muestra la imagen descrita en la
  *     // matriz de bytes 'imagenBoton'. La esquina superior izquierda se situa en (10, 10) y tiene
  *     // 80 puntos de ancho por 29 puntos de alto. Inicialmente se muestra visible y habilitado.
  *     // Sobre el botón se muestra el texto "Reset" utilizando un juego de caracteres 'juegoAlpha15',
  *     // con 1 punto de separación entre caracteres consecutivos y en color negro 0xFF000000. Cuando se
  *     // pulsa el botón, se ejecuta la función fBoton().
  *
  *     while(1) {
  *         LCD_setFondoColor(0xFF0000FF);  // Borra el frame buffer oculto con el color 0xFF0000FF
  *
  *         contador ++;  // Incrementa el contador
  *
  *         sprintf(cadena, "%d", contador);  // Convierte el entero a cadena de caracteres
  *
  *         LCD_dibujaCadenaCaracteresAlpha(10, 50, cadena, 0xFFFFFFFF, 1, &juegoAlpha15, 0, 100);
  *         // Visualiza el contador en la posición (10, 50) en blanco separando con 1 punto los
  *         // caracteres consecutivos y utilizando el juego de caracteres juegoAlpha15
  *
  *         LCD_actualizaPulsacion();
  *         // Comprueba en la interfaz táctil si hubo alguna pulsación y determina dónde se ha pulsado
  *
  *         LCD_atiendeBoton(&boton);
  *         // Dibuja el botón en el frame buffer oculto.
  *         // Ejecuta la función asignada al botón si se pulsó sobre él.
  *
  *         LCD_intercambiaBuffers();  // Hace que el frame buffer oculto sea visible y el visible sea oculto
  *     }
  * }
  * @endcode
  *
  * @see LCD_Boton, LCD_inicializaBoton(), LCD_setVisibilidadBoton(), LCD_setHabilitacionBoton(),
  *     LCD_setTextoBoton(), LCD_setColorTextoBoton(), LCD_setImagenBoton(), LCD_atiendeBoton()
  *
  *
  * @section S5 Interruptores
  *
  * Un interruptor actúa como un botón que puede tener dos estados: ON y OFF. En cada estado se muestra
  * una imagen diferente. Cuando se pulsa, cambia de estado y se llama a una función a la que se le pasa el
  * nuevo estado. Puede habilitarse o deshabilitarse (en cuyo caso se muestra en niveles de gris y
  * semitransparente. Puede hacerse visible o invisible.
  *
  * Ejemplo:
  * @code{.c}
  * LCD_Interruptor interruptor;  // Estructura para manejar un interruptor
  * LCD_Etiqueta etiqueta;  // Etiqueta para mostrar un mensaje
  *
  *
  * void fInterruptor() {  // Función que atiende la pulsación sobre el botón
  *
  *     if (LCD_getEstadoInterruptor(&interruptor))
  *         LCD_setTextoEtiqueta("ON", &etiqueta);
  *      else LCD_setTextoEtiqueta("OFF", &etiqueta);
  *     // Indica en la etiqueta el estado del interruptor
  * }
  *
  *
  * void StartDefaultTask(void *argument)
  * {
  *   // init code for USB_HOST
  *   MX_USB_HOST_Init();
  *   // USER CODE BEGIN 5
  *
  *   LCD_inicializa2Buffers(1);  // Inicializa la pantalla para utilizar doble frame buffer
  *
  *   LCD_inicializaEtiqueta("", 10, 50, &juegoAlpha15, 1, 100, 20, 5, 4, LCD_ALINEACION_IZQUIERDA, 0xFFFFFF00,
  *       0x00000000, 1, 1, &etiqueta);
  *   // Crea una etiqueta para mostrar mensajes
  *
  *   LCD_inicializaInterruptor(10, 10, 50, 24, interruptor17On_50x24, interruptor17Off_50x24, fInterruptor,
  *       1, 1, &interruptor);
  *   // Inicializa un interruptor representado por la estructura 'interruptor'. Muestra la imagen descrita en
  *   // la matriz de bytes 'interruptor17On_50x24' cuando está en estado ON y muestra la imagen descrita en
  *   // 'interruptor17Off_50x24' cuando está en estado OFF. La esquina superior izquierda se situa en (10, 10)
  *   // y tiene 50 puntos de ancho por 24 puntos de alto. Inicialmente se muestra visible y habilitado.
  *   // Cuando se pulsa el interruptor, se ejecuta la función fInterruptor().
  *
  *   while(1) {
  *       LCD_setFondoColor(0xFF0000FF);  // Borra el frame buffer oculto con el color 0xFF0000FF
  *       LCD_actualizaPulsacion();  // Comprueba en la interfaz táctil si hubo alguna pulsación
  *
  *       LCD_atiendeInterruptor(&interruptor);
  *       // Dibuja el interruptor en el frame buffer oculto y ejecuta la función asignada si se pulsó sobre él
  *
  *       LCD_atiendeEtiqueta(&etiqueta);  // Dibuja la etiqueta en el buffer oculto
  *
  *       LCD_intercambiaBuffers();  // Hace que el frame buffer oculto sea visible y el visible sea oculto
  *   }
  *
  *   // USER CODE END 5
  * }
  * @endcode
  *
  * @see LCD_Interruptor, LCD_inicializaInterruptor(), LCD_setVisibilidadInterruptor(),
  *     LCD_setHabilitacionInterruptor(), LCD_getEstadoInterruptor(), LCD_setEstadoInterruptor(),
  *     LCD_atiendeInterruptor()
  *
  *
  * @section S6 Barras
  *
  * Componente en el que se muestra un valor numérico en texto y también gráficamente en una barra
  * horizontal coloreada. Antes del valor se puede mostrar un texto. Se puede hacer visible o invisible.
  * Se pueden modificar los colores utilizados.
  *
  * Ejemplo:
  * @code{.c}
  * LCD_inicializa2Buffers(1);  // Inicializa la pantalla para utilizar doble frame buffer
  *
  * LCD_Barra barra;
  * LCD_inicializaBarra("Dato:", 0, 100, 120, 30, 10, 10, 5, 1, 0xFFFFFF00, 0xFFFF0000, 0xFF000000,
  *   &juegoAlpha15, 1, &barra);
  * // Inicializa la estructura 'barra' para mostrar valores precedidos por la cadena "Dato:" comprendidos
  * // entre 0 y 100, en una barra de 120 puntos de ancho y 30 de alto, con esquina superior izquierda en
  * // (10, 10), utilizando hasta 5 caracteres para el valor, con 1 decimal, mostrando el valor y la cadena
  * // en color amarillo 0xFFFFFF00, la barra en rojo 0xFFFF0000 y el fondo en negro 0xFF000000, utilizando
  * // el juego de caracteres 'juegoAlpha15', con 1 punto de separación entre caracteres consecutivos.
  *
  * float valor = 0;  // Valor a mostrar en la barra
  * while(1) {
  *    LCD_setFondoColor(0xFF0000FF);  // Borra el frame buffer oculto con el color 0xFF0000FF
  *    valor += 1.2345;  // Actualiza el valor
  *    if (valor > 100) valor = 0;  // Valores entre 0 y 100
  *    LCD_setValorBarra(valor, &barra);  // Establece el valor a mostrar en la barra
  *    LCD_atiendeBarra(&barra);  // Muestra la barra
  *    LCD_intercambiaBuffers();  // Hace que el frame buffer oculto sea visible y el visible sea oculto
  * }
  * @endcode
  *
  * @see LCD_Barra, LCD_inicializaBarra(), LCD_setVisibilidadBarra(), LCD_setColorTextoBarra(),
  *     LCD_setColorBarra(), LCD_setColorFondoBarra(), LCD_setValorBarra(), LCD_atiendeBarra()
  *
  *
  * @section S7 Editor de un valor
  *
  * Componente que muestra un valor numérico y que permite modificarlo, pulsando sobre el mismo editor.
  * Si se pulsa sobre el primer cuarto del editor, situado a la izquierda, se realiza un decremento mayor.
  * Si se pulsa sobre el segundo cuarto, se realiza un decremento menor.
  * Si se pulsa sobre el tercer cuarto, se realiza un incremento menor.
  * Si se pulsa sobre el cuarto cuarto, situado a la derecha, se realiza un incremento mayor.
  * Se utilizan dos cantidades, una para realizar los decrementos/incrementos menores y otra para los mayores.
  *
  * Ejemplo:
  * @code{.c}
  * LCD_inicializa2Buffers(0);  // Inicializa la pantalla para utilizar doble frame buffer
  *
  * LCD_Editor editor;  // Estructura de datos para manejar un editor de un valor numérico
  *
  * LCD_inicializaEditor(10, 0.1, 1, 10, 10, 100, 20, 5, 4, LCD_ALINEACION_CENTRO, 0xFF000000, 0xFFFFFFFF,
  *   &juegoAlpha15, 1, 1, &editor);
  * // Inicializa la estructura 'editor' para editar un valor numérico que parte de 10 y donde se realizan
  * // incrementos/decrementos menores de 0.1 unidades e incrementos/decrementos mayores de 1 unidad.
  * // El editor se sitúa en la posición (10, 10) y tiene 100 puntos de ancho y 20 de alto.
  * // El texto se muestra con un margen superior de 5 puntos y alineado al centro. El texto se muestra en
  * // color blanco 0xFFFFFFFF sobre un fondo negro 0xFF000000 utilizando un juego de caracteres juegoAlpha15
  * // y separando con un punto caracteres consecutivos. El valor se muestra con un decimal.
  *
  * while(1) {
  *   LCD_setFondoColor(0xFF0000FF);  // Borra el frame buffer oculto con el color azul 0xFF0000FF
  *   LCD_actualizaPulsacion();  // Actualiza el estado de la pulsación sobre la pantalla
  *   LCD_atiendeEditor(&editor);  // Muestra el editor y actualiza el valor si se pulsa sobre el editor
  *   LCD_intercambiaBuffers();  // Hace que el frame buffer oculto sea visible y el visible sea oculto
  * }
  * @endcode
  *
  * @see LCD_EditorDato, LCD_inicializaEditorDato(), LCD_setVisibilidadEditorDato, LCD_getValorEditorDato(),
  *     LCD_atiendeEditorDato(), LCD_editorDatoIntroduciendo
 */



///////////////////////////////////////////////////////////////////////////////////////////////////////



/**
 * @brief LCD_Alineacion
 *
 * Enumerado para indicar cómo se alinea el texto: a la izquierda, a la derecha o centrado.
 *
 */
typedef enum{
    /** @brief Alineación de texto a la izquierda */
    LCD_ALINEACION_IZQUIERDA,
    /** @brief Alineación de texto centrado */
    LCD_ALINEACION_CENTRO,
    /** @brief Alineación de texto a la derecha */
    LCD_ALINEACION_DERECHA} LCD_Alineacion;

//-------------------------------------------------------------------------------------------------------
/**
 * @brief Estructura para una etiqueta de texto.
 *
 * Una variable de este tipo representa a una etiqueta donde se muestra un cierto texto en un juego de
 * caracteres, con una cierta separación entre caracteres consecutivos y utilizando un color para el
 * texto y un color para el fondo de la etiqueta.
 * Como máximo se pueden visualizar 100 caracteres. Los caracteres posibles son los que se pueden
 * representar mediante un juego de caracteres del tipo `LCD_JuegoCaracteresAlpha`.
 * El texto se separa de los bordes de la etiqueta con un cierto margen.
 * La etiqueta tiene un cierto ancho en puntos.
 * Tiene un alto definido por el alto del juego de caracteres y por el margen establecido entre
 * el borde de la etiqueta y el texto.
 * El texto se puede mostrar centrado o alineado a la izquierda o a la derecha.
 * Puede hacerse visible o invisible.
 * Puede estar habilitada o deshabilitada, en cuyo caso se muestra semitransparente y en niveles de grises.
 *
 * @see LCD_inicializaEtiqueta(), LCD_setTextoEtiqueta(), LCD_setColorTextoEtiqueta(),
 *     LCD_setColorFondoEtiqueta(), LCD_setAlineacionEtiqueta(),
 *     LCD_setVisibilidadEtiqueta(), LCD_setHabilitacionEtiqueta(), LCD_setPosicionEtiqueta(),
 *     LCD_atiendeEtiqueta()
 *
 */
typedef struct {
    /** @brief Texto mostrado en la etiqueta, como máximo 100 caracteres */
    char texto[101];
    /** @brief Coordenada X de la esquina superior izquierda */
    uint16_t x;
    /** @brief Coordenada Y de la esquina superior izquierda */
    uint16_t y;
    /** @brief Juego de caracteres utilizado */
    const LCD_JuegoCaracteresAlpha * juego;
    /** @brief Separación en puntos entre dos caracteres consecutivos */
    uint16_t separacion;
    /** @brief Ancho de la etiqueta en puntos */
    uint16_t ancho;
    /** @brief Alto de la etiqueta en puntos */
    uint16_t alto;
    /** @brief Margen horizontal en puntos desde el borde izquierdo de la etiqueta al texto si la
     *     alineación es a la izquierda, y desde el borde derecho de la etiqueta al texto si la
     *     alineación es a la derecha */
    uint16_t margenHorizontal;
    /** @brief Margen vertical en puntos desde el borde superior de la etiqueta al texto */
    uint16_t margenVertical;
    /** @brief Ancho del texto mostrado en la etiqueta en puntos */
    uint16_t anchoTexto;
    /** @brief Alineación del texto */
    LCD_Alineacion alineacion;
    /** @brief Color del texto en formato ARGB en 32 bits */
    uint32_t color;
    /** @brief Color de fondo de la etiqueta en formato ARGB en 32 bits*/
    uint32_t colorFondo;
    /** @brief Buleano cierto si el fondo es transparente */
    int transparente;
    /** @brief Buleano que indica si la etiqueta es visible */
    int visible;
    /** @brief Buleano que indica si la etiqueta está habilitada o deshabilitada */
    int habilitada;
} LCD_Etiqueta;


/**
 * @brief Inicializa una etiqueta para mostrar texto.
 *
 * Inicializa una estructura de tipo `LCD_Etiqueta` a partir de la información pasada por parámetro.
 *
 * @param texto Texto de hasta 100 caracteres que se muestra inicialmente. Los caracteres posibles son los
 *     que se pueden representar mediante un juego de caracteres del tipo `LCD_JuegoCaracteresAlpha`.
 * @param x Coordenada X de la esquina superior izquierda de la etiqueta
 * @param y Coordenada Y de la esquina superior izquierda de la etiqueta
 * @param juego Juego de caracteres utilizado
 * @param separacion Separación en puntos entre caracteres consecutivos
 * @param ancho Ancho de toda la etiqueta, expresado en puntos
 * @param alto Alto de toda la etiqueta, expresado en puntos
 * @param margenHorizontal Margen horizontal en puntos desde el borde izquierdo de la etiqueta al texto si la
 *     alineación es a la izquierda, y desde el borde derecho de la etiqueta al texto si la
 *     alineación es a la derecha
 * @param margenVertical Margen vertical en puntos desde el borde superior de la etiqueta al texto
 * @param alineacion Enumerado de tipo `LCD_Alineacion` para indicar si el texto se alinea a la izquierda,
 *     a la derecha o se muestra centrado
 * @param color Color del texto, en formato ARGB de 32 bits
 * @param colorFondo Color del fondo de la etiqueta, en formato ARGB de 32 bits
 * @param visible Buleano que indica si la etiqueta hay que mostrarla visible inicialmente.
 * @param habilitada Buleano que indica si la etiqueta hay que mostrarla habilitada inicialmente.
 * @param etiqueta Puntero a la estructura de tipo `LCD_Etiqueta` que hay que inicializar con los
 *     parámetros anteriores
 *
 * @see LCD_setTextoEtiqueta(), LCD_setColorTextoEtiqueta(),
 *     LCD_setColorFondoEtiqueta(), LCD_setAlineacionEtiqueta(),
 *     LCD_setVisibilidadEtiqueta(), LCD_setHabilitacionEtiqueta(), LCD_setPosicionEtiqueta(),
 *     LCD_atiendeEtiqueta()
 *
 */
void LCD_inicializaEtiqueta(const char* texto, uint16_t x, uint16_t y,
    const LCD_JuegoCaracteresAlpha * juego,
    uint16_t separacion, uint16_t ancho, uint16_t alto, uint16_t margenHorizontal, uint16_t margenVertical,
    LCD_Alineacion alineacion, uint32_t color, uint32_t colorFondo, int visible, int habilitada,
	LCD_Etiqueta * etiqueta);


/**
 * @brief Establece si la etiqueta tiene que ser visible o invisible
 *
 * Hace que la función atiendeEtiqueta() tenga que dibujarla en el frame buffer oculto o no en función
 * del buleano pasado por parámetro.
 *
 * @param visibilidad Buleano para indicar la visibilidad de la etiqueta
 * @param etiqueta Puntero a la estructura de tipo `LCD_Etiqueta` que representa a la etiqueta
 *
 * @see LCD_inicializaEtiqueta(), LCD_setTextoEtiqueta(), LCD_setColorTextoEtiqueta(),
 *     LCD_setColorFondoEtiqueta(), LCD_setAlineacionEtiqueta(),
 *     LCD_setHabilitacionEtiqueta(), LCD_setPosicionEtiqueta(),
 *     LCD_atiendeEtiqueta()
 */
void LCD_setVisibilidadEtiqueta(int visibilidad, LCD_Etiqueta * etiqueta);


/**
 * @brief Establece si la etiqueta tiene que mostrarse habilitada o deshabilitada
 *
 * Hace que la función atiendeEtiqueta() tenga que dibujarla en el frame buffer oculto con sus colores
 * originales si está habilitada o en niveles de gris y semitransparente si está deshabilitada
 *
 * @param habilitacion Buleano para indicar la habilitación de la etiqueta
 * @param etiqueta Puntero a la estructura de tipo `LCD_Etiqueta` que representa a la etiqueta
 *
 * @see LCD_inicializaEtiqueta(), LCD_setTextoEtiqueta(), LCD_setColorTextoEtiqueta(),
 *     LCD_setColorFondoEtiqueta(), LCD_setAlineacionEtiqueta(),
 *     LCD_setVisibilidadEtiqueta(),  LCD_setPosicionEtiqueta(),
 *     LCD_atiendeEtiqueta()
 */
void LCD_setHabilitacionEtiqueta(int habilitacion, LCD_Etiqueta * etiqueta);


/**
 * @brief Establece el texto de una etiqueta
 *
 * Modifica el texto mostrado en la etiqueta
 *
 * @param texto Texto a mostrar en la etiqueta, como máximo 100 caracteres. Los caracteres posibles son
 *     los que se pueden representar mediante un juego de caracteres del tipo `LCD_JuegoCaracteresAlpha`.
 * @param etiqueta Puntero a la estructura de tipo `LCD_Etiqueta` para manejo de la etiqueta
 *
 * @see LCD_inicializaEtiqueta(), LCD_setColorTextoEtiqueta(),
 *     LCD_setColorFondoEtiqueta(), LCD_setAlineacionEtiqueta(),
 *     LCD_setVisibilidadEtiqueta(), LCD_setHabilitacionEtiqueta(), LCD_setPosicionEtiqueta(),
 *     LCD_atiendeEtiqueta()
 */
void LCD_setTextoEtiqueta(const char* texto, LCD_Etiqueta * etiqueta);


/**
 * @brief Establece el color del texto de una etiqueta
 *
 * Modifica el color con el que se muestra el texto en la etiqueta
 *
 * @param color Color del texto, en formato ARGB en 32 bits
 * @param etiqueta Puntero a la estructura de tipo `LCD_Etiqueta` para manejo de la etiqueta
 *
 * @see LCD_inicializaEtiqueta(), LCD_setTextoEtiqueta(),
 *     LCD_setColorFondoEtiqueta(), LCD_setAlineacionEtiqueta(),
 *     LCD_setVisibilidadEtiqueta(), LCD_setHabilitacionEtiqueta(), LCD_setPosicionEtiqueta(),
 *     LCD_atiendeEtiqueta()
 */
void LCD_setColorTextoEtiqueta(uint32_t color, LCD_Etiqueta * etiqueta);


/**
 * @brief Establece el color de fondo de una etiqueta
 *
 * Modifica el color con el que se muestra el fondo de la etiqueta. Si el color tiene una componente Alpha
 * con valor 0, no se dibuja el rectángulo de fondo de la etiqueta.
 *
 * @param color Color del fondo, en formato ARGB en 32 bits
 * @param etiqueta Puntero a la estructura de tipo `LCD_Etiqueta` para manejo de la etiqueta
 *
 * @see LCD_inicializaEtiqueta(), LCD_setTextoEtiqueta(), LCD_setColorTextoEtiqueta(),
 *     LCD_setAlineacionEtiqueta(),
 *     LCD_setVisibilidadEtiqueta(), LCD_setHabilitacionEtiqueta(), LCD_setPosicionEtiqueta(),
 *     LCD_atiendeEtiqueta()
 */
void LCD_setColorFondoEtiqueta(uint32_t colorFondo, LCD_Etiqueta * etiqueta);


/**
 * @brief Establece la posición de una etiqueta
 *
 * Sitúa la etiqueta en la pantalla estableciendo las coordenadas de su esquina superior izquierda
 *
 * @param x Coordenada X de la etiqueta
 * @param y Coordenada Y de la etiqueta
 * @param etiqueta Puntero a la estructura de tipo `LCD_Etiqueta` para manejo de la etiqueta
 *
 * @see LCD_inicializaEtiqueta(), LCD_setTextoEtiqueta(), LCD_setColorTextoEtiqueta(),
 *     LCD_setColorFondoEtiqueta(), LCD_setAlineacionEtiqueta(),
 *     LCD_setVisibilidadEtiqueta(), LCD_setHabilitacionEtiqueta(),
 *     LCD_atiendeEtiqueta()
 */
void LCD_setPosicionEtiqueta(uint16_t x, uint16_t y, LCD_Etiqueta * etiqueta);


/**
 * @brief Establece la alineación del texto en una etiqueta
 *
 * Establece cómo se alinea el texto dentro de la etiqueta: alineado a la izquierda, a la derecha o centrado
 *
 * @param alineacion Enumerado de tipo `LCD_Alineacion` para indicar el tipo de alineación
 * @param etiqueta Puntero a la estructura de tipo `LCD_Etiqueta` para manejo de la etiqueta
 *
 * @see LCD_inicializaEtiqueta(), LCD_setTextoEtiqueta(), LCD_setColorTextoEtiqueta(),
 *     LCD_setColorFondoEtiqueta(),
 *     LCD_setVisibilidadEtiqueta(), LCD_setHabilitacionEtiqueta(), LCD_setPosicionEtiqueta(),
 *     LCD_atiendeEtiqueta()
 */
void LCD_setAlineacionEtiqueta(LCD_Alineacion alineacion, LCD_Etiqueta * etiqueta);


/**
 * @brief Atiende al redibujado de la etiqueta
 *
 * Hay que llamar a esta función en el bucle del hilo que actualiza la información en pantalla, antes de
 * intercambiar los frame buffers, para que se redibuje la etiqueta en el frame buffer oculto.
 *
 * Ejemplo:
 * @code{.c}
 * LCD_Etiqueta etiqueta;
 * LCD_inicializaEtiqueta("Temp", 10, 100, & JuegoAlpha15, 1, 110, 2, LCD_ALINEACION_IZQUIERDA,
 *     0xFFFFFF00, 0xFF004400, 1, 1, &etiqueta);
 * // Crea una estructura para manejar una etiqueta con el texto inicial "Temp", mostrada en pantalla
 * // en (10, 100) con un juego de 15 puntos de alto, 1 punto de separación entre caracteres, la etiqueta
 * // tiene 110 puntos de ancho, con 2 puntos de margen, texto alineado a la izquierda, mostrado en
 * // color amarillo y etiqueta con fondo verde. Inicialmente se muestra visible y habilitada.
 *
 * float t1 = 20.0;  // Variable real para manejar una temperatura
 * char cadena[30];  // Para montar el texto que se va a mostrar en la etiqueta
 *
 * while(1) {
 *
 *      osDelay(500);  // Espera bloqueado 500 ms
 *
 *      LCD_setFondoColor(0xFF000000);  // Pone todos los puntos a color negro
 *
 *      t1 += 0.17976;  // Incrementa la temperatura
 *
 *      sprintf(cadena, "Temp %.2f\x8e", t1);
 *      // Guarda en la cadena el valor de la temperatura con dos decimales, ej. "Temp 23.51º"
 *
 *      LCD_setTextoEtiqueta(cadena, &etiqueta);  // Actualiza el texto en la etiqueta
 *
 *      LCD_atiendeEtiqueta(&etiqueta);  // Redibuja la etiqueta en el frame buffer oculto
 *      LCD_intercambiaBuffers();  // Intercamnbia los frame buffers
 *  }
 * @endcode
 *
 * @param etiqueta Puntero a la estructura de tipo `LCD_Etiqueta` para manejo de la etiqueta
 *
 * @see LCD_inicializaEtiqueta(), LCD_setTextoEtiqueta(), LCD_setColorTextoEtiqueta(),
 *     LCD_setColorFondoEtiqueta(), LCD_setAlineacionEtiqueta(), LCD_setPosicionEtiqueta(),
 *     LCD_setVisibilidadEtiqueta(), LCD_setHabilitacionEtiqueta()
 */
void LCD_atiendeEtiqueta(LCD_Etiqueta * etiqueta);


//--------------------------------------------------------------------------------------------------------
/**
 * @brief Estructura de datos para un componente que maneja una imagen.
 *
 * Permite la visualización de una imagen con unas dimensiones. Se puede establecer y modificar la posición,
 * la habilitación de la imagen (si no está habilitada se muestra semitransparente y en blanco y negro),
 * su visibilidad y se puede cambiar de imagen.
 *
 * @see LCD_inicializImagen(), LCD_setImagen(), LCD_setPosicionImagen(), LCD_setVisibilidadImagen(),
 *     LCD_setHabilitacionImagen(), LCD_atiendeImagen()
 */
typedef struct {
    /** @brief Coordenada X de la esquina superior izquierda de la imagen */
    uint16_t x;
    /** @brief Coordenada Y de la esquina superior izquierda de la imagen */
    uint16_t y;
    /** @brief Ancho de la imagen */
    uint16_t ancho;
    /** @brief Ancho de la imagen */
    uint16_t alto;
    /** @brief Puntero a la zona de memoria donde se describen los colores de los puntos de la imagen,
     *     en formato ARGB de 32 bits */
    const uint8_t * colores;
    /** @brief Buleano que indica si la imagen es visible */
    int visible;
    /** @brief Buleano que indica si la imagen está habilitada */
    int habilitada;
} LCD_Imagen;


/**
 * @brief Inicializa un componente de tipo LCD_Imagen
 *
 * Inicializa una estructura de tipo LCD_Imagen para manejar una imagen en pantalla.
 *
 * @param x Coordenada X de la esquina superior izquierda de la imagen
 * @param y Coordenada Y de la esquina superior izquierda de la imagen
 * @param ancho Ancho de la imagen en puntos
 * @param alto Alto de la imagen en puntos
 * @param colores Puntero a la zona de memoria donde se describen los colores de los puntos de la imagen,
 *       en formato ARGB de 32 bits
 * @param visible Buleano que indica si la imagen es visible
 * @param habilitada Buleano que indica si la imagen está habilitada
 * @param pImagen Puntero a la estructura de tipo LCD_imagen que hay que inicializar
 *
 * @see LCD_Imagen, LCD_setImagen(), LCD_setPosicionImagen(), LCD_setVisibilidadImagen(),
 *     LCD_setHabilitacionImagen(), LCD_atiendeImagen()
 */
void LCD_inicializaImagen(uint16_t x, uint16_t y, uint16_t ancho, uint16_t alto, const uint8_t * colores,
    int visible, int habilitada, LCD_Imagen * pImagen);


/**
 * @brief Modifica la imagen a mostrar en un componente de tipo LCD_Imagen
 *
 * Hace que se muestre una imagen diferente en un componente LCD_Imagen
 *
 * @param colores Puntero a la zona de memoria donde se describen los colores de los puntos de la imagen,
 *       en formato ARGB de 32 bits
 * @param pImagen Puntero a la estructura de tipo LCD_imagen que hay que modificar
 *
 * @see LCD_Imagen, LCD_inicializaImagen(), LCD_setPosicionImagen(),
 *     LCD_setVisibilidadImagen(), LCD_setHabilitacionImagen(), LCD_atiendeImagen()
 */
void LCD_setImagen(const uint8_t * colores, LCD_Imagen * pImagen);


/**
 * @brief Modifica la posición donde se muestra un componente de tipo LCD_Imagen
 *
 * Hace que la imagen se muestre en otra posición en la pantalla
 *
 * @param x Coordenada X de la esquina superior izquierda de la imagen
 * @param y Coordenada Y de la esquina superior izquierda de la imagen
 * @param pImagen Puntero a la estructura de tipo LCD_imagen que hay que modificar
 *
 * @see LCD_Imagen, LCD_inicializaImagen(), LCD_setImagen(),
 *     LCD_setVisibilidadImagen(), LCD_setHabilitacionImagen(), LCD_atiendeImagen()
 */
void LCD_setPosicionImagen(uint16_t x, uint16_t y, LCD_Imagen * pImagen);


/**
 * @brief Modifica la visibilidad de un componente de tipo LCD_Imagen
 *
 * Hace que la imagen se muestre o no se muestre en la pantalla
 *
 * @param visibilidad Buleano que indica si hay que mostrar la imagen
 * @param pImagen Puntero a la estructura de tipo LCD_imagen que hay que modificar
 *
 * @see LCD_Imagen, LCD_inicializaImagen(), LCD_setImagen(),
 *     LCD_setPosicionImagen(), LCD_setHabilitacionImagen(), LCD_atiendeImagen()
 */
void LCD_setVisibilidadImagen(int visibilidad, LCD_Imagen * pImagen);


/**
 * @brief Modifica la habilitación de un componente de tipo LCD_Imagen
 *
 * Hace que la imagen se muestre habilitada (en cuyo caso se muestra con sus colores originales) o no
 * (se muestra semitransparente y en blanco y negro)
 *
 * @param habilitada Buleano que indica si hay que mostrarla habilitada
 * @param pImagen Puntero a la estructura de tipo LCD_imagen que hay que modificar
 *
 * @see LCD_Imagen, LCD_inicializaImagen(), LCD_setImagen(),
 *     LCD_setVisibilidadImagen(), LCD_setPosicionImagen(), LCD_atiendeImagen()
 */
void LCD_setHabilitacionImagen(int habilitada, LCD_Imagen * pImagen);


/**
 * @brief Atiende al redibujado de una imagen
 *
 * Hay que llamar a esta función en el bucle del hilo que actualiza la información en pantalla, antes de
 * intercambiar los frame buffers, para que se redibuje la imagen en el frame buffer oculto.
 *
 * @param pImagen Puntero a la estructura de tipo `LCD_Imagen`
 *
 * @see LCD_Imagen, LCD_inicializaImagen(), LCD_setImagen(),
 *     LCD_setVisibilidadImagen(), LCD_setPosicionImagen(), LCD_setHabilitacionImagen()
 */
void LCD_atiendeImagen(LCD_Imagen * pImagen);


//-------------------------------------------------------------------------------------------------------
/**
 * @brief Estructura para manejo de un botón.
 *
 * Representa a un botón que se muestra en pantalla en una posición con una imagen de unas dimensiones,
 * de forma que cuando se pulsa se ejecuta el código de una función. Se puede establecer un texto que se
 * muestra sobre el botón.
 *
 * @see LCD_atiendeBoton(), LCD_setVisibilidadBoton(), LCD_setHabilitacionBoton().
 */
typedef struct {
    /** @brief Coordenada X de la esquina superior izquierda */
    uint16_t x;
    /** @brief Coordenada Y de la esquina superior izquierda */
    uint16_t y;
    /** @brief Anchura del botón en puntos */
    uint16_t ancho;
    /** @brief Altura del botón en puntos */
    uint16_t alto;
    /** @brief Imagen para visualizar el botón */
    const uint8_t * imagen;
    /** @brief Texto a mostrar sobre el botón, como máximo 30 caracteres */
    char texto[31];
    /** @brief Juego de caracteres utilizado para mostrar el texto */
    const LCD_JuegoCaracteresAlpha * pJuegoCaracteres;
    /** @brief Separación en puntos entre caracteres consecutivos */
    uint8_t separacion;
    /** @brief Color del texto en formato ARGB de 32 bits */
    uint32_t colorTexto;
    /** @brief Posición del texto dentro del botón */
    uint16_t xTexto, yTexto;
    /** @brief Puntero a la función a ejecutar cuando se pulsa el botón */
    void (* funcion)();
    /** @brief Buleano cierto si el botón está habilitado. Si no está habilitado, se muestra
     * semitransparente y en niveles de gris y si se pulsa no se ejecuta la función. */
    int habilitado;
    /** @brief Buleano cierto si el botón está pulsado. */
    int pulsado;
    /** @brief Buleano cierto si el botón está visible. Si está invisible, no se dibuja. */
    int visible;
} LCD_Boton;


/**
 * @brief Inicializa una estructura de tipo LCD_Boton.
 *
 * Ejemplo:
 * @code{.c}
 * void f() {  // Función a ejecutar cuando se pulsa el botón
 *     ...  // Aquí irían sus instrucciones
 * }
 *
 * const uint8_t im[] = { ... };
 * // Imagen para mostrar el botón. En ... se indican los colores de sus puntos.
 *
 * LCD_Boton b;  // Estructura para manejar un botón
 *
 * LCD_inicializaBoton(10, 10, 100, 50, im, f, 1, 1, & b);
 * // Inicializa la estructura b para representar a un botón que se muestra con la imagen im
 * // de 100 puntos de ancho y 50 de alto, situando su esquina superior izquierda en (10, 10) y
 * // de forma que se ejecutará la función f cada vez que se pulse.
 * @endcode
 *
 * @param x Coordenada X de la esquina superior izquierda.
 * @param y Coordenada Y de la esquina superior izquierda.
 * @param ancho Anchura del botón
 * @param alto Altura del botón
 * @param imagen Puntero a dónde están guardados los colores de la imagen utilizada para dibujar el botón,
 *     el color de cada punto expresado en formato ARGB en 32 bits.
 * @param texto Texto a mostrar sobre el botón, como máximo 30 caracteres.
 * @param pJuegoCaracteres Puntero a una estructura de tipo LCD_JuegoCaracteresAlpha para indicar con
 *     qué juego de caracteres se muestra el texto.
 * @param separacion Separación en puntos entre caracteres consecutivos.
 * @param funcion Puntero a la función a ejecutar cuando se pulsa el botón.
 * @param habilitado Buleano para habilitar o deshabilitar el botón. Si no está habilitado, se muestra
 *     semitransparente y en niveles de gris y si se pulsa no se ejecuta la función.
 * @param visible Buleano para mostrar u ocultar el botón.
 * @param pBoton Puntero a la estructura que hay que inicializar.
 *
 * @see LCD_Boton, LCD_setVisibilidadBoton(), LCD_setHabilitacionBoton(), LCD_setTextoBoton(),
 *     LCD_setColorTextoBoton(), LCD_setImagenBoton(), LCD_atiendeBoton()
 */
void LCD_inicializaBoton(uint16_t x, uint16_t y, uint16_t ancho, uint16_t alto, const uint8_t * imagen,
    char * texto, const LCD_JuegoCaracteresAlpha * pJuegoCaracteres, uint8_t separacion, uint32_t colorTexto,
    void (*funcion)(), int habilitado, int visible, LCD_Boton * pBoton);


/**
 * @brief Establece la visibilidad de un botón.
 *
 * Ejemplo:
 * @code{.c}
 * LCD_Boton b;  // Estructura para representar a un botón
 * LCD_inicializaBoton(..., & b);  // Inicializa la estructura con los parámetros que se indicarían en ...
 * LCD_setVisibilidadBoton(0, & b);  // Oculta el botón
 * @endcode
 *
 * @param visibilidad Buleano que establece si el botón va a ser visible o invisible.
 * @param pBoton Puntero a la estructura que representa el botón.
 *
 * @see LCD_Boton, LCD_inicializaBoton(), LCD_setHabilitacionBoton(), LCD_setTextoBoton(),
 *     LCD_setColorTextoBoton(), LCD_setImagenBoton(), LCD_atiendeBoton()
 */
void LCD_setVisibilidadBoton(int visibilidad, LCD_Boton * pBoton);


/**
 * @brief Habilita o deshabilita un botón.
 *
 * Cuando un botón está habilitado y se pulsa, se ejecuta la función asociada al botón y se muestra
 * en pantalla con los colores originales de su imagen. Cuando está deshabilitado no se ejecuta la función
 * y el botón se muestra semitransparente en niveles de gris.
 *
 * Ejemplo:
 * @code{.c}
 * LCD_Boton b;  // Estructura para representar a un botón
 * LCD_inicializaBoton(..., & b);  // Inicializa la estructura con los parámetros que se indicarían en ...
 * LCD_setHabilitacionBoton(0, & b);  // Deshabilita el botón
 * @endcode
 *
 * @param habilitacion Buleano que establece si el botón va a estar habilitado o deshabilitado.
 * @param pBoton Puntero a la estructura que representa el botón.
 *
 * @see LCD_Boton, LCD_inicializaBoton(), LCD_setVisibilidadBoton(), LCD_setTextoBoton(),
 *     LCD_setColorTextoBoton(), LCD_setImagenBoton(), LCD_atiendeBoton()
 */
void LCD_setHabilitacionBoton(int habilitacion, LCD_Boton * pBoton);


/**
 * @brief Establece el texto mostrado sobre el botón
 *
 * Se puede mostrar un texto sobre la imagen asociada al botón, con una longitud máxima de 30 caracteres.
 *
 * Ejemplo:
 * @code{.c}
 * LCD_Boton b;  // Estructura para representar a un botón
 * LCD_inicializaBoton(..., & b);  // Inicializa la estructura con los parámetros que se indicarían en ...
 * LCD_setTextoBoton("Reset", & b);  // Muestra el texto "Reset" sobre el botón
 * @endcode
 *
 * @param texto Cadena de caracteres que describe el texto a mostrar sobre el botón. Se pueden visualizar
 *     los caracteres soportados por los juegos de caracteres de tipo LCD_juegoCaracteresAlpha. Como máximo
 *     se pueden utilizar hasta 30 caracteres.
 * @param pBoton Puntero a la estructura que representa el botón.
 *
 * @see LCD_Boton, LCD_inicializaBoton(), LCD_setVisibilidadBoton(), LCD_setHabilitacionBoton(),
 *     LCD_setColorTextoBoton(), LCD_setImagenBoton(), LCD_atiendeBoton()
 */
void LCD_setTextoBoton(const char* texto, LCD_Boton * pBoton);


/**
 * @brief Establece el color del texto mostrado sobre el botón
 *
 * Se puede mostrar un texto sobre la imagen asociada al botón, con esta función se modifica su color.
 *
 * Ejemplo:
 * @code{.c}
 * LCD_Boton b;  // Estructura para representar a un botón
 * LCD_inicializaBoton(..., & b);  // Inicializa la estructura con los parámetros que se indicarían en ...
 * LCD_setColorTextoBoton(0xFFFF0000, & b);  // El texto se va a mostrar en rojo
 * @endcode
 *
 * @param color Parámetro que describe el color del texto a mostrar sobre el botón.
 *     Hay que indicarlo en un entero de 32 bits sin signo en formato ARGB.
 * @param pBoton Puntero a la estructura que representa el botón.
 *
 * @see LCD_Boton, LCD_inicializaBoton(), LCD_setVisibilidadBoton(), LCD_setHabilitacionBoton(),
 *     LCD_setTextoBoton(), LCD_setImagenBoton(), LCD_atiendeBoton()
 */
void LCD_setColorTextoBoton(uint32_t color, LCD_Boton * pBoton);


/**
 * @brief Establece la imagen utilizada para representar al botón
 *
 * Con esta función se modifica la imagen que se utiliza para mostrar el botón en pantalla. Todas las
 * imágenes utilizadas para el mismo botón tienen que ser de las mismas dimensiones.
 *
 * Ejemplo:
 * @code{.c}
 * LCD_Boton b;  // Estructura para representar a un botón
 * const uint8_t imagen[] = {...};  // Matriz donde se describe una imagen
 * LCD_inicializaBoton(..., & b);  // Inicializa la estructura con los parámetros que se indicarían en ...
 * LCD_setImagenBoton(imagen, & b);  // Establece la imagen del botón
 * @endcode
 *
 * @param imagen Matriz de bytes donde se describen los colores de los puntos de la imagen en formato ARGB.
 * @param pBoton Puntero a la estructura que representa el botón.
 *
 * @see LCD_Boton, LCD_inicializaBoton(), LCD_setVisibilidadBoton(), LCD_setHabilitacionBoton(),
 *     LCD_setTextoBoton(), LCD_setColorTextoBoton(), LCD_atiendeBoton()
 */
void LCD_setImagenBoton(const uint8_t * imagen, LCD_Boton * pBoton);


/**
 * @brief Atiende la posible pulsación sobre el botón.
 *
 * Dibuja el botón, con su imagen y texto, en el frame buffer oculto.
 * Comprueba si se ha pulsado el botón y si es así y está habilitado y visible, ejecuta la función asociada.
 *
 * La llamada a esta función debería de realizarse desde un bucle en la tarea que se encarga de la
 * visualización en pantalla.
 *
 * Ejemplo:
 * @code{.c}
 * LCD_Boton b;  // Estructura para manejar un botón
 *
 * LCD_inicializaBoton(..., & b);  // Inicializa la estructura según los parámetros indicados en ...
 *
 * while(1) {
 *     LCD_actualizaPulsacion();  // Revisa el estado de la interfaz táctil
 *     LCD_atiendeBoton(& b);  // Atiende una posible pulsación sobre el botónl
 * }
 * @endcode
 *
 * @see LCD_Boton, LCD_inicializaBoton(), LCD_setVisibilidadBoton(), LCD_setHabilitacionBoton(),
 *     LCD_setTextoBoton(), LCD_setColorTextoBoton(), LCD_setImagenBoton()
 */
void LCD_atiendeBoton(LCD_Boton * pBoton);


//--------------------------------------------------------------------------------------------------------
/**
 * @brief Estructura para representar un interruptor.
 *
 * Una variable de este tipo representa a un interruptor con estados ON y OFF. Para cada estado se asocia
 * una imagen diferente. Cuando se pulsa el interruptor en pantalla, cambia su estado de ON a OFF o viceversa.
 * Al pulsar el interruptor, se llama a una función a la que se le pasa el nuevo estado del interruptor mediante
 * un buleano.
 *
 * El interruptor se puede hacer visible o invisible. También se puede habilitar o deshabilitar. Si está
 * deshabilitado, cuando se pulsa no cambia de estado ni se ejecuta la función asociada, además se muestra
 * semitransparente y en niveles de gris.
 *
 * @see LCD_inicializaInterruptor(), LCD_atiendeInterruptor(), LCD_setVisibilidadInterruptor(),
 *     LCD_setHabilitacionInterruptor(), LCD_getEstadoInterruptor(), LCD_setExtadoInterruptor().
 */
typedef struct {
    /** @brief Coordenada X de la esquina superior izquierda del interruptor en pantalla. */
    uint16_t x;
    /** @brief Coordenada Y de la esquina superior izquierda del interruptor en pantalla. */
    uint16_t y;
    /** @brief Ancho del interruptor en pantalla. */
    uint16_t ancho;
    /** @brief Alto del interruptor en pantalla. */
    uint16_t alto;
    /** @brief Imagen que se muestra cuando el interruptor está en estado ON */
    const uint8_t * imagenOn;
    /** @brief Imagen que se muestra cuando el interruptor está en estado OFF */
    const uint8_t * imagenOff;
    /** @brief Buleano que indica si está a ON o a OFF*/
    int estado;
    /** @brief Buleano que indica si el interruptor es visible. */
    int visible;
    /** @brief Buleano que indica si está habilitado. */
    int habilitado;
    /** @brief Buleano que indica si se está pulsando el interruptor. */
    int pulsado;
    /** @brief Función que se ejecuta cuando se pulsa el interruptor mientras está habilitado. Se le pasa por
     *     parámetro el nuevo estado del interruptor. */
    void (*funcion)(int);
} LCD_Interruptor;


/**
 * @brief Inicializa un interruptor.
 *
 * Inicializa una estructura de datos de tipo LCD_Interruptor para manejar un interruptor en pantalla con estados
 * ON y OFF. Si se pulsa y es visible y está habilitado, entonces cambia su estado. Muestra una
 * imagen diferente en estado ON y en estado OFF. Cuando cambia de estado se llama a una función a la que se le
 * pasa un buleano indicando el nuevo estado.
 *
 * Ejemplo:
 * @code{.c}
 * LCD_Interruptor in;  // Estructura para manejar un interruptor
 *
 * LCD_inicializaInterruptor(..., & in);  // Inicializa la estructura in con los datos pasados por parámetro
 *
 * while(1) {
 *     LCD_actualizaPulsacion();  // Actualiza el estado de pulsación sobre la pantalla
 *     LCD_atiendeInterruptor(& in);  // Atiende a una posible pulsación sobre el interruptor
 *     LCD_intercambiaBuffers();  // Actualiza en pantalla lo que se dibujó en el buffer oculto.
 * }
 * @endcode
 *
 * @param x Coordenada X de la esquina superior izquierda del interruptor en pantalla.
 * @param y Coordenada Y de la esquina superior izquierda del interruptor en pantalla.
 * @param ancho Número de puntos de ancho del interruptor en pantalla.
 * @param alto Número de puntos de alto del interruptor en pantalla.
 * @param imagenOn Puntero a la imagen que se muestra cuando el interruptor está en estado ON.
 * @param imagenOff Puntero a la imagen que se muestra cuando el interruptor está en estado OFF.
 * @param funcion Puntero a la función que se ejecuta cuando se pulsa el interruptor y cambia su estado. Se
 *     pasa por parámetro un buleano indicando su nuevo estado.
 * @param habilitado Buleano que indica si el interruptor está inicialmente habilitado.
 * @param visible Buleano que indica si el interruptor se muestra inicialmente.
 * @param pInterruptor Puntero a una estructura de tipo LCD_Interruptor que se inicializa con toda la información
 *     pasada en los otros parámetros.
 *
 * @see LCD_Interruptor, LCD_setHabilitacionInterruptor(), LCD_setVisibilidadInterruptor(),
 *     LCD_atiendeInterruptor(), LCD_getEstadoInterruptor(), LCD_setEstadoInterruptor().
 */
void LCD_inicializaInterruptor(uint16_t x, uint16_t y, uint16_t ancho, uint16_t alto,
    const uint8_t * imagenOn, const uint8_t * imagenOff, void (*funcion)(int),
    int habilitado, int visible, LCD_Interruptor * pInterruptor);


/**
 * @brief Habilita o deshabilita un interruptor.
 *
 * Modifica el estado de habilitación de un interruptor. Cuando está deshabilitado y se pulsa, no se ejecuta
 * la función asociada al interruptor y se muestra semitransparente y en niveles de grises.
 *
 * @param habilitacion Buleano donde se indica si se habilita o deshabilita el interruptor.
 * @param pInterruptor Puntero a la estructura que representa al interruptor.
 *
 * @see LCD_Interruptor, LCD_inicializaInterruptor(), LCD_setVisibilidadInterruptor(),
 *     LCD_atiendeInterruptor(), LCD_setEstadoInterruptor(), getEstadoInterruptor().
 */
void LCD_setHabilitacionInterruptor(int habilitacion, LCD_Interruptor * pInterruptor);


/**
 * @brief Muestra u oculta un interruptor.
 *
 * Hace que un interruptor sea visible o que no se muestre en pantalla.
 *
 * @param visibilidad Buleano donde se indica si el interruptor va a se visible o no.
 * @param pInterruptor Puntero a una estructura de tipo LCD_Interruptor que representa a un interruptor.
 *
 * @see LCD_Interruptor, LCD_inicializaInterruptor(), LCD_setHabilitacionInterruptor(),
 *     LCD_atiendeInterruptor(), LCD_getEstadoInterruptor(), LCD_setEstadoInterruptor().
 */
void LCD_setVisibilidadInterruptor(int visibilidad, LCD_Interruptor * pInterruptor);


/**
 * @brief Obtiene el estado del interruptor.
 *
 * Devuelve un buleano indicando el estado actual del interruptor: ON (buleano cierto) u OFF (falso).
 *
 * @see LCD_Interruptor, LCD_inicializaInterruptor(), LCD_setVisibilidadInterruptor(), LCD_setHabilitacionInterruptor(),
 *     LCD_atiendeInterruptor(), LCD_setEstadoInterruptor().
 */
int LCD_getEstadoInterruptor(LCD_Interruptor * pInterruptor);


/**
 * @brief Atiende a una posible pulsación sobre un interruptor.
 *
 * Comprueba si el interruptor se ha pulsado, en cuyo caso, si el interruptor es visible y está habilitado,
 * se ejecuta la función asociada. Esta función está preparada para ser llamada desde un bucle contínuo ejecutado
 * en la tarea encargada de visualizar información en pantalla, como en el siguiente ejemplo:
 *
 * Ejemplo:
 * @code{.c}
 * LCD_Interruptor in;  // Estructura de datos para manejar un interruptor
 *
 * LCD_inicializaInterruptor(..., & in);  // Inicializa la estructura in con los parámetros indicados en ...
 *
 * while(1) {
 *     LCD_actualizaPulsacion();  // Comprueba si se ha pulsado la pantalla
 *     LCD_atiendeInterruptor(& in);  // Atiende a una posible pulsación sobre el interruptor
 *     LCD_intercambiaBuffers();  // Actualiza la visualización en pantalla
 * }
 * @endcode
 *
 * @param pInterruptor Puntero a una estructura de tipo LCD_Interruptor para manejar un interruptor.
 *
 * @see LCD_Interruptor, LCD_inicializaInterruptor(), LCD_setHabilitacionInterruptor(), LCD_setVisibilidadInterruptor(),
 *     LCD_getExtadoInterruptor(), LCD_setEstadoInterruptor().
 */
void LCD_atiendeInterruptor(LCD_Interruptor * pInterruptor);


/**
 * Establece el estado de un interruptor.
 *
 * Modifica el estado de un interruptor a ON o a OFF. Si el nuevo estado es diferente del que tenía, se
 * modifica su estado, cambiando la imagen visualizada y ejecutando la función asociada al interruptor.
 *
 * @param estado Buleano indicando el nuevo estado, ON (valor cierto) u OFF (valor falso).
 * @param pInterruptor Puntero a una estructura de tipo LCD_Interruptor que representa al interruptor.
 *
 * @see LCD_Interruptor, LCD_inicializaInterruptor(), LCD_setHabilitacionInterruptor(), LCD_setVisibilidadInterruptor(),
 *     LCD_atiendeInterruptor(), LCD_getEstadoInterruptor().
 */
void LCD_setEstadoInterruptor(int estado, LCD_Interruptor * pInterruptor);


//-------------------------------------------------------------------------------------------------------
/**
 * @brief Valor mostrado en una barra horizontal.
 *
 * Estructura de datos para representar a un valor numérico mostrado en texto y en horizontal en unaa
 * barra coloreada. El valor se mueve entre un mínimo y un máximo y se muestra en texto con una cierta
 * cantidad de decimales. Se puede indicar un texto fijo que se mostrará a la izquierda del valor.
 *
 * @see LCD_inicializaBarra(), LCD_setVisibilidadBarra(), LCD_setColorTextoBarra(),
 *     LCD_setColorBarra(), LCD_setColorFondoBarra(), LCD_setValorBarra(), LCD_atiendeBarra()
 */
typedef struct {
    /** @brief Texto que se muestra a la izquierda del valor */
    const char* texto;
    /** @brief Valor mínimo posible */
    float minimo;
    /** @brief Valor máximo posible */
    float maximo;
    /** @brief Formato para sprintf utilizado en la visualización del valor en texto */
    char formato[12];
    /** @brief Valor mostrado */
    float valor;
    /** @brief Factor de escala, desde el valor a puntos en pantalla para el ancho de la barra */
    float escala;
    /** @brief Coordenada X de la esquina superior izquierda de la barra */
    uint16_t x;
    /** @brief Coordenada Y de la esquina superior izquierda de la barra */
    uint16_t y;
    /** @brief Márgen izquierdo del texto con respecto al borde izquierdo de la barra */
    uint8_t margenTextoX;
    /** @brief Márgen superior del texto con respecto al borde superior de la barra */
    uint8_t margenTextoY;
    /** @brief Anchura de la barra en puntos */
    uint8_t largo;
    /** @brief Altura de la barra en puntos */
    uint8_t grosor;
    /** @brief Número de decimales utilizados en la visualización del valor */
    uint8_t decimales;
    /** @brief Buleano cierto si se dibujó previamente algún valor */
    uint8_t inicializada;
    /** @brief Buleano que indica si la barra es visible */
    uint8_t visible;
    /** @brief Color utilizado para visualizar el valor en texto */
    uint32_t colorTexto;
    /** @brief Color utilizado para visualizar gráficamente el valor en la barra */
    uint32_t colorBarra;
    /** @brief Color de fondo de la barra */
    uint32_t colorFondo;
    /** @brief Juego de caracteres utilizado para visualizar el valor en texto */
    const LCD_JuegoCaracteresAlpha * juegoCaracteres;
    /** @brief Separación entre caracteres consecutivos en el texto */
    uint8_t separacion;
} LCD_Barra;


/* @brief Inicialización de una barra horizontal
 *
 * Inicializa la estructura de tipo `LCD_Barra` para representar a una barra horizontal en la que mostrar
 * un dato numérico gráficamente y también en texto utilizando un número de decimales con un juego de
 * caracteres.
 *
 * @param texto Texto que se muestra antes del valor
 * @param minimo Valor mínimo posible
 * @param maximo Valor máximo posible
 * @param largo Longitud de la barra en puntos
 * @param grosor Altura de la barra en puntos
 * @param x Coordenada X de la esquina superior izquierda de la barra
 * @param y Coordenada Y de la esquina superior izquierda de la barra
 * @param numCaracteres Número total de caracteres utilizados para visualizar el valor
 * @param decimales Número de decimales para visualizar el valor
 * @param colorTexto Color del texto donde se visualiza el valor
 * @param colorBarra Color de la barra, de longitud proporcional al valor
 * @param colorFondo Color restante
 * @param pJuegoCaracteres Juego de caracteres utilizado para visualizar el valor
 * @param separacion Separación en puntos entre caracteres consecutivos
 * @param pBarra Puntero a la estructura que representa a la barra
 *
 * @see LCD_Barra, LCD_setVisibilidadBarra(), LCD_setColorTextoBarra(),
 *     LCD_setColorBarra(), LCD_setColorFondoBarra(), LCD_setValorBarra(), LCD_atiendeBarra()
 */
void LCD_inicializaBarra(
    const char* texto, float minimo, float maximo, uint8_t largo, uint8_t grosor, uint16_t x, uint16_t y,
    uint8_t numCaracteres, uint8_t decimales, uint32_t colorTexto, uint32_t colorBarra, uint32_t colorFondo,
    const LCD_JuegoCaracteresAlpha * pJuegoCaracteres, uint8_t separacion, LCD_Barra * pBarra);


/* @brief Visibilidad de una barra horizontal
 *
 * Establece is una barra es visible o invisible
 *
 * @param visible Buleano que establece la visibilidad
 * @param pBarra Puntero a la estructura que representa a la barra
 *
 * @see LCD_Barra, LCD_inicializaBarra(), LCD_setColorTextoBarra(),
 *     LCD_setColorBarra(), LCD_setColorFondoBarra(), LCD_setValorBarra(), LCD_atiendeBarra()
 */
void LCD_setVisibilidadBarra(int visible, LCD_Barra * pBarra);


/* @brief Establece el color del texto
 *
 * Establece el color (expresado en formato ARGB) donde se muestra el valor representado en la barra
 *
 * @param color Color del texto en formato ARGB
 * @param pBarra Puntero a la estructura que representa a la barra
 *
 * @see LCD_Barra, LCD_inicializaBarra(), LCD_setVisibilidadBarra(),
 *     LCD_setColorBarra(), LCD_setColorFondoBarra(), LCD_setValorBarra(), LCD_atiendeBarra()
 */
void LCD_setColorTextoBarra(uint32_t color, LCD_Barra * pBarra);


/* @brief Establece el color de la barra
 *
 * Establece el color (en formato ARGB) de una zona rectangular cuya longitud es proporcional al
 * valor mostrado en la barra
 *
 * @param color Color de la barra en formato ARGB
 * @param pBarra Puntero a la estructura que representa a la barra
 *
 * @see LCD_Barra, LCD_inicializaBarra(), LCD_setVisibilidadBarra(), LCD_setColorTextoBarra(),
 *     LCD_setColorFondoBarra(), LCD_setValorBarra(), LCD_atiendeBarra()
 */
void LCD_setColorBarra(uint32_t color, LCD_Barra * pBarra);


/* @brief Establece el color de fondo en la barra
 *
 * Establece el color utilizado en la zona no ocupada por la barra que representa el valor
 *
 * @param color Color del fondo en formato ARGB
 * @param pBarra Puntero a la estructura que representa a la barra
 *
 * @see LCD_Barra, LCD_inicializaBarra(), LCD_setVisibilidadBarra(), LCD_setColorTextoBarra(),
 *     LCD_setColorBarra(), LCD_setValorBarra(), LCD_atiendeBarra()
 */
void LCD_setColorFondoBarra(uint32_t color, LCD_Barra * pBarra);


/* @Brief Establece el valor mostrado en la barra
 *
 * Establece el valor que se va a mostrar en texto y también gráficamente fijando la longitud
 * de la barra
 *
 * @param valor Número real que establece el nuevo valor
 * @param pBarra Puntero a la estructura que representa a la barra
 *
 * @see LCD_Barra, LCD_inicializaBarra(), LCD_setVisibilidadBarra(), LCD_setColorTextoBarra(),
 *     LCD_setColorBarra(), LCD_setColorFondoBarra(), LCD_atiendeBarra()
 */
void LCD_setValorBarra(float valor, LCD_Barra * pBarra);


/* @brief Visualiza la barra
 *
 * Visualiza la barra, dibujando el fondo, la barra y el valor en texto
 *
 * @param pBarra Puntero a la estructura que representa a la barra
 *
 * @see LCD_Barra, LCD_inicializaBarra(), LCD_setVisibilidadBarra(), LCD_setColorTextoBarra(),
 *     LCD_setColorBarra(), LCD_setColorFondoBarra(), LCD_setValorBarra(),
 */
void LCD_atiendeBarra(LCD_Barra * pBarra);


//-------------------------------------------------------------------------------------------------------
/**
 * @brief Componente para la introducción de un valor numérico
 *
 * Estructura de datos para representar a un valor numérico que se puede editar pulsando en la zona izquierda
 * o derecha del componente para aplicar al valor operaciones de decremento o incremento. La zona de pantalla
 * donde se muestra este componente se divide en horizontal en cuatro partes. Yendo de izquierda a derecha,
 * pulsando en la primera parte se realiza un decremento mayor, en la segunda parte se realiza un decremento
 * menor, en la tercera un incremento menor y en la cuarta un incremento mayor. Esta zona tiene un color de
 * fondo y el valor se muestra en texto con otro color, con un número máximo de caracteres y un número de
 * decimales, alineado a la izquierda, a la derecha o centrado.
 *
 * @see LCD_inicializaEditor(), LCD_getValorEditor(), LCD_setValorEditor(), LCD_atiendeEditor()
 */
typedef struct {
	/** @brief Coordenada X en pantalla de la esquina superior izquierda del editor */
	uint16_t x;
	/** @brief Coordenada X en pantalla del 25% del editor */
	uint16_t x25;
	/** @brief Coordenada X en pantalla del 50% del editor */
	uint16_t x50;
	/** @brief Coordenada X en pantalla del 75% del editor */
	uint16_t x75;
	/** @brief Coordenada X en pantalla del borde derecho del editor */
	uint16_t x100;
	/** @brief Coordenada Y en pantalla de la esquina superior izquierda del editor */
	uint16_t y;
	/** @brief Número de puntos de ancho del editor */
	uint16_t ancho;
	/** @brief Número de puntos de alto del editor */
	uint16_t alto;
    /** @brief Márgen horizontal del texto con respecto al borde de la barra cuando la alineación es
        a la izquierda o a la derecha */
    uint8_t margenTextoX;
    /** @brief Márgen del texto con respecto al borde superior de la barra */
    uint8_t margenTextoY;
    /** @brief Alineación del valor a la izquierda, al centro o a la derecha */
    LCD_Alineacion alineacion;
    /** @brief Valor del dato a editar */
    float valor;
	/** @brief Incremento o decremento menor aplicado cuando se hace click en la parte central del editor */
	float incrementoMenor;
	/** @brief Incremento o decremento mayor aplicado cuando se hace click en los extremos del editor */
	float incrementoMayor;
    /** @brief Formato para sprintf utilizado en la visualización del valor en texto */
    char formato[12];
    /** @brief Buleano cierto si se está pulsando el editor */
	int pulsado;
    /** @brief Color de fondo del editor */
	uint32_t colorFondo;
	/** @brief Color del texto con el que se muestra el valor en el editor */
	uint32_t colorTexto;
	/** @brief Juego de caracteres utilizado para mostrar el valor */
	const LCD_JuegoCaracteresAlpha * pJuego;
	/** @brief Separación en puntos entre caracteres consecutivos */
	uint8_t separacion;
	/** @brief Número de decimales utilizados para visualizar el valor */
	uint8_t decimales;
} LCD_Editor;


/* @brief Inicializa un componente de introducción de un valor numérico
 *
 * Inicializa una estructura de tipo `LCD_Editor`para que represente a un un valor numérico que se puede
 * editar pulsando en la zona izquierda o derecha del componente para aplicar al valor operaciones de
 * decremento o incremento.
 *
 * @param valorInicial Valor inicial mostrado en el editor
 * @param incrementoMenor Cantidad que incrementa o decrementa el valor cuando se pulsa a la izquierda o derecha
 *     del centro del editor
 * @param incrementoMayor Cantidad que incrementa o decrementa el valor cuando se pulsa en los extremos
 *     del editor
 * @param x Coordenada X de la esquina superior izquierda del editor
 * @param y Coordenada Y de la esquina superior izquierda del editor
 * @param ancho Ancho en puntos del editor
 * @param alto Alto en puntos del editor
 * @param margenTextoX Si el texto en el editor está alineado a la izquierda, aquí se indica el margen
 *   horizontal desde el borde izquierdo hasta el texto y si está alineado a la derecha, el margen
 *   horitontal desde el borde derecho hasta el texto
 * @param margenTextoY Margen vertical desde el borde superior hasta el texto
 * @param alineacion Enumerado de tipo `LCD_Alineacion` que establece alineación de texto a la izquierda,
 *   centrado o a la derecha
 * @param colorFondo Color de fondo del editor en formato ARGB
 * @param colorTexto Color del texto mostrado en el editor en formato ARGB
 * @param pJuego Puntero a una estructura de tipo `LCD_JuegoCaracteresAlpha` para establecer el juego
 *   de caracteres del texto del editor
 * @param separacion Separación en puntos entre dos caracteres consecutivos
 * @param pEditor Puntero a la estructura que representa al editor
 *
 * @see LCD_Editor, LCD_getValorEditor(), LCD_setValorEditor(), LCD_atiendeEditor()
 */
void LCD_inicializaEditor(float valorInicial, float incrementoMenor, float incrementoMayor,
	uint16_t x, uint16_t y, uint16_t ancho, uint16_t alto, uint16_t margenTextoX, uint16_t margenTextoY,
	LCD_Alineacion alineacion,
	uint32_t colorFondo, uint32_t colorTexto, const LCD_JuegoCaracteresAlpha * pJuego, uint8_t separacion,
	uint8_t decimales, LCD_Editor * pEditor);


/* @brief Obtiene el valor editado
 *
 * Función para obtener el valor establecido por el usuario en el editor
 *
 * @param pEditor Puntero a la estructura que representa al editor
 * @return Devuelve en un real de tipo float el valor editado
 *
 * @see LCD_Editor, LCD_inicializaEditor(), LCD_getValorEditor(), LCD_setValorEditor(), LCD_atiendeEditor()
 */
float LCD_getValorEditor(const LCD_Editor * pEditor);


/* @brief Establece el valor a editar
 *
 * Modifica el valor mostrado en el editor para que sea punto de partida para que el usuario lo pueda
 * incrementar y/o decrementar
 *
 * @param valor Nuevo valor a mostrar en el editor
 * @param pEditor Puntero a la estructura que representa al editor
 *
 * @see LCD_Editor, LCD_inicializaEditor(), LCD_getValorEditor(), LCD_setValorEditor(), LCD_atiendeEditor()
 */
void LCD_setValorEditor(float valor, LCD_Editor * pEditor);


/* @brief Atiende al editor
 *
 * Visualiza el editor en pantalla y atiende a la posible interacción táctil del usuario con el editor
 *
 * @param pEditor Puntero a la estructura que representa al editor
 *
 * @see LCD_Editor, LCD_inicializaEditor(), LCD_getValorEditor(), LCD_setValorEditor(), LCD_atiendeEditor()
 */
void LCD_atiendeEditor(LCD_Editor * pEditor);



#endif /* INTERFAZLCD_H_ */
