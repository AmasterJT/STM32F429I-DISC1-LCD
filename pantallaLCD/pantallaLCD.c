
//12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567

#include <stdio.h>  // Para sprintf()
#include <stdlib.h>  // para abs()
#include <cmsis_os2.h>  // para osDelay()
#include <pantallaLCD.h>


// -----------------------------------------------------------------------------------------
// Variables estáticas accesibles sólo desde el código de este archivo. 
// Contienen información sobre la pantalla LCD.

static uint16_t lcd_ancho, lcd_alto;  // Ancho y alto de la pantalla en puntos
static uint32_t* lcd_buffers[2];  // Direcciones de memoria de los frame buffers
static int lcd_bufferOculto;  // Frame buffer oculto donde se dibuja, puede ser 0 o 1
static uint16_t lcd_xPulsacion, lcd_yPulsacion;  // Coordenadas del punto de la última pulsación
static int lcd_pulsando;  // Buleano cierto si la pantalla se está pulsando
static int horizontal;  // Buleano cierto si la pantalla se utiliza en orientación horizontal


static int invertida;

// -----------------------------------------------------------------------------------------

void LCD_inicializa2Buffers(int enHorizontal, int estaInvertida) {

	horizontal = enHorizontal;  // Guarda el parámetro
	invertida = estaInvertida;

    BSP_LCD_Init();
    BSP_LCD_Init();  // Inicializa la pantalla LCD

    if (horizontal) {
    	lcd_ancho = BSP_LCD_GetYSize();  // Obtiene el ancho de la pantalla
    	lcd_alto = BSP_LCD_GetXSize();  // Obtiene el alto de la pantalla
    } else {
        lcd_ancho = BSP_LCD_GetXSize();  // Obtiene el ancho de la pantalla
        lcd_alto = BSP_LCD_GetYSize();  // Obtiene el alto de la pantalla
    }
    lcd_buffers[0] = (uint32_t*)LCD_FRAME_BUFFER_LAYER_0;  // Dirección del frame buffer número 0
    lcd_buffers[1] = (uint32_t*)LCD_FRAME_BUFFER_LAYER_1;  // Dirección del frame buffer número 1

    lcd_pulsando = 0;  // Inicialmente la pantalla no se está pulsando

    BSP_LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER_LAYER_0);  // Inicializa frame buffer 0
    BSP_LCD_LayerDefaultInit(1, LCD_FRAME_BUFFER_LAYER_1);  // Inicializa frame buffer 1

    lcd_bufferOculto = 0;  // Selecciona el frame buffer 0 para dibujar

    BSP_TS_Init(lcd_ancho, lcd_alto);
    // Indica a la interfaz táctil el ancho y alto de la pantalla en puntos
}

// -----------------------------------------------------------------------------------------

uint16_t LCD_ancho() {
    // Devuelve el ancho de la pantalla en puntos

    return lcd_ancho;
}

// -----------------------------------------------------------------------------------------

uint16_t LCD_alto() {
    // Devuelve el alto de la pantalla en puntos

    return lcd_alto;
}

// -----------------------------------------------------------------------------------------

int LCD_pulsando() {
	// Devuelve un buleano que indica si se está pulsando la pantalla

	return lcd_pulsando;
}

// -----------------------------------------------------------------------------------------

void LCD_resetPulsacion() {
	lcd_pulsando = 0;  // Indica que no está pulsada para que no se traten pulsaciones
}

// -----------------------------------------------------------------------------------------

uint16_t LCD_xPulsacion() {
	// Devuelve la coordenada X del punto donde se detectó la última pulsación

	return lcd_xPulsacion;
}

// -----------------------------------------------------------------------------------------

uint16_t LCD_yPulsacion() {
	// Devuelve la coordenada Y del punto donde se detectó la última pulsación

	return lcd_yPulsacion;
}

// -----------------------------------------------------------------------------------------

void LCD_actualizaPulsacion() {
	// Actualiza el estado de pulsación de la pantalla

    TS_StateTypeDef estadoTactil;  // Estructura para datos de interfaz táctil

    BSP_TS_GetState(&estadoTactil);  // Obtiene el estado de la interfaz táctil

    if (estadoTactil.TouchDetected) {  // Si se detectó pulsación
    	if (horizontal) {
    		lcd_xPulsacion = estadoTactil.Y;
    		lcd_yPulsacion = 239 - estadoTactil.X;
    	} else {
            lcd_xPulsacion = estadoTactil.X;  // Recoge coordenada X
            lcd_yPulsacion = 319 - estadoTactil.Y;  // y coordenada Y
    	}
        lcd_pulsando = 1;  // Indica que se pulsó
    } else lcd_pulsando = 0;  // Indica que no se pulsó
}

// -----------------------------------------------------------------------------------------

void LCD_setFondoColor(uint32_t color) {
    // Borra toda la pantalla y establece como fondo el color indicado por parámetro

    LCD_dibujaRectanguloRellenoOpaco(0, 0, lcd_ancho, lcd_alto, color);
    // Pone todos los puntos en el frame buffer oculto al color indicado
}

// -----------------------------------------------------------------------------------------

void LCD_setFondoImagen(const uint8_t * imagen) {
    // Borra toda la pantalla y establece como fondo la imagen de 240x320 puntos indicada por parámetro

    LCD_dibujaImagenOpaca(0, 0, lcd_ancho, lcd_alto, imagen);
    // Dibuja la imagen de fondo
}

// -----------------------------------------------------------------------------------------

void LCD_intercambiaBuffers() {
    // Cambia en qué frame buffer se dibuja y qué frame buffer se visualiza

    while (!(LTDC->CDSR & LTDC_CDSR_VSYNCS));  // Espera a que la GPU finalice sincronismo con pantalla
    if (lcd_bufferOculto == 0) {  // Si se dibujó en el buffer 0
        BSP_LCD_SetLayerVisible(1, DISABLE);  // Buffer 1 invisible
        BSP_LCD_SetLayerVisible(0, ENABLE);  // Buffer 0 visible
        lcd_bufferOculto = 1;  // Buffer donde se va a dibujar
    } else {
        BSP_LCD_SetLayerVisible(0, DISABLE);  // Buffer 0 invisible
        BSP_LCD_SetLayerVisible(1, ENABLE);  // Buffer 1 ivisible
        lcd_bufferOculto = 0;  // Buffer donde se dibuja
    }
}

// -----------------------------------------------------------------------------------------

void LCD_dibujaPunto(uint16_t x, uint16_t y, uint32_t color, int enBlancoYNegro, int transparencia) {
    // Dibuja un punto en coordenadas ('x', 'y') con el color indicado en 'color'
    // Si 'enBlancoYNegro' es un buleano cierto, convierte el color a niveles de gris
    // 'transparencia' es un factor entre 0 y 100 para aplicar un nivel de transparencia al color, desde
    // completamente transparente (valor 0) a transparencia original (valor 100)

	if (x > lcd_ancho || y > lcd_alto || x < 0 || y < 0) return;


    uint8_t * pColor = (uint8_t*) &color;  // Para acceder a los bytes del nuevo color

    if (enBlancoYNegro) {  // Si hay que convertir a niveles de gris ...

        uint8_t gris = ((int)pColor[0] + pColor[1] + pColor[2]) / 3;
        // Obtiene la media de las componentes RGB

        pColor[0] = gris;
        pColor[1] = gris;
        pColor[2] = gris;
        // Guarda la media en las componentes RGB del color
    }

    uint8_t alpha = pColor[3] * transparencia / 100;
    // Canal alpha del nuevo color modificado por el grado de transparencia a aplicar

    uint32_t * p32;
  	if (invertida) {

  		 x = lcd_ancho - x;
		 y = lcd_alto - y;
    }

    if (horizontal)
    	p32 = lcd_buffers[lcd_bufferOculto] + (239 - y) + lcd_alto * x;
    else
    	p32 = lcd_buffers[lcd_bufferOculto] + y * lcd_ancho + x;

    if (alpha > 0 && alpha < 255) {
        // Si el nuevo color es semitransparente, hay que mezclar

        uint8_t * p8 = (uint8_t*)p32;  // Dirección del punto en el frame buffer

        p8[0] = (p8[0] * (0xFF - alpha) + pColor[0] * alpha) / 0xFF;
        p8[1] = (p8[1] * (0xFF - alpha) + pColor[1] * alpha) / 0xFF;
        p8[2] = (p8[2] * (0xFF - alpha) + pColor[2] * alpha) / 0xFF;
        p8[3] = 0xFF;
        // Mezcla los colores

    } else if (alpha == 0xFF) { // El color es opaco y hay que sustituir el color
        pColor[3] = alpha;  // Le pone al nuevo color un alpha de 0xFF
        *p32 = color;
        // Copia el nuevo color en el frame buffer
    }
}


// -----------------------------------------------------------------------------------------

void LCD_dibujaPuntoOpaco(uint16_t x, uint16_t y, uint32_t color) {
// Dibuja un punto sin transparencia con un color opaco en una posición (x,y)

	if ((x > lcd_ancho || y > lcd_alto) || (x<0 || y<0)) return;

  	if (invertida) {

  		 x = lcd_ancho - x;
		 y = lcd_alto - y;
    }
	if (horizontal)
		*(lcd_buffers[lcd_bufferOculto] + (239 - y) + lcd_alto * x) = color;
	else
        *(lcd_buffers[lcd_bufferOculto] + y * lcd_ancho + x) = color;
    // Copia el nuevo color en el frame buffer
}

// -----------------------------------------------------------------------------------------

void LCD_dibujaRectangulo(uint16_t xInicial, uint16_t yInicial, uint16_t ancho, uint16_t alto,
    uint32_t color, int enBlancoYNegro, int transparencia) {
    // Dibuja el contorno de un rectángulo cuya esquina superior izquierda está situada en (xInicial, yInicial) y
    // donde se indica el alto y ancho y el color.
    // Si 'enBlancoYNegro' es un buleano cierto, convierte el color a gris
    // 'transparencia' es un factor entre 0 y 100 para aplicar un nivel de transparencia al color, desde
    // completamente transparente (valor 0) a transparencia original (valor 100)

    for(int x = xInicial; x < xInicial + ancho; x++) { // Recorriendo el ancho del rectángulo
        LCD_dibujaPunto(x, yInicial, color, enBlancoYNegro, transparencia);
        LCD_dibujaPunto(x, yInicial + alto - 1, color, enBlancoYNegro, transparencia);
    }
    for(int y = yInicial; y < yInicial + alto; y++) { // Recorriendo el alto del rectángulo
        LCD_dibujaPunto(xInicial, y, color, enBlancoYNegro, transparencia);
        LCD_dibujaPunto(xInicial + ancho - 1, y, color, enBlancoYNegro, transparencia);
    }
}

// -----------------------------------------------------------------------------------------

void LCD_dibujaRectanguloRelleno(uint16_t xInicial, uint16_t yInicial, uint16_t ancho, uint16_t alto,
								 uint32_t color, int enBlancoYNegro, int transparencia) {
    // Dibuja un rectángulo relleno cuya esquina superior izquierda está situada en (xInicial, yInicial) y
    // donde se indica el alto y ancho el color de sus puntos.
    // Si 'enBlancoYNegro' es un buleano cierto, convierte el color a gris
    // 'transparencia' es un factor entre 0 y 100 para aplicar un nivel de transparencia al color, desde
    // completamente transparente (valor 0) a transparencia original (valor 100)

    for(int x = xInicial; x < xInicial + ancho; x++)  // Recorriendo el lcd_ancho del rectángulo
        for(int y = yInicial; y < yInicial + alto; y++)  // y el alto
            LCD_dibujaPunto(x, y, color, enBlancoYNegro, transparencia);
            // Dibuja cada punto del rectángulo con su color
}

// -----------------------------------------------------------------------------------------

void LCD_dibujaRectanguloRellenoOpaco(uint16_t xInicial, uint16_t yInicial, uint16_t ancho, uint16_t alto,
									 uint32_t color) {
    // Dibuja un rectángulo relleno de un color opaco.
    // 'xInicial' e 'yInicial' indican las coordenadas de la esquina superior izquierda.
    // 'ancho' y 'alto' son las dimensiones del rectángulo.
	if ((xInicial > lcd_ancho || yInicial > lcd_alto) || (xInicial<0 || yInicial<0)) return;
    int yFinal = yInicial + alto - 1;
    int xFinal = xInicial + ancho - 1;

    // Si la pantalla está invertida, ajustamos las coordenadas iniciales y finales
    if (invertida) {
        xInicial = lcd_ancho - xFinal - 1;
        yInicial = lcd_alto - yFinal - 1;
        xFinal = xInicial + ancho - 1;
        yFinal = yInicial + alto - 1;
    }

    // Dibujo del rectángulo, dependiendo de la orientación
    if (horizontal) {
        uint32_t* p = lcd_buffers[lcd_bufferOculto] + (239 - yInicial - alto + 1) + lcd_alto * xInicial;
        uint16_t salto = lcd_alto - alto;
        for (int x = xInicial; x <= xFinal; x++) { // Para cada columna de puntos
            for (int y = yInicial; y <= yFinal; y++) // Recorre los puntos de la columna
                *(p++) = color;  // Establece el color del punto y apunta al siguiente
            p += salto;  // Apunta al primer punto de la siguiente columna
        }
    } else {
        uint32_t* p = lcd_buffers[lcd_bufferOculto] + yInicial * lcd_ancho + xInicial;
        uint16_t salto = lcd_ancho - ancho;
        for (int y = yInicial; y <= yFinal; y++) { // Para cada fila de puntos
            for (int x = xInicial; x <= xFinal; x++) // Recorre los puntos de la fila
                *(p++) = color;  // Establece el color del punto y apunta al siguiente
            p += salto;  // Apunta al primer punto de la siguiente fila
        }
    }
}


// -----------------------------------------------------------------------------------------

void LCD_dibujaLinea(int x0, int y0, int x1, int y1, uint32_t color, int enBlancoYNegro, int transparencia) {
    // Dibuja una línea que une el punto indicado en 'x0', 'y0' con el punto indicado en 'x1', 'y1'
    // en el color 'color' utilizando el algoritmo de Bressenham
    // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
    // Si 'enBlancoYNegro' es un buleano cierto, convierte el color a gris
    // 'transparencia' es un factor entre 0 y 100 para aplicar un nivel de transparencia al color, desde
    // completamente transparente (valor 0) a transparencia original (valor 100)

    int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
    int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
    int err = (dx>dy ? dx : -dy)/2, e2;

    while(1) {
        LCD_dibujaPunto(x0, y0, color, enBlancoYNegro, transparencia);
        if (x0==x1 && y0==y1) break;
        e2 = err;
        if (e2 >-dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }
    }
}

// -----------------------------------------------------------------------------------------

static void dibujaPuntosCircunferencia(int xc, int yc, int x, int y, uint32_t color, int enBlancoYNegro,
    int transparencia) {
    // Utilizada por la función LCD_dibujaCircunferencia

    LCD_dibujaPunto(xc+x, yc+y, color, enBlancoYNegro, transparencia);
    LCD_dibujaPunto(xc-x, yc+y, color, enBlancoYNegro, transparencia);
    LCD_dibujaPunto(xc+x, yc-y, color, enBlancoYNegro, transparencia);
    LCD_dibujaPunto(xc-x, yc-y, color, enBlancoYNegro, transparencia);
    LCD_dibujaPunto(xc+y, yc+x, color, enBlancoYNegro, transparencia);
    LCD_dibujaPunto(xc-y, yc+x, color, enBlancoYNegro, transparencia);
    LCD_dibujaPunto(xc+y, yc-x, color, enBlancoYNegro, transparencia);
    LCD_dibujaPunto(xc-y, yc-x, color, enBlancoYNegro, transparencia);
}


void LCD_dibujaCircunferencia(int xc, int yc, int r, uint32_t color, int enBlancoYNegro, int transparencia) {
    // Dibuja una circunferencia cuyo centro se indica en 'xc', 'yc', de radio 'r' y con el color
    // indicado en 'color' utilizando el algoritmo de Bressenham
    // https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/
    // Si 'enBlancoYNegro' es un buleano cierto, convierte el color a gris
    // 'transparencia' es un factor entre 0 y 100 para aplicar un nivel de transparencia al color, desde
    // completamente transparente (valor 0) a transparencia original (valor 100)

    int x = 0, y = r;
    int d = 3 - 2 * r;
    dibujaPuntosCircunferencia(xc, yc, x, y, color, enBlancoYNegro, transparencia);
    while (y >= x) {
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else d = d + 4 * x + 6;
        dibujaPuntosCircunferencia(xc, yc, x, y, color, enBlancoYNegro, transparencia);
    }
}

// -----------------------------------------------------------------------------------------

static void dibujaPuntosCirculo(int xc, int yc, int x, int y, uint32_t color, int enBlancoYNegro,
    int transparencia) {
    // Utilizada por la función LCD_dibujaCirculo

	LCD_dibujaLinea(xc-x, yc+y, xc+x, yc+y, color, enBlancoYNegro, transparencia);
	LCD_dibujaLinea(xc-x, yc-y, xc+x, yc-y, color, enBlancoYNegro, transparencia);
	LCD_dibujaLinea(xc-y, yc+x, xc+y, yc+x, color, enBlancoYNegro, transparencia);
	LCD_dibujaLinea(xc-y, yc-x, xc+y, yc-x, color, enBlancoYNegro, transparencia);
}


void LCD_dibujaCirculo(int xc, int yc, int r, uint32_t color, int enBlancoYNegro, int transparencia) {
    // Dibuja un círculo cuyo centro se indica en 'xc', 'yc', de radio 'r' y con el color
    // indicado en 'color' utilizando el algoritmo de Bressenham
    // https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/
    // Si 'enBlancoYNegro' es un buleano cierto, convierte el color a gris
    // 'transparencia' es un factor entre 0 y 100 para aplicar un nivel de transparencia al color, desde
    // completamente transparente (valor 0) a transparencia original (valor 100)

    int x = 0, y = r;
    int d = 3 - 2 * r;
    dibujaPuntosCirculo(xc, yc, x, y, color, enBlancoYNegro, transparencia);
    while (y >= x) {
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else d = d + 4 * x + 6;
        dibujaPuntosCirculo(xc, yc, x, y, color, enBlancoYNegro, transparencia);
    }
}

// -----------------------------------------------------------------------------------------

void LCD_dibujaImagen(uint16_t xImagen, uint16_t yImagen, uint16_t ancho, uint16_t alto,
    const uint8_t * imagen, int enBlancoYNegro, int transparencia) {
    // Dibuja una imagen situando su esquina superior izquierda en 'xImagen', 'yImagen', que tiene
    // una resolución de 'ancho' puntos en horizontal y 'alto' puntos en vertical. Los bytes que
    // expresan el color de todos sus puntos (cada punto en 4 bytes en formato ARGB)
    // se encuentran en 'imagen'.
    // La magen se convierte a niveles de gris si 'enBlancoYNegro' es cierto.
    // Se le aplica el grado de transparencia indicado en 'transparencia', de 0 a 100.

    uint32_t * p = (uint32_t*) imagen;  // Para acceder a cada punto por separado

    for(uint16_t y = yImagen; y < yImagen + alto; y++)  // Recorriendo filas
        for (uint16_t x = xImagen; x < xImagen + ancho; x++) {  // y columnas
            LCD_dibujaPunto(x, y, *p, enBlancoYNegro, transparencia);  // dibuja cada punto
            p++;  // Para acceder al color del siguiente punto
        }
}

// -----------------------------------------------------------------------------------------

void LCD_dibujaImagenOpaca(uint16_t xImagen, uint16_t yImagen, uint16_t ancho, uint16_t alto,
    const uint8_t * imagen) {

	if ((xImagen > lcd_ancho || yImagen > lcd_alto) || (xImagen<0 || yImagen<0)) return;

    uint16_t xFinal = xImagen + ancho - 1;  // Coordenada X máxima de la imagen
    uint16_t yFinal = yImagen + alto - 1;   // Coordenada Y máxima de la imagen
    uint32_t * pImagen = (uint32_t*) imagen; // Para acceder a cada punto por separado

    // Si la pantalla está invertida, ajustamos las coordenadas iniciales y finales
    if (invertida) {
        xImagen = lcd_ancho - xFinal - 1;
        yImagen = lcd_alto - yFinal - 1;
        xFinal = xImagen + ancho - 1;
        yFinal = yImagen + alto - 1;
    }

    // Dibujo de la imagen dependiendo de la orientación de la pantalla
    if (horizontal) {  // Si pantalla en horizontal ...
        uint32_t * pFrameBuffer = lcd_buffers[lcd_bufferOculto] + (239 - yImagen - alto + 1) +
            lcd_alto * xImagen;  // Puntero al primer punto en el frame buffer
        uint16_t salto = lcd_alto - alto;  // Para saltar a la siguiente fila de puntos en la imagen
        for (uint16_t y = yImagen; y <= yFinal; y++) { // Recorriendo filas
            for (uint16_t x = xImagen; x <= xFinal; x++) // y columnas
                *(pFrameBuffer++) = *(pImagen++);  // Copia los puntos en el frame buffer
            pFrameBuffer += salto;  // Salta a la siguiente fila
        }
    } else {  // Programación similar si pantalla en vertical
        uint32_t * pFrameBuffer = lcd_buffers[lcd_bufferOculto] + yImagen * lcd_ancho + xImagen;
        uint16_t salto = lcd_ancho - ancho;
        for (uint16_t x = xImagen; x <= xFinal; x++) {
            for (uint16_t y = yImagen; y <= yFinal; y++)
                *(pFrameBuffer++) = *(pImagen++);
            pFrameBuffer += salto;
        }
    }
}


// -----------------------------------------------------------------------------------------

void LCD_dibujaCaracterAlpha(uint16_t x, uint16_t y, char caracter, uint32_t color,
	const LCD_JuegoCaracteresAlpha * pJuegoCaracteres,
    int enBlancoYNegro, int transparencia) {
    // Dibuja el carácter 'caracter' en las coordenadas 'x', 'y' utilizando el juego de caracteres
    // apuntado por 'pJuegoCaracteres'.
    // Si 'enBlancoYNegro' es un buleano cierto, convierte el color a gris.
    // 'transparencia' es un factor entre 0 y 100 para aplicar un nivel de transparencia al color, desde
    // completamente transparente (valor 0) a transparencia original (valor 100).

	const uint8_t * p = pJuegoCaracteres->caracteres + pJuegoCaracteres->posiciones[caracter - 32];
	// Apunta a donde se describe el caracter a visualizar

	int ancho = * p;  // El primer byte indica el ancho del carácter

	p++;
	for (int x1 = 0; x1 < ancho; x1++)  // Recorriendo el ancho
		for (int y1 = 0; y1 < pJuegoCaracteres->alto; y1++) {  // y el alto del carácter
			uint32_t colorPunto = color & 0x00FFFFFF;  // Utiliza las componentes RGB
			colorPunto |= p[x1 + y1 * ancho] << 24;  // Le añade la componente Alpha
			LCD_dibujaPunto(x + x1, y + y1, colorPunto, enBlancoYNegro, transparencia);  // DIbuja el punto
		}
}

void LCD_dibujaCaracterAlphaRotado(uint16_t x, uint16_t y, char caracter, uint32_t color,
    const LCD_JuegoCaracteresAlpha * pJuegoCaracteres, int enBlancoYNegro, int transparencia) {
    // Dibuja el carácter 'caracter' en las coordenadas 'x', 'y' (esquina inferior izquierda del carácter rotado),
    // usando el juego de caracteres apuntado por 'pJuegoCaracteres' con una rotación de 90 grados antihorario.
    // Si 'enBlancoYNegro' es un valor cierto, convierte el color a gris.
    // 'transparencia' es un factor entre 0 y 100 para aplicar un nivel de transparencia al color.

    const uint8_t * p = pJuegoCaracteres->caracteres + pJuegoCaracteres->posiciones[caracter - 32];
    int ancho = *p;  // Ancho original del carácter (antes de la rotación)
    int alto = pJuegoCaracteres->alto;  // Alto del carácter (constante)

    p++;  // Avanzamos el puntero al mapa de bits del carácter

    // Ajustamos la posición inicial para que (x, y) sea la esquina inferior izquierda del carácter rotado
    uint16_t xInicio = x - alto;  // Al rotar, el ancho del carácter se convierte en su altura
    uint16_t yInicio = y;  // La coordenada Y de inicio es la misma

    // Recorremos el mapa de bits del carácter rotado 90 grados
    for (int x1 = 0; x1 < alto; x1++) {  // Altura del carácter antes de rotación
        for (int y1 = 0; y1 < ancho; y1++) {  // Ancho del carácter antes de rotación
            uint32_t colorPunto = color & 0x00FFFFFF;  // Extraemos componentes RGB
            colorPunto |= p[y1 + x1 * ancho] << 24;  // Agregamos la componente Alpha

            if (!(xInicio + x1 < 0 || xInicio + x1>lcd_ancho) || !(yInicio - y1 < 0 || yInicio - y1>lcd_alto))
            LCD_dibujaPunto(xInicio + x1 , yInicio - y1, colorPunto, enBlancoYNegro, transparencia);  // Dibujamos el punto rotado
        }
    }
}


/*
void LCD_dibujaCaracterAlphaRotado(uint16_t x, uint16_t y, char caracter, uint32_t color,
	const LCD_JuegoCaracteresAlpha * pJuegoCaracteres, int enBlancoYNegro, int transparencia) {
    // Dibuja el carácter 'caracter' en las coordenadas 'x', 'y' rotado 90 grados en sentido antihorario
    // utilizando el juego de caracteres apuntado por 'pJuegoCaracteres'.
    // Si 'enBlancoYNegro' es un valor cierto, convierte el color a gris.
    // 'transparencia' es un valor entre 0 y 100 para ajustar el nivel de transparencia del color.

	const uint8_t * p = pJuegoCaracteres->caracteres + pJuegoCaracteres->posiciones[caracter - 32];
	// Apunta a donde se describe el carácter a visualizar

	int ancho = *p;  // El primer byte indica el ancho del carácter
	p++;
	for (int x1 = 0; x1 < ancho; x1++) {  // Recorriendo el ancho original
		for (int y1 = 0; y1 < pJuegoCaracteres->alto; y1++) {  // y el alto original
			uint32_t colorPunto = color & 0x00FFFFFF;  // Utiliza las componentes RGB
			colorPunto |= p[x1 + y1 * ancho] << 24;  // Añade la componente Alpha

			// Para rotar 90 grados antihorario: x' = y1, y' = ancho - x1 - 1
			LCD_dibujaPunto(x + y1 - 4, y + (ancho - x1 - 1), colorPunto, enBlancoYNegro, transparencia);
		}
	}
}
*/
// -----------------------------------------------------------------------------------------

uint16_t LCD_anchoCaracterAlpha(char caracter, const LCD_JuegoCaracteresAlpha * pJuegoCaracteres) {
	// Devuelve el ancho de un carácter en un juego de caracteres

	return pJuegoCaracteres->caracteres[pJuegoCaracteres->posiciones[caracter - 32]];
} /// LCD_anchoCaracterAlpha

// -----------------------------------------------------------------------------------------

uint16_t LCD_anchoCadenaCaracteresAlpha(const char* cadena,
	const LCD_JuegoCaracteresAlpha * pJuegoCaracteres, uint16_t separacion) {
	// Devuelve el ancho en puntos de un texto en un juego de caracteres, utilizando una cierta separación
	// entre caracteres consecutivos

	uint16_t resultado = (strlen(cadena) - 1) * separacion;  // Suma todas las separaciones

	int i = 0;

	while(cadena[i])   // Para cada carácter en el texto
		resultado += LCD_anchoCaracterAlpha(cadena[i++], pJuegoCaracteres);
		// Añade el ancho del carácter

	return resultado;
} // LCD_anchoCadenaCaracteresAlpha

// -----------------------------------------------------------------------------------------

void LCD_dibujaCadenaCaracteresAlpha(uint16_t xCadena, uint16_t yCadena, const char* cadena, uint32_t color, uint16_t separacion,
    const LCD_JuegoCaracteresAlpha * pJuegoCaracteres, int enBlancoYNegro, int transparencia) {
    // Dibuja la cadena de caracteres apuntada por 'cadena' a partir de las coordenadas 'x', 'y'
    // utilizando el juego de caracteres apuntado por 'pJuegoCaracteres'.
    // Si 'enBlancoYNegro' es un buleano cierto, convierte el color a gris
    // 'transparencia' es un factor entre 0 y 100 para aplicar un nivel de transparencia al color, desde
    // completamente transparente (valor 0) a transparencia origina, l (valor 100)

    int i = 0;  // Índice para acceder a cada posición de la cadena

    uint16_t x = xCadena;  // Coordenada X de comienzo de la cadena

    while(cadena[i]) {  // Mientras no se llegue al 0 final de la cadena ...

        LCD_dibujaCaracterAlpha(x,
            yCadena, cadena[i], color, pJuegoCaracteres, enBlancoYNegro, transparencia);
        // Dibuja el carácter i-ésimo de la cadena

        x += LCD_anchoCaracterAlpha(cadena[i], pJuegoCaracteres);  // Añade el ancho del carácter

        for (int x1 = 0; x1 < separacion; x1++)
        	for (int y1 = 0; y1 < pJuegoCaracteres->alto; y1++)
        		LCD_dibujaPunto(x + x1, yCadena + y1, color, enBlancoYNegro, 0);

        x += separacion;  // Salta la separación entre caracteres

        i++;  // Para acceder al siguiente carácter
    }
} // LCD_dibujaCadenaCaracteresAlpha

/*
void LCD_dibujaCadenaCaracteresAlpha(uint16_t xCadena, uint16_t yCadena, const char* cadena, uint32_t color, uint16_t separacion,  const LCD_JuegoCaracteresAlpha * pJuegoCaracteres, int enBlancoYNegro, int transparencia) {
    // Dibuja la cadena de caracteres apuntada por 'cadena' a partir de las coordenadas 'x', 'y'
    // utilizando el juego de caracteres apuntado por 'pJuegoCaracteres'.
    // Si 'enBlancoYNegro' es un buleano cierto, convierte el color a gris
    // 'transparencia' es un factor entre 0 y 100 para aplicar un nivel de transparencia al color, desde
    // completamente transparente (valor 0) a transparencia original (valor 100)

	// el texto se ajusta al sentido de la pantalla, si el texto es muy largo se realizan los saltos de linea correspondientes (LimiteH)
	// si no se puede seguir escribiendo verticalemente porque se supera el espacio vertical (LimiteV) se deja de escribir

    // Completar
	 int longitud = strlen(cadena);
	 uint8_t Alto = pJuegoCaracteres -> alto; // el alto de todos los caracteres es el mismo
	 uint8_t AnchoIncremental =0;
	 uint16_t AchoCaracter;

	 int LimiteH, LimiteV;
	 int lcd_ancho = LCD_ancho(); // ancho de la pantalla
	 int lcd_alto = LCD_alto(); // alto de la pantalla

	 //limites de tamaño del texto
	 if (horizontal == 0){ //pantalla vertical
		 LimiteH= lcd_ancho; // 240
		 LimiteV = lcd_alto; // 320
	 }
	 else{ //pantalla horizontal
		 LimiteV= lcd_ancho; // 240
		 LimiteH = lcd_alto; // 320
	 }

	 int PosX, PosY; // coordenadas de cada caracter

	 // variables para los saltos de linea
	 int saltar=0;
	 int SaltoLinea = Alto + separacion;


	 // comenzamos a dibujar la cadena de caracteres
	 int PosXActual=0;
	 for (int i = 0; i < longitud + saltar ; i++) {
		 AchoCaracter = LCD_anchoCaracterAlpha(cadena[i-saltar],pJuegoCaracteres);

		 // definimos las coordenadas de cada caracter de la cadena de texto
		 PosX = xCadena + separacion*PosXActual + AnchoIncremental;
		 PosY = yCadena + saltar*SaltoLinea;

		 if (PosX+AchoCaracter <= LimiteH && PosY + Alto <= LimiteV){ // el texto no debe superar el ancho ni el alto de la pantalla
			 LCD_dibujaCaracterAlpha(PosX,PosY,cadena[i-saltar],color,pJuegoCaracteres,enBlancoYNegro,transparencia);

			 //accedemos al siguiente caracter
			 PosXActual++;
			 AnchoIncremental += AchoCaracter;
		 }
		 else{
			 saltar ++;// si se supera el ancho de la pantalla hacemos una salto de linea

			 // reiniciamos las variables para poder empezar a escribir la nueva linea a la misma distancia del borde que la linea origen
			 PosXActual=0;
			 AnchoIncremental = 0;
			 AchoCaracter=0;
		 }
	 }
} // LCD_dibujaCadenaLargaCaracteresAlpha
*/


void LCD_dibujaCadenaCaracteresAlphaVertical(uint16_t xCadena, uint16_t yCadena, const char* cadena, uint32_t color, uint16_t separacion,
    const LCD_JuegoCaracteresAlpha * pJuegoCaracteres, int enBlancoYNegro, int transparencia) {
    // Dibuja la cadena de caracteres apuntada por 'cadena' de manera vertical ascendente
    // a partir de las coordenadas 'xCadena', 'yCadena' utilizando el juego de caracteres
    // apuntado por 'pJuegoCaracteres'.
    // Si 'enBlancoYNegro' es un valor cierto, convierte el color a gris.
    // 'transparencia' es un factor entre 0 y 100 para ajustar el nivel de transparencia del color.

    int i = 0;  // Índice para acceder a cada carácter en la cadena
    uint16_t y = yCadena;  // Coordenada Y inicial de la cadena, se ajusta para escribir en sentido ascendente

    while(cadena[i]) {  // Mientras no se llegue al 0 final de la cadena ...

        // Dibuja el carácter i-ésimo de la cadena rotado 90 grados en sentido antihorario
        LCD_dibujaCaracterAlphaRotado(xCadena, y, cadena[i], color, pJuegoCaracteres, enBlancoYNegro, transparencia);

        // Añade el alto del carácter y la separación para pasar al siguiente carácter en sentido ascendente
        y -= pJuegoCaracteres->alto + separacion;

        i++;  // Avanza al siguiente carácter
    }
} // LCD_dibujaCadenaCaracteresAlphaVertical


// -------------------------------------------------------------------------------------------------------

void LCD_dibujaCuadrado(uint16_t xInicial, uint16_t yInicial, uint16_t lado,
    uint32_t color, int enBlancoYNegro, int transparencia) {

	LCD_dibujaRectanguloRelleno(xInicial, yInicial, lado, lado, color, enBlancoYNegro, transparencia);
} // LCD_dibujaCuadrado



