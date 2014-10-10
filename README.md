#CSA : Corrector de saturación de audio.


#Sintaxis: 
    $ csa   <archivo_de_audio_original>  <ganancia> <offset> [p]


Como archivo de audio original usa uno con muestras de audio mono canal en formato PCM, little endian, con signo y de 16 bits. La fecuencia de muestreo debe ser de 8KHz.
La ganancia es un número real mayor que 1 usado para simular saturación en el proceso de grabado.

Offset permite especificar el tiempo en [ms] desde el inicio del archivo a mostrar en los gráficos generados por la aplicación.
Si el parámetro p es ingresado, csa reproduce el archivo de audio original completo, luego la versión saturada y finalmente la corregida.

#Descripción
csa muestra tres gráficos de 40 [ms] desde el tiempo especificado por offset respecto del inicio del archivo: 
    *El primer gráfico muestra el segmento de señal de audio original, 
    *El segundo muestra el segmento de la señal saturada, y 
    *El tercero el segmento de la señal corregida.  
    *El cuarto, muestra el error para la correción hecha al archivo.(1)

csa termina cuando el usuario presiona cualquier tecla.

(1) El índice de error E se calcula como:
    E = Suma de i=0 a i=N de (Si - si)2. donde Si es la señal original, si es la señal corregida, N es el número de muestras del archivo de audio.
    Para uniformar el indicador de error, csa aplica una atenuación de igual valor que la ganancia. Así las muestras no saturadas logran aproximar bien a las muestras de la señal original.    

NOTA: Si bien los gráficos muestran solo 40 milisegundos de la señal, el procesamiento es realizado sobre toda la señal (archivo completo).
