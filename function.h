#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

int n, m, T, MT, MD;
int ThiefPosition[3];

int RandX() { //Make a random number between (0,n)
	time_t t = time(NULL);
	srand(t);
	return (rand() % n);
}

int RandY() { //Make a random number between (0,m)
	time_t t = time(NULL);
	srand(t);
	return (rand() % m);
}

bool CheckInside(int x, int y) { //Check (x,y) is in matrix or not
	if (x >= 0 && x < n && y >= 0 && y < m) {
		return true;
	}
	return false;
}

int*** FirstPositions(int** Matrix, int* NumofPolices, int NumofPoliceStations) { //Make the first positions of thief and polices randomly and return a 3D array consist of polices's info
	ThiefPosition[0] = -1; //First element of the array is symbol of the character in matrix and for thief it is -1
	ThiefPosition[1] = RandX();
	ThiefPosition[2] = RandY();
	Matrix[ThiefPosition[1]][ThiefPosition[2]] = ThiefPosition[0]; //Locate the thief in matrix
	int ***Polices = (int***)malloc(NumofPoliceStations * sizeof(int**));
	for (int i = 0; i < NumofPoliceStations; i++) {
		Polices[i] = (int**)malloc(NumofPolices[i] * sizeof(int*));
		for (int j = 0;j < NumofPolices[i];j++) {
			Polices[i][j] = (int*)malloc(3 * sizeof(int));
			int x = RandX(), y = RandY();
			while (Matrix[x][y] != 0) { //Make random cordinate while it refer a full location
				x = RandX();
				y = RandY();
			}
			Polices[i][j][0] = i + 1; //First element of the array is symbol of the character in matrix and for every polices it is the number of its policestation number
			Polices[i][j][1] = x;
			Polices[i][j][2] = y;
			Matrix[x][y] = Polices[i][j][0]; //Locate polices in matrix
		}
	}
	return Polices;
}

void UpdateFlag(int** Matrix, int* Flag, int NumofPoliceStations) { //Update which stations know location of the thief
	for (int i = 0;i < NumofPoliceStations;i++) { //Initialize the flag to zero
		Flag[i] = 0;
	}
	for (int i = -2; i < 3;i++) { //It checks the locations which the thief can be observed
		for (int j = -2;j < 3;j++) {
			if (CheckInside(ThiefPosition[1] + i,ThiefPosition[2] + j)) {
				if (Matrix[ThiefPosition[1] + i][ThiefPosition[2] + j] == -1)
					continue;
				else if (Matrix[ThiefPosition[1] + i][ThiefPosition[2] + j]) //If there where a police in these locations
					Flag[Matrix[ThiefPosition[1] + i][ThiefPosition[2] + j] - 1] = 1; //Flag of its station will be 1
			}
		}
	}
}

int Move(int x, int y, int* origin, int** Matrix) { //A police or thief want to move from origin to (x,y) cordinate
	if (!Matrix[x][y]) { //Movement without any spicial condition
		Matrix[origin[1]][origin[2]] = 0;
		Matrix[x][y] = origin[0];
		origin[1] = x;
		origin[2] = y;
		return 1;
	}
	else if (Matrix[x][y] == -1) { //A police get the thief
		return -1;
	}
	else if (Matrix[x][y] && origin[0] == -1) { //The thief go to location of police
		ThiefPosition[1] = x;
		ThiefPosition[2] = y;
		return -1;
	}
}

int RandMoveThief(int** Matrix) {
	int i, j, sign = 0;
	int x = ThiefPosition[1];
	int y = ThiefPosition[2];
	time_t t = time(NULL);
	srand(t);
	if (x == 0) {
		i = rand() % 2;
	}
	else if (x == n-1) {
		i = -1 + rand() % 2;
	}
	else {
		i = -1 + rand() % 3;
	}
	if (y == 0) {
		j = rand() % 2;
	}
	else if (y == m-1) {
		j = -1 + rand() % 2;
	}
	else {
		j = -1 + rand() % 3;
	}
	if (i == 0 && j == 0) {
		return sign;
	}
	else {
		sign = Move(x + i, y + j, ThiefPosition, Matrix);
	}
}

int MoveThief(int** Matrix) {
	int sign = RandMoveThief(Matrix);
	if (sign == 1) {
		MT++;
	}
	if (sign == -1) {
		MT++;
		printf("The End at (%d, %d)\nIn %d secend(s) the thief moved %d time(s) and polices moved %d time(s)\n", ThiefPosition[1] + 1, ThiefPosition[2] + 1, T, MT, MD);
	}
	return sign;
}

int RandMovePolice(int* origin, int** Matrix) {
	int sign = 0, empty = 1;
	int x = origin[1];
	int y = origin[2];
	time_t t = time(NULL);
	srand(t);
	for (int i = -1;i < 2;i++) {
		for (int j = -1;j < 2;j++) {
			if (CheckInside(x + i, y + j) && Matrix[x + i][y + j] < 1) {
				empty++;
			}
		}
	}
	int temp = rand() % empty;
	if (temp == 0) {
		return sign;
	}
	else {
		while (1) {
			for (int i = -1;i < 2;i++) {
				for (int j = -1;j < 2;j++) {
					if (CheckInside(x + i, y + j) && Matrix[x + i][y + j] < 1) {
						temp--;
						if (!temp) {
							sign = Move(x + i, y + j, origin, Matrix);
							return sign;
						}
					}
				}
			}
		}
	}
}

int Trace(int* origin, int** Matrix) {
	int sign = 0;
	int x = ThiefPosition[1] - origin[1];
	int y = ThiefPosition[2] - origin[2];
	if (x) {
		x /= abs(x);
	}
	if (y) {
		y /= abs(y);
	}
	if (CheckInside(origin[1] + x, origin[2] + y) && Matrix[origin[1] + x][origin[1] + y] < 1) {
		sign = Move(origin[1] + x, origin[2] + y, origin, Matrix);
	}
	return sign;
}

int MovePolices(int Flag, int* PolicePosition, int** Matrix) {
	int sign = 1;
	if (Flag) {
		sign = Trace(PolicePosition, Matrix);
		if (sign == -1) {
			MD++;
			printf("The End at (%d, %d)\nIn %d secend(s) the thief moved %d time(s) and polices moved %d time(s)\n", ThiefPosition[1] + 1, ThiefPosition[2] + 1, T, MT, MD);
		}
		if (sign == 1) {
			MD++;
		}
	}
	else {
		sign = RandMovePolice(PolicePosition, Matrix);
		if (sign == -1) {
			MD++;
			printf("The End at (%d, %d)\nIn %d secend(s) the thief moved %d time(s) and polices moved %d time(s)\n",ThiefPosition[1] + 1, ThiefPosition[2] + 1, T, MT, MD);
		}
		if (sign == 1) {
			MD++;
		}
	}
	return sign;
}


void Print(int** Matrix) {
	system("cls");		
	for (int i = 0;i < n;i++) {
		for (int j = 0;j < m;j++) {
			if (Matrix[i][j] == -1) {
				printf("%-10c|", 'T');
			}
			else if (Matrix[i][j] == 0) {
				printf("%-10c|", ' ');
			}
			else {
				printf("D%d", Matrix[i][j]);
				for (int k = 0;k < (8 - log10(Matrix[i][j]));k++) {
					printf(" ");
				}
				printf("|");
			}
		}
		printf("\n");
		for (int j = 0;j < 2;j++) {
			for (int k = 0;k < 10 * m;k++) {
				printf(" ");
				if (k % 10 == 9) {
					printf("|");
				}
			}
			printf("\n");
		}
		printf("\n");
	}
}
