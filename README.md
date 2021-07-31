# Parallel Matrix Multiplication (MPI)
>  A demonstration of parallel computing in C using the [Open MPI](https://www.open-mpi.org) library. This program multiplies a set of NxN square matrices using a manager-worker paradigm, where the workload is distributed among available worker processes and coordinated by a manager process. 

## Prerequisites
This project requires the Open MPI library.

- [Build from source](https://www.open-mpi.org/faq/?category=building#easy-build)
- [Install using Homebrew on macOS](https://formulae.brew.sh/formula/open-mpi)
- [Install using Windows binaries](https://www.open-mpi.org/software/ompi/v1.6/ms-windows.php)

## Compilation
```bash
mpicc main.c -o parallel
```

## Execution
```bash
mpirun parallel
```

## Clone

- Clone this repo to your local machine using `git clone git@github.com:camchambers/parallel-matrix-multiplication.git`

## License

[![License](http://img.shields.io/:license-mit-blue.svg?style=flat-square)](http://badges.mit-license.org)

- **[MIT license](http://opensource.org/licenses/mit-license.php)**
