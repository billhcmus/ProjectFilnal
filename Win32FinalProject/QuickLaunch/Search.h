#pragma once
#include "stdafx.h"
#include <vector>
#include <string>
#include "Program.h"
using namespace std;


void computeLPSArray(WCHAR *pat, int M, int *lps);

int KMPSearch(WCHAR *keyword, WCHAR* fileName);

void SortProgram(vector<Program> &CandidateProgram); // Sap xep chuong trinh sao cho hop ly