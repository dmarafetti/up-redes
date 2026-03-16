# Trabajo Práctico - Programación en Redes
### Sockets - Web Server Implementation

**Revisión:** 3.0  
**Año:** 2026
**Profesor:** Ing. Diego Marafetti

# Contenido

- [Objetivos y definición](#objetivos-y-definición)
- [Entregas](#entregas)
  - [Iteración 1](#iteración-1)
  - [Iteración 2](#iteración-2)
  - [Iteración 3](#iteración-3)
  - [Iteración 4](#iteración-4)
  - [Iteración 5](#iteración-5)


## Objetivos y definición

El trabajo práctico se basa en la implementación del primer prototipo de un sistema distribuido que permita mostrar contenido al usuario implementando una versión simplificada del protocolo HTTP. Será una aplicación muy simple programada en C que se ejecutará sobre la plataforma Unix. Utilizará el protocolo HTTP v1.1 para aceptar como cliente a un browser (por ejemplo, Google Chrome o Firefox). Expondrá una serie de endpoints a través de los cuales los clientes podrán acceder a los recursos estáticos y dinámicos, aunque en principio solo habrá uno.

Se espera que:
- ✓ El alumno se familiarice con la API de Berkeley Sockets.
- ✓ Entiendan la importancia de una norma o protocolo estándar en la comunicación entre procesos.
- ✓ Dominen los problemas específicos de este tipo de implementaciones.
- ✓ Se fomente el uso de herramientas e Inteligencia Artificial (IA) generativa (ej. LLMs) como asistentes de desarrollo. Los alumnos podrán utilizarlas para consultar documentación técnica de sistemas operativos, generar código, o validar patrones de diseño de red, siempre bajo la premisa de comprender en profundidad cada línea de código implementada.

**Algunas consideraciones extra:**
- ✓ El lenguaje de programación que se utilizará será ANSI C.
- ✓ El protocolo que se utilizará será TCP sobre IPv4.
- ✗ No se pueden utilizar bibliotecas externas para manejar sockets. Solo se puede usar la API de Berkeley directamente.

> [!IMPORTANT]
> Ver requerimientos y entorno de desarrollo

<br>

---

## Entregas

El desarrollo del TP estará dividido en 5 iteraciones con una demo de la aplicación al final de cada una.

### Iteración 1
Se desarrollará la primera versión del webserver. Este proceso será un programa que se levantará por consola con la posibilidad de recibir ciertos parámetros como el puerto de escucha y el modelo de concurrencia. El servidor deberá soportar dos esquemas arquitectónicos, configurables en tiempo de ejecución mediante el argumento `--concurrency=process|thread`:

* **Multithread** (`thread`): Cada request recibido deberá atenderse en un nuevo hilo. Se utilizará el soporte de la librería `pthreads` y los threads serán creados en modo *detached* (ver `man pthread_create`).
* **Multiproceso** (`process`): Cada request recibido deberá atenderse delegando el trabajo a un nuevo proceso hijo utilizando la *system call* `fork()`. 

En ambos casos, la entidad encargada (proceso o hilo) deberá finalizar al terminar de atender el pedido, liberando todos los recursos asociados de forma prolija para evitar *zombies* o fugas de memoria. En paralelo, se desarrollará un pequeño proceso cliente cuya única función será la de establecer una conexión TCP al webserver. Enviará un mensaje con el string “PING” y el server responderá con el string “PONG”. Al recibir la respuesta, la imprimirá por consola y finalizará adecuadamente.

### Iteración 2
Para esta iteración se modificará el webserver desarrollado para agregar soporte a *non-blocking I/O* sobre un *single-thread*. Esta nueva modalidad se activará extendiendo las opciones del parámetro de consola implementado en la fase anterior, utilizando el valor `--concurrency=polling`. 
Al configurarse en este modo, el servidor utilizará la función `select()` o `poll()` para atender todas las conexiones TCP entrantes y administrar los descriptores de archivo simultáneamente sin bloquear el flujo principal de ejecución. El resto de la funcionalidad responderá igual que en la iteración anterior, manteniendo la compatibilidad con el cliente PING/PONG.

### Iteración 3
El proceso webserver levantará un thread el cual creará un socket UDP cuyo único propósito será el de responder 
al mensaje de *heartbeat*. Al mismo tiempo, se programará un pequeño proceso que, periódicamente y de forma indefinida, 
envíe el mensaje al server para indicar la normal operación del mismo. Si durante cierto período de tiempo o tras 
varios intentos no se recibió respuesta, entonces se deberá indicar por consola sobre dicho evento.

### Iteración 4
El webserver implementará una versión muy reducida del protocolo HTTP. El cliente programado para testear el 
servidor será dado de baja y, a partir de ahora, los clientes serán los browsers del mercado. 
El único endpoint que expone el servidor es `HTTP GET http://direcciónIp:puerto/imagen.jpg`. 

Un request a dicha URL deberá traer el archivo de imagen. Para el envío de los datos, el servidor **deberá implementar "HTTP Chunked Transfer Encoding"**. Esto requerirá:
1. Configurar en la respuesta HTTP el header correspondiente: `Transfer-Encoding: chunked`.
2. Leer y enviar la imagen fragmentada (*chunks*) especificando el tamaño de cada bloque en formato hexadecimal según la especificación del protocolo HTTP/1.1.
3. Finalizar correctamente la transmisión enviando un *chunk* de tamaño cero.

![Screenshot 2025-02-27 at 15 44 18](https://github.com/user-attachments/assets/8fc3cce3-987f-4bc3-81f6-c461735a8b86)

### Iteración 5
Como último paso, la implementación multithread del webserver deberá incluir un *pool* de threads cuyo 
tamaño de *worker threads* será variable por línea de comando. Cuando se acepte una nueva conexión, 
se deberá seleccionar el primer thread del *pool* para atender la tarea. 
Al finalizar, el thread deberá volver al *pool* sin finalizar. En el caso de no haber thread disponible, 
la tarea deberá quedar en espera hasta que haya uno disponible nuevamente. La opción de *timeout* queda a 
criterio del alumno.
