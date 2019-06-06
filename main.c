/* Matrix Multiplication
 * Cameron Chambers
 * 1 February 2017
 *
 */

#include <stdio.h>
#include "mpi.h"
#include <time.h>

#define N 2400 // matrix dimension

MPI_Status status;

// Define matricies
double matrix1[N][N];
double matrix2[N][N];
double productMatrix[N][N];

int main(int argc, char **argv) {

    int numberOfProcessors; // Number of processors
    int processorRank; // Processor rank
    int numberOfWorkers; // Number of workers
    int sourceProcessor; // Processor sending data
    int destinationProcessor; // Processor to receive data
    int rows; // The number of rows for a worker processor to process
    int matrixSubset; // The subset of a matrix to be processed by workers
    int i, j, k; // Counter variables

    MPI_Init(&argc, &argv); // Initialize MPI environment
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcessors); // Determine number of processors
    MPI_Comm_rank(MPI_COMM_WORLD, &processorRank); // Determine rank of calling process

    numberOfWorkers = numberOfProcessors - 1;

    /* ---------- Code for master processor ---------- */

    if (processorRank == 0) {

        clock_t begin = clock(); // start the timer

        printf("Multiplying a %d by %d matrix using %d processors.\n",N,N,numberOfProcessors);

        // Populate the matrices
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                matrix1[i][j] = 1.0;
                matrix2[i][j] = 2.0;
            }
        }

        /* Send the matrix to the worker processes */
        rows = N / numberOfWorkers;
        matrixSubset = 0;

        // Iterate through all of the workers and assign work
        for (destinationProcessor = 1; destinationProcessor <= numberOfWorkers; destinationProcessor++) {

            // Determine the subset of the matrix to send to the destination processor
            MPI_Send(&matrixSubset, 1, MPI_INT, destinationProcessor, 1, MPI_COMM_WORLD);

            // Send the number of rows to process to the destination worker processor
            MPI_Send(&rows, 1, MPI_INT, destinationProcessor, 1, MPI_COMM_WORLD);

            // Send rows from matrix 1 to destination worker processor
            MPI_Send(&matrix1[matrixSubset][0], rows * N, MPI_DOUBLE, destinationProcessor, 1, MPI_COMM_WORLD);

            // Send entire matrix 2 to destination worker processor
            MPI_Send(&matrix2, N * N, MPI_DOUBLE, destinationProcessor, 1, MPI_COMM_WORLD);

            // Determine the next chunk of data to send to the next processor
            matrixSubset = matrixSubset + rows; 
        }

        // Retrieve results from all workers processors
        for (i = 1; i <= numberOfWorkers; i++) {
            sourceProcessor = i;
            MPI_Recv(&matrixSubset, 1, MPI_INT, sourceProcessor, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&rows, 1, MPI_INT, sourceProcessor, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&productMatrix[matrixSubset][0], rows * N, MPI_DOUBLE, sourceProcessor, 2, MPI_COMM_WORLD, &status);
        }

        // Print the matrix results
        // @todo write this to a file instead
        printf("Resulting matrix:\n");
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                //printf("%6.2f   ", c[i][j]);
            }
            //printf("\n");
        }

        clock_t end = clock(); // end the timer
        double runTime = (double)(end - begin) / CLOCKS_PER_SEC; // determine the total runtime
        printf("Runtime: %f seconds\n", runTime);

    }

    /* ---------- Code for worker processors ---------- */

    if (processorRank > 0) {
        sourceProcessor = 0;
        MPI_Recv(&matrixSubset, 1, MPI_INT, sourceProcessor, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, sourceProcessor, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&matrix1, rows * N, MPI_DOUBLE, sourceProcessor, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&matrix2, N * N, MPI_DOUBLE, sourceProcessor, 1, MPI_COMM_WORLD, &status);

        /* Matrix multiplication */
        for (k = 0; k < N; k++) {
            for (i = 0; i < rows; i++) {
                productMatrix[i][k] = 0.0;
                for (j = 0; j < N; j++) {
                    productMatrix[i][k] = productMatrix[i][k] + matrix1[i][j] * matrix2[j][k];
                }
            }
        }

        MPI_Send(&matrixSubset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&productMatrix, rows * N, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}
