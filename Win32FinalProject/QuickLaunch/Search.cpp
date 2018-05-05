#include "stdafx.h"
#include "Search.h"
// Fills lps[] for given patttern pat[0..M-1]
void computeLPSArray(WCHAR *pat, int M, int *lps)
{
	// length of the previous longest prefix suffix
	int len = 0;

	lps[0] = 0; // lps[0] is always 0

				// the loop calculates lps[i] for i = 1 to M-1
	int i = 1;
	while (i < M)
	{
		if (pat[i] == pat[len])
		{
			len++;
			lps[i] = len;
			i++;
		}
		else // (pat[i] != pat[len])
		{
			// This is tricky. Consider the example.
			// AAACAAAA and i = 7. The idea is similar 
			// to search step.
			if (len != 0)
			{
				len = lps[len - 1];

				// Also, note that we do not increment
				// i here
			}
			else // if (len == 0)
			{
				lps[i] = 0;
				i++;
			}
		}
	}
}

int KMPSearch(WCHAR *keyword, WCHAR* fileName)
{
	WCHAR* filename = new WCHAR[wcslen(fileName) + 1];
	wcscpy(filename, fileName);
	keyword = _wcsupr(keyword);
	filename = _wcsupr(filename);
	int M = wcslen(keyword);
	int N = wcslen(filename);

	// values for pattern
	int* lps = new int[M];

	// Preprocess the pattern (calculate lps[] array)
	computeLPSArray(keyword, M, lps);

	int i = 0;  // index for txt[]
	int j = 0;  // index for pat[]
	while (i < N)
	{
		if (keyword[j] == filename[i])
		{
			j++;
			i++;
		}

		if (j == M)
		{
			return i - j; // vi tri tim thay chuoi
		}

		// mismatch after j matches
		else if (i < N && keyword[j] != filename[i])
		{
			if (j != 0)
				j = lps[j - 1];
			else
				i = i + 1;
		}
	}
	return -1;
}

void SortProgram(vector<Program> &CandidateProgram)
{
	for (int i = 0; i < CandidateProgram.size() - 1; i++)
	{
		for (int j = i + 1; j < CandidateProgram.size(); j++)
		{
			if (CandidateProgram[i].posMatch > CandidateProgram[j].posMatch)
			{
				Program tmp = CandidateProgram[i];
				CandidateProgram[i] = CandidateProgram[j];
				CandidateProgram[j] = tmp;
			}
		}
	}
}
