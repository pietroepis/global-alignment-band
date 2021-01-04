#include "main.h"

// Returns the index of BLOSUM62 matrix which relates to the score values of character c
int get_blosum_index(char c)
{
	char* symbols = "ARNDCQEGHILKMFPSTWYVBZX";
					 
	int i;
	for (i=0; i<strlen(symbols); i++)
		if (toupper(c) == symbols[i])
			return i;
			
	return -1;
}

int calc_alignment(char* s1, char* s2, int b, int extra)
{
	int64_t l1 = strlen(s1) + 1;	// matrix height. +1 in order to consider empty string too
	int64_t alignment = 0;
	M = (int64_t**) malloc(sizeof(int64_t*) * l1);	// Dynamic Programming Matrix
	P = (char**) malloc(sizeof(char*) * l1);	// Ancestors Matrix
	int64_t b_width = b + 2 * extra;
	int64_t i, j;
	int64_t initial_j, final_j;
	
	for (i=0; i<l1; i++)
	{
		M[i] = (int64_t*) malloc(sizeof(int64_t) * b_width);
		P[i] = (char*) malloc(sizeof(char) * b_width);
	}
	
	// Set every cell to minimum possible value (needed for later comparison to find max)
	for (i = 0; i<l1; i++)
		for (j = 0; j<b_width; j++)
			M[i][j] = INT_MIN;
	
	for (i=0; i<l1; i++) {
		// Calculate offsets for actual columns interval
		initial_j = extra - i;
		final_j = initial_j + strlen(s2) + 1;
		
		if(final_j > b_width) 
			final_j = b_width;
		
		if (initial_j < 0)
			initial_j = 0;
				
		if(i == 0)
		{
			// Stop character for reconstruction
			M[0][initial_j] = 0;
			P[0][initial_j] = '#';		
		}
		
		for (j=initial_j; j<final_j; j++)
		{				
			// Base case		
			if (i == 0 && j > initial_j)
			{
				M[i][j] = M[i][j - 1] - 4;
				P[i][j] = '-';
			}
			
			// Base case	
			if (i > 0 && j == initial_j)
			{
				M[i][j] = M[i - 1][j + 1] - 4;
				P[i][j] = '|';
			}
			
			// Recursive Step
			if (i > 0 && j > initial_j)
			{
				M[i][j] = M[i - 1][j] + blosum[get_blosum_index(s1[i - 1])][get_blosum_index(s2[j + i - extra - 1])];	// Score values of the two characters
				P[i][j] = '\\';
				
				if (j < b_width - 1 && M[i - 1][j + 1] - 4 > M[i][j])
				{
					M[i][j] = M[i - 1][j + 1] - 4;	// Score value of s2 character with an indel (j + 1 because of offset)
					P[i][j] = '|';
				}
				
				if (M[i][j - 1] - 4 > M[i][j])
				{
					M[i][j] = M[i][j - 1] - 4;	// Score value of s1 character with an indel
					P[i][j] = '-';
				}
				
				// Take the maximum among the three calculated scores
			}
		}
	}
	
	return M[l1 - 1][strlen(s2) - (l1 - 1) + extra];	// Cell containing the alignment value
}

void create_new_strings(char* s1, char* s2, char* out_s1, char* out_s2, int b, int extra)
{
	int64_t l1 = strlen(s1) + 1, l2 = strlen(s2);
	int64_t b_width = b + 2 * extra;
	int64_t c1 = 0, c2 = 0;		// Growing indexes two generate output strings
	int64_t i = l1 - 1;
	int64_t j = l2 - (l1 - 1) + extra;	// Start from the cell which contains the alignment value
	
	while(P[i][j] == '-' || P[i][j] == '|' || P[i][j] == '\\')
	{
		if (P[i][j] == '-')
		{
			out_s1[c1] = '-';
			out_s2[c2] = s2[j + i - extra - 1];		
			// Go to left
			j--;
		} 
		else if (P[i][j] == '|')
		{
			out_s1[c1] = s1[i - 1];
			out_s2[c2] = '-';	
			// Go up
			i--;	
			j++;
		}
		else
		{
			out_s1[c1] = s1[i - 1];
			out_s2[c2] = s2[j + i - extra - 1];
			// Move diagonally
			i--;
		}
		
		c1++;
		c2++;
	}
	
	out_s1[c1] = '\0';
	out_s2[c2] = '\0';
	
	// Strings are currently in reverse order and must be flipped
	out_s1 = strrev(out_s1);
	out_s2 = strrev(out_s2);	
}

// Returns the alignment value of a string with itself
int self_alignment(char* s1)
{
	int64_t l1 = strlen(s1), i;
	int64_t alignment = 0;
	
	for (i=0; i<l1; i++) {
		alignment += blosum[get_blosum_index(s1[i])][get_blosum_index(s1[i])];
	}
		
	return alignment;
}

int main(int argc, char *argv[])
{
	FILE *fin, *fout;					// input and output file streams
	int64_t l1, l2, b, extra;			// input strings length, band width and extra (b + 2 * extra)
	int64_t self, bound, alignment;		// best alignment, upper bound, current alignment
	char* s1;							// 1st input string
	char* s2;							// 2nd input string
	char* out_s1;						// 1st string aligned (with indels)
	char* out_s2;						// 2nd string aligned (with indels)
	
	// Input file is in the following form:
	// l1 s1 l2 s2
	fin = fopen("input.txt", "r");
	
	fscanf(fin, "%ld", &l1);
	s1 = malloc(sizeof(char) * l1);
	fscanf(fin, "%s", s1);
	
	fscanf(fin, "%ld", &l2);
	s2 = malloc(sizeof(char) * l2);
	fscanf(fin, "%s", s2);
	
	fclose(fin);
	
	// Longest string is always assigned to s2
	if (strlen(s1) > strlen(s2))
	{
		char* tmp;
		tmp = s1;
		s1 = s2;
		s2 = tmp;
	}
	
	self = self_alignment(s1);	// The best possible alignment value of s1 is the one with itself 
	b = abs(strlen(s1) - strlen(s2)) + 1;	// b is initially set as the lenght difference of the input strings (+1 prevents it from being 0)
	extra = 1;	
	
	do {
		alignment = calc_alignment(s1, s2, b, extra);
		bound = self + (-4) * b;	// -4 is the penalty for an indel
		extra *= 2;		// double extra at every iteration
	} while ((b + 2 * extra) <= (strlen(s1) > strlen(s2) ? strlen(s1) : strlen(s2)) && bound >= alignment);
	// while the band width isn't greater than neither of the strings length and the current alignment is lower than the upper bound
	
	// output strings won't surely be longer than l1 + l2
	out_s1 = malloc(l1 + l2 + 1);	
	out_s2 = malloc(l1 + l2 + 1);
	create_new_strings(s1, s2, out_s1, out_s2, b, extra/2);
	
	fout = fopen("output.txt", "w");
	fprintf(fout, "ALIGNMENT: %ld\n\n", alignment);
	fprintf(fout, "%s\n", out_s1);
	fprintf(fout, "%s", out_s2);
	fclose(fout);
	
	printf("\nOutput File successfully created\n");	
	
	return 0;
}
