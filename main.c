#include <stdio.h>
#include <stdlib.h>
#include <windows.h> 
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include "function.h"

int n, m, T, MT, MD;;
int ThiefPosition[3];

int RandX();
int RandY();
int*** FirstPositions(int** Matrix, int* NumofPolices, int NumofPoliceStations);
bool CheckInside(int x, int y);
void UpdateFlag(int** Matrix, int* Flag, int NumofPoliceStations);
int Move(int x, int y, int* origin, int** Matrix);
int RandMoveThief(int** Matrix);
int MoveThief(int** Matrix);
int RandMovePolice(int* origin, int** Matrix);
int Trace(int* origin, int** Matrix);
int MovePolices(int Flag, int* PolicePosition, int** Matrix);
void Print(int** Matrix);


int main() {
	int NumofPoliceStations;
	printf("Please enter length and width of the field:\n");
	scanf_s("%d %d", &n, &m);
	int** mat = (int**)malloc(n * sizeof(int*)); //make and initialize a 2D array for matrix
	for (int i = 0;i < n;i++) {
		mat[i] = (int*)malloc(m * sizeof(int));
		for (int j = 0;j < m;j++) {
			mat[i][j] = 0;
		}
	}
	printf("How many policestation would you like to have?\n");
	scanf_s("%d", &NumofPoliceStations);
	int* NumofPolices = (int*)malloc(NumofPoliceStations * sizeof(int));
	for (int i = 0; i < NumofPoliceStations; i++) {
		printf("How many polices are there at station %d?\n", i+1);
		scanf_s("%d", &NumofPolices[i]);
	}
	int*** Polices = FirstPositions(mat, NumofPolices, NumofPoliceStations);
	int* Flag = (int*)malloc(NumofPoliceStations * sizeof(int));
	for (int i = 0;i < NumofPoliceStations;i++) {
		Flag[i] = 0;
	}
	UpdateFlag(mat, Flag, NumofPoliceStations);
	Print(mat);
	Sleep(1000);
	while (1) {
		T++;
		if (MoveThief(mat) == -1) {
			return 0;
		}
		UpdateFlag(mat, Flag, NumofPoliceStations);
		for (int i = 0;i < NumofPoliceStations;i++) {
			for (int j = 0;j < NumofPolices[i];j++) {
				if (MovePolices(Flag[i], Polices[i][j], mat) == -1) {
					return 0;
				}
			}
		}
		Print(mat);
		Sleep(1000);
	}
}