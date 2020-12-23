#include "main.h"

int get_blosum_index(char c)
{
	char* symbols = "ARNDCQEGHILKMFPSTWYV";
	int i;
	for (i=0; i<strlen(symbols); i++)
		if (toupper(c) == symbols[i])
			return i;
			
	return -1;
}

int calc_alignment(char* s1, char* s2)
{
	int l1 = strlen(s1) + 1, l2 = strlen(s2) + 1;
	int alignment = 0;
	int** M = malloc(sizeof(int) * l1);
	char** P = malloc(sizeof(char) * l1);
	
	int i, j;
	for (i=0; i<l1; i++)
	{
		M[i] = malloc(sizeof(int) * l2);
		P[i] = malloc(sizeof(char) * l2);
	}
	
	M[0][0] = 0;
	
	for (i=0; i<=l1; i++)
		for (j=0; j<=l2; j++)
		{			
			if (i == 0 && j > 0)
				M[i][j] = M[i][j-1] - 4;
			if (i > 0 && j == 0)
				M[i][j] = M[i-1][j] - 4;
			if (i > 0 && j > 0)
			{
				M[i][j] = M[i-1][j-1] + blosum[get_blosum_index(s1[i-1])][get_blosum_index(s2[j-1])];
			
				if (M[i-1][j] - 4 > M[i][j])
					M[i][j] = M[i-1][j] - 4;
				if (M[i][j-1] - 4 > M[i][j])
					M[i][j] = M[i][j-1] - 4;
			}
		}
	
	return M[l1][l2];
}

int main(int argc, char *argv[])
{
	FILE *fp;
	int length = 30;
	
	char* s1 = malloc(sizeof(char) * length);
	char* s2 = malloc(sizeof(char) * length);
	
	fp = fopen("test.txt", "r");
	fscanf(fp, "%s", s1);
	fscanf(fp, "%s", s2);
	
	int a = calc_alignment(s1, s2);
	printf("ALLINEAMENTO: %d", a);
}
