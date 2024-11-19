#include <cmsis_os2.h> // para osDelay()
#include <graph.h>
#include <math.h>
#include <stdio.h>  // Para sprintf()
#include <stdlib.h> // para abs()
#include <string.h>

// importamos las funciones para el manejo de la pantalla
#include "graph.h"
#include "interfazLCD.h"
#include "pantallaLCD.h"

#include "ColoresHexa.h"     // importamos la paleta de colores en formato hexadecimal ARGB
#include <Imagenes.h>        // importamos las imagenes
#include <JuegoCaracteres.h> // importamos las tipografias

//------------------------------------------------------------------------------------------------------------
// Getters y setters

// Getters Graph
uint16_t Graph_getXInicial(const Graph *graph) { return graph->xInicial; }
uint16_t Graph_getYInicial(const Graph *graph) { return graph->yInicial; }
uint16_t Graph_getAncho(const Graph *graph) { return graph->ancho; }
uint16_t Graph_getAlto(const Graph *graph) { return graph->alto; }
uint32_t Graph_getBackgroundColor(const Graph *graph) { return graph->backgroundColor; }
uint32_t Graph_getAxisColor(const Graph *graph) { return graph->axisColor; }
int Graph_getGraphPadding(const Graph *graph) { return graph->graphPadding; }
int Graph_getEnBlancoYNegro(const Graph *graph) { return graph->enBlancoYNegro; }
int Graph_getTransparencia(const Graph *graph) { return graph->transparencia; }
int Graph_getEstaDibujada(const Graph *graph) { return graph->estaDibujada; }

// Setters Graph
void Graph_setXInicial(Graph *graph, uint16_t xInicial) { graph->xInicial = xInicial; }
void Graph_setYInicial(Graph *graph, uint16_t yInicial) { graph->yInicial = yInicial; }
void Graph_setAncho(Graph *graph, uint16_t ancho) { graph->ancho = ancho; }
void Graph_setAlto(Graph *graph, uint16_t alto) { graph->alto = alto; }
void Graph_setBackgroundColor(Graph *graph, uint32_t backgroundColor) { graph->backgroundColor = backgroundColor; }
void Graph_setAxisColor(Graph *graph, uint32_t axisColor) { graph->axisColor = axisColor; }
void Graph_setGraphPadding(Graph *graph, int graphPadding) { graph->graphPadding = graphPadding; }
void Graph_setEnBlancoYNegro(Graph *graph, int enBlancoYNegro) { graph->enBlancoYNegro = enBlancoYNegro; }
void Graph_setTransparencia(Graph *graph, int transparencia) { graph->transparencia = transparencia; }
void Graph_setEstaDibujada(Graph *graph, int estaDibujada) { graph->estaDibujada = estaDibujada; }

// Getters Curva
uint16_t Curva_getMaxValue(const Curva *curva) { return curva->maxValue; }
uint16_t Curva_getMinValue(const Curva *curva) { return curva->minValue; }
uint32_t Curva_getCurvaColor(const Curva *curva) { return curva->curvaColor; }
float Curva_getY(const Curva *curva, int index) { return (index >= 0 && index < alto_lcd()) ? curva->y[index] : 0.0f; }
int Curva_getCantidadDePuntos(const Curva *curva) { return curva->cantidadDePuntos; }

// Setters Curva
void Curva_setMaxValue(Curva *curva, uint16_t maxValue) { curva->maxValue = maxValue; }
void Curva_setMinValue(Curva *curva, uint16_t minValue) { curva->minValue = minValue; }
void Curva_setCurvaColor(Curva *curva, uint32_t curvaColor) { curva->curvaColor = curvaColor; }
void Curva_setY(Curva *curva, int index, float value) { if (index >= 0 && index < alto_lcd()) curva->y[index] = value; }
void Curva_setCantidadDePuntos(Curva *curva, int cantidadDePuntos) { curva->cantidadDePuntos = cantidadDePuntos; }


//------------------------------------------------------------------------------------------------------------
// funciones: inicializacion de los componenetes

void Graph_inicializaLineGraph(Graph *graph, int16_t xInicial, uint16_t yInicial, uint16_t ancho, uint16_t alto,
						 uint32_t backgroundColor, uint32_t axisColor, int graphPadding, int enBlancoYNegro,
						 int transparencia) {

    graph->xInicial = xInicial;
    graph->yInicial = yInicial;
    graph->ancho = ancho;
    graph->alto = alto;
    graph->backgroundColor = backgroundColor;
    graph->axisColor = axisColor;
    graph->graphPadding = graphPadding;
    graph->enBlancoYNegro = enBlancoYNegro;
    graph->transparencia = transparencia;

    graph->tipo = LINEAS;
    graph->estaDibujada = 0;
    graph->setTitulo = 0;
    graph->setAxisNames = 0;
    graph->setGrid = 0;
    graph->divisionesVerticales = 0;
    graph->rangoMaximoCurva = 0;
    graph->setFondoTranspparente = 0;
    graph->setCurvasIndependientes = 0;

} // Graph_inicializaLineGraph

void Graph_InicializaCurva(Curva *curva, float minValue, float maxValue, uint32_t curvaColor, int cantidadDePuntos) {
    curva->minValue = minValue;
    curva->maxValue = maxValue;
    curva->curvaColor = curvaColor;
    curva->cantidadDePuntos = cantidadDePuntos;

    curva->rangoCurva = maxValue - minValue;
    curva->valorDesplazamientoVertical = 0;

    for (int i = 0; i < 320; i++)
        curva->y[i] = 0; // inicializamos la grafia a cero

} // Graph_InicializaCurva

//-------------------------------------------------------------------------------------------------------
// funciones: ajustes de la grafica

void Graph_cambiarTipo(Graph *graph, tipoGrafica tipo){
    graph->tipo = tipo; // Asigna el nuevo tipo de gráfico
} // Graph_cambiarTipo

void setRangoMaximo(Graph *graph, Curva *curva){
	//obtenemos el rango de la curva dada y si esta tiene el rango mas grande
	//ajustamos las demas curvas para representarlas correctamente en relacion
	//las unas de las otras
	if (curva->rangoCurva > graph->rangoMaximoCurva) graph->rangoMaximoCurva = curva->rangoCurva;
} // setRangoMaximo

void Graph_independizarCurvas(Graph *graph){
	graph->setCurvasIndependientes = 1;
} // Graph_independizarCurvas

void Graph_relacionarCurvas(Graph *graph){
	graph->setCurvasIndependientes = 0;
} // Graph_relacionarCurvas

void Graph_ocultarFondo(Graph *graph){
	graph->setFondoTranspparente = 1;
} // Graph_ocultarFondo

void Graph_mostrarFondo(Graph *graph){
	graph->setFondoTranspparente = 0;
} // Graph_mostrarFondo

//-------------------------------------------------------------------------------------------------------
// funciones: Dibujamos la grafica y las curvas

void Graph_drawGraph(Graph *graph) {

	// extraemos los datos de la grafica
    int16_t xInicial = graph->xInicial;
    int16_t yInicial = graph->yInicial;
    int16_t ancho = graph->ancho;
    int16_t alto = graph->alto;

    int setFondoTransparente = graph->setFondoTranspparente;
    int enBlancoYNegro = graph->enBlancoYNegro;
    int transparencia = graph->transparencia;

    int padding = graph->graphPadding;
    uint32_t backgroundColor = graph->backgroundColor;
    uint32_t axisColor = graph->axisColor;

    if (setFondoTransparente == 1) backgroundColor &= 0x00000000; // ponemos el fondo transparente

    // dibujamos la grafica (no se dibujan las curvas)
    LCD_dibujaRectanguloRelleno(xInicial, yInicial - padding, ancho, alto + 2 * padding, backgroundColor, enBlancoYNegro, transparencia);
    LCD_dibujaLinea(xInicial, yInicial + alto / 2, xInicial + ancho, yInicial + alto / 2, axisColor, enBlancoYNegro, transparencia);
    LCD_dibujaLinea(xInicial, yInicial - padding, xInicial, yInicial + alto + padding, axisColor, enBlancoYNegro, transparencia);

    if (graph->setGrid == 1) // dibujamos la cuadricula
    	Graph_dibujaCuadricula(graph);

    if (graph->setTitulo == 1) // dibujamos el titulo de la grafica
        Graph_escribeTitulo(graph);

    if (graph->setAxisNames == 1) // dibujamos los nombres de los ejes de la grafica
        Graph_escribeAxisNames(graph);

    graph->estaDibujada = 1; // pperimitimos que se dibujen las curvas

} // Graph_drawGraph

void Graph_atiendeCurva(Graph *graph, Curva *curva) {

	// si queremos dibuja esta curva debemos ver si el rango de esta curva afecta a la
	// representacion de la demas curvas
	setRangoMaximo(graph, curva);

	// extraemos los datos de la curva
    int16_t xInicial = graph->xInicial;
    int16_t yInicial = graph->yInicial;
    int16_t anchoGraph = graph->ancho;
    int16_t altoGraph = graph->alto;

    int enBlancoYNegro = graph->enBlancoYNegro;
    int transparencia = graph->transparencia;

    int estaDibujada = graph->estaDibujada;

    uint32_t curvaColor = curva->curvaColor;

    if (estaDibujada == 0) return; // comprobamos si tenemos permiso para dibuja la curvas

    float minValue = (curva->minValue);
    float maxValue = (curva->maxValue);

    // factores de escala
    float escalaVertical = (altoGraph / 2) / (maxValue - minValue);
    float escalaCurvas;

    if (graph->setCurvasIndependientes){ // si no hay que tener en cuenta las relaciones entre todas las curvas
    	escalaCurvas = 1.0;
    } else{
		escalaCurvas = curva->rangoCurva / graph->rangoMaximoCurva;
    }

    // cero de la grafica. punto medio en el eje Y
    float ceroGrafica = yInicial + altoGraph / 2;

    switch (graph->tipo){ // como debemos dibujar la grafica
		case LINEAS:
			Graph_dibujaTipoLineas(curva, xInicial, yInicial, ceroGrafica,
								   anchoGraph, altoGraph, escalaVertical, escalaCurvas,
								   curvaColor, enBlancoYNegro, transparencia);
			break;
		case PUNTOS:
			Graph_dibujaTipoPuntos(curva, xInicial, yInicial, ceroGrafica,
								   anchoGraph, altoGraph, escalaVertical, escalaCurvas,
								   curvaColor, enBlancoYNegro, transparencia);
			break;
    } // end switch

} // Graph_atiendeCurva



void Graph_dibujaTipoLineas(Curva *curva, uint16_t xInicial, uint16_t yInicial, float ceroGrafica,
		int16_t anchoGraph, int16_t altoGraph, float escalaVertical, float escalaCurvas,
		uint32_t curvaColor, int enBlancoYNegro, int transparencia){

	float valorDesplazamientoVertical = curva->valorDesplazamientoVertical;
    LCD_dibujaPunto(xInicial, ceroGrafica + (curva->y[0] - valorDesplazamientoVertical) * escalaVertical * escalaCurvas,
    				curvaColor, enBlancoYNegro, transparencia);

    for (int i = 1; i < anchoGraph; i++) {

        float valorPrevio = ((curva->y[i - 1] - valorDesplazamientoVertical) * escalaVertical * escalaCurvas) ;
        float valorActual = ((curva->y[i] - valorDesplazamientoVertical) * escalaVertical * escalaCurvas);

        // calculamos las coordenadas de los puntos a dibujar
        int x1 = xInicial + (int)(i - 1);
        int y1 = ceroGrafica + (int)valorPrevio;

        int x2 = xInicial + (int)i;
        int y2 = ceroGrafica + (int)valorActual;

        // saturamos la salida de la grafica si los valores superan los limites
        y1 = (y1 > altoGraph + yInicial) ? altoGraph + yInicial : ((y1 < yInicial) ? yInicial : y1);
        y2 = (y2 > altoGraph + yInicial) ? altoGraph + yInicial : ((y2 < yInicial) ? yInicial : y2);

        if (y1 > LCD_alto() || y2 > LCD_alto() || y1 < 0 || y2 < 0) break;
        // dibujamos la linea
        LCD_dibujaLinea(x1, y1, x2, y2, curvaColor, enBlancoYNegro, transparencia);
    }

} // Graph_dibujaTipoLineas

void Graph_dibujaTipoPuntos(Curva *curva, uint16_t xInicial, uint16_t yInicial, float ceroGrafica,
		int16_t anchoGraph, int16_t altoGraph, float escalaVertical, float escalaCurvas,
		uint32_t curvaColor, int enBlancoYNegro, int transparencia){

	float valorDesplazamientoVertical = curva->valorDesplazamientoVertical;

	for (int i = 0; i < anchoGraph; i++) {
        float valorActual = ((curva->y[i] - valorDesplazamientoVertical) * escalaVertical * escalaCurvas);
        int x = xInicial + (int)i;
        int y = ceroGrafica + (int)valorActual;
        y = (y > altoGraph + yInicial) ? altoGraph + yInicial : ((y < yInicial) ? yInicial : y);

        if (y > LCD_alto() || y < 0) break;
	    LCD_dibujaPunto(x, y, curvaColor, enBlancoYNegro, transparencia);
	}
} // Graph_dibujaTipoPuntos

void Graph_agregarNuevoPunto(Graph *graph, Curva *curva, float nuevoPunto) {

    int16_t ancho = graph->ancho;

    for (int i = 0; i < ancho - 1; i++)
        curva->y[i] = curva->y[i + 1];

    curva->y[ancho - 1] = nuevoPunto;

} // Graph_agregarNuevoPunto

void Graph_desplazarCurvaVertical(Curva *curva, float valorDesplazamiento){

	curva->valorDesplazamientoVertical = valorDesplazamiento;
} // Graph_desplazarCurvaVertical

//-------------------------------------------------------------------------------------------------------
// funciones: personalizacion de la grafica

void Graph_setTittle(Graph *graph, const char *titulo, const LCD_JuegoCaracteresAlpha *pJuegoCaracteres, uint32_t titleColor, uint16_t separacion, int alineacion) {

    graph->titleColor = titleColor;
    graph->separacionCaracteresTitulo = separacion;
    graph->pJuegoCaracteresTittle = pJuegoCaracteres;
    graph->titulo = titulo;
    graph->alineacion = alineacion;

    graph->anchoTitulo = LCD_anchoCadenaCaracteresAlpha(titulo, pJuegoCaracteres, separacion);

    graph->setTitulo = 1;

} // Graph_setTittle

void Graph_setAxisNames(Graph *graph, const char *xAxisName, const char *yAxisName, const LCD_JuegoCaracteresAlpha *pJuegoCaracteres, uint32_t axisNameColor, uint16_t separacion, int alineacion) {

    graph->axisNameColor = axisNameColor;
    graph->separacionCaracteresAxis = separacion;
    graph->pJuegoCaracteresAxis = pJuegoCaracteres;
    graph->xAxisName = xAxisName;
    graph->yAxisName = yAxisName;
    graph->alineacion = alineacion;

    graph->anchoXAxis = LCD_anchoCadenaCaracteresAlpha(xAxisName, pJuegoCaracteres, separacion);
    graph->anchoYAxis = LCD_anchoCadenaCaracteresAlpha(yAxisName, pJuegoCaracteres, separacion);

    graph->setAxisNames = 1;

} //Graph_setAxisNames

void Graph_setGrid(Graph *graph, uint32_t gridColor, int enBlancoYNegro, int transparencia){

	graph->gridColor = gridColor;
	graph->gridEnBlancoYNegro = enBlancoYNegro;
	graph->gridTransparencia = transparencia;

	graph->setGrid = 1;

} // Graph_setAxisNames

void Graph_escribeTexto(Graph *graph, int xInicial, int yInicial, const char *cadena, int anchoTexto, int32_t colorTexto, LCD_JuegoCaracteresAlpha *pJuegoCaracteres, uint16_t separacion,
                        int alineacion) {

    // Dibuja la cadena de caracteres apuntada por 'cadena' a partir de las coordenadas 'x', 'y'
    // utilizando el juego de caracteres apuntado por 'pJuegoCaracteres'.
    // Si 'enBlancoYNegro' es un booleano cierto, convierte el color a gris.
    // 'transparencia' es un factor entre 0 y 100 para aplicar un nivel de transparencia al color,
    // desde completamente transparente (valor 0) a sin transparencia (valor 100).
    //
    // El texto se ajusta a la orientación de la pantalla. Si el texto es muy largo, se realizan
    // saltos de línea cuando se supera el límite horizontal (LimiteH).
    // Si el texto supera el espacio vertical permitido (LimiteV), se deja de escribir.

    int16_t anchoGraph = graph->ancho;          // Ancho del gráfico donde se dibuja el texto.
    uint8_t Alto = pJuegoCaracteres->alto;      // Altura fija de cada carácter en el juego de caracteres.

    int enBlancoYNegro = graph->enBlancoYNegro; // Indicador si el texto debe ser dibujado en blanco y negro.
    int transparencia = graph->transparencia;   // Nivel de transparencia del texto.

    int longitud = strlen(cadena);              // Longitud de la cadena de texto.

    // Calcular alineamiento horizontal del texto en función de la posición deseada.
    int16_t alineamiento = 0;
    if (alineacion == 1) {                       // Centro del gráfico
        alineamiento = (anchoGraph - anchoTexto) / 2;
    } else if (alineacion == 2) {                // Derecha del gráfico
        alineamiento = (anchoGraph - anchoTexto);
    }

    int16_t xCadena = xInicial + alineamiento;   // Coordenada inicial 'x' de la cadena, ajustada por alineación.
    int16_t yCadena = yInicial;                  // Coordenada inicial 'y' de la cadena.

    uint8_t AnchoIncremental = 0;                // Acumulador del ancho de los caracteres ya dibujados.
    uint16_t AchoCaracter;                       // Ancho de cada carácter individual.

    int LimiteH, LimiteV;
    LimiteH = xCadena + anchoGraph;              // Límite horizontal (ancho de la pantalla o gráfico).
    LimiteV = LCD_alto();                        // Límite vertical (altura de la pantalla).

    if (alineamiento < 0)
        LimiteH -= 2 * alineamiento - 1;         // Ajuste en caso de alineación negativa.

    int PosX, PosY;                              // Posiciones 'x' y 'y' de cada carácter.

    // Variables para gestionar los saltos de línea.
    int saltar = 0;                              // Contador de saltos de línea.
    int SaltoLinea = Alto + separacion;          // Espacio vertical entre líneas (altura + separación).

    // Iniciamos el dibujo de la cadena de caracteres.
    int PosXActual = 0;                          // Posición horizontal actual del carácter en la línea.
    for (int i = 0; i < longitud + saltar; i++) {
        AchoCaracter = LCD_anchoCaracterAlpha(cadena[i - saltar], pJuegoCaracteres); // Ancho del carácter actual.

        // Definir las coordenadas de cada carácter en la cadena de texto.
        PosX = xCadena + separacion * PosXActual + AnchoIncremental;
        PosY = yCadena + saltar * SaltoLinea;

        if (PosX + AchoCaracter <= LimiteH && PosY + Alto <= LimiteV) { // Verificar que el carácter cabe en la pantalla.
            LCD_dibujaCaracterAlpha(PosX, PosY, cadena[i - saltar], colorTexto, pJuegoCaracteres, enBlancoYNegro, transparencia);

            // Avanzamos al siguiente carácter en la línea actual.
            PosXActual++;
            AnchoIncremental += AchoCaracter;
        } else {
            saltar++;             // Si supera el ancho de pantalla, hacemos un salto de línea.

            // Reiniciar variables para escribir la nueva línea con el mismo alineamiento horizontal.
            PosXActual = 0;
            AnchoIncremental = 0;
            AchoCaracter = 0;
        }
    }
} // Fin de Graph_escribeTexto

void Graph_escribeTextoRotado(Graph *graph, int xInicial, int yInicial, const char *cadena, int anchoTexto, int32_t colorTexto, LCD_JuegoCaracteresAlpha *pJuegoCaracteres, uint16_t separacion,
                              int alineacion) {

    // Ajusta la cadena de texto para que se dibuje verticalmente, de abajo hacia arriba,
    // empezando en las coordenadas (xInicial, yInicial).

    int16_t altoGraph = graph->alto;
    int padding = graph->graphPadding;
    int enBlancoYNegro = graph->enBlancoYNegro;
    int transparencia = graph->transparencia;
    int longitud = strlen(cadena);

    // Alineación vertical del texto
    uint16_t alineamiento = 0;
    if (alineacion == 1) { // Centrado en el gráfico
        alineamiento = (altoGraph - anchoTexto) / 2;
    } else if (alineacion == 2) { // A la arriba del gráfico
        alineamiento = (altoGraph - anchoTexto);
    }

    int16_t xCadena = xInicial;
    int16_t yCadena = yInicial - alineamiento - padding;

    uint8_t altoCaracter = pJuegoCaracteres->alto;
    uint16_t anchoCaracter;

    int limiteH = xCadena + altoGraph; // Límite horizontal
    int limiteV = 0;                   // Límite vertical (dibujado desde abajo hacia arriba)

    int posX = xCadena;
    int posY = yCadena;

    // Comienza a dibujar la cadena de caracteres en sentido vertical ascendente
    for (int i = 0; i < longitud; i++) {
        anchoCaracter = LCD_anchoCaracterAlpha(cadena[i], pJuegoCaracteres);

        // Coordenadas de cada carácter en sentido vertical ascendente

        // Verifica límites de pantalla antes de dibujar
        if (posX + anchoCaracter <= limiteH && posY - altoCaracter >= limiteV) {
            LCD_dibujaCaracterAlphaRotado(posX, posY, cadena[i], colorTexto, pJuegoCaracteres, enBlancoYNegro, transparencia);
        } else {
            break; // Detenemos si se supera el límite vertical
        }

        posY -= anchoCaracter + separacion;
    }
} // Graph_escribeTextoRotado

void Graph_escribeTitulo(Graph *graph) {

    LCD_JuegoCaracteresAlpha *pJuegoCaracteresTittle = graph->pJuegoCaracteresTittle;

    int y = graph->yInicial - (int)pJuegoCaracteresTittle->alto - graph->graphPadding;
    int x = graph->xInicial;
    const char *titulo = graph->titulo;
    int32_t titleColor = graph->titleColor;
    uint16_t separacion = graph->separacionCaracteresTitulo;
    int alineacion = graph->alineacion;
    int anchoTexto = graph->anchoTitulo;

    Graph_escribeTexto(graph, x, y, titulo, anchoTexto, titleColor, pJuegoCaracteresTittle, separacion, alineacion);

} // Graph_escribeTitulo

void Graph_escribeAxisNames(Graph *graph) {

    LCD_JuegoCaracteresAlpha *pJuegoCaracteresAxis = graph->pJuegoCaracteresAxis;

    int x = graph->xInicial;
    int y = graph->yInicial + graph->alto + graph->graphPadding;

    const char *xName = graph->xAxisName;
    const char *yName = graph->yAxisName;

    int32_t axisNameColor = graph->axisNameColor;
    uint16_t separacion = graph->separacionCaracteresAxis;
    int alineacion = graph->alineacion;

    int anchoXAxis = graph->anchoXAxis;
    int anchoYAxis = graph->anchoYAxis;

    Graph_escribeTexto(graph, x, y, xName, anchoXAxis, axisNameColor, pJuegoCaracteresAxis, separacion, alineacion);
    Graph_escribeTextoRotado(graph, x, y, yName, anchoYAxis, axisNameColor, pJuegoCaracteresAxis, separacion, alineacion);
}

int MCD(int a, int b) {
    // Algoritmo de Euclides para calcular el máximo común divisor (MCD)
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;

} // Graph_escribeAxisNames

int Graph_calculaMCD(Graph *graph) {
    // Calcula el MCD entre el ancho y el alto de la gráfica
    int ancho = graph->ancho;
    int alto = graph->alto;

    return MCD(ancho, alto);

} // Graph_calculaMCD

void Graph_dibujaCuadricula(Graph *graph) {

    // Calcula el MCD entre el ancho y el alto de la gráfica para usarlo como tamaño de celda
    int tamCelda = Graph_calculaMCD(graph);

    int ancho = graph->ancho;
    int alto = graph->alto;

    graph->divisionesVerticales = alto / tamCelda;

    int xGrafica = graph->xInicial;
    int yGrafica = graph->yInicial;

    int padding = graph->graphPadding;

    uint32_t colorLinea = graph->gridColor;
    int enBlancoYNegro = graph->gridEnBlancoYNegro;
    int transparencia = graph->gridTransparencia;

    // Dibuja las líneas verticales de la cuadrícula
    for (int x = tamCelda; x < ancho; x += tamCelda) {
        // Dibuja una línea vertical desde (x, 0) hasta (x, alto)

    	 LCD_dibujaLinea(xGrafica +  x, yGrafica - padding, xGrafica + x, yGrafica + alto + padding, colorLinea, enBlancoYNegro, transparencia);
    }

    // Dibuja las líneas horizontales de la cuadrícula
    for (int y = 0; y < alto + padding; y += tamCelda) {
        // Dibuja una línea horizontal desde (0, y) hasta (ancho, y)

        LCD_dibujaLinea(xGrafica, yGrafica + y, xGrafica + ancho, yGrafica + y, colorLinea, enBlancoYNegro, transparencia);
    }

} // Graph_dibujaCuadricula



