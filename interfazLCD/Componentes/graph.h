/**
 * @file Graficas.h
 * @brief Definiciones y funciones para el manejo de gráficos y curvas en pantalla LCD.
 *
 * @example GraficoEjemplo.c
 *
 * @brief Ejemplo de uso de las funciones de `Graficas.h` para inicializar y dibujar un gráfico
 *        en una pantalla LCD, incluyendo título, nombres de los ejes y una curva de datos.
 *
 * Este ejemplo muestra cómo configurar un gráfico con título y nombres de ejes opcionales,
 * cómo inicializar una curva dentro del gráfico y cómo actualizar el gráfico y la curva
 * en la pantalla LCD. Si no se agrega ningun punto a la curva, por defecto es cero
 *
 * @code
 * #include "Graficas.h"
 *
 * // 1. Inicializar el gráfico
 * Graph grafica;
 * InicializaLineGraph(&grafica, 10, 20, 300, 200, 0xFFFFFF, 0x000000, 10, 0, 100);
 *
 * // 2. Configurar título y nombres de los ejes (opcional)
 * setTittle(&grafica, "Temperatura vs Tiempo", &miJuegoCaracteres, 0xFF0000, 2, 1);
 * setAxisNames(&grafica, "Tiempo (s)", "Temperatura (C)", &miJuegoCaracteres, 0x0000FF, 2, 1);
 * ... existen mas personaliziaciones
 *
 * // 3. Inicializar la curva de datos
 * Curva curvaTemperatura;
 * InicializaCurva(&curvaTemperatura, 0, 100, 0x00FF00, 320, 5);
 *
 *
 * // 4. Dibujar el gráfico en pantalla
 * DrawGraph(&grafica);
 *
 * // 5. Agregar y dibujar puntos en la curva
 * float nuevoPunto = 23.5;
 * agregarNuevoPunto(&grafica, &curvaTemperatura, nuevoPunto);
 * atiendeCurva(&grafica, &curvaTemperatura);
 *
 * @endcode
 */

#ifndef Graficas_H_
#define Graficas_H_

#include "pantallaLCD.h"
#include "interfazLCD.h"

#include <cmsis_os2.h> ///< Biblioteca para función osDelay().
#include <math.h>
#include <stdint.h>
#include <stdio.h>  ///< Para función sprintf().
#include <stdlib.h> ///< Para función abs().
#include <string.h>

// -----------------------------------------------------------------------------------------------------------------
// Estructuras para el manejo de gráficos

typedef enum {
    LINEAS,  ///< Gráfico lineas
    PUNTOS,  ///< Gráfico de puntos
} tipoGrafica;


/**
 * @brief Estructura que representa los parámetros y propiedades de un gráfico.
 *
 * Esta estructura contiene todos los parámetros necesarios para personalizar y dibujar un gráfico en pantalla.
 * Permite configurar posición, tamaño, colores, y propiedades adicionales para el título, nombres de los ejes y cuadrícula.
 */
typedef struct {
    uint16_t xInicial;        ///< Coordenada X inicial del gráfico en la pantalla.
    uint16_t yInicial;        ///< Coordenada Y inicial del gráfico en la pantalla.
    uint16_t ancho;           ///< Ancho del gráfico en píxeles.
    uint16_t alto;            ///< Alto del gráfico en píxeles.
    uint32_t backgroundColor; ///< Color de fondo del gráfico en formato RGB.
    uint32_t axisColor;       ///< Color de los ejes del gráfico en formato RGB.
    int graphPadding;         ///< Distancia entre el gráfico y los bordes verticales en píxeles.
    int enBlancoYNegro;       ///< Modo de visualización en blanco y negro: 1 para activado, 0 para desactivado.
    int transparencia;        ///< Nivel de transparencia del gráfico (0 a 100), donde 0 es completamente transparente.
    int estaDibujada;         ///< Bandera que indica si el gráfico ya ha sido dibujado: 1 si está dibujado, 0 si no.

    // Variables para la representación de las curvas
    int setCurvasIndependientes; ///< Bandera para indicar si las curvas son independientes: 1 para independientes, 0 para relacionadas.
    float rangoMaximoCurva;      ///< Rango máximo permitido para las curvas.

    tipoGrafica tipo;         ///< Tipo de gráfico: LINEAS, PUNTOS.

    //------------------------------------------------------------------------------------
    // PERSONALIZACIÓN DEL GRÁFICO

    int alineacion; ///< Tipo de alineación del gráfico: 0 para izquierda, 1 para centrado, 2 para derecha.

    // Personalización del título
    int setTitulo;                                    ///< Bandera para mostrar el título del gráfico: 1 si se muestra, 0 si no.
    LCD_JuegoCaracteresAlpha *pJuegoCaracteresTittle; ///< Puntero al juego de caracteres para el título.
    uint8_t separacionCaracteresTitulo;               ///< Espacio en píxeles entre caracteres en el título.
    const char *titulo;                               ///< Cadena de texto que representa el título del gráfico.
    uint16_t anchoTitulo;                             ///< Ancho total del título en píxeles.
    uint32_t titleColor;                              ///< Color del título en formato RGB.

    // Personalización de los nombres de los ejes
    int setAxisNames;                               ///< Bandera para mostrar los nombres de los ejes: 1 si se muestran, 0 si no.
    LCD_JuegoCaracteresAlpha *pJuegoCaracteresAxis; ///< Puntero al juego de caracteres para los nombres de los ejes.
    uint8_t separacionCaracteresAxis;               ///< Espacio en píxeles entre caracteres en los nombres de los ejes.
    const char *xAxisName;                          ///< Nombre del eje X como cadena de texto.
    const char *yAxisName;                          ///< Nombre del eje Y como cadena de texto.
    uint16_t anchoXAxis;                            ///< Ancho del nombre del eje X en píxeles.
    uint16_t anchoYAxis;                            ///< Ancho del nombre del eje Y en píxeles.
    uint32_t axisNameColor;                         ///< Color de los nombres de los ejes en formato RGB.

    // Personalización de la cuadrícula
    int setGrid;                     ///< Bandera para habilitar la cuadrícula en el gráfico: 1 para habilitada, 0 para deshabilitada.
    int setFondoTranspparente;       ///< Bandera para indicar si el fondo de la cuadrícula es transparente: 1 para transparente, 0 para sólido.
    uint32_t gridColor;              ///< Color de la cuadrícula en formato RGB.
    int gridEnBlancoYNegro;          ///< Modo de visualización en blanco y negro para la cuadrícula: 1 para activado, 0 para desactivado.
    int gridTransparencia;           ///< Nivel de transparencia de la cuadrícula (0 a 100), donde 0 es completamente transparente.
    int divisionesVerticales;        ///< Número de divisiones verticales en la cuadrícula.

} Graph;


/**
 * @brief Estructura que representa los parámetros y propiedades de una curva en el gráfico.
 *
 * Esta estructura almacena la información necesaria para representar una curva en un gráfico.
 * Define los límites de valores, el color, la cantidad de puntos y los valores de la curva,
 * además del desplazamiento vertical y el rango dinámico de la curva.
 */
typedef struct {
    uint16_t maxValue;    ///< Valor máximo de la curva, define el límite superior en el eje Y.
    uint16_t minValue;    ///< Valor mínimo de la curva, define el límite inferior en el eje Y.
    uint32_t curvaColor;  ///< Color de la curva en formato RGB (32 bits).
    float y[320];         ///< Array de valores Y que representan los puntos de la curva en el gráfico.
    int cantidadDePuntos; ///< Número total de puntos que componen la curva.

    float rangoCurva;     ///< Rango dinámico de la curva, calculado como la diferencia entre el valor máximo y el mínimo.
    float valorDesplazamientoVertical; ///< Valor de desplazamiento vertical aplicado a la curva en el gráfico.
} Curva;

//------------------------------------------------------------------------------------------------------------
// Getters y setters

// ------------------------------------------------------------------------------------------------------------
// Getters Graph

/**
 * @brief Obtiene la coordenada X inicial del gráfico.
 * @param[in] graph Puntero a la estructura del gráfico.
 * @return Coordenada X inicial del gráfico.
 */
uint16_t Graph_getXInicial(const Graph *graph);

/**
 * @brief Obtiene la coordenada Y inicial del gráfico.
 * @param[in] graph Puntero a la estructura del gráfico.
 * @return Coordenada Y inicial del gráfico.
 */
uint16_t Graph_getYInicial(const Graph *graph);

/**
 * @brief Obtiene el ancho del gráfico en píxeles.
 * @param[in] graph Puntero a la estructura del gráfico.
 * @return Ancho del gráfico en píxeles.
 */
uint16_t Graph_getAncho(const Graph *graph);

/**
 * @brief Obtiene el alto del gráfico en píxeles.
 * @param[in] graph Puntero a la estructura del gráfico.
 * @return Alto del gráfico en píxeles.
 */
uint16_t Graph_getAlto(const Graph *graph);

/**
 * @brief Obtiene el color de fondo del gráfico.
 * @param[in] graph Puntero a la estructura del gráfico.
 * @return Color de fondo del gráfico en formato RGB.
 */
uint32_t Graph_getBackgroundColor(const Graph *graph);

/**
 * @brief Obtiene el color de los ejes del gráfico.
 * @param[in] graph Puntero a la estructura del gráfico.
 * @return Color de los ejes del gráfico en formato RGB.
 */
uint32_t Graph_getAxisColor(const Graph *graph);

/**
 * @brief Obtiene el espaciado del gráfico.
 * @param[in] graph Puntero a la estructura del gráfico.
 * @return Espaciado del gráfico en píxeles.
 */
int Graph_getGraphPadding(const Graph *graph);

/**
 * @brief Verifica si el gráfico está en modo blanco y negro.
 * @param[in] graph Puntero a la estructura del gráfico.
 * @return 1 si está en blanco y negro, 0 si está en color.
 */
int Graph_getEnBlancoYNegro(const Graph *graph);

/**
 * @brief Obtiene el nivel de transparencia del gráfico.
 * @param[in] graph Puntero a la estructura del gráfico.
 * @return Nivel de transparencia (0 a 100).
 */
int Graph_getTransparencia(const Graph *graph);

/**
 * @brief Verifica si el gráfico ha sido dibujado.
 * @param[in] graph Puntero a la estructura del gráfico.
 * @return 1 si el gráfico está dibujado, 0 si no.
 */
int Graph_getEstaDibujada(const Graph *graph);

// ------------------------------------------------------------------------------------------------------------
// Setters Graph

/**
 * @brief Establece la coordenada X inicial del gráfico.
 * @param[in,out] graph Puntero a la estructura del gráfico.
 * @param[in] xInicial Coordenada X inicial.
 */
void Graph_setXInicial(Graph *graph, uint16_t xInicial);

/**
 * @brief Establece la coordenada Y inicial del gráfico.
 * @param[in,out] graph Puntero a la estructura del gráfico.
 * @param[in] yInicial Coordenada Y inicial.
 */
void Graph_setYInicial(Graph *graph, uint16_t yInicial);

/**
 * @brief Establece el ancho del gráfico en píxeles.
 * @param[in,out] graph Puntero a la estructura del gráfico.
 * @param[in] ancho Ancho del gráfico en píxeles.
 */
void Graph_setAncho(Graph *graph, uint16_t ancho);

/**
 * @brief Establece el alto del gráfico en píxeles.
 * @param[in,out] graph Puntero a la estructura del gráfico.
 * @param[in] alto Alto del gráfico en píxeles.
 */
void Graph_setAlto(Graph *graph, uint16_t alto);


/**
 * @brief Establece el color de fondo del gráfico.
 * @param[in,out] graph Puntero a la estructura del gráfico.
 * @param[in] backgroundColor Color de fondo en formato RGB.
 */
void Graph_setBackgroundColor(Graph *graph, uint32_t backgroundColor);

/**
 * @brief Establece el color de los ejes del gráfico.
 * @param[in,out] graph Puntero a la estructura del gráfico.
 * @param[in] axisColor Color de los ejes en formato RGB.
 */
void Graph_setAxisColor(Graph *graph, uint32_t axisColor);

/**
 * @brief Establece el espaciado del gráfico.
 * @param[in,out] graph Puntero a la estructura del gráfico.
 * @param[in] graphPadding Espaciado en píxeles.
 */
void Graph_setGraphPadding(Graph *graph, int graphPadding);

/**
 * @brief Activa o desactiva el modo blanco y negro.
 * @param[in,out] graph Puntero a la estructura del gráfico.
 * @param[in] enBlancoYNegro 1 para activar blanco y negro, 0 para color.
 */
void Graph_setEnBlancoYNegro(Graph *graph, int enBlancoYNegro);

/**
 * @brief Establece el nivel de transparencia del gráfico.
 * @param[in,out] graph Puntero a la estructura del gráfico.
 * @param[in] transparencia Nivel de transparencia (0 a 100).
 */
void Graph_setTransparencia(Graph *graph, int transparencia);

/**
 * @brief Establece el estado de si el gráfico ha sido dibujado.
 * @param[in,out] graph Puntero a la estructura del gráfico.
 * @param[in] estaDibujada 1 si está dibujado, 0 si no.
 */
void Graph_setEstaDibujada(Graph *graph, int estaDibujada);

// ------------------------------------------------------------------------------------------------------------
// Getters Curva

/**
 * @brief Obtiene el valor máximo de la curva.
 * @param[in] curva Puntero a la estructura de la curva.
 * @return Valor máximo de la curva.
 */
uint16_t Curva_getMaxValue(const Curva *curva);

/**
 * @brief Obtiene el valor mínimo de la curva.
 * @param[in] curva Puntero a la estructura de la curva.
 * @return Valor mínimo de la curva.
 */
uint16_t Curva_getMinValue(const Curva *curva);

/**
 * @brief Obtiene el color de la curva.
 * @param[in] curva Puntero a la estructura de la curva.
 * @return Color de la curva en formato RGB.
 */
uint32_t Curva_getCurvaColor(const Curva *curva);

/**
 * @brief Obtiene el valor Y de un punto específico en la curva.
 * @param[in] curva Puntero a la estructura de la curva.
 * @param[in] index Índice del punto Y en la curva.
 * @return Valor Y del punto.
 */
float Curva_getY(const Curva *curva, int index);

/**
 * @brief Obtiene la cantidad de puntos en la curva.
 * @param[in] curva Puntero a la estructura de la curva.
 * @return Cantidad de puntos en la curva.
 */
int Curva_getCantidadDePuntos(const Curva *curva);


// ------------------------------------------------------------------------------------------------------------
// Setters Curva

/**
 * @brief Establece el valor máximo de la curva.
 * @param[in,out] curva Puntero a la estructura de la curva.
 * @param[in] maxValue Valor máximo a establecer.
 */
void Curva_setMaxValue(Curva *curva, uint16_t maxValue);

/**
 * @brief Establece el valor mínimo de la curva.
 * @param[in,out] curva Puntero a la estructura de la curva.
 * @param[in] minValue Valor mínimo a establecer.
 */
void Curva_setMinValue(Curva *curva, uint16_t minValue);

/**
 * @brief Establece el color de la curva.
 * @param[in,out] curva Puntero a la estructura de la curva.
 * @param[in] curvaColor Color de la curva en formato RGB.
 */
void Curva_setCurvaColor(Curva *curva, uint32_t curvaColor);

/**
 * @brief Establece el valor Y de un punto específico en la curva.
 * @param[in,out] curva Puntero a la estructura de la curva.
 * @param[in] index Índice del punto.
 * @param[in] value Valor Y a establecer en el punto.
 */
void Curva_setY(Curva *curva, int index, float value);

/**
 * @brief Establece la cantidad de puntos en la curva.
 * @param[in,out] curva Puntero a la estructura de la curva.
 * @param[in] cantidadDePuntos Cantidad de puntos a establecer.
 */
void Curva_setCantidadDePuntos(Curva *curva, int cantidadDePuntos);

//------------------------------------------------------------------------------------------------------------
// Funciones de inicialización de componentes

/**
 * @brief Inicializa los parámetros de un gráfico de líneas.
 *
 * @param[in,out] graph Estructura del gráfico a inicializar.
 * @param xInicial Coordenada X inicial del gráfico.
 * @param yInicial Coordenada Y inicial del gráfico.
 * @param ancho Ancho del gráfico.
 * @param alto Alto del gráfico.
 * @param T Periodo de muestreo.
 * @param backgroundColor Color de fondo del gráfico.
 * @param axisColor Color de los ejes del gráfico.
 * @param graphPadding Espaciado del gráfico.
 * @param enBlancoYNegro Bandera para activar el modo blanco y negro.
 * @param transparencia Nivel de transparencia del gráfico.
 */
void Graph_inicializaLineGraph(Graph *graph, int16_t xInicial, uint16_t yInicial, uint16_t ancho,
		 	 	 	 	 	   uint16_t alto, uint32_t backgroundColor, uint32_t axisColor,
							   int graphPadding, int enBlancoYNegro, int transparencia);

/**
 * @brief Inicializa los parámetros de una curva para el gráfico.
 *
 * @param[in,out] curva Estructura de la curva a inicializar.
 * @param minValue Valor mínimo de la curva.
 * @param maxValue Valor máximo de la curva.
 * @param graphColor Color de la curva.
 * @param cantidadDePuntos Número de puntos de la curva.
 */
void Graph_InicializaCurva(Curva *curva, float minValue, float maxValue,
						   uint32_t graphColor, int cantidadDePuntos);

//-------------------------------------------------------------------------------------------------------
// funciones: ajustes de la grafica

/**
 * @brief Cambia el tipo de gráfico de la estructura Graph.
 *
 * Esta función permite modificar el tipo de gráfico de una estructura `Graph`.
 * Se puede establecer el tipo de gráfico como `LINEAS` o `PUNTOS`. Esta acción
 * podría influir en el modo de visualización del gráfico, la representación de
 * las curvas, o en otros aspectos específicos del gráfico según el tipo elegido.
 *
 * @param[in,out] graph Puntero a la estructura de gráfico cuyo tipo será modificado.
 * @param[in] tipo Tipo de gráfico a asignar. Puede ser uno de los valores del enumerado `tipoGrafica`,
 *                como `LINEAS` o `PUNTOS`.
 */
void Graph_cambiarTipo(Graph *graph, tipoGrafica tipo);

/**
 * @brief Establece el rango máximo de una gráfica según los valores de una curva.
 *
 * @param graph Puntero a la estructura Graph, que representa la gráfica.
 * @param curva Puntero a la estructura Curva, cuya escala o límites se usan para definir el rango máximo.
 */
void setRangoMaximo(Graph *graph, Curva *curva);

/**
 * @brief Oculta el fondo de la gráfica, haciéndolo invisible en pantalla.
 *
 * @param graph Puntero a la estructura Graph, cuya representación de fondo se ocultará.
 */
void Graph_ocultarFondo(Graph *graph);

/**
 * @brief Muestra el fondo de la gráfica en pantalla.
 *
 * @param graph Puntero a la estructura Graph, cuya representación de fondo se mostrará.
 */
void Graph_mostrarFondo(Graph *graph);

/**
 * @brief Establece las curvas de la gráfica como independientes, permitiendo que se configuren o modifiquen por separado.
 *
 * @param graph Puntero a la estructura Graph, cuyas curvas se harán independientes entre sí.
 */
void Graph_independizarCurvas(Graph *graph);

/**
 * @brief Relaciona las curvas de la gráfica, sincronizándolas o vinculándolas entre sí.
 *
 * @param graph Puntero a la estructura Graph, cuyas curvas se relacionarán para compartir una escala o sincronización.
 */
void Graph_relacionarCurvas(Graph *graph);


//-------------------------------------------------------------------------------------------------------
// Funciones de dibujo de gráficos y curvas

/**
 * @brief Dibuja el gráfico en la pantalla LCD.
 *
 * @param graph Estructura del gráfico a dibujar.
 */
void Graph_drawGraph(Graph *graph);

/**
 * @brief Dibuja una curva de tipo línea en el gráfico.
 *
 * Esta función dibuja la curva representada por líneas conectando puntos consecutivos,
 * utilizando una escala vertical y horizontal específicas.
 * Los valores de la curva son limitados para ajustarse al área del gráfico.
 *
 * @param curva Puntero a la estructura `Curva` que contiene los datos a dibujar.
 * @param xInicial Coordenada X inicial del gráfico en la pantalla.
 * @param yInicial Coordenada Y inicial del gráfico en la pantalla.
 * @param ceroGrafica Valor en Y que representa el nivel cero de la gráfica.
 * @param anchoGraph Ancho del área de dibujo del gráfico en píxeles.
 * @param altoGraph Alto del área de dibujo del gráfico en píxeles.
 * @param escalaVertical Escala en el eje Y para ajustar los valores de la curva.
 * @param escalaCurvas Escala adicional para la curva en el eje Y.
 * @param curvaColor Color de la curva en formato RGB.
 * @param enBlancoYNegro Modo de color: 1 para blanco y negro, 0 para color.
 * @param transparencia Nivel de transparencia (0 a 100).
 */
void Graph_dibujaTipoLineas(Curva *curva, uint16_t xInicial, uint16_t yInicial, float ceroGrafica,
                            int16_t anchoGraph, int16_t altoGraph, float escalaVertical, float escalaCurvas,
                            uint32_t curvaColor, int enBlancoYNegro, int transparencia);

/**
 * @brief Dibuja una curva de tipo puntos en el gráfico.
 *
 * Esta función dibuja cada valor de la curva como un punto individual en el área del gráfico,
 * aplicando una escala vertical y horizontal. Limita la altura de
 * cada punto para mantenerse dentro del área de dibujo del gráfico.
 *
 * @param curva Puntero a la estructura `Curva` que contiene los datos a dibujar.
 * @param xInicial Coordenada X inicial del gráfico en la pantalla.
 * @param yInicial Coordenada Y inicial del gráfico en la pantalla.
 * @param ceroGrafica Valor en Y que representa el nivel cero de la gráfica.
 * @param anchoGraph Ancho del área de dibujo del gráfico en píxeles.
 * @param altoGraph Alto del área de dibujo del gráfico en píxeles.
 * @param escalaVertical Escala en el eje Y para ajustar los valores de la curva.
 * @param escalaCurvas Escala adicional para la curva en el eje Y.
 * @param curvaColor Color de la curva en formato RGB.
 * @param enBlancoYNegro Modo de color: 1 para blanco y negro, 0 para color.
 * @param transparencia Nivel de transparencia (0 a 100).
 */
void Graph_dibujaTipoPuntos(Curva *curva, uint16_t xInicial, uint16_t yInicial, float ceroGrafica,
                            int16_t anchoGraph, int16_t altoGraph, float escalaVertical, float escalaCurvas,
                            uint32_t curvaColor, int enBlancoYNegro, int transparencia);

/**
 * @brief Actualiza y dibuja los puntos de una curva en el gráfico.
 *
 * @param graph Estructura del gráfico asociado.
 * @param curva Estructura de la curva a actualizar y dibujar.
 */
void Graph_atiendeCurva(Graph *graph, Curva *curva);

/**
 * @brief Agrega un nuevo punto a la curva y desplaza los puntos previos.
 *
 * @param graph Estructura del gráfico asociado.
 * @param curva Estructura de la curva donde se agregará el nuevo punto.
 * @param nuevoPunto Valor del nuevo punto a agregar.
 */
void Graph_agregarNuevoPunto(Graph *graph, Curva *curva, float nuevoPunto);

/**
 * @brief Desplaza verticalmente una curva en el gráfico.
 *
 * Esta función desplaza verticalmente los valores de la curva en una cantidad especificada.
 * Es útil para ajustar la posición de la curva en el eje vertical del gráfico.
 *
 * @param curva Puntero a la estructura `Curva` que contiene los datos de la curva a desplazar.
 * @param valorDesplazamiento Cantidad de desplazamiento vertical que se aplicará a cada punto de la curva.
 *                            Un valor positivo desplaza la curva hacia arriba, mientras que un valor negativo
 *                            desplaza la curva hacia abajo.
 */
void Graph_desplazarCurvaVertical(Curva *curva, float valorDesplazamiento);


//-------------------------------------------------------------------------------------------------------
// funciones: personalizacion de la grafica

/**
 * @brief Configura el título de un gráfico.
 *
 * @param[in,out] graph         Puntero a la estructura del gráfico donde se establecerá el título.
 * @param[in] titulo            Cadena de texto que contiene el título a mostrar en el gráfico.
 * @param[in] pJuegoCaracteres  Puntero al juego de caracteres que define el estilo del título.
 * @param[in] titleColor        Color del título en formato RGB.
 * @param[in] separacion        Espacio entre caracteres del título.
 * @param[in] alineacion        Alineación del título (0 = izquierda, 1 = centrado, 2 = derecha).
 *
 * Configura las propiedades y el contenido del título para un gráfico. Esta función no dibuja el
 * título, solo establece los parámetros que serán utilizados posteriormente.
 */
void Graph_setTittle(Graph *graph, const char *titulo, const LCD_JuegoCaracteresAlpha *pJuegoCaracteres, uint32_t titleColor, uint16_t separacion, int alineacion);

/**
 * @brief Configura los nombres de los ejes X e Y de un gráfico.
 *
 * @param[in,out] graph          Puntero a la estructura del gráfico donde se definirán los nombres de los ejes.
 * @param[in] xAxisName          Cadena de texto que contiene el nombre del eje X.
 * @param[in] yAxisName          Cadena de texto que contiene el nombre del eje Y.
 * @param[in] pJuegoCaracteres   Puntero al juego de caracteres para los nombres de los ejes.
 * @param[in] axisNameColor      Color de los nombres de los ejes en formato RGB.
 * @param[in] separacion         Espacio entre caracteres en los nombres de los ejes.
 * @param[in] alineacion         Alineación de los nombres de los ejes (0 = izquierda, 1 = centrado, 2 = derecha).
 *
 * Configura los parámetros para los nombres de los ejes X e Y. Esta función no dibuja los nombres
 * de los ejes, solo configura los valores que se usarán para mostrarlos posteriormente.
 */
void Graph_setAxisNames(Graph *graph, const char *xAxisName, const char *yAxisName, const LCD_JuegoCaracteresAlpha *pJuegoCaracteres, uint32_t axisNameColor, uint16_t separacion, int alineacion);


/**
 * @brief Configura los parámetros de la cuadrícula en el gráfico.
 *
 * Esta función permite definir el color, el modo de visualización y la transparencia
 * de la cuadrícula dentro del gráfico, personalizando así su apariencia y visibilidad.
 *
 * @param graph Puntero a la estructura `Graph` en la cual se aplicará la configuración de la cuadrícula.
 * @param gridColor Color de la cuadrícula en formato RGB (32 bits). Define el color de las líneas de la cuadrícula.
 * @param enBlancoYNegro Modo de visualización en blanco y negro:
 *                       - 1 para visualizar la cuadrícula en blanco y negro.
 *                       - 0 para visualizarla en color (usando `gridColor`).
 * @param transparencia Nivel de transparencia de la cuadrícula, que varía de 0 a 100:
 *                      - 0 indica completamente transparente.
 *                      - 100 indica completamente opaco.
 */
void Graph_setGrid(Graph *graph, uint32_t gridColor, int enBlancoYNegro, int transparencia);


/**
 * @brief Escribe un texto en una posición específica dentro de un gráfico.
 *
 * @param[in] graph             Puntero a la estructura del gráfico donde se escribirá el texto.
 * @param[in] xInicial          Coordenada X inicial para la posición del texto en el gráfico.
 * @param[in] yInicial          Coordenada Y inicial para la posición del texto en el gráfico.
 * @param[in] cadena            Cadena de texto a escribir en el gráfico.
 * @param[in] anchoTexto        Ancho total del texto en píxeles.
 * @param[in] colorTexto        Color del texto en formato RGB.
 * @param[in] pJuegoCaracteres  Puntero al juego de caracteres para el estilo del texto.
 * @param[in] separacion        Espacio entre caracteres del texto.
 * @param[in] alineacion        Alineación del texto (0 = izquierda, 1 = centrado, 2 = derecha).
 *
 * Esta función permite escribir una cadena de texto en una posición específica dentro del área del gráfico,
 * aplicando el estilo, color, y alineación configurados.
 */
void Graph_escribeTexto(Graph *graph, int xInicial, int yInicial, const char *cadena, int anchoTexto, int32_t colorTexto, LCD_JuegoCaracteresAlpha *pJuegoCaracteres, uint16_t separacion,
                        int alineacion);

/**
 * @brief Escribe un texto rotado en una posición específica dentro de un gráfico.
 *
 * @param[in] graph             Puntero a la estructura del gráfico donde se escribirá el texto rotado.
 * @param[in] xInicial          Coordenada X inicial para la posición del texto en el gráfico.
 * @param[in] yInicial          Coordenada Y inicial para la posición del texto en el gráfico.
 * @param[in] cadena            Cadena de texto a escribir en el gráfico.
 * @param[in] anchoTexto        Ancho total del texto en píxeles.
 * @param[in] colorTexto        Color del texto en formato RGB.
 * @param[in] pJuegoCaracteres  Puntero al juego de caracteres para el estilo del texto.
 * @param[in] separacion        Espacio entre caracteres del texto.
 * @param[in] alineacion        Alineación del texto (0 = izquierda, 1 = centrado, 2 = derecha).
 *
 * Escribe un texto rotado dentro del área de un gráfico, utilizando los parámetros de posición,
 * color, y alineación proporcionados.
 */
void Graph_escribeTextoRotado(Graph *graph, int xInicial, int yInicial, const char *cadena, int anchoTexto, int32_t colorTexto, LCD_JuegoCaracteresAlpha *pJuegoCaracteres, uint16_t separacion,
                              int alineacion);

/**
 * @brief Dibuja el título del gráfico si ha sido configurado.
 *
 * @param[in] graph Puntero a la estructura del gráfico cuyo título será dibujado.
 *
 * Esta función dibuja el título previamente configurado en el gráfico mediante `setTittle()`.
 * Si no se ha configurado un título, la función no realiza ninguna operación.
 */
void Graph_escribeTitulo(Graph *graph);

/**
 * @brief Dibuja los nombres de los ejes X e Y si han sido configurados.
 *
 * @param[in] graph Puntero a la estructura del gráfico cuyos nombres de ejes serán dibujados.
 *
 * Esta función dibuja los nombres de los ejes X e Y, configurados mediante `setAxisNames()`.
 * Si los nombres no han sido configurados, la función no realiza ninguna operación.
 */
void Graph_escribeAxisNames(Graph *graph);


/**
 * @brief Calcula el máximo común divisor (MCD) entre dos números enteros.
 *
 * @param a Primer número entero.
 * @param b Segundo número entero.
 * @return int Máximo común divisor entre 'a' y 'b'.
 */
int MCD(int a, int b);

/**
 * @brief Calcula el máximo común divisor (MCD) entre el ancho y el alto de una gráfica.
 *
 * @param graph Puntero a la estructura Graph, que representa la gráfica cuyas dimensiones se usan para el cálculo.
 * @return int Máximo común divisor entre el ancho y el alto de la gráfica.
 */
int Graph_calculaMCD(Graph *graph);

/**
 * @brief Dibuja una cuadrícula en la gráfica utilizando el MCD entre el ancho y el alto para determinar el tamaño de las celdas.
 *
 * @param graph Puntero a la estructura Graph, sobre la cual se dibujará la cuadrícula.
 */
void Graph_dibujaCuadricula(Graph *graph);


#endif /* Graficas_H_ */
