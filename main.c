#include "main.h"

int** M;
char** P;

int get_blosum_index(char c)
{
	char* symbols = "ARNDCQEGHILKMFPSTWYVBZX";//"ARNDCQEGHILKMFPSTWYV";
					 
	int i;
	for (i=0; i<strlen(symbols); i++)
		if (toupper(c) == symbols[i])
			return i;
			
	return -1;
}

void print_matrix(char** M, int h, int w)
{
	int j, i;
	for (i = 0; i< h; i++ ){
		for (j = 0; j<w; j++)
			printf("%c\t", M[i][j]);
		printf("\n");
	}
	printf("\n");
}

int calc_alignment(char* s1, char* s2, int b, int extra)
{
	int l1 = strlen(s1) + 1, l2 = strlen(s2) + 1;
	int alignment = 0;
	M = (int**) malloc(sizeof(int*) * l1);
	P = (char**) malloc(sizeof(char*) * l1);
	int b_width = 2 * b + extra;
	int i, j;
	int initial_j, final_j;
	
	for (i=0; i<l1; i++)
	{
		M[i] = (int*)malloc(sizeof(int) * b_width);
		P[i] = (char*)malloc(sizeof(char) * b_width);
	}
	
	for (i = 0; i<l1; i++)
		for (j = 0; j<b_width; j++)
			M[i][j] = INT_MIN;
	
	for (i=0; i<l1; i++) {
		initial_j = b_width / 2 - i;
		final_j = initial_j + strlen(s2) + 1;
		if(final_j>b_width) final_j = b_width;
		if (initial_j < 0)
		{
			initial_j = 0;
		}
		if(i==0)
		{
			M[0][initial_j] = 0;
			P[0][initial_j] = '#';		
		}
		
		for (j=initial_j; j<final_j; j++)
		{						
			if (i == 0 && j > initial_j)
			{
				M[i][j] = M[i][j - 1] - 4;
				P[i][j] = '-';
			}
			if (i > 0 && j == initial_j)
			{
				M[i][j] = M[i - 1][j + 1] - 4;
				P[i][j] = '|';
			}
			
			if (i > 0 && j > initial_j)
			{
				M[i][j] = M[i - 1][j] + blosum[get_blosum_index(s1[i - 1])][get_blosum_index(s2[j + i - (b_width / 2) - 1])];
				P[i][j] = '\\';
				
				if (j < b_width - 1 && M[i-1][j+1] - 4 > M[i][j])
				{
					M[i][j] = M[i - 1][j + 1] - 4;
					P[i][j] = '|';
				}
				if (M[i][j - 1] - 4 > M[i][j])
				{
					M[i][j] = M[i][j - 1] - 4;
					P[i][j] = '-';
				}
			}
		}
	}
	
	return M[l1 - 1][strlen(s2) - (l1 - 1) + (b_width / 2)];
}

void create_new_strings(char* s1, char* s2, char* out_s1, char* out_s2, int b, int extra)
{
	int l1 = strlen(s1) + 1, l2 = strlen(s2);
	int b_width = 2 * b + extra;
	int c1 = 0, c2 = 0;
	int i = l1 - 1;
	int j = l2 - (l1 - 1) + (b_width / 2);
	
	while(P[i][j] == '-' || P[i][j] == '|' || P[i][j] == '\\')
	{
		if (P[i][j] == '-')
		{
			out_s1[c1] = '-';
			out_s2[c2] = s2[j + i - (b_width / 2) - 1];
			j--;
		} 
		else if (P[i][j] == '|')
		{
			out_s1[c1] = s1[i - 1];
			out_s2[c2] = '-';
			i--;
			j++;
		}
		else
		{
			out_s1[c1] = s1[i - 1];
			out_s2[c2] = s2[j + i - (b_width / 2) - 1];
			i--;
		}
		
		c1++;
		c2++;
	}
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
	FILE *fin, *fout;
	int l1, l2, b, extra;
	int self, bound, alignment;
	char* s1;
	char* s2;
	char* out_s1;
	char* out_s2;
	
	fin = fopen("input.txt", "r");
	
	fscanf(fin, "%d", &l1);
	s1 = malloc(sizeof(char) * l1);
	fscanf(fin, "%s", s1);
	
	fscanf(fin, "%d", &l2);
	s2 = malloc(sizeof(char) * l2);
	fscanf(fin, "%s", s2);
	
	fclose(fin);
	
	self = self_alignment(s1);
	b = abs(strlen(s1) - strlen(s2));
	extra = 1;	
	if (b == 0)
		b += 1;
	
	do {
		alignment = calc_alignment(s1, s2, b, extra);
		bound = self + (-4) * b;
		extra *= 2;
	} while ((b + 2 * extra) <= (strlen(s1) > strlen(s2) ? strlen(s1) : strlen(s2)) && bound >= alignment);
	
	out_s1 = malloc(l1 + l2 + 1);
	out_s2 = malloc(l1 + l2 + 1);
	create_new_strings(s1, s2, out_s1, out_s2, b/2, extra);
	
	printf("ALIGNMENT: %d\n", alignment);
	printf("S1: %s\nS2: %s\n", s1, s2);
	printf("OUT_S1: %s\nOUT_S2: %s\n", out_s1, out_s2);
	
	fout = fopen("output.txt", "w");
	fprintf(fout, "ALIGNMENT: %d\n\n", alignment);
	fprintf(fout, "%s\n", out_s1);
	fprintf(fout, "%s", out_s2);
	fclose(fout);	
	
	return 0;
}
