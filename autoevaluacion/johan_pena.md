# Autoevaluación — Proyecto 2: System Calls en xv6
 
**Nombre:** Johan Peña
 
## Aporte al proyecto
Participé en las pruebas de las syscalls `trace` y `sysinfo` en QEMU, verificando casos exitosos, casos de error y concurrencia.
 
## Comprensión de la implementación
Entiendo `trace` como una especie de "espía" activado a nivel del kernel: uno le dice qué syscall vigilar por su nombre, y a partir de ahí, cada vez que esa syscall se dispara en cualquier parte del sistema, aparece en consola un reporte con el proceso que la llamó, el resultado que dio y el estado de algunos registros en ese momento. No es algo que uno active por proceso, sino que queda "prendido" para todo el sistema hasta que se reinicia.
 
Por su parte, `sysinfo` es más como una fotografía del estado del sistema en un instante dado: cuánta RAM queda libre, cuántas páginas de memoria están ocupadas, cuántas disponibles, y cuántos procesos están esperando su turno para correr. El kernel arma esos números y se los entrega al programa que los pidió.
 
## Dificultades encontradas
Lo más difícil fue entender por qué `trace` afectaba a procesos que no había mencionado, como la propia shell. Lo resolví revisando que es un interruptor global y repitiendo pruebas hasta confirmar el comportamiento.
 
## Aprendizajes
Aprendí a verificar el funcionamiento de un sistema probando casos de error y concurrencia, no solo el caso ideal, y cómo un kernel expone información interna a los programas de usuario mediante una syscall.
 
## Uso de herramientas de IA
Utilicé IA para apoyar el diseño de mis pruebas y resolver dudas puntuales sobre el comportamiento del sistema. Verifiqué los resultados ejecutando cada prueba en QEMU.
 
## Autocalificación
**Nota propuesta (0.0–5.0): 4.0**
 
**Justificación:** Cumplí con mi rol de pruebas y verificación, y comprendo el funcionamiento de ambas syscalls, aunque no participé en la implementación directa del código.
