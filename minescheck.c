#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

void setMines(int N, int grid[N][N]);
void displayGrid(int N, int grid[N][N], int visibilty[N][N]);
void setNeighbourCount(int N, int grid[N][N], int i, int j);
int reveal(int N, int grid[N][N], int visiblity[N][N], int row, int col);
void bellSound();
void floodCall(int N, int grid[N][N], int visibility[N][N], int row, int col);
int wonGame(int N, int visibility[N][N]);
void textDisplay(char *text);

int main(){
	int N = 10;
	int grid[N][N];
	int visibility[N][N];
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			visibility[i][j] = 0;
		}
	}
	int gameOver = 0;
	setMines(N,grid);
	int row, col;
	while(gameOver!=1)
	{
		system("clear");
		displayGrid(N,grid,visibility);

		int nextStep;
		do{
			printf("\nRow Column = ");
			scanf("%d %d", &row, &col);
			if(row == -1 && col == -1){
				return 1;
			}
		}while(row < 1 || row > N || col < 1 || col > N);
		printf("\nEnter choice[r - reveal/ f - flag]:");
		char choice;
		scanf(" %c",&choice);
		if(tolower(choice) == 'f'){
			if(visibility[row-1][col-1] == 0){
				visibility[row-1][col-1] = 2;
			}else if(visibility[row-1][col-1] == 2){
				visibility[row-1][col-1] = 0;
			}
			nextStep=0;
		}
		else{
			nextStep = reveal(N, grid, visibility, row-1, col-1);
		}
		if(nextStep == -1){
			system("clear");
			for(int i=0;i<N;i++){
				for(int j=0;j<N;j++){
					if(grid[i][j] == -1){
						visibility[i][j] = 1;
					}
				}
			}
			displayGrid(N, grid, visibility);
			gameOver = 1;
			textDisplay("You Lose!");
		}
		if(wonGame(N,visibility)){
			textDisplay("You Won!");
			return 1;
		}
	}
}

void bellSound(){
	printf("\a");
}

void textDisplay(char *text){
    int length = strlen(text);
    printf("\n");
    for(int i=0;i<length;i++){
    	printf("%c",text[i]);
    	fflush(stdout);
    	usleep(9999);
    }
}

int reveal(int N, int grid[N][N], int visibility[N][N], int row, int col){
	if(visibility[row][col] == 1 || visibility[row][col] == 2){
		bellSound();
		return 0;
	}
	if(grid[row][col] == -1){
		visibility[row][col] = 1;
		return -1;
	}
	if(grid[row][col] == 0){
		floodCall(N, grid, visibility, row, col);
	}
	else{
		visibility[row][col] = 1;
	}
	return 1;
}

void floodCall(int N, int grid[N][N], int visibility[N][N], int row, int col){
	for(int i=-1;i<=1;i++){
		for(int j=-1;j<=1;j++){
			int r = row + i;
			int c = col + j;
			if(r > -1 && r < N && c > -1 && c < N){
				if(grid[r][c] != -1 && visibility[r][c] != 1 && visibility[r][c]!=2){
					visibility[r][c] = 1;
					if(grid[r][c]==0)floodCall(N, grid, visibility, r, c);
				}
			}
		}
	}
}
	
int wonGame(int N, int visibility[N][N]){
	int unRevealedCount = 0;
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			if(visibility[i][j]==0 || visibility[i][j]==2){
				unRevealedCount++;	
			}
		}
	}
	if(unRevealedCount==10){
		return 1;
	}
	return 0;
}

void setMines(int N, int grid[N][N]){
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			grid[i][j] = 0;
		}
	}
	srand(time(0));
	int bombCount = 10;
	int r, limit = 1;
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			r = 1 + (rand() % 10);
			if(r <= limit && bombCount!=0){
				grid[i][j] = -1;
				setNeighbourCount(N,grid,i,j);
				bombCount--;
			}
			if(bombCount!=0 && i > N-2){
				limit++;
			}
		}
	}
}


void setNeighbourCount(int N, int grid[N][N], int i, int j){
	if(grid[i][j+1]!=-1 && j+1<=N-1)grid[i][j+1]++;
	if(grid[i][j-1]!=-1 && j-1>=0)grid[i][j-1]++;
	if(grid[i+1][j]!=-1 && i+1<=N-1)grid[i+1][j]++;
	if(grid[i-1][j]!=-1 && i-1>=0)grid[i-1][j]++;
	if(grid[i+1][j-1]!=-1 && i+1<=N-1 && j-1>=0)grid[i+1][j-1]++;
	if(grid[i+1][j+1]!=-1 && i+1<=N-1 && j+1<=N-1)grid[i+1][j+1]++;
	if(grid[i-1][j-1]!=-1 && i-1>=0 && j-1>=0)grid[i-1][j-1]++;
	if(grid[i-1][j+1]!=-1 && j+1<=N-1 && i-1>=0)grid[i-1][j+1]++;
}

void displayGrid(int N, int grid[N][N], int visibility[N][N]){
	char bomb = '@';
	char empySpace = '-';
	char unrevealed = ' ';
	char verticalBorder = '|';
	char* horizontalBorder = "-----";
	for(int i=0;i<N;i++){
		if(i==0){
			for(int l=0;l<N;l++){
				printf("  %d  ",l+1);
			}
			printf("\n");
		}
	
		for(int k=0; k<N;k++){
			printf("%5s",horizontalBorder);
		}
		printf("\n");
		for(int j=0;j<N;j++){
			if(visibility[i][j]==1){
				if(grid[i][j] == -1){
					printf("| %c  ",bomb);	
				}
				else if(grid[i][j]==0){
					printf("| %c  ",empySpace);
				}
				else{
					printf("| %d  ",grid[i][j]);
				}
			}
			else if(visibility[i][j] == 2){
				printf("| F  ");
			}	
			else{
				printf("| %c  ",unrevealed);
			}
			if(j == N-1){
				printf("|");
			}
			if(j==N-1){
				printf(" %d ",i+1);
			}
		}
		printf("\n");
	}
	for(int i=0; i<N;i++){
		printf("%5s",horizontalBorder);
	}
	printf("\n");
}