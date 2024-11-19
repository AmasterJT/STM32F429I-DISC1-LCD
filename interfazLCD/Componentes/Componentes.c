#include <cmsis_os2.h> // para osDelay()
#include <math.h>
#include <stdio.h>  // Para sprintf()
#include <stdlib.h> // para abs()
#include <string.h>


// importamos las funciones para el manejo de la pantalla
#include "ComponentesH.h"
#include "interfazLCD.h"

#include "ColoresHexa.h" // importamos la paleta de colores en formato hexadecimal ARGB
#include <Imagenes.h>
#include <JuegoCaracteres.h> //importamos las tipografias


//------------------------------------------------------------------------------------------------------
// funciones: inicializacion de los componentes

void InicializaBarraProgreso(BarraProgreso *barraProgreso, int16_t xBarra,
                             uint16_t yBarra, uint16_t ancho, uint16_t alto,  const LCD_JuegoCaracteresAlpha * pJuegoCaracteres, uint8_t separacion,
                             uint32_t color, void (*pFunc)(void)) {
  barraProgreso->Posx = xBarra;
  barraProgreso->Posy = yBarra;
  barraProgreso->alto = alto;
  barraProgreso->ancho = ancho;
  barraProgreso ->pJuegoCaracteres = pJuegoCaracteres;
  barraProgreso->separacion = separacion;
  barraProgreso->color = color;
  barraProgreso->finalizacion = 0;
  barraProgreso->pFuncion = pFunc;

} // InicializaBarraProgreso

void InicializaPulsador(Pulsador *pulsador, int16_t xPulsador, uint16_t yPulsador,
                        uint16_t ancho, uint16_t alto, uint32_t colorFondo,  uint32_t colorTexto,
                        int activacion, int pulsado, void (*pFuncAct)(void),
                        void (*pFuncDes)(void)) {
  pulsador->Posx = xPulsador;
  pulsador->Posy = yPulsador;
  pulsador->ancho = ancho;
  pulsador->alto = alto;
  pulsador->colorFondo = colorFondo;
  pulsador->colorTexto = colorTexto;
  pulsador->pulsado = pulsado;
  pulsador->activacion = activacion;
  pulsador->pFuncionActivacion = pFuncAct;
  pulsador->pFuncionDesactivacion = pFuncDes;

} // InicializaBoton

void InicializaInterruptor(Interruptor *interruptor, int16_t xInterruptor,
                           uint16_t yInterruptor, uint16_t ancho, uint16_t alto,
                           uint32_t color, uint16_t activacion,
                           void (*pFunc)(int estado)) {
  interruptor->Posx = xInterruptor;
  interruptor->Posy = yInterruptor;
  interruptor->ancho = ancho;
  interruptor->alto = alto;
  interruptor->color = color;
  interruptor->activacion = activacion;
  interruptor->pFuncionActivacion = pFunc;

} // InicializaInterruptor

void inicializaBarraTexto(BarraTexto *barraTexto, uint16_t xBarraTexto,
                          uint16_t yBarraTexto, uint16_t ancho, uint16_t alto,
						  const LCD_JuegoCaracteresAlpha * pJuegoCaracteres, uint8_t separacion, uint32_t colorFondo, uint32_t colorTexto,
                          int alineacion, char *texto) {
  barraTexto->Posx = xBarraTexto;
  barraTexto->Posy = yBarraTexto;
  barraTexto->ancho = ancho;
  barraTexto->alto = alto;
  barraTexto->pJuegoCaracteres = pJuegoCaracteres;
  barraTexto->separacion=separacion;
  barraTexto->colorFondo = colorFondo;
  barraTexto->colorTexto = colorTexto;
  barraTexto->alineacion = alineacion;
  strcpy(barraTexto->texto, texto);

} // inicializaBarraTexto

void inicializaEtiqueta(Etiqueta *etiqueta, uint16_t xEtiqueta,
                        uint16_t yEtiqueta, uint16_t ancho, uint16_t alto,
                        const uint8_t *imagen, int habilitacion,
                        int visibilidad) {
  etiqueta->Posx = xEtiqueta;
  etiqueta->Posy = yEtiqueta;
  etiqueta->ancho = ancho;
  etiqueta->alto = alto;
  etiqueta->imagen = imagen;
  etiqueta->habilitacion = habilitacion;
  etiqueta->visibilidad = visibilidad;

} // inicializaEtiqueta

void inicializaEditorTexto(EditorTexto *editorTexto, uint16_t xEditor,
                           uint16_t yEditor, uint16_t anchoBotones,
                           uint16_t anchoVisualizador, uint16_t alto,
						   const LCD_JuegoCaracteresAlpha * pJuegoCaracteres, uint8_t separacion,
                           uint32_t colorFondo, uint32_t colorTexto,
                           uint32_t colorBotones, uint32_t colorTextoBotones,
                           int Cuenta, int incremento) {

  editorTexto->Posx = xEditor;
  editorTexto->Posy = yEditor;
  editorTexto->anchoVisualizador = anchoVisualizador;
  editorTexto->anchoBotones = anchoBotones;
  editorTexto->alto = alto;
  editorTexto->pJuegoCaracteres = pJuegoCaracteres;
  editorTexto ->separacion=separacion;
  editorTexto->colorFondo = colorFondo;
  editorTexto->colorTexto = colorTexto;
  editorTexto->colorBotones = colorBotones;
  editorTexto->colorTextoBotones = colorTextoBotones;
  editorTexto->Cuenta = Cuenta;
  editorTexto->incremento = incremento;

  editorTexto->pulsacionAnterior = 0;

  InicializaPulsador(&editorTexto->PulResta, xEditor, yEditor, anchoBotones,
                     alto, colorBotones, colorTextoBotones, 1, 0, 0, 0);  // no tenemos ninguna funcion de activacion y desacticacion

  inicializaBarraTexto(&editorTexto->barraTexto, xEditor + anchoBotones,
                       yEditor, anchoVisualizador, alto, pJuegoCaracteres, separacion, colorFondo, colorTexto,
                       1, "");

  InicializaPulsador(&editorTexto->PulSuma,
                     xEditor + anchoBotones + anchoVisualizador, yEditor,
                     anchoBotones, alto, colorBotones, colorTextoBotones, 1, 0, 0, 0); // no tenemos ninguna funcion de activacion y desacticacion
} // inicializaEditorTexto

//-------------------------------------------------------------------------------------------------------
// funciones: Dibujamos los componentes

void DibujaPulsador(Pulsador *pulsador, int enBlancoYNegro, int transparencia) {
  // extraemos toda la informacion del boton
  uint16_t xBoton = pulsador->Posx;
  uint16_t yBoton = pulsador->Posy;
  uint16_t ancho = pulsador->ancho;
  uint16_t alto = pulsador->alto;
  uint32_t colorFondo = pulsador->colorFondo;
  uint32_t colorTexto = pulsador->colorTexto;

  uint8_t AltoTexto = juegoAlpha15.alto;

  // dibujamos el boton
  char texto[10] = "BOTON";

  LCD_dibujaRectanguloRelleno(xBoton, yBoton, ancho, alto, colorFondo,
                              enBlancoYNegro, transparencia);
  LCD_dibujaCadenaCaracteresAlpha(
      xBoton + ancho / 2 - LCD_anchoCadenaCaracteresAlpha(texto, &juegoAlpha15, 1) / 2,
      yBoton + alto / 2 - AltoTexto / 2, texto, colorTexto, 1, &juegoAlpha15,
      enBlancoYNegro, transparencia);

} // DibujaPulsador


void DibujaInterruptor(Interruptor *interruptor, int enBlancoYNegro,
                       int transparencia) {
  // extraemos toda la informacion de la barra
  uint16_t xInterruptor = interruptor->Posx;
  uint16_t yInterruptor = interruptor->Posy;
  uint16_t ancho = interruptor->ancho;
  uint16_t alto = interruptor->alto;
  uint32_t color = interruptor->color;

  uint8_t AltoTexto = juegoAlpha15.alto;

  // dibujamos el boton
  char texto[20] = "INTERRUPTOR";

  LCD_dibujaRectanguloRelleno(xInterruptor, yInterruptor, ancho, alto, color,
                              enBlancoYNegro, transparencia);
  LCD_dibujaCadenaCaracteresAlpha(
      xInterruptor + ancho / 2 -
          LCD_anchoCadenaCaracteresAlpha(texto, &juegoAlpha15, 1) / 2,
      yInterruptor + alto / 2 - AltoTexto / 2, texto, COLOR_AMARILLO, 1,
      &juegoAlpha15, enBlancoYNegro, transparencia);

} // DibujaInterruptor

void dibujaBarraTexto(BarraTexto *barraTexto, int enBlancoYNegro,
                      int transparencia) {
  uint16_t xBarraTexto = barraTexto->Posx;
  uint16_t yBarraTexto = barraTexto->Posy;
  uint16_t ancho = barraTexto->ancho;
  uint16_t alto = barraTexto->alto;
  uint32_t colorFondo = barraTexto->colorFondo;
  uint32_t colorTexto = barraTexto->colorTexto;
  int alineacion = barraTexto->alineacion;

  uint8_t AltoTexto = (barraTexto->pJuegoCaracteres)->alto;

  LCD_dibujaRectanguloRelleno(xBarraTexto, yBarraTexto, ancho, alto, colorFondo,
                              enBlancoYNegro, transparencia);

  if (alineacion == 0)
    LCD_dibujaCadenaCaracteresAlpha(
        xBarraTexto, yBarraTexto + alto / 2 - AltoTexto / 2, barraTexto->texto,
        colorTexto, 1, &juegoAlpha15, enBlancoYNegro, transparencia);
  else if (alineacion == 1)
    LCD_dibujaCadenaCaracteresAlpha(
        xBarraTexto + ancho / 2 -
            LCD_anchoCadenaCaracteresAlpha(barraTexto->texto, barraTexto->pJuegoCaracteres,
                                           1) /
                2,
        yBarraTexto + alto / 2 - AltoTexto / 2, barraTexto->texto, colorTexto,
        1, &juegoAlpha15, enBlancoYNegro, transparencia);
  else
    LCD_dibujaCadenaCaracteresAlpha(
        xBarraTexto + ancho -
            LCD_anchoCadenaCaracteresAlpha(barraTexto->texto, barraTexto->pJuegoCaracteres, 1),
        yBarraTexto + alto / 2 - AltoTexto / 2, barraTexto->texto, colorTexto,
        1, &juegoAlpha15, enBlancoYNegro, transparencia);

} // dibujaBarraTexto

void dibujaEtiqueta(Etiqueta *etiqueta) {
  int transparencia = 0;
  int habilitacion = 1;
  if (etiqueta->habilitacion == 1)
    habilitacion = 0;
  if (etiqueta->visibilidad == 0)
    transparencia = 100;

  LCD_dibujaImagen(etiqueta->Posx, etiqueta->Posy, etiqueta->ancho,
                   etiqueta->alto, etiqueta->imagen, habilitacion,
                   transparencia);
} // dibujaEtiqueta

void dibujaEditorTexto(EditorTexto *editorTexto, int enBlancoYNegro,
                       int transparencia) {

  // dibujamos el boton de resta
  Pulsador PulResta = editorTexto->PulResta;
  Pulsador PulSuma = editorTexto->PulSuma;
  BarraTexto barraTexto = editorTexto->barraTexto;

  uint16_t AltoTexto = juegoAlpha15.alto;

  char texto[31];
  sprintf(texto, "%d", editorTexto->Cuenta);
  uint16_t AnchoTexto =LCD_anchoCadenaCaracteresAlpha(texto, editorTexto->pJuegoCaracteres, editorTexto->separacion);



  LCD_dibujaCuadrado(PulResta.Posx, PulResta.Posy, PulResta.ancho, PulResta.colorFondo, 0, 0);
  LCD_dibujaCuadrado(PulSuma.Posx, PulSuma.Posy, PulSuma.ancho, PulSuma.colorFondo, 0, 0);
  //dibujaBarraTexto(&barraTexto,0,0);

  LCD_dibujaRectanguloRelleno(
      barraTexto.Posx, barraTexto.Posy, barraTexto.ancho, barraTexto.alto,
      barraTexto.colorFondo, enBlancoYNegro, transparencia);



  LCD_dibujaCaracterAlpha(
      PulResta.Posx +
          (PulResta.ancho - LCD_anchoCaracterAlpha('-', editorTexto->pJuegoCaracteres)) / 2,
      PulResta.Posy + (PulResta.alto - AltoTexto) / 2, '-', PulResta.colorTexto,
	  editorTexto->pJuegoCaracteres, enBlancoYNegro, transparencia);

  LCD_dibujaCaracterAlpha(
      PulSuma.Posx +
          (PulSuma.ancho - LCD_anchoCaracterAlpha('+', editorTexto->pJuegoCaracteres)) / 2,
      PulSuma.Posy + (PulSuma.alto - AltoTexto) / 2, '+', PulSuma.colorTexto,
	  editorTexto->pJuegoCaracteres, enBlancoYNegro, transparencia);

  LCD_dibujaCadenaCaracteresAlpha(
      barraTexto.Posx + (barraTexto.ancho -AnchoTexto) / 2,
      barraTexto.Posy + (PulSuma.alto - AltoTexto) / 2, texto, barraTexto.colorTexto, editorTexto->separacion,
	  editorTexto->pJuegoCaracteres, enBlancoYNegro, transparencia);

} // dibujaEditorTexto



//-------------------------------------------------------------------------------------------------------
// funciones: comportamiento de los componentes

void VerificaBarraProgreso(uint16_t Variable, BarraProgreso *barraProgreso,
                           int enBlancoYNegro, int transparencia) {

  // extraemos toda la informacion de la barra
  uint16_t xBarra = barraProgreso->Posx;
  uint16_t yBarra = barraProgreso->Posy;
  uint16_t ancho = barraProgreso->ancho;
  uint16_t alto = barraProgreso->alto;
  uint32_t color = barraProgreso->color;

  uint8_t AltoTexto = barraProgreso->pJuegoCaracteres->alto; // altura del texto
  char texto[10];

  if (Variable >= ancho - 2) {
    sprintf(texto, "%d", 100); // pasamos el numero entero a texto
    Variable = ancho - 2; // una vez que llega al ancho maximo el rectangulo
                          // deja de cambiar de tamaño
  } else {
    LCD_dibujaRectanguloRelleno(
        xBarra + Variable + 1, yBarra + 1, ancho - Variable - 2, alto - 2,
        0xFF000000, enBlancoYNegro, transparencia); // fondo de la barra
    sprintf(texto, "%d",
            (Variable * 100) / ancho); // pasamos el numero entero a texto
  }
  strcat(texto, "%"); // concatenamos el % al final del texto

  // dibujamos la barra
  LCD_dibujaRectangulo(xBarra, yBarra, ancho, alto, color, enBlancoYNegro,
                       transparencia); // borde de la barra
  LCD_dibujaRectanguloRelleno(xBarra + 1, yBarra + 1, Variable, alto - 2, color,
                              enBlancoYNegro, transparencia);

  Variable = (Variable * 100) / ancho; // hallamos el porcentaje de progreso

  // dibujamos el porcentaje de progreso en el centro de la barra
  // el tipo de letra es fija juegoAlpha15 de color blanco y separacion tenemos
  // 1 pixel
  LCD_dibujaCadenaCaracteresAlpha(
      xBarra + ancho / 2 -
          LCD_anchoCadenaCaracteresAlpha(texto, barraProgreso->pJuegoCaracteres, barraProgreso->separacion) / 2,
      yBarra + alto / 2 - AltoTexto / 2, texto, 0xFFFFFFFF,  barraProgreso->separacion, barraProgreso->pJuegoCaracteres,
      enBlancoYNegro, transparencia);

  // establecemos la salida de la barra
  if (Variable + 1 == 100) {
    barraProgreso->pFuncion();
    barraProgreso->finalizacion = 1;
  } else
    barraProgreso->finalizacion = 0;

} // VerificaBarraProgreso

void fBarraProgreso(BarraProgreso *barraProgreso, int contador) {

  LCD_intercambiaBuffers2(); // dibujamos en el buffer visible

  if (barraProgreso->finalizacion == 0) {

    int finalizacion = barraProgreso->finalizacion;

    VerificaBarraProgreso(contador, barraProgreso, finalizacion, 0);

    if ((contador * 100) / barraProgreso->ancho == 100)
      barraProgreso->pFuncion();

    osDelay(100);
  }

} // fBarraProgreso

void VerificaPulsador(Pulsador *boton, int pulsada, int xPulsacion,
                   int yPulsacion) {

  // si se pulso la pantalla sobre el boton cambiamos el estado del interruptor
  int xBoton = boton->Posx;
  int yBoton = boton->Posy;
  int alto = boton->alto;
  int ancho = boton->ancho;

  if (pulsada == 1) {
    // si se pulso sobre el boton
    if ((xPulsacion >= xBoton && xPulsacion <= xBoton + ancho) &&
        (yPulsacion >= yBoton && yPulsacion <= yBoton + alto) &&
        boton->activacion == 0) { // si la pulsacion es sobre el boton
      if (boton->activacion == 0)
        boton->pFuncionActivacion();
    }
  } else if (boton->activacion == 0)
    boton->pFuncionDesactivacion();

} // VerificaBoton

void VerificaInterruptor(Interruptor *interruptor, int estado, int pulsada,
                         int xPulsacion, int yPulsacion) {

  // si se pulso la pantalla sobre el interruptor cambiamos el estado
  int yInterruptor = interruptor->Posy;
  int xInterruptor = interruptor->Posx;
  int alto = interruptor->alto;
  int ancho = interruptor->ancho;

  if (pulsada &&
      (xPulsacion >= xInterruptor && xPulsacion <= xInterruptor + ancho) &&
      (yPulsacion >= yInterruptor &&
       yPulsacion <=
           yInterruptor + alto)) { // si la pulsacion es sobre el interruptor
    if (interruptor->activacion == 1)
      interruptor->activacion = 0;
    else
      interruptor->activacion = 1; // cambiamos el estado
    osDelay(100);
  }

  interruptor->pFuncionActivacion(
      estado); // ejecutamos la funcion asociada al estado del interruptor

} // VerificaInterruptor

void verificaEditorTexto(EditorTexto *editorTexto, int pulsada, int xPulsacion,
                         int yPulsacion) {

  if (pulsada && editorTexto->pulsacionAnterior == 0) {

    BarraTexto barraTexto = editorTexto->barraTexto;
    Pulsador PulResta = editorTexto->PulResta;
    Pulsador PulSuma = editorTexto->PulSuma;
    uint8_t AltoTexto = juegoAlpha15.alto;

    // verificamos si se pulsa el boton de resta
    if (editorTexto->Cuenta > 0 &&
        (xPulsacion >= PulResta.Posx &&
         xPulsacion <= PulResta.Posx + PulResta.ancho) &&
        (yPulsacion >= PulResta.Posy &&
         yPulsacion <= PulResta.Posy + PulResta.alto)) {

      editorTexto->Cuenta = editorTexto->Cuenta - editorTexto->incremento;
      editorTexto->pulsacionAnterior = 1;
    }
    // verificamos si se pulsa el boton de suma
    if ((xPulsacion >= PulSuma.Posx &&
         xPulsacion <= PulSuma.Posx + PulSuma.ancho) &&
        (yPulsacion >= PulSuma.Posy &&
         yPulsacion <= PulSuma.Posy + PulSuma.alto)) {
      editorTexto->Cuenta = editorTexto->Cuenta + editorTexto->incremento;
      editorTexto->pulsacionAnterior = 1;
    }

    // redibujamos el vusualizador para mostrar el nuevo numero
    char texto[31];
    sprintf(texto, "%d", editorTexto->Cuenta);


    LCD_dibujaRectanguloRelleno(barraTexto.Posx, barraTexto.Posy,
                                barraTexto.ancho, barraTexto.alto,
                                barraTexto.colorFondo, 0, 0);

    LCD_dibujaCadenaCaracteresAlpha(barraTexto.Posx + (barraTexto.ancho - LCD_anchoCadenaCaracteresAlpha(texto, editorTexto->pJuegoCaracteres, editorTexto->separacion)) / 2,
                                    barraTexto.Posy +
                                        (editorTexto->alto - AltoTexto) / 2,
                                    texto, barraTexto.colorTexto, editorTexto->separacion, editorTexto->pJuegoCaracteres, 0, 0);
  } else
    editorTexto->pulsacionAnterior = 0;

} // verificaEditorTexto
