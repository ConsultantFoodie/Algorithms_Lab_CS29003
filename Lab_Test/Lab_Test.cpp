//Written by Hardik Tibrewal
//18EC10020
#include<bits/stdc++.h>

using namespace std;

int ischessboard(int **F, int row_stt, int row_fin, int col_stt, int col_fin){ //Function to check if a given subarray is a chessboard
	int i, j;
	for(i=row_stt; i<=row_fin;i++){
		if(i!=row_stt){
			if(F[i][col_stt]==F[i-1][col_stt]){ //For elements in first column, they should be opposite the column above them
				return 0;
			}
		}
		for(j=col_stt+1;j<=col_fin;j++){ //In a row, adjacent elements should be different
			if(F[i][j]==F[i][j-1]){
				return 0;
			}
		}
	}
	return 1; //If loop completes, then it is a chessboard
}

void print_chess(int **F, int m, int n, int row_stt, int row_fin, int col_stt, int col_fin){ //Function to print the chessboard
	int i, j;
	for(i=0;i<m;i++){
		printf("\t");
		for(j=0;j<n;j++){
			if(i>=row_stt && i<=row_fin && j>=col_stt && j<=col_fin){ //If the element is part of the chess board, printing B or W, otherwise printing '.'
				if(F[i][j]==0){
					cout<<"W ";
				}
				else{
					cout<<"B ";
				}
			}
			else cout<<". ";
		}
		cout<<endl;
	}
}

void essquare(int **F, int m, int n){ //Exhaustive search in a square
	int i, j, l, max_i=-1, max_j=-1, max_l=-1;
	for(i=0;i<m;i++){
		for(j=0;j<n;j++){
			l=min(m-i, n-j);
			for(;l>=1;l--){
				if(ischessboard(F, i, i+l-1, j, j+l-1)==1){ //If a given l makes a chessboard, breaking since l decreases on every iteration
					break; //So this has to be the maximal square board starting at (i,j)
				}
			}
			if(l>=max_l){
				max_i=i;
				max_j=j;
				max_l=l;
			}
		}
	}

	//Printing the answer
	cout<<"+++ Exhaustive search: Square"<<endl;
	print_chess(F, m, n, max_i, max_i+max_l-1, max_j, max_j+max_l-1);
	cout<<"\tArea = "<<max_l*max_l<<endl<<endl;
	return;
}

void esrectangle(int **F, int m, int n){ //Exhaustive Search in a rectangle
	int i, j, l, k, max_i=-1, max_j=-1, max_l=0, max_k=0;
	for(i=0;i<m;i++){
		for(j=0;j<n;j++){
			k=m-i;
			for(; k>=1;k--){
				l=n-j;
				for(;l>=1;l--){ //Same logic as square, but now iterating over rows an columns separately
					if(ischessboard(F, i, i+k-1, j, j+l-1)==1){
						break; //breaking since l decreases of every iteration, and for fixed k, area will decrease, so this is maximal
					}
				}
				if(l*k>=max_l*max_k){
					max_i=i;
					max_j=j;
					max_l=l;
					max_k=k;
				}
			}
		}
	}

	//Printing the answer
	cout<<"+++ Exhaustive search: Rectangle\n";
	print_chess(F, m, n, max_i, max_i+max_k-1, max_j, max_j+max_l-1);
	cout<<"\tArea = "<<max_k*max_l<<endl<<endl;
	return;
}

void dpsquare(int** F, int m, int n){ //Function using DP to find maximal square board
	int **T, i, j, l1, l2, max_i=-1, max_j=-1, max_l=-1;
	T=(int**)malloc(m*sizeof(int*));
	for(i=0;i<m;i++){
		T[i]=(int*)malloc(n*sizeof(int));
		T[i][n-1]=1; //initialising T array in this loop, maximal square size is 1 for last row and last column
		if(i==m-1){
			for(j=0;j<n;j++){
				T[i][j]=1; 
			}
		}
	}

	i=m-2;
	for(;i>=0;i--){
		j=n-2;
		for(;j>=0;j--){
			if(F[i][j]==F[i+1][j] || F[i][j]==F[i][j+1]){ //If square matches any adjacent square, max board size is 1
				T[i][j]=1;
			}
			else{
				l1=T[i+1][j]; //Checking which side gives a square board
				l2=T[i][j+1];
				if(l1>l2){
					T[i][j]=l2+1;
				}
				else if(l2>l1){
					T[i][j]=l1+1;
				}
				else{
					if(F[i+l1][j+l1]!=F[i+l1-1][j+l1] && F[i+l1][j+l1]!=F[i+l1][j+l1-1]){ //Checking if the corner element matches its adjacent squares
						T[i][j]=l1+1; //if it does, it is part of the biggest board
					}
					else{
						T[i][j]=l1; //else it is not
					}
				}
			}
		}
	}

	//Finding the maximal board's starting coordinates
	for(i=0;i<m;i++){
		for(j=0;j<n;j++){
			if(T[i][j]>=max_l){
				max_i=i;
				max_j=j;
				max_l=T[i][j];
			}
		}
	}

	//Printing the answer
	cout<<"+++ Dynamic Programming: Square"<<endl;
	print_chess(F, m, n, max_i, max_i+max_l-1, max_j, max_j+max_l-1);
	cout<<"\tArea = "<<max_l*max_l<<endl<<endl;
	return;	
}

void dprectangle(int** F, int m, int n){ //Function using DP to find maximal rectangular board
	int i, j, k, height, **A, **B, max_i, max_j, max_area, min_height, area, max_l, max_k;
	A=(int**)malloc(m*sizeof(int*)); //Contains the original array with every second element flipped
	B=(int**)malloc(m*sizeof(int*)); //Contains the maximum height of similar elements with (i,j) on top
	for(i=0;i<m;i++){
		A[i]=(int*)malloc(n*sizeof(int));
		B[i]=(int*)malloc(n*sizeof(int));		
	}

	for(i=0;i<m;i++){
		for(j=0;j<n;j++){
			if((i+j)%2==0){
				A[i][j]=(F[i][j]+1)%2; //This flips every second element
			}
			else{
				A[i][j]=F[i][j];
			}
		}
	}

	for(i=0;i<m;i++){
		for(j=0;j<n;j++){
			k=i;
			height=1;
			while(k<m-1 && A[k][j]==A[k+1][j]){ //Finds the height of the similar element column
				height++;
				k++;
			}
			B[i][j]=height;
		}
	}
	
	max_i=-1; max_j=-1; min_height=INT_MAX; max_area=0;
	for(i=0;i<m;i++){
		for(j=0;j<n;j++){
			k=j+1;
			min_height=B[i][j];
			if(min_height>max_area){ //Considering only the first column to initialise the checking conditions
				max_area=min_height;
				max_i=i;
				max_j=j;
				max_k=min_height;
				max_l=1;
			}
			while(k<n && A[i][k-1]==A[i][k]){ //checking if adding a new column increases the area even if it decreases the height
				min_height=min(min_height, B[i][k]);
				area=min_height*(k-j+1);

				if(area>max_area){
					max_area=area;
					max_i=i;
					max_j=j;
					max_k=min_height;
					max_l=k-j+1;
				}
				k++;
			}
			
		}
	}
	//Printing the answer
	cout<<"+++ Dynamic Programming: Rectangle"<<endl;
	print_chess(F, m, n, max_i, max_i+max_k-1, max_j, max_j+max_l-1);
	cout<<"\tArea = "<<max_k*max_l<<endl<<endl;
	return;
}

int main(){ //Main Function
	int i, j, m, n, **F;
	F=(int**)malloc(50*sizeof(int*));
	for(i=0;i<50;i++){
		F[i]=(int*)malloc(50*sizeof(int));
	}
	cin>>m>>n;
	for(i=0;i<m;i++){
		for(j=0;j<n;j++){
			cin>>F[i][j];
		}
	}

	//Calling all functions to get the output
	cout<<endl;
	essquare(F, m, n);
	esrectangle(F, m, n);
	dpsquare(F, m, n);
	dprectangle(F, m, n);
	return 0;
}