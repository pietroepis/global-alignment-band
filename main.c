#include "main.h"

int get_blosum_index(char c)
{
	char* symbols = "ARNDCQEGHILKMFPSTWYVBZX";//"ARNDCQEGHILKMFPSTWYV";
					 
	int i;
	for (i=0; i<strlen(symbols); i++)
		if (toupper(c) == symbols[i])
			return i;
			
	return -1;
}

void print_matrix(int** M, int h, int w)
{
	int j, i;
	for (i = 0; i< h; i++ ){
		for (j = 0; j<w; j++)
			printf("%d\t", M[i][j]);
		printf("\n");
	}
	printf("\n");
}

int calc_alignment(char* s1, char* s2, int b)
{
	int l1 = strlen(s1) + 1;
	int alignment = 0;
	int** M = (int**) malloc(sizeof(int*) * l1);
	char** P = (char**) malloc(sizeof(char*) * l1);
	int b_width = 2 * b + 1;
	
	int i, j;
	for (i=0; i<l1; i++)
	{
		M[i] = (int*)malloc(sizeof(int) * b_width);
		P[i] = (char*)malloc(sizeof(char) * b_width);
	}
	
	for (i = 0; i<l1; i++)
		for (j = 0; j<b_width; j++)
			M[i][j] = INT_MIN;
	
	for (i=0; i<l1; i++) {
		int initial_j = b - i;
		int final_j = initial_j + strlen(s2) + 1;
		if(final_j>b_width) final_j = b_width;
		if (initial_j < 0)
		{
			initial_j = 0;
		}
		if(i==0) M[0][initial_j] = 0;
		//printf("initial_j %d, final_j %d \n", initial_j, final_j);
		for (j=initial_j; j<final_j; j++)
		{						
			if (i == 0 && j > initial_j)
				M[i][j] = M[i][j-1] - 4;
			if (i > 0 && j == initial_j)
				M[i][j] = M[i-1][j+1] - 4;
			
			if (i > 0 && j > initial_j)
			{
				M[i][j] = M[i-1][j] + blosum[get_blosum_index(s1[i-1])][get_blosum_index(s2[j+i-b-1])];
				P[i][j] = '\\';
				//printf("%d", blosum[get_blosum_index(s1[i-1])][get_blosum_index(s2[j+i-b-1])]);
				if (j < b_width - 1 && M[i-1][j+1] - 4 > M[i][j])
				{
					M[i][j] = M[i-1][j+1] - 4;
					P[i][j] = '|';
				}
				if (M[i][j-1] - 4 > M[i][j])
				{
					M[i][j] = M[i][j-1] - 4;
					P[i][j] = '-';
				}
			}
			//printf("%d %d %d", M[i][j], i, j);
			//system("pause");
		}
	}
	
/*	print_matrix(M, l1, b_width);
	printf("ALIGNMENT: %d\n", M[l1-1][strlen(s2) - l1 + b + 1]);
	printf("%d", strlen(s2) - l1 + b - 1);
	system("pause");*/
	return M[l1-1][strlen(s2) - (l1 - 1) + b];
}

int self_alignment(char* s1)
{
	int l1 = strlen(s1);
	int i, alignment = 0;
	
	for (i=0; i<l1; i++) {
		alignment += blosum[get_blosum_index(s1[i])][get_blosum_index(s1[i])];
	}
		
	return alignment;
}

int main(int argc, char *argv[])
{
	FILE *fp;
	int l1, l2;
	
	fp = fopen("input.txt", "r");
	
	fscanf(fp, "%d", &l1);
	char* s1 = malloc(sizeof(char) * l1);
	fscanf(fp, "%s", s1);
	
	fscanf(fp, "%d", &l2);
	char* s2 = malloc(sizeof(char) * l2);
	fscanf(fp, "%s", s2);
	
	int self = self_alignment(s1);
	int b = abs(strlen(s1) - strlen(s2));
	if (b == 0)
		b += 1;
	
	int bound, alignment;
	do {
		alignment = calc_alignment(s1, s2, b);
		bound = self + (-4) * b;
		b *= 2;
		//printf("bound = %d b = %d\n", bound, b);
	} while (b/2 <= (strlen(s1) > strlen(s2) ? strlen(s1) : strlen(s2)) && bound >= alignment);
	
	printf("%d", alignment);
	
	return 0;
}
