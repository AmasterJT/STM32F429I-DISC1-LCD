# 🚀 STM32F429I-DISC1-LCD
Practica 1 de la asignatura 'Sistemas de control a tiempo real' de la Uvigo curso 24-25

## 🏆 Instalación de la libreria en un proyecto

### Tenemos dos formas de importar las carpetas

#### 🔵 Forma 1:
1. Creamos el proyecto en "stm32cubeide" y lo llamamos, por ejemplo, como `Practica1_24-25`
2. Para añadir esta libreria al proyecto `Practica1_24` clonamos el repositorio en la carpeta "Middlewares" -> `Practica1_24/Middlewares/`
3. En la configuracion del proyecto haciendo click derecho en el nombre del proyecto `Properties`
4. Navegamos hasta `Properties/'C/C++ Build'/Settings/'MCU/MPU GCC Compiler'/'Include paths'`
5. Añadimos todas las carpetas de la libreria haciendo click en el icono ![image](https://github.com/user-attachments/assets/2d0c4e31-ee0d-43be-844a-34a703ad98d3)
 señalado con la flecha roja

![image](https://github.com/user-attachments/assets/7702750a-d4ab-49eb-8bdd-0ba61ec8b85a)

5. Hacemos click en `Workspace...`

![image](https://github.com/user-attachments/assets/80266d75-9541-4e03-8599-7be901582192)

6. Buscamos las carpetas y subcarpetas de `Practica1_24/Middlewares/STM32F429I-DISC1-LCD`

![image](https://github.com/user-attachments/assets/4014c379-9969-4f9e-9ac7-7291261fe5f9)

‼️ No seleccionar las carpetas `.git`, `ENUNCIADO` y `Documentacion`
⚠️ hacer repetidamente los pasos 5 y 6 hasta terminar de añadir todas las carpetas 

Ejemplo video


#### 🔵 Forma 2:

1. hacemos los pasos 1, 2, 3 y 4 de la `forma1`
2. Copiar y pegar las siguientes lineas en el primer cuadro blanco

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
