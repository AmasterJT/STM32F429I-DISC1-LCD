#include "interfazLCD.h"
#include <stdio.h>  // Para sprintf()
#include <string.h> // Para strcmp()

// ---------------------------------------------------------------------------------------------------
// Etiquetas

void LCD_inicializaEtiqueta(const char *texto, uint16_t x, uint16_t y, const LCD_JuegoCaracteresAlpha *juego, uint16_t separacion, uint16_t ancho, uint16_t alto, uint16_t margenHorizontal,
                            uint16_t margenVertical, LCD_Alineacion alineacion, uint32_t color, uint32_t colorFondo, int visible, int habilitada, LCD_Etiqueta *etiqueta) {
    strcpy(etiqueta->texto, texto);
    etiqueta->anchoTexto = LCD_anchoCadenaCaracteresAlpha(texto, juego, separacion);
    etiqueta->x = x;
    etiqueta->y = y;
    etiqueta->juego = juego;
    etiqueta->separacion = separacion;
    etiqueta->margenHorizontal = margenHorizontal;
    etiqueta->margenVertical = margenVertical;
    etiqueta->ancho = ancho;
    etiqueta->alto = alto;
    etiqueta->alineacion = alineacion;
    etiqueta->color = color;
    etiqueta->colorFondo = colorFondo;
    etiqueta->transparente = (colorFondo & 0xFF000000) == 0;
    etiqueta->visible = visible;
    etiqueta->habilitada = habilitada;
}

void LCD_setVisibilidadEtiqueta(int visibilidad, LCD_Etiqueta *etiqueta) { etiqueta->visible = visibilidad; }

void LCD_setHabilitacionEtiqueta(int habilitacion, LCD_Etiqueta *etiqueta) { etiqueta->habilitada = habilitacion; }

void LCD_setTextoEtiqueta(const char *texto, LCD_Etiqueta *etiqueta) {
    if (strcmp(etiqueta->texto, texto)) {
        strcpy(etiqueta->texto, texto);
        etiqueta->anchoTexto = LCD_anchoCadenaCaracteresAlpha(texto, etiqueta->juego, etiqueta->separacion);
    }
}

void LCD_setColorTextoEtiqueta(uint32_t color, LCD_Etiqueta *etiqueta) { etiqueta->color = color; }

void LCD_setColorFondoEtiqueta(uint32_t colorFondo, LCD_Etiqueta *etiqueta) {
    if (etiqueta->colorFondo != colorFondo) {
        etiqueta->colorFondo = colorFondo;
        etiqueta->transparente = (colorFondo & 0xFF000000) == 0;
    }
}

void LCD_setAlineacionEtiqueta(LCD_Alineacion alineacion, LCD_Etiqueta *etiqueta) { etiqueta->alineacion = alineacion; }

void LCD_atiendeEtiqueta(LCD_Etiqueta *etiqueta) {
    uint16_t xTexto, yTexto;
    if (etiqueta->visible) {
        int enBlancoYNegro, opacidad;
        if (etiqueta->habilitada) {
            enBlancoYNegro = 0;
            opacidad = 100;
        } else {
            enBlancoYNegro = 1;
            opacidad = 70;
        }
        if (etiqueta->alineacion == LCD_ALINEACION_IZQUIERDA)
            xTexto = etiqueta->x + etiqueta->margenHorizontal;
        else if (etiqueta->alineacion == LCD_ALINEACION_DERECHA)
            xTexto = etiqueta->x + etiqueta->ancho - etiqueta->margenHorizontal - etiqueta->anchoTexto;
        else
            xTexto = etiqueta->x + etiqueta->ancho / 2 - etiqueta->anchoTexto / 2;
        yTexto = etiqueta->y + etiqueta->margenVertical;
        if (!etiqueta->transparente)
            LCD_dibujaRectanguloRelleno(etiqueta->x, etiqueta->y, etiqueta->ancho, etiqueta->alto, etiqueta->colorFondo, enBlancoYNegro, opacidad);
        LCD_dibujaCadenaCaracteresAlpha(xTexto, yTexto, etiqueta->texto, etiqueta->color, etiqueta->separacion, etiqueta->juego, enBlancoYNegro, opacidad);
    }
}

//-------------------------------------------------------------------------------------------------------
// Imágenes

void LCD_inicializaImagen(uint16_t x, uint16_t y, uint16_t ancho, uint16_t alto, const uint8_t *colores, int visible, int habilitada, LCD_Imagen *pImagen) {
    pImagen->x = x;
    pImagen->y = y;
    pImagen->ancho = ancho;
    pImagen->alto = alto;
    pImagen->colores = colores;
    pImagen->visible = visible;
    pImagen->habilitada = habilitada;
}

void LCD_setImagen(const uint8_t *colores, LCD_Imagen *pImagen) { pImagen->colores = colores; }

void LCD_setPosicionImagen(uint16_t x, uint16_t y, LCD_Imagen *pImagen) {
    pImagen->x = x;
    pImagen->y = y;
}

void LCD_setVisibilidadImagen(int visible, LCD_Imagen *pImagen) { pImagen->visible = visible; }

void LCD_setHabilitacionImagen(int habilitada, LCD_Imagen *pImagen) { pImagen->habilitada = habilitada; }

void LCD_atiendeImagen(LCD_Imagen *pImagen) {
    int transparencia, enBlancoYNegro;
    if (pImagen->visible) {
        if (pImagen->habilitada) {
            transparencia = 100;
            enBlancoYNegro = 0;
        } else {
            transparencia = 70;
            enBlancoYNegro = 1;
        }
    } else
        transparencia = 0;
    LCD_dibujaImagen(pImagen->x, pImagen->y, pImagen->ancho, pImagen->alto, pImagen->colores, enBlancoYNegro, transparencia);
}

//-------------------------------------------------------------------------------------------------------
// Botones

void LCD_inicializaBoton(uint16_t x, uint16_t y, uint16_t ancho, uint16_t alto, const uint8_t *imagen, char *texto, const LCD_JuegoCaracteresAlpha *pJuegoCaracteres, uint8_t separacion,
                         uint32_t colorTexto, void (*funcion)(), int habilitado, int visible, LCD_Boton *pBoton) {
    // Inicializa la estructura de datos apuntada por 'pBoton' para representar a un botón que se
    // va a mostrar en unas coordenadas 'x', 'y' con unas dimensiones 'ancho' y 'alto', visualizado
    // con la imagen apuntada por 'imagen', de forma que ejecute la función apuntada por 'funcion'
    // cuando se pulse e indicando si inicialmente se muestra visible y/o inicializado según los
    // buleanos 'visible' y 'habilitado'.

    pBoton->x = x;
    pBoton->y = y;
    // Coordenadas de la esquina superior izquierda del botón

    pBoton->ancho = ancho;
    pBoton->alto = alto;
    // Dimensiones del botón en puntos

    pBoton->imagen = imagen;
    // Imagen utilizada para mostrar el botón

    strcpy(pBoton->texto, texto);
    pBoton->pJuegoCaracteres = pJuegoCaracteres;
    pBoton->separacion = separacion;
    pBoton->xTexto = (ancho - LCD_anchoCadenaCaracteresAlpha(texto, pJuegoCaracteres, separacion)) / 2;
    pBoton->yTexto = (alto - pJuegoCaracteres->alto) / 2;
    pBoton->colorTexto = colorTexto;
    // Texto y juego de caracteres mostrado sobre el botón

    pBoton->funcion = funcion;
    // Función a ejecutar cuando se pulsa el botón

    pBoton->habilitado = habilitado;
    // Indica si se muestra habilitado

    pBoton->pulsado = 0;
    // Inicialmente no está pulsado

    pBoton->visible = visible;
    // Indica si hay que mostrar el botón
}

void LCD_setVisibilidadBoton(int visibilidad, LCD_Boton *pBoton) {
    // Establece la visibilidad del botón representado por la estructura apuntada por 'pBoton',
    // según el buleano 'visibilidad'

    pBoton->visible = visibilidad; // Guarda la nueva visibilidad
}

void LCD_setHabilitacionBoton(int habilitacion, LCD_Boton *pBoton) {
    // Establece si el botón representado por la estructura apuntada por 'pBoton' está habilitado,
    // según el buleano 'habilitacion'

    pBoton->habilitado = habilitacion; // Lo recuerda
}

void LCD_setTextoBoton(const char *texto, LCD_Boton *pBoton) {
    strcpy(pBoton->texto, texto);
    pBoton->xTexto = (pBoton->ancho - LCD_anchoCadenaCaracteresAlpha(texto, pBoton->pJuegoCaracteres, pBoton->separacion)) / 2;
}

void LCD_setColorTextoBoton(uint32_t color, LCD_Boton *pBoton) { pBoton->colorTexto = color; }

void LCD_setImagenBoton(const uint8_t *imagen, LCD_Boton *pBoton) { pBoton->imagen = imagen; }

void LCD_atiendeBoton(LCD_Boton *pBoton) {
    // Atiende a la posible pulsación del botón representado por la estructura apuntada por 'pBoton'.
    // También dibuja el botón en el buffer oculto.

    int opacidad;       // Para expresar el grado de opacidad (de 0 a 100) con el que se va a dibujar
    int enBlancoYNegro; // Buleano cierto si hay que dibujarlo en blanco y negro

    if (pBoton->visible) {        // Si el botón tiene que ser visible ...
        if (pBoton->habilitado) { // Si está habilitado ...
            opacidad = 100;       // Totalmente opaco
            enBlancoYNegro = 0;   // Se va a mostra en color
        } else {                  // Si es visible pero no está habilitado
            opacidad = 70;        // Se muestra semitransparente
            enBlancoYNegro = 1;   // y en niveles de gris
        }
    } else {          // Si no es visible
        opacidad = 0; // Se muestra completamente transparente
        enBlancoYNegro = 0;
    }

    LCD_dibujaImagen(pBoton->x, pBoton->y, pBoton->ancho, pBoton->alto, pBoton->imagen, enBlancoYNegro, opacidad);
    // Finalmente dibuja la imagen para mostrar el botón con la opacidad y color establecidos

    LCD_dibujaCadenaCaracteresAlpha(pBoton->x + pBoton->xTexto, pBoton->y + pBoton->yTexto, pBoton->texto, pBoton->colorTexto, pBoton->separacion, pBoton->pJuegoCaracteres, enBlancoYNegro, opacidad);
    // Dibuja el texto sobre el botón

    if (pBoton->habilitado && pBoton->visible) { // Si el botón está habilitado y es visible ...

        if (LCD_pulsando() && LCD_xPulsacion() > pBoton->x && LCD_xPulsacion() < pBoton->x + pBoton->ancho && LCD_yPulsacion() > pBoton->y && LCD_yPulsacion() < pBoton->y + pBoton->alto) {
            // Si se ha pulsado la pantalla dentro de las coordenadas de la superficie ocupada por el
            // botón ...

            if (!pBoton->pulsado)  // Si el botón no estaba pulsado previamente ...
                pBoton->funcion(); // Ejecuta la función asociada al botón

            pBoton->pulsado = 1; // Indica que se está pulsando el botón

        } else
            pBoton->pulsado = 0; // Si no, indica que no se está pulsando el botón

    } else
        pBoton->pulsado = 0; // Si no, indica que no se está pulsando el botón
}

//----------------------------------------------------------------------------------------------------
// Interruptores

void LCD_inicializaInterruptor(uint16_t x, uint16_t y, uint16_t ancho, uint16_t alto, const uint8_t *imagenOn, const uint8_t *imagenOff, void (*funcion)(int), int habilitado, int visible,
                               LCD_Interruptor *pInterruptor) {
    // Inicializa la estructura apuntada por 'pInterruptor' para representar a un interruptor mostrado
    // en pantalla a partir de los siguientes datos:
    // - La posición 'x' e 'y'
    // - Las dimensiones 'ancho' y 'alto'
    // - Las imágenes 'imagenOn' e 'imagenOff' a mostrar cuando el interruptor está en estado ON u OFF.
    // - La función que se va a ejecutar cuando el usuario haga click sobre el botón. Se le va a pasar
    //   por parámetro un buleano indicando si el botón está a ON o a OFF.
    // - Un buleano 'visible' para inidicar si inicialmente el botón se muestra en pantalla.
    // - Un buleano 'habilitado' de forma que si el botón está visible y habilitado, se muestra con sus
    //   colores originales. Si está visible y deshabilitado, se muestra en blanco y negro y semmitransparente.

    pInterruptor->x = x;
    pInterruptor->y = y;
    // Guarda la posición del botón en la pantalla

    pInterruptor->ancho = ancho;
    pInterruptor->alto = alto;
    // Guarda las dimensiones del botón

    pInterruptor->imagenOn = imagenOn;
    pInterruptor->imagenOff = imagenOff;
    // Guarda punteros a las imágenes a mostrar cuando el interruptor está en On o en OFF.

    pInterruptor->funcion = funcion;
    // Guarda un puntero a la función a ejecutar cuando se acciona el interruptor

    pInterruptor->habilitado = habilitado;
    pInterruptor->visible = visible;
    // Guarda buleanos que indican si el interruptor está visible o habilitado
}

void LCD_setHabilitacionInterruptor(int habilitacion, LCD_Interruptor *pInterruptor) {
    // Establece si el interruptor representado por la estructura apuntada por 'pInterruptor' está habilitado,
    // según el buleano 'habilitacion'

    pInterruptor->habilitado = habilitacion; // La recuerda
}

void LCD_setVisibilidadInterruptor(int visibilidad, LCD_Interruptor *pInterruptor) {
    // Establece la visibilidad del interruptor representado por la estructura apuntada por 'pInterruptor',
    // según el buleano 'visibilidad'

    pInterruptor->visible = visibilidad; // La recuerda
}

void LCD_atiendeInterruptor(LCD_Interruptor *pInterruptor) {
    // Atiende a la posible pulsación del interruptor representado por la estructura apuntada por 'pInterruptor'

    int opacidad;       // Para expresar el grado de opacidad (de 0 a 100) con el que se va a dibujar
    int enBlancoYNegro; // Buleano cierto si hay que dibujarlo en blanco y negro

    if (pInterruptor->visible) {        // Si el interruptor tiene que ser visible ...
        if (pInterruptor->habilitado) { // Si está habilitado ...
            opacidad = 100;             // hay que mostrarlo totalmente opaco
            enBlancoYNegro = 0;         // se va a mostra en color
        } else {                        // Si es visible pero no está habilitado ...
            opacidad = 70;              // se muestra semitransparente
            enBlancoYNegro = 1;         // y en niveles de gris
        }
    } else {          // Si no es visible
        opacidad = 0; // Se muestra completamente transparente
        enBlancoYNegro = 0;
    }

    const uint8_t *imagen;               // Puntero a la imagen que se va a mostrar
    if (pInterruptor->estado)            // Si está en estado ON ...
        imagen = pInterruptor->imagenOn; // se muestra la imagen ON
    else
        imagen = pInterruptor->imagenOff; // si no, se muestra la imagen OFF

    LCD_dibujaImagen(pInterruptor->x, pInterruptor->y, pInterruptor->ancho, pInterruptor->alto, imagen, enBlancoYNegro, opacidad);
    // Finalmente dibuja la imagen para mostrar el interruptor con la opacidad y color establecidos

    if (pInterruptor->habilitado && pInterruptor->visible) { // Si el botón está habilitado y vible ...
        if (LCD_pulsando() && LCD_xPulsacion() > pInterruptor->x && LCD_xPulsacion() < pInterruptor->x + pInterruptor->ancho && LCD_yPulsacion() > pInterruptor->y &&
            LCD_yPulsacion() < pInterruptor->y + pInterruptor->alto) {
            // Si se ha pulsado la pantalla dentro de las coordenadas de la superficie ocupada por el
            // interruptor ..

            if (!pInterruptor->pulsado) {                     // Si el interruptor no estaba pulsado previamente ...
                pInterruptor->estado = !pInterruptor->estado; // Cambia el estado del interruptor
                pInterruptor->funcion(pInterruptor->estado);  // Ejecuta la función asociada al interruptor
            }

            pInterruptor->pulsado = 1; // Indica que se está pulsando el interruptor

        } else
            pInterruptor->pulsado = 0; // si no, indica que no se está pulsando el interruptor

    } else
        pInterruptor->pulsado = 0; // si no, indica que no se está pulsando el interruptor
}

int LCD_getEstadoInterruptor(LCD_Interruptor *pInterruptor) {
    // Devuelve un buleano indicando el estado actual del interruptor

    return pInterruptor->estado;
}

void LCD_setEstadoInterruptor(int estado, LCD_Interruptor *pInterruptor) {
    // Establece el estado del interruptor a ON (con un buleano cierto en 'estado') o a OFF (con un buleano falso).
    // Si cambia el estado, entonces se ejecuta la función asociada al interruptor.

    if (pInterruptor->estado != estado) { // Si cambia el estado del interruptor ...
        pInterruptor->funcion(estado);    // Ejecuta la función pasándole el nuevo estado
        pInterruptor->estado = estado;    // Copia el nuevo estado en la estructura
    }
}

//----------------------------------------------------------------------------------------------------
// Barras

void LCD_inicializaBarra(const char *texto,                                // Texto que se muestra antes del valor
                         float minimo, float maximo,                       // Mínimo y máximo del valor a mostrar
                         uint8_t largo, uint8_t grosor,                    // Longitud y grosor de la barra en puntos
                         uint16_t x, uint16_t y,                           // posición de la esquina superior izquierda de la barra
                         uint8_t numCaracteres,                            // Número total de caracteres utilizados para visualizar el valor
                         uint8_t decimales,                                // Número de decimales para visualizar el valor
                         uint32_t colorTexto,                              // Color del texto donde se visualiza el valor
                         uint32_t colorBarra,                              // Color para representar el valor
                         uint32_t colorFondo,                              // Color restante del fondo de la barra
                         const LCD_JuegoCaracteresAlpha *pJuegoCaracteres, // Juego de caracteres para visualizar el valor
                         uint8_t separacion,                               // Separación entre caracteres consecutivos
                         LCD_Barra *pBarra) {                              // Puntero a la estructura donde se copian los datos
                                                                           // Inicializa la estructura apuntada por 'pBara' para representar a una barra horizontal en la que mostrar
                                                                           // un dato numérico gráficamente y también en texto utilizando un número de decimales con un juego de
                                                                           // caracteres.

    pBarra->texto = texto;
    pBarra->minimo = minimo;
    pBarra->valor = minimo;
    pBarra->maximo = maximo;
    pBarra->largo = largo;
    pBarra->grosor = grosor;
    pBarra->escala = largo / (maximo - minimo);
    pBarra->x = x;
    pBarra->y = y;
    pBarra->colorTexto = colorTexto;
    pBarra->colorBarra = colorBarra;
    sprintf(pBarra->formato, "%%s%%-%d.%df", numCaracteres, decimales);
    pBarra->colorFondo = colorFondo;
    pBarra->juegoCaracteres = pJuegoCaracteres;
    pBarra->separacion = separacion;
    // Copia todos los datos en la estructura apuntada por pDial

    pBarra->inicializada = 0; // Inidica que aún no se asignó ningún valor
    pBarra->margenTextoX = 5;
    pBarra->margenTextoY = (pBarra->grosor - pBarra->juegoCaracteres->alto) / 2;
}

void LCD_setVisibilidadBarra(int visible, LCD_Barra *pBarra) {
    // Establece la visibilidad de la barra en función del buleano 'visible'

    pBarra->visible = visible; // hay que redibujarla en los dos frame lcd_buffers
}

void LCD_setColorTextoBarra(uint32_t color, LCD_Barra *pBarra) { pBarra->colorTexto = color; }

void LCD_setColorBarra(uint32_t color, LCD_Barra *pBarra) { pBarra->colorBarra = color; }

void LCD_setColorFondoBarra(uint32_t color, LCD_Barra *pBarra) { pBarra->colorFondo = color; }

void LCD_setValorBarra(float valor, LCD_Barra *pBarra) { pBarra->valor = valor; }

void LCD_atiendeBarra(LCD_Barra *pBarra) {
    // Actualiza la visualización del valor en la barra. Este es un método que hay que llamar
    // continuamente en un bucle en el programa para actualizar la visualización.

    if (pBarra->visible) {
        uint8_t puntosValor; // Largo de la parte coloreada correspondiente al valor
        puntosValor = (pBarra->valor - pBarra->minimo) * pBarra->escala;
        LCD_dibujaRectanguloRellenoOpaco(pBarra->x, pBarra->y, puntosValor, pBarra->grosor, pBarra->colorBarra);
        LCD_dibujaRectanguloRellenoOpaco(pBarra->x + puntosValor, pBarra->y, pBarra->largo - puntosValor, pBarra->grosor, pBarra->colorFondo);
        char cadena[30];
        int parteEntera, parteDecimal;
        parteEntera = pBarra->valor;
        parteDecimal = (pBarra->valor - parteEntera) * 10;
        if (parteDecimal < 0)
            parteDecimal = -parteDecimal;
        if (parteDecimal > 0)
            sprintf(cadena, "%s%d.%d", pBarra->texto, parteEntera, parteDecimal);
        else
            sprintf(cadena, "%s%d", pBarra->texto, parteEntera);
        while (strlen(cadena) < 6)
            strcat(cadena, " ");
        LCD_dibujaCadenaCaracteresAlpha(pBarra->x + pBarra->margenTextoX, pBarra->y + pBarra->margenTextoY, cadena, pBarra->colorTexto, pBarra->separacion, pBarra->juegoCaracteres, 0, 100);
    } else
        LCD_dibujaRectanguloRellenoOpaco(pBarra->x, pBarra->y, pBarra->largo, pBarra->grosor, 0x00000000);
}

//---------------------------------------------------------------------------------------------
// Editor de dato

void LCD_inicializaEditor(float valorInicial, float incrementoMenor, float incrementoMayor, uint16_t x, uint16_t y, uint16_t ancho, uint16_t alto, uint16_t margenTextoX, uint16_t margenTextoY,
                          LCD_Alineacion alineacion, uint32_t colorFondo, uint32_t colorTexto, const LCD_JuegoCaracteresAlpha *pJuego, uint8_t separacion, uint8_t decimales, LCD_Editor *pEditor) {

    pEditor->valor = valorInicial;
    pEditor->incrementoMenor = incrementoMenor;
    pEditor->incrementoMayor = incrementoMayor;
    sprintf(pEditor->formato, "%%.%df", decimales);
    pEditor->decimales = decimales;
    pEditor->x = x;
    pEditor->x25 = x + ancho / 4;
    pEditor->x50 = x + ancho / 2;
    pEditor->x75 = x + ancho * 3 / 4;
    pEditor->x100 = x + ancho;
    pEditor->y = y;
    pEditor->margenTextoX = margenTextoX;
    pEditor->margenTextoY = margenTextoY;
    pEditor->alineacion = alineacion;
    pEditor->ancho = ancho;
    pEditor->alto = alto;
    pEditor->pulsado = 0;
    pEditor->colorFondo = colorFondo;
    pEditor->colorTexto = colorTexto;
    pEditor->pJuego = pJuego;
    pEditor->separacion = separacion;
}

float LCD_getValorEditor(const LCD_Editor *pEditor) { return pEditor->valor; }

void LCD_setValorEditor(float valor, LCD_Editor *pEditor) { pEditor->valor = valor; }

void LCD_atiendeEditor(LCD_Editor *pEditor) {
    uint16_t xPulsacion = LCD_xPulsacion();
    uint16_t yPulsacion = LCD_yPulsacion();
    if (LCD_pulsando() && yPulsacion > pEditor->y && yPulsacion < pEditor->y + pEditor->alto) {
        if (xPulsacion > pEditor->x && xPulsacion < pEditor->x25) {
            if (!pEditor->pulsado)
                pEditor->valor -= pEditor->incrementoMayor;
            pEditor->pulsado = 1; // Indica que se está pulsando el editor
        } else if (xPulsacion >= pEditor->x25 && xPulsacion < pEditor->x50) {
            if (!pEditor->pulsado)
                pEditor->valor -= pEditor->incrementoMenor;
            pEditor->pulsado = 1;
        } else if (xPulsacion >= pEditor->x50 && xPulsacion < pEditor->x75) {
            if (!pEditor->pulsado)
                pEditor->valor += pEditor->incrementoMenor;
            pEditor->pulsado = 1;
        } else if (xPulsacion >= pEditor->x75 && xPulsacion < pEditor->x100) {
            if (!pEditor->pulsado)
                pEditor->valor += pEditor->incrementoMayor;
            pEditor->pulsado = 1;
        } else
            pEditor->pulsado = 0;
    } else
        pEditor->pulsado = 0;

    uint16_t xTexto, yTexto, anchoTexto;
    char cadena[30];
    sprintf(cadena, pEditor->formato, pEditor->valor);
    anchoTexto = LCD_anchoCadenaCaracteresAlpha(cadena, pEditor->pJuego, pEditor->separacion);
    if (pEditor->alineacion == LCD_ALINEACION_IZQUIERDA)
        xTexto = pEditor->x + pEditor->margenTextoX;
    else if (pEditor->alineacion == LCD_ALINEACION_DERECHA)
        xTexto = pEditor->x + pEditor->ancho - pEditor->margenTextoX - anchoTexto;
    else
        xTexto = pEditor->x + pEditor->ancho / 2 - anchoTexto / 2;
    yTexto = pEditor->y + pEditor->margenTextoY;
    LCD_dibujaRectanguloRelleno(pEditor->x, pEditor->y, pEditor->ancho, pEditor->alto, pEditor->colorFondo, 0, 100);
    LCD_dibujaCadenaCaracteresAlpha(xTexto, yTexto, cadena, pEditor->colorTexto, pEditor->separacion, pEditor->pJuego, 0, 100);
}
