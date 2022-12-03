#include<time.h>
#include<string.h>
#include<math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <mpi.h>

using namespace std;

struct float4_t
{
	double x, y, z, t;
};

int main(int argc, char* argv[])
{
	int  tag = 0, tasks, id;
	double T0, T1, T2, T3, T4, T5, T6,steps;
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

	int iteracion = 0;
	while (h_error_acum == 0) {
		iteracion = iteracion + 1;

		for (z = 0; z < steps; z++) {
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


	fp = fopen("Test_Data3D_test_Serial.xls", "w");
	fprintf(fp, "\"X\", \"Y\", \"Z\", \"T\"\n");


	for (i = 0; i < count; i++)
	{
		fprintf(fp, "%f,%f,%f,%f \n", h_temp[i].x, h_temp[i].y, h_temp[i].z, h_temp[i].t);

	}

	fclose(fp);
	cout << h_error_acum << " in " << count << " iteracions" << endl;
	MPI_Finalize();
	return 0;
	
}
