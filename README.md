# Operative-Systems

Systems-programming coursework in C for the *Sistemas Operativos* (Operating
Systems) course at Pontificia Universidad Javeriana, Colombia. The repository
covers the classic building blocks of operating systems — processes, fork,
pipes, FIFOs, POSIX threads and synchronization — building up to a complete
publisher/subscriber news-distribution system.

## Repository layout

| Path | Content |
| ---- | ------- |
| [`Códigos/`](Códigos/) | In-class C exercises, grouped by academic term (*Corte*). |
| [`Parcial/`](Parcial/) | Midterm: a multi-threaded counter (`main.c` + `hilos.c`). |
| [`Taller 1 - Compilación Modular/`](Taller%201%20-%20Compilación%20Modular/) | Modular compilation with a small C library. |
| [`Taller 2 - Cliente/Servidor Unidireccional/`](Taller%202%20-%20Cliente/Servidor%20Unidireccional/) | Unidirectional client/server over a FIFO. |
| [`Taller 3 - Fork y Pipes/`](Taller%203%20-%20Fork%20y%20Pipes/) | `fork()` + pipes: two child processes sum two files. |
| [`Taller 4 - PipeName Bidireccional/`](Taller%204%20-%20PipeName%20Bidireccional/) | Bidirectional two-way echo over a FIFO. |
| [`Taller 5 - Máquina Virtual/`](Taller%205%20-%20Máquina%20Virtual/) | Client/server over FIFOs + a POSIX producer/consumer. |
| [`Taller 6 - Rendimiento/`](Taller%206%20-%20Rendimiento/) | Performance: classic vs transposed matrix multiplication. |
| [`Proyecto_Sistema_Noticias/`](Proyecto_Sistema_Noticias/) | News distribution system over named pipes (final project). |

## Workshops

### Taller 1 — Modular compilation
A program plus a reusable library (`librerialab.c`/`librerialab.h`) showing how
to compile a C project in separate translation units.

```bash
cd "Taller 1 - Compilación Modular"
make
./labmodular
```

### Taller 2 — Unidirectional client/server
`pipeServidor01.c` (server) and `pipeCliente01.c` (client, keyed by MAC address)
communicate through a named pipe (`MYFIFO`); `fork_procesos.c` demonstrates
process forking and PID reporting.

```bash
cd "Taller 2 - Cliente/Servidor Unidireccional"
gcc -o fork_procesos fork_procesos.c
gcc -o pipeServidor01 pipeServidor01.c
gcc -o pipeCliente01 pipeCliente01.c
./fork_procesos 3      # forks 3 children
```

The FIFO client/server pair requires two terminals and a running server; it was
compile-verified only.

### Taller 3 — Fork and pipes
`fork()` + two unidirectional pipes. The parent spawns two child processes
that each sum the integers in a text file; the first child aggregates both
results and reports the grand total back to the parent.

```bash
cd "Taller 3 - Fork y Pipes"
make            # builds `ejecutable` and runs it on the sample files
```

Verified locally: `PrimerHijo` reports `231 + 435 = 666` and the parent
confirms the total received via pipe.

> Tip: `make` runs the binary automatically (default target `all: ...
> run`). Use `make ejecutable` to build without running.

### Taller 4 — Bidirectional named pipe
A client and server echo a string back through a two-way FIFO
(`/tmp/fifo_twoway`). Compile-verified locally.

```bash
cd "Taller 4 - PipeName Bidireccional"
gcc -o servidor servidor.c
gcc -o cliente cliente.c
```

Run `./servidor` in one terminal and `./cliente` in another.

### Taller 5 — Virtual machine
FIFO client/server plus `prod_cons_posix.c`, a producer/consumer pair synchronized
with a mutex and condition variables (POSIX threads).

```bash
cd "Taller 5 - Máquina Virtual"
gcc -pthread -o prod_cons_posix prod_cons_posix.c
gcc -o cliente cliente.c
gcc -o servidor servidor.c
```

`prod_cons_posix` runs in an infinite loop (stop it with <kbd>Ctrl+C</kbd>), so
it was compile-verified only.

### Taller 6 — Performance: classic vs transposed matrix multiplication
Two pthreads-based matrix products (classic access order vs a transposed access
pattern), each instrumented with a microsecond timer. The Perl scripts
(`lanzaClasico.pl`, `lanzaTranspuesta.pl`) benchmark sizes 600/800/1000/1800 on
1/2/4 threads, calling the binaries repeatedly and recording the timings in
`resultados_*.dat`.

```bash
cd "Taller 6 - Rendimiento/Código"
gcc mm_clasico.c mm_clasicoBiblioteca.c -o clasico -lpthread
gcc mm_transpuesta.c mm_transpuestaBiblioteca.c -o transpuesta -lpthread
./clasico <N> <threads>
./transpuesta <N> <threads>
```

> Note: the binaries statically allocate the matrices (3×N×N doubles), so a
> large build environment and RAM are required. The two variants are timing
> benchmarks, not numerically interchangeable implementations.

## Midterm — `Parcial/`

Five threads share a counter protected by a mutex, each idling over an
array of 300 integers.

```bash
cd Parcial
gcc -pthread -o programa main.c hilos.c
./programa
```

## In-class exercises — `Códigos/`

- **Corte 1** — Dynamic memory management in C: pointers, `malloc`/`calloc`/
  `realloc`, a dynamic matrix, a vector of pointers, and a header-based
  modular example (`Inclusión_header/`).
- **Corte 2** — `fork()` process creation (`Fork.c`).
- **Corte 3** — POSIX examples (`posix.c`; `hola.c` is an empty placeholder).

Most files are single-file programs:

```bash
gcc -o /tmp/ejemplo "Códigos/Corte 1/Punteros.c"
/tmp/ejemplo
```

Verified locally: 12 of the 15 exercises compile and link as-is. Two files
(`Función_dinámica.c`, `Realloc_ejemplo.c`) reproduce the classic C pitfalls
they were written to demonstrate (returning a stack address; several type
typos) and deliberately do not compile — left untouched as teaching examples.

## News distribution system — `Proyecto_Sistema_Noticias/`

A publisher/subscriber system where news is distributed through named pipes
(FIFOs). A dedicated course project; its Spanish README and two PDFs describe
the design. See [`Taller guide`](Proyecto_Sistema_Noticias/Proyecto_Sistema_Noticias/README.md)
in the project folder.

**Architecture**

```
data/noticias*.txt ─▶ ./publicador ──pipePSC──▶ ./sistema ──pipeSSC──▶ ./suscriptor
                    (one-shot send)            (in-memory news            (filters by
                                                store, select()             topic)
                                                with timeout)
```

1. `./sistema` opens the `pipePSC` and `pipeSSC` FIFOs and registers every
   subscriber that connects to `pipeSSC` along with its topics of interest.
2. `./publicador` writes the news file, line by line (`topic: text`), into
   `pipePSC`.
3. `./sistema` receives each news item, stores it in memory
   (`SCBiblioteca`), and broadcasts it only to subscribers that requested its
   topic. When no new publisher appears within its inactivity window
   (`-t`), it broadcasts the end-of-transmission marker.

```bash
cd "Proyecto_Sistema_Noticias/Proyecto_Sistema_Noticias"
make
# Terminal 1
./sistema -p pipePSC -s pipeSSC -t 3
# Terminal 2
printf 'A E\n' | ./suscriptor -s pipeSSC
# Terminal 3
./publicador -p pipePSC -f data/noticias1.txt -t 1
```

**Verified locally (end to end):** with topics `A E`, the subscriber received
exactly the art (`A`) and entertainment (`E`) news items and then the end of
broadcast — topic filtering works.

## Local verification summary

Everything below was compiled with macOS (`clang`/`gcc`, `make`) on the author's
machine. Details per item are in the sections above.

| Component | Build | Runtime |
| --------- | :---: | :-----: |
| Taller 1 `labmodular` | ✅ | ✅ ran |
| Taller 2 `fork_procesos` | ✅ | ✅ ran |
| Taller 2 FIFO client/server | ✅ | ⚪ interactive (not exercised) |
| Taller 3 `ejecutable` | ✅ | ✅ ran (grand total 666) |
| Taller 4 FIFO echo | ✅ | ⚪ interactive (not exercised) |
| Taller 5 `prod_cons_posix` | ✅ | ⚪ infinite loop (compile only) |
| Taller 5 FIFO client/server | ✅ | ⚪ interactive (not exercised) |
| Taller 6 `clasico`/`transpuesta` | ✅ | ✅ ran (3×3, 2 threads, µs timings) |
| `Parcial/programa` | ✅ | ✅ ran |
| `Códigos` (12 of 15) | ✅ | ✅ spot-run |
| News system | ✅ | ✅ end-to-end run with topic filter |

The course software targets Linux (Ubuntu); macOS was used for this pass.

## License

[MIT](LICENSE) — Copyright (c) 2026 Daniel Felipe Castro Moreno. Coursework in
the subfolders remains original work by its authors.