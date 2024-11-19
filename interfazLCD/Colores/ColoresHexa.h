
#ifndef ColoresHexa_H_
#define ColoresHexa_H_

#include <stdint.h>

/*
 * Los colores estan en formato hexadecimal ARGB de 32 bits
 * todos los collores son opacos --> sin transparencia --> FF
 * Transparencia (A): 8 bits
 * Rojo (R): 8 bits
 * Verde (G): 8 bits
 * Azul (B): 8 bits
 */

// negro
#define COLOR_NEGRO 0xFF000000

// blanco
#define COLOR_BLANCO 0xFFFFFFFF

// gris
#define COLOR_GRIS_OSCURO 0xFF353535
#define COLOR_GRIS 0xFF707070
#define COLOR_GRIS_CLARO 0xFFB0B0B0

// amarillo
#define COLOR_AMARILLO_OSCURO 0xFFC0BA08
#define COLOR_AMARILLO 0xFFFFF000
#define COLOR_AMARILLO_CLARO 0xFFEFFF82

// azul
#define COLOR_AZUL_OSCURO 0x00009A
#define COLOR_AZUL 0xFF0000FF
#define COLOR_AZUL_CLARO 0xFF2873FF

// rojo
#define COLOR_ROJO_OSCURO 0xFF9B0000
#define COLOR_ROJO 0xFFFF0000
#define COLOR_ROJO_CLARO 0xFFE35050

// verde
#define COLOR_VERDE_OSCURO 0xFF007100
#define COLOR_VERDE 0xFF00FF00
#define COLOR_VERDE_CLARO 0xFF79FF79

// rosa
#define COLOR_ROSA_OSCURO 0xFF3a0b30
#define COLOR_ROSA 0xFFFF00DD
#define COLOR_ROSA_CLARO 0xFF3a0b30

// naranja
#define COLOR_NARANJA_OSCURO 0xFFCE4100
#define COLOR_NARANJA 0xFFFF6000
#define COLOR_NARANJA_CLARO 0xFFEDA31A

// cian
#define COLOR_CIAN_OSCURO 0xFF04958D
#define COLOR_CIAN 0xFF1AEDE1
#define COLOR_CIAN_CLARO 0xFF8AF3ED

// violeta
#define COLOR_VIOLETA_OSCURO 0xFF5D2F70
#define COLOR_VIOLETA 0xFFB400FF
#define COLOR_VIOLETA_CLARO 0xFFBF8FD3

#endif /* ColoresHexa_H_ */
