# STM32F429I-DISC1-LCD_COMPONENTS
Practica 1 de la asignatura 'Sistemas de control a tiempo real' de la Uvigo curso 24-25

## Instalación de la libreria en un proyecto
1. Creamos el proyecto en "stm32cubeide" y lo llamamos, por ejemplo, como `proyectoSTM32`
2. Para añadir esta libreria al proyecto `proyectoSTM32` clonamos el repositorio en la carpeta "Middlewares" -> `proyectoSTM32/Middlewares/`
3. En la configuracion del proyecto haciendo click derecho en el nombre del proyecto `Properties/'C/C++ Build'/Settings/'MCU/MPU GCC Compiler'/'Include paths'` añadimos todas las carpetas de la libreria haciendo click en el icono señalado con la flecha roja

![image](https://github.com/user-attachments/assets/3d423f3b-959d-42b5-9787-a7b40991bb86)

NOTA: Para incluir los directorios en el `Include paths` tambien se puede copiar y pegar las siguientes lineas en el primer cuadro blanco

``` python
"${workspace_loc:/${ProjName}/Middlewares/STM32F429I-DISC1-LCD/interfazLCD/Colores}"
"${workspace_loc:/${ProjName}/Middlewares/STM32F429I-DISC1-LCD/interfazLCD/Componentes}"
"${workspace_loc:/${ProjName}/Middlewares/STM32F429I-DISC1-LCD/interfazLCD/Imagenes}"
"${workspace_loc:/${ProjName}/Middlewares/STM32F429I-DISC1-LCD/pantallaLCD/Componentes}"
"${workspace_loc:/${ProjName}/Middlewares/STM32F429I-DISC1-LCD/pantallaLCD/Imagenes}"
../Core/Inc
"${workspace_loc:/${ProjName}/Middlewares/STM32F429I-DISC1-LCD/pantallaLCD/juegosCaracteres}"
"${workspace_loc:/${ProjName}/Middlewares/STM32F429I-DISC1-LCD/Dependencias/Src}"
"${workspace_loc:/${ProjName}/Middlewares/STM32F429I-DISC1-LCD/pantallaLCD}"
"${workspace_loc:/${ProjName}/Middlewares/STM32F429I-DISC1-LCD/interfazLCD}"
"${workspace_loc:/${ProjName}/Middlewares/STM32F429I-DISC1-LCD/Dependencias}"
"${workspace_loc:/${ProjName}/Middlewares/STM32F429I-DISC1-LCD}"
"${workspace_loc:/${ProjName}/Middlewares/STM32F429I-DISC1-LCD/Dependencias/Inc}"
```
