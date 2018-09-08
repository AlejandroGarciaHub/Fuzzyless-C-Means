# Titulo del proyecto

# "Fuzzyless C-Means (FLCM)"

## Pasos iniciales

A continuación se mencionan las instrucciones para la ejecución del algoritmo en el equipo de computo.

### Prerrequisitos

Que se necesita instalar para ejecutar el algoritmo

```
- Compilador g++ 
- IDE o editor de textos (XCode, CodeBlocks, Sublimetext, etc.) (opcional)

```

### Instalación

1.- Copiar el archivo fuente main.c al directorio de trabajo o añadirlo al proyecto del IDE en que se este trabajando. 

2.- Verificar que se tenga instalado el compilador para g++, de no tenerlo se deben seguir los pasos correspondientes al sistema operativo en el cual se esté trabajando (Linux, MacOS, Windows)  

```
Linux (si se utiliza una distribución debian el siguente comando puede ser suficiente)
sudo apt-get install g++

MacOS
Instalando XCode se añadiran los compiladores correspondientes para utilizarlos desde el IDE o desde la Terminal

Windows
Instala MinGW, este es un manejador de utilidades que permite instalar el compilador g++
```

## Ejecutar el algoritmo

A continuacion se especifica como preparar los datos para ser leídos por el algoritmo, así como su ejecución

### Preparar los datos a ser clasificados

Los datos deben preferentemente de contenerse en directorios con la estructura explicada a continuación. Importante considerar que el ejecutable 'main' debe preferentemente encontrarse al mismo nivel que los directorios mencionados a continuación (Datos, Centroides, Resultados)

```
main.c
Datos/{datos.txt}/

```
Dentro de la carpeta Datos se deben de encontrar los archivos de texto con todos los datos que se clasificaran, la estructura de directorios debe ser la siguiente:

```

main.c
Datos/
		1000.dat
		4000.dat
		250000.dat

```
La estructura interna de los archivos debe ser de una fila por cada registro, y una columna para cada característica, separando las características por espacios simples o tabulaciones. No deben existir filas con sintaxis distintas, mas o menos columnas o menos registros que los indicados en el nombre del archivo, esto para obtener resultados certeros o evitar errores de ejecución.

Ejemplo de 2 registros con 4 características:

```

5.1 3.5 1.4 0.2 
4.9 3.0 1.4 0.2 

```
--------------------------------------

### Directorio de centroides

Para la carpeta contenedora de los centroides se siguen el mismo estilo de lineamientos mencionados anteriormente. Tanto la carpeta Datos como la carpeta Centroides deben de encontrarse al mismo nivel.

```

main.c
Datos/
Centroides/{Tamaño-Instancia}/{Numero de centroides}/{centroides.txt}

```

Dentro de cada directorio deben de contenerse todos los conjuntos de centroides que se quieran utilizar. Todos los archivos de centroides deben de contener el mismo numero de filas (centroides) que se especifica en el nombre del directorio, y deben de tener el mismo numero de columnas (características).

Ejemplo

```

main.c
Datos/
Centroides/
      1000/
	        150/
	       		centroides1.txt
	       		centroides2.txt
	       		centroides3.txt
```

### Directorio de resultados

Adicionalmente es preferible tener un directorio de resultados, ya sea como se menciona a continuación o utilizando la ruta de preferencia

```

main.c
Datos/
Centroides/
Resultados/{Tamaño-Instancia}/{Numero de centroides}/{resultados.txt}

```

Dentro de cada directorio deben de encontrarse la cantidad de datos

Ejemplo
```

main.c
Datos/
Centroides/
Resultados/
      1000/
	        150/
	       		resultadosConMejora.txt
	       		resultadosSinMejora.txt
```

### Ejecución

Para ejecutar el algoritmo pueden hacerse pruebas individuales o utilizar un script Korn Shell para realizar varias pruebas consecutivas.


#### Prueba individual

Abrir una Terminal y situarse en el directorio donde se encuentran las carpetas Datos, Centroides y Resultados
```

main.c
Datos/
Centroides/
Resultados/

```
Compilar el código fuente si no se ha hecho.

```
g++ main.c -o main
```


Ejecutar el ejercicio de agrupamiento

```
./main {parametro_2} {parametro_2} {parametro_3} {parametro_4} {parametro_5} {parametro_6} {parametro_7} {parametro_8} {parametro_9} {parametro_10} {parametro_11}
```
* {parametro_1} = ruta del archivo con los datos
* {parametro_2} = numero de datos
* {parametro_3} = numero de características
* {parametro_4} = ruta del archivo con los centroides
* {parametro_5} = numero de centroides
* {parametro_6} = grado de difusividad (utilizar el indicado y experimentar con otros valores si es necesario)
* {parametro_7} = épsilon (utilizar el indicado y experimentar con otros valores si es necesario)
* {parametro_8} = bandera booleana para activar o desactivar la mejora (0 == desactivada, 1 == activada)
* {parametro_9} = numero de iteración a partir de la cual se aplicara la mejora
* {parametro_10} = ruta donde se guardaran los resultados del experimento (numero de iteraciones, tiempo, error)
* {parametro_11} = umbral de cercanía (utilizar el indicado y experimentar con otros valores si es necesario)

Ejemplo
```
./main Datos/150.dat 150 4 Centroides/150/3/centroides1.txt 3 2.0 0.1 0 2 Resultados/150/3/ConMejora/resultados_test.txt 0.1
```

#### Script Korn Shell

Abrir una Terminal y situarse en el directorio donde se encuentran las carpetas Datos, Centroides y Resultados
```

main.c
Datos/
Centroides/
Resultados/

```
Crear un documento con la extensión .ksh y añadir un código similar al mostrado a continuación, dependiendo de las pruebas a realizar.

```

for var1 in 150 200
do
	for var2 in 3 10
	do
		for var3 in {1..30}
		do
			./fcm Datos/$var1.dat $var1 4 Centroides/$var1/$var2/centroides$var3.txt $var2 2.0 0.1 1 2 Resultados/$var1/$var2/ConMejora/resultados.txt 0.1
		done
	done	
done
```

En el código anterior var1 tomar valor de 150 y 200 siendo este el nombre de las instancias y a su vez el numero de datos que contienen. var2 toma valor de 3 y 10 siendo este el nombre de las carpetas con los centroides y a su vez el numero de centroides que contiene cada archivo. Var3 toma valores desde el 1 hasta el 30 siendo esto el numero de iteraciones que se quieran realizar con los mismo parametros, 30 es un numero viable para realizar posteriormente un promedio de los resultados obtenidos. 
En el ejemplo mostrado se realizarían 60 pruebas para cada una de las dos instancias, 30 con 3 centroides y 30 con 10 centroides. Se debe considerar que no todas las instancias llegan a tener el mismo numero de características (en el ejemplo son 4), por lo tanto se debe actualizar este valor en tiempo de ejecución, para lo cual se pueden utilizar estructuras de decisión como lo menciona la documentación [Ksh scripting](http://www.well.ox.ac.uk/~johnb/comp/unix/ksh.html).

* {var1} = nombre de las instancias y cantidad de datos en las mismas
* {var2} = nombre de las carpetas de los centroides y cantidad de centroides contenidos en los archivos de las mismas
* {var3} = cantidad de iteraciones que se quieren realizar bajo los mismos parametros para despues promediar los resultados


## Build realizados con

* [XCode](https://developer.apple.com/xcode/) - IDE utilizado
* [G++](https://gcc.gnu.org/gcc-5/) - Compilador

## Autor

* **Miguel Garcia** - *Trabajo inicial* - [MG08](https://github.com/MG08/Fuzzyless-C-Means)


## README en PDF generado en

* [Dillinger](https://dillinger.io/) - Markdown a PDF
