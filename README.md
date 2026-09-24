# Proyecto 2 — System Calls en xv6

Sistemas Operativos 2026-2 · Universidad EAFIT
Docente: José Luis Montoya Pareja

## Información General

- **Nombre del proyecto:** System Calls en xv6 — implementación de `trace` y `sysinfo`.
- **Integrantes:**
  - Cristian Cabezas
  - Johan Peña
- **Base:** [xv6-riscv](https://github.com/mit-pdos/xv6-riscv) (repositorio oficial del MIT).

## Descripción de la Solución

Se extendió el kernel de xv6-riscv con dos nuevas llamadas al sistema orientadas al monitoreo e introspección:

- **`trace(char *nombre_syscall)`**: activa el monitoreo de una syscall específica identificada por su nombre (por ejemplo `"sys_kill"`). A partir de ese momento, cada vez que **cualquier** proceso invoque esa syscall, el kernel imprime en consola: PID del proceso, nombre de la syscall, valor de retorno y un conjunto de registros RISC-V relevantes (`s0`, `s1`, `a0`, `a1`).
- **`sysinfo(struct sysinfo *info)`**: recopila el estado actual del sistema (memoria libre en MB, páginas usadas, páginas disponibles y número de procesos en estado `RUNNABLE`) y lo transfiere al espacio de usuario mediante una estructura `struct sysinfo` definida por el equipo.

Se incluyen dos programas de usuario para probar ambas syscalls:

- `user/trace.c` — habilita el trace sobre una syscall y luego ejecuta (`exec`) el comando indicado, de forma que las syscalls que este dispare durante su ejecución queden monitoreadas.
- `user/sysinfo.c` — invoca `sysinfo()` e imprime los cuatro valores reportados.

## Archivos Modificados

| Archivo | Tipo | Descripción |
|---|---|---|
| `kernel/syscall.h` | Modificado | Se agregan los números `SYS_trace` (23) y `SYS_sysinfo` (24), y la constante `MAXSYSCALLNAME`. |
| `kernel/syscall.c` | Modificado | Se agrega la tabla `syscall_names[]` (nombre ↔ número de syscall), la variable global `traced_syscall_num`, la función `syscall_num_by_name()`, las entradas de `trace`/`sysinfo` en la tabla `syscalls[]`, y se modifica el dispatcher `syscall()` para imprimir la información de trace cuando corresponde. |
| `kernel/sysproc.c` | Modificado | Se agregan los handlers `sys_trace()` y `sys_sysinfo()`. |
| `kernel/kalloc.c` | Modificado | Se agregan `freepages()` (páginas libres en la freelist) y `totalpages()` (páginas físicas totales gestionadas por el kernel). |
| `kernel/proc.c` | Modificado | Se agrega `countrunnable()`, que recorre la tabla de procesos y cuenta los que están en estado `RUNNABLE`. |
| `kernel/defs.h` | Modificado | Se agregan los prototipos de las funciones nuevas (`syscall_num_by_name`, `freepages`, `totalpages`, `countrunnable`) y el `extern` de `traced_syscall_num`. |
| `kernel/sysinfo.h` | **Nuevo** | Define `struct sysinfo`, compartida entre kernel y espacio de usuario. |
| `user/user.h` | Modificado | Se declaran los prototipos `trace()` y `sysinfo()` y el forward-declare `struct sysinfo`. |
| `user/usys.pl` | Modificado | Se agregan las entradas `entry("trace")` y `entry("sysinfo")` para generar los stubs de `ecall`. |
| `user/trace.c` | **Nuevo** | Programa de usuario para probar `trace`. |
| `user/sysinfo.c` | **Nuevo** | Programa de usuario para probar `sysinfo`. |
| `Makefile` | Modificado | Se agregan `$U/_trace` y `$U/_sysinfo` a la lista `UPROGS` para que se incluyan en el `fs.img`. |

## Diseño Realizado

### `trace`

- Se optó por un **interruptor global a nivel de kernel** (`int traced_syscall_num`) en lugar de un mecanismo por proceso: el proyecto pide monitorear "una llamada al sistema específica" (ejemplo: `trace sys_kill`), no un proceso específico, por lo que un solo valor global identificando la syscall activa es suficiente y más simple de razonar/depurar.
- `trace()` recibe el nombre como string, lo busca en una tabla `syscall_names[]` (paralela a la tabla de punteros a función `syscalls[]`) y guarda el índice encontrado en `traced_syscall_num`. Si el nombre no coincide con ninguna syscall conocida, retorna `-1`.
- En el dispatcher `syscall()`, antes de invocar el handler se guardan los argumentos originales (`a0`, `a1`), porque `a0` se sobreescribe luego con el valor de retorno. Si el número de la syscall coincide con `traced_syscall_num`, se imprime toda la información en **una sola llamada a `printk()`**: esto es importante porque `printk()` toma un lock interno por cada invocación, y de haber usado varias llamadas separadas, dos syscalls trazadas ejecutándose en paralelo en distintos harts (el proyecto corre con `-smp 3`) podrían intercalar sus líneas de salida y producir un bloque ilegible.
- El programa `user/trace.c` no hace `fork()`: llama a `trace(nombre)` y luego reemplaza su propia imagen con `exec(comando, args)`, de modo que el PID se conserva y cualquier syscall que dispare el comando ejecutado queda trazada.

### `sysinfo`

- Se definió `struct sysinfo` en `kernel/sysinfo.h`, siguiendo el mismo patrón que usa xv6 para `struct stat` (un header que se incluye tanto desde el kernel como directamente desde los programas de usuario).
- `freepages()` recorre la lista enlazada `kmem.freelist` del asignador físico (protegida por `kmem.lock`) y cuenta sus nodos.
- `totalpages()` calcula el total de páginas físicas administradas como `(PHYSTOP - end) / PGSIZE`, usando el símbolo `end` (definido por el linker, primera dirección después de la imagen del kernel) y la constante `PHYSTOP` (128 MB, fijada en `memlayout.h` e igual al `-m 128M` con el que corre `make qemu`).
- `countrunnable()` recorre `proc[]` adquiriendo el lock de cada proceso (mismo patrón usado en el resto de `proc.c`) y cuenta los que están en `RUNNABLE`.
- `sys_sysinfo()` arma la estructura en el kernel y la transfiere a la dirección de usuario recibida como argumento mediante `copyout()`, retornando `-1` si la dirección no es válida.

### Manejo de errores

- `sys_trace()` retorna `-1` si el nombre no corresponde a ninguna syscall registrada.
- `sys_sysinfo()` retorna `-1` si `copyout()` falla (puntero de usuario inválido).
- Los programas de usuario (`trace.c`, `sysinfo.c`) reportan cualquier error a **stderr** (`fprintf(2, ...)`) con un mensaje descriptivo y terminan con `exit(1)`.

## Compilación

Este repositorio **no incluye el código completo de xv6**, solo los archivos modificados/creados, por lo que para compilar hay que aplicarlos sobre una copia limpia del repositorio oficial:

```bash
git clone https://github.com/mit-pdos/xv6-riscv.git
cd xv6-riscv
```

Copiar (sobrescribiendo) los archivos de este repositorio dentro de `xv6-riscv/`, respetando la misma estructura de carpetas (`kernel/`, `user/`, y el `Makefile` en la raíz):

```bash
cp -r /ruta/a/este/repo/kernel/* kernel/
cp -r /ruta/a/este/repo/user/* user/
cp /ruta/a/este/repo/Makefile .
```

Requisitos: toolchain `riscv64-unknown-elf-gcc` (o `riscv64-linux-gnu-gcc`) y `qemu-system-riscv64` instalados (en Linux nativo, o en WSL/Ubuntu si se trabaja desde Windows).

Compilar:

```bash
make clean
make
```

La compilación no debe arrojar errores (solo puede aparecer una advertencia preexistente del linker sobre permisos RWX del segmento LOAD del kernel, que es propia de xv6 base y no está relacionada con los cambios de este proyecto).

## Ejecución

Levantar xv6 en QEMU:

```bash
make qemu
```

Dentro de la shell de xv6:

**Probar `sysinfo`:**
```
$ sysinfo
Free Memory: 127 MB
Used Pages: 200
Available Pages: 32535
Runnable Processes: 0
```

**Probar `trace`** (uso: `trace <nombre_syscall> <comando> [args...]`):
```
$ trace sys_write echo hola_mundo
hola_mundo
PID: 4
SYSCALL: sys_write
RETURN: 11
s0: 0x3fb0
s1: 0x3fc8
a0: 0x1
a1: 0x3fe0
...
```

**Casos de error:**
```
$ trace sys_noexiste echo hola
trace: unknown syscall 'sys_noexiste'
$ trace sys_kill programa_inexistente
trace: exec programa_inexistente failed
```

Para salir de QEMU: `Ctrl-a` seguido de `x`.

## Video de demostración

El video con la compilación, ejecución de `trace`, ejecución de `sysinfo`
y resultados obtenidos está disponible en:

[Ver video de demostración](https://www.loom.com/share/c5cb5a3289b64facac41da642130ef79)

[Video Demostracion xv6](https://youtu.be/J0hDocwfLX4)

## Uso de IA

Se utilizó **Claude**, a través de Claude, como asistente de IA generativa durante el desarrollo de este proyecto. La herramienta se usó para:

- Analizar los requerimientos del enunciado del proyecto.
- Proponer el diseño de `trace` (interruptor global + tabla de nombres) y de `sysinfo` (helpers en `kalloc.c`/`proc.c` + struct compartida).
- Compilar y probar la solución en QEMU (WSL/Ubuntu) para verificar su correcto funcionamiento antes de la entrega.

Todo el código fue revisado y probado por los integrantes del equipo, quienes son responsables de su contenido y deben poder explicar cualquier fragmento durante la sustentación, conforme a la política de uso de IA generativa del curso.
