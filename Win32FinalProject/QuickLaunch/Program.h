#pragma once
#include "stdafx.h"
#include <string>
#include <cstring>
#include <vector>
using namespace std;

struct Program
{
	wstring FilePath;
	wstring InstallLocation;
	wstring FileName;
	int nIcon;
	int index; // chi so cua tung phan tu trong vector
	int freq;
	int posMatch;
};

struct FreqProgram
{
	wstring FileName;
	int freq;
};
