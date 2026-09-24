# Autoevaluación — Proyecto 2: System Calls en xv6

**Nombre:** Cristian Cabezas

## Aporte al proyecto

Participé en la implementación y revisión de las syscalls `trace` y `sysinfo`, las pruebas en QEMU, la documentación del README y la organización del repositorio.

## Comprensión de la implementación

`trace` recibe el nombre de una syscall, busca su número y lo guarda para monitorearla. Cuando se ejecuta, el kernel imprime su PID, nombre, retorno y registros.

`sysinfo` obtiene la memoria libre, las páginas usadas y disponibles y los procesos `RUNNABLE`. Los datos se guardan en una estructura y se copian al programa de usuario mediante `copyout`.

## Dificultades encontradas

Lo más difícil fue entender el recorrido de una syscall, el uso de los registros de RISC-V y la transferencia de datos entre el kernel y el usuario. Lo resolví revisando el código de xv6 y realizando pruebas en QEMU.

## Aprendizajes

Aprendí cómo se crean e integran syscalls en xv6, cómo funciona el cambio entre modo usuario y kernel, cómo se administra la memoria y cómo se utilizan los locks para acceder de forma segura a estructuras compartidas.

## Uso de herramientas de IA

Utilicé IA para analizar los requerimientos, apoyar el diseño, revisar el código y preparar la documentación. Verifiqué los resultados compilando y probando la solución en QEMU, y estudié el código para poder explicarlo.

## Autocalificación

**Nota propuesta (0.0–5.0):** 4.8

**Justificación:** Cumplí con mis responsabilidades, participé en el desarrollo y las pruebas, y comprendí la implementación. Considero que el proyecto cumple los requerimientos y funciona correctamente.
