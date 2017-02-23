#include <stdio.h>
#define SIZE 2


void print_array(double *x)
{
	int i;
	for(i=0; i<SIZE; i++)
		printf("%f ", x[i]);
	printf("\n");
}


int solve_direct_gauss(double A[][SIZE], double b[], double x[])
{
	int i, j, p;
	double factor, temp;

	/* step 1 - elimination */
	for(p=0; p<SIZE-1; p++) /* pivot */
	{
		if (A[p][p] == 0) {
			printf("NO SOLUTION - Division by zero in Gauss elemination\n");
			return 1;
		}
		for(i=p+1; i<SIZE; i++)
		{
			if(A[i][p] != 0)
			{
				factor = A[i][p] / A[p][p];
				for(j=p+1; j<SIZE; j++)
				{
					A[i][j] -= factor * A[p][j];
				}
				b[i] -= factor * b[p];
			}
		}
	}

	/* step 2  - substitution */
	for(i=SIZE-1; i >=0; i--)
	{
		if (A[i][i] == 0) {
			printf("NO SOLUTION - Division by zero in Gauss substitution\n");
			return 2;
		}
		temp = 0;
		for (j=i+1; j<SIZE; j++)
			temp += A[i][j] * x[j];
		x[i] = (b[i] - temp) / A[i][i];
	}

	return 0;
}


int main(void) {

	double x[SIZE];

	double A[SIZE][SIZE] = {{1, 5}, {-2, -7}};
	double b[SIZE] = {7, -5};  /* has the result: -8, 3 */

	//double A[SIZE][SIZE] = {{1, -2, -6}, {2, 4, 12}, {1, -4, -12}};
	//double b[SIZE] = {12, -17, 22}; /* does not have a solution  */

	//double A[SIZE][SIZE] = {{1, 2, 3}, {1, 1, 1}, {3, 3, 1}};
	//double b[SIZE] = {2, 2, 0}; /* has the result: 5, -6, 3  */

	solve_direct_gauss(A, b, x);
	print_array(x);

	return 0;
}
