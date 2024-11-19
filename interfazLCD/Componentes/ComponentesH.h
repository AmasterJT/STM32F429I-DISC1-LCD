#ifndef Componentes_H_
#define Componentes_H_


#include <stdio.h>	   // Para sprintf()
#include <stdlib.h>	   // para abs()
#include <cmsis_os2.h> // para osDelay()
#include <math.h>
#include <string.h>
#include <stdint.h>
#include "pantallaLCD.h"

// -----------------------------------------------------------------------------------------------------------------
// estructuras de datos

typedef struct{
	// la barra se situa en las corrdenadas (Posx, Posy) y tiene un ancho, alto, color
	// al completarse la carga se devuelve un booleano cierto: finalizacion = 1
	// el color debbe ser expresado en formato ARGB hexadecimal
	uint16_t Posx;
	uint16_t Posy;
	uint16_t ancho;
	uint16_t alto;
	uint32_t color;

	const LCD_JuegoCaracteresAlpha * pJuegoCaracteres;
	uint8_t separacion;

	void (* pFuncion)(void);

	int finalizacion; // parametro que indica si la barra de progreso esta al 100%
}BarraProgreso;


typedef struct{
	// el boton se situa en las corrdenadas (Posx, Posy) y tiene un ancho, alto, color y un texto
	// por defecto el boton devuelve un 1 si se pulsa, pero puede cambiarse: pulsacion = 1
	// el color debbe ser expresado en formato ARGB hexadecimal
	uint16_t Posx;
	uint16_t Posy;
	uint16_t ancho;
	uint16_t alto;
	uint32_t colorFondo;
	uint32_t colorTexto;

	void (* pFuncionActivacion)(void);
	void (* pFuncionDesactivacion)(void);

	int pulsado;
	int activacion; // indica que el boton fue pulsado
}Pulsador;


typedef struct{
	// el interruptor se situa en las corrdenadas (Posx, Posy) y tiene un ancho, alto, color y un texto
	// por defecto el boton devuelve un 1 si se activa, pero puede cambiarse: activacion = 1
	// el color debbe ser expresado en formato ARGB hexadecimal
	uint16_t Posx;
	uint16_t Posy;
	uint16_t ancho;
	uint16_t alto;
	uint32_t color;

	void (* pFuncionActivacion)(int estado);
	//void (* pFuncionDesactivacion)(void);

	int activacion; // indica que el boton fue pulsado
}Interruptor;



typedef struct{

	uint16_t Posx;
	uint16_t Posy;
	uint16_t ancho;
	uint16_t alto;

	const LCD_JuegoCaracteresAlpha * pJuegoCaracteres;
	uint8_t separacion;

	uint32_t colorFondo;
	uint32_t colorTexto;
	int alineacion; // 1: izquierda, 2: centrado, 3: derecha
	char texto[31]; // texto a motrar en la imagen

}BarraTexto;


typedef struct{

	uint16_t Posx;
	uint16_t Posy;
	uint16_t ancho;
	uint16_t alto;

	const uint8_t * imagen;

	int habilitacion;
	int visibilidad;

}Etiqueta;



typedef struct{
	uint16_t Posx;
	uint16_t Posy;
	uint16_t anchoBotones;
	uint16_t anchoVisualizador;
	uint16_t alto;

	uint32_t colorFondo;
	uint32_t colorTexto;

	uint32_t colorBotones;
	uint32_t colorTextoBotones;

	int Cuenta;
	int incremento;

	Pulsador PulSuma;
	Pulsador PulResta;
	BarraTexto barraTexto;

	const LCD_JuegoCaracteresAlpha * pJuegoCaracteres;
	uint8_t separacion;

	int pulsacionAnterior;


}EditorTexto;


//------------------------------------------------------------------------------------------------------------
// funciones: inicialiszacion de los componenetes

void InicializaBarraProgreso(BarraProgreso *barraProgreso, int16_t xBarra,
                             uint16_t yBarra, uint16_t ancho, uint16_t alto,  const LCD_JuegoCaracteresAlpha * pJuegoCaracteres, uint8_t separacion,
                             uint32_t color, void (*pFunc)(void));

void InicializaPulsador(Pulsador *pulsador, int16_t xPulsador, uint16_t yPulsador, uint16_t ancho, uint16_t alto, uint32_t colorFondo,  uint32_t colorTexto, int activacion, int pulsado, void (*pFuncAct)(void),  void (*pFuncDes)(void));

void InicializaInterruptor(Interruptor * interruptor, int16_t xInterruptor, uint16_t yInterruptor, uint16_t ancho, uint16_t alto, uint32_t color, uint16_t activacion,  void (*pFunc)(int estado));

void InicializaInterruptor(Interruptor *interruptor, int16_t xInterruptor,
                           uint16_t yInterruptor, uint16_t ancho, uint16_t alto,
                           uint32_t color, uint16_t activacion,
                           void (*pFunc)(int estado));

void inicializaEtiqueta(Etiqueta * etiqueta, uint16_t xEtiqueta, uint16_t yEtiqueta, uint16_t ancho, uint16_t alto, const uint8_t * imagen, int habilitacion, int visibilidad);

void inicializaBarraTexto(BarraTexto *barraTexto, uint16_t xBarraTexto,
                          uint16_t yBarraTexto, uint16_t ancho, uint16_t alto,
						  const LCD_JuegoCaracteresAlpha * pJuegoCaracteres, uint8_t separacion, uint32_t colorFondo,
						  uint32_t colorTexto, int alineacion, char *texto);
void inicializaEditorTexto(EditorTexto *editorTexto, uint16_t xEditor,
                           uint16_t yEditor, uint16_t anchoBotones,
                           uint16_t anchoVisualizador, uint16_t alto,
						   const LCD_JuegoCaracteresAlpha * pJuegoCaracteres, uint8_t separacion,
                           uint32_t colorFondo, uint32_t colorTexto,
                           uint32_t colorBotones, uint32_t colorTextoBotones,
                           int Cuenta, int incremento);



//-----------------------------------------------------------------------------------------------------------
// funciones: dibujamos los componentes

void DibujaPulsador(Pulsador * pulsador, int enBlancoYNegro, int transparencia);

void DibujaInterruptor(Interruptor * interruptor, int enBlancoYNegro, int transparencia);

void dibujaBarraTexto(BarraTexto * barraTexto, int enBlancoYNegro, int transparencia);

void dibujaEtiqueta(Etiqueta * etiqueta);

void dibujaEditorTexto(EditorTexto * editorTexto, int enBlancoYNegro, int transparencia);

//------------------------------------------------------------------------------------------------------------
// funciones: Comportamiento de los componentes

void fBarraProgreso(BarraProgreso * barraProgreso, int contador);

void VerificaBarraProgreso(uint16_t Variable, BarraProgreso * barraProgreso, int enBlancoYNegro, int transparencia);

void VerificaPulsador(Pulsador * boton, int pulsada, int xPulsacion, int yPulsacion);

void VerificaInterruptor(Interruptor * interruptor , int estado, int pulsada, int xPulsacion, int yPulsacion);

void verificaEditorTexto(EditorTexto * editorTexto, int pulsada, int xPulsacion, int yPulsacion);

#endif /* Componentes_H_ */
