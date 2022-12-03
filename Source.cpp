#include <time.h>
#include<string.h>
#include<math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <mpi.h>
#include <iomanip>

using namespace std;

struct float4_t
{
    double x, y, z, t;
};

int main(int argc, char* argv[])
{
    clock_t start, end;
    start = clock(); // Recording the starting clock tick.
    int first, last, sec, tag = 0, tasks, id;
    double T0, T1, T2, T3, T4, T5, T6, steps;
    const int alto = 20;
    const int ancho = 20;
    const int profundo = 50;
    int i, j, k;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &tasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    double error = 0.00000000000001;
    T0 = 25;
    T1 = 150;
    T2 = 35;
    T3 = 55;
    T4 = 40;
    T5 = 65;
    T6 = 75;
    const int count = (ancho + 2) * (alto + 2) * (profundo + 2);

    float4_t* h_temp = new float4_t[count];
    float4_t* d_temp;
    float4_t* h_tempprima = new float4_t[count];
    float4_t* d_tempprima;
    double h_error_acum = 0;
    double* d_error_acum = 0;
    FILE* fp;

    int z = 0;
    clock_t start_1, end_1;
    start_1 = clock(); // Recording the starting clock tick.
    while (z < count)
    {

        for (i = 0; i < profundo + 2; i++) // X
        {
            for (j = 0; j < ancho + 2; j++) //Y
            {
                for (k = 0; k < alto + 2; k++)// Z
                {
                    if (k == 0) {
                        h_temp[z].x = k;
                        h_temp[z].y = j;
                        h_temp[z].z = i;
                        h_temp[z].t = T1;
                        h_temp[z].t = T1;
                        h_tempprima[z].x = k;
                        h_tempprima[z].y = j;
                        h_tempprima[z].z = i;
                        h_tempprima[z].t = T1;
                        z = z + 1;
                    }
                    else if (k > 0 && j == 0) {
                        h_temp[z].x = k;
                        h_temp[z].y = j;
                        h_temp[z].z = i;
                        h_temp[z].t = T2;
                        h_tempprima[z].x = k;
                        h_tempprima[z].y = j;
                        h_tempprima[z].z = i;
                        h_tempprima[z].t = T2;
                        z = z + 1;
                    }
                    else if (k > 0 && j == (ancho + 1)) {
                        h_temp[z].x = k;
                        h_temp[z].y = j;
                        h_temp[z].z = i;
                        h_temp[z].t = T3;
                        h_tempprima[z].x = k;
                        h_tempprima[z].y = j;
                        h_tempprima[z].z = i;
                        h_tempprima[z].t = T3;
                        z = z + 1;
                    }
                    else if (k == (alto + 1))
                    {
                        h_temp[z].x = k;
                        h_temp[z].y = j;
                        h_temp[z].z = i;
                        h_temp[z].t = T4;
                        h_tempprima[z].x = k;
                        h_tempprima[z].y = j;
                        h_tempprima[z].z = i;
                        h_tempprima[z].t = T4;
                        z = z + 1;
                    }
                    else if (i == 0)
                    {
                        h_temp[z].x = k;
                        h_temp[z].y = j;
                        h_temp[z].z = i;
                        h_temp[z].t = T5;
                        h_tempprima[z].x = k;
                        h_tempprima[z].y = j;
                        h_tempprima[z].z = i;
                        h_tempprima[z].t = T5;
                        z = z + 1;
                    }
                    else if (k == (profundo + 1))
                    {
                        h_temp[z].x = k;
                        h_temp[z].y = j;
                        h_temp[z].z = i;
                        h_temp[z].t = T6;
                        h_tempprima[z].x = k;
                        h_tempprima[z].y = j;
                        h_tempprima[z].z = i;
                        h_tempprima[z].t = T6;
                        z = z + 1;
                    }
                    else {
                        h_temp[z].x = k;
                        h_temp[z].y = j;
                        h_temp[z].z = i;
                        h_temp[z].t = T0;
                        h_tempprima[z].x = k;
                        h_tempprima[z].y = j;
                        h_tempprima[z].z = i;
                        h_tempprima[z].t = T0;
                        z = z + 1;
                    }
                }
            }
        }
    }
    steps = count / tasks;
    if (count % tasks > 0) {
        // Add 1 in case the number of ranks doesn't divide the number of numbers
        steps += 1;
    }
    // Figure out the first and the last iteration for this rank
    first = id * steps;
    last = first + steps;

    sec = 0;
    // Recording the end clock tick.
    end_1 = clock();
    // Calculating time required to complete Matrix Assignation
    double time_taken_1 = double(end_1 - start_1) / double(CLOCKS_PER_SEC);

    cout << " sec " << endl;
    int iteracion = 0;
    clock_t start_2, end_2;
    while (h_error_acum == 0) {
        iteracion = iteracion + 1;

        for (z = first; z < last; z++) {
            if (h_temp[z].x == 0 || h_temp[z].y == 0 || h_temp[z].z == 0 || h_temp[z].x == (ancho + 1) || h_temp[z].y == (ancho + 1) || h_temp[z].z == (profundo + 1)) {

                h_tempprima[z].t = h_temp[z].t;
            }
            else {

                h_tempprima[z].t = (h_temp[z + ancho + 2].t + h_temp[z - ancho - 2].t + h_temp[z - 1].t + h_temp[z + 1].t + h_temp[z - (ancho + 2) * (alto + 2)].t + h_temp[z + (ancho + 2) * (alto + 2)].t) / 6;
                h_error_acum = fabs(h_temp[z].t - h_tempprima[z].t);

                h_temp[z].t = h_tempprima[z].t;
            }

        }

        if (h_error_acum > error) h_error_acum = 0;
    }


    fp = fopen("datampi.xls", "w");
    fprintf(fp, "\"X\", \"Y\", \"Z\", \"T\"\n");


    for (i = 0; i < count; i++)
    {
        fprintf(fp, "%f,%f,%f,%f \n", h_temp[i].x, h_temp[i].y, h_temp[i].z, h_temp[i].t);

    }
    end_2 = clock();
    // Calculating time required to complete Matrix Assignation
    double time_taken_2 = double(end_2 - start_2) / double(CLOCKS_PER_SEC);

    cout << " sec " << endl;

    clock_t start_3, end_3;
    start_3 = clock();
    fclose(fp);
    cout << h_error_acum << " in " << count << " iteracions" << endl;
    end_3 = clock();
    // Calculating time required to complete Matrix Assignation
    double time_taken_3 = double(end_3 - start_3) / double(CLOCKS_PER_SEC);

    cout << " sec " << endl;


    end = clock();
    double total_time = double(end - start) / double(CLOCKS_PER_SEC);
    MPI_Finalize();
    cout << "Time taken for Section One \" Matrix assignation\" was : " << fixed << time_taken_1 << setprecision(5);
    cout << "Time taken for Section Two \"Matrix calculation\" was : " << fixed << time_taken_2 << setprecision(5);
    cout << "Time taken For Section Three \" Printing Results\" was : " << fixed << time_taken_3 << setprecision(5);
    cout << "Final error value: " << fixed << h_error_acum << setprecision(8) << " in " << count << " iterations" << endl;
    cout << "Total time taken by the entire code : " << fixed << total_time << setprecision(5);
    cout << " sec " << endl;

    return 0;
}