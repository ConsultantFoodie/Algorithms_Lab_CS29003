// Written by Hardik Tibrewal
// 18EC10020
#include <bits/stdc++.h>

using namespace std;

double profit(int m, double p, double s, double t){ // Returns the expected profit given the probabilities, price, and the number of machines
	double ans, temp=pow(p,m);
	ans=(1-temp)*s+temp*t;
	return ans;
}

void optimalbuy1(int n, int C, int* c, double* s, double* t, double* p){ //Optimal buying wihtout maintenance
	int i, j, k; //Row i represent machines from 0 to i.

	double **T, **S, max;
	T=(double**)malloc(n*sizeof(double*));
	S=(double**)malloc(n*sizeof(double*));

	for(i=0;i<n;i++){
		T[i]=(double*)malloc((C+1)*sizeof(double));
		S[i]=(double*)malloc((C+1)*sizeof(double));
	}

	//Initialising the first row
	for(j=0;j<C+1;j++){
		if(j<c[0]){
			T[0][j]=-DBL_MAX;
			S[0][j]=0;
		}
		else{
			T[0][j]=profit(j/c[0], p[0], s[0], t[0]);
			S[0][j]=j/c[0];
		}
	}

	// Calculating best buy for other rows 
	for(i=1;i<n;i++){
		for(j=0;j<C+1;j++){
			if(j<c[i]){
				T[i][j]=-DBL_MAX;
				S[i][j]=0;
			}
			else{
				max=DBL_MIN;
				for(k=1;k<=j/c[i];k++){
					if((T[i-1][j-k*c[i]]+profit(k, p[i], s[i], t[i]))>max){
						max=T[i-1][j-k*c[i]]+profit(k, p[i], s[i], t[i]);
						S[i][j]=k;
					}
				}
				T[i][j]=max;
			}
		}
	}

	// Finding the best buy for given query using the dp table
	int *arr=(int*)malloc(n*sizeof(int)), temp=C;
	for(i=n-1;i>=0;i--){
		arr[i]=S[i][temp];
		temp-=S[i][temp]*c[i];
	}

	//printing
	temp=0;
	printf("+++ Part 1: Best buying option\n");
	for(i=0;i<n;i++){
		printf("\tMachine %2d: %3d copies, cost = %5d\n", i, arr[i], arr[i]*c[i]);
		temp+=arr[i]*c[i];
	}
	printf("--- Total buying cost            = %5d\n", temp);
	printf("--- Expected total profit        = %5lf\n", T[n-1][C]);
}

void optimalbuy2(int n, int C, int* c, double* s, double* t, double* r, double* p, double* q){ // optimal buying with maintenance of machines
	int i, j, k;

	double **T, **S, **R, maxm, w_rep, wo_rep;
	T=(double**)malloc(n*sizeof(double*));
	S=(double**)malloc(n*sizeof(double*));
	R=(double**)malloc(n*sizeof(double*)); // repair flags

	for(i=0;i<n;i++){
		T[i]=(double*)malloc((C+1)*sizeof(double));
		S[i]=(double*)malloc((C+1)*sizeof(double));
		R[i]=(double*)malloc((C+1)*sizeof(double));
	}

	//init first row
	for(j=0;j<C+1;j++){
		if(j<c[0]){
			T[0][j]=-DBL_MAX;
			S[0][j]=0;
			R[0][j]=0;
		}
		else{
			wo_rep=profit(j/c[0], p[0], s[0], t[0]);
			w_rep=profit(j/c[0], q[0], s[0], t[0])-(j/c[0])*r[0];
			if(w_rep>wo_rep){ //checking which case is better, repair, or no repair
				T[0][j]=w_rep;
				R[0][j]=1;
			}
			else{
				T[0][j]=wo_rep;
				R[0][j]=0;
			}
			S[0][j]=j/c[0];
		}
	}

	//Filling table
	for(i=1;i<n;i++){
		for(j=0;j<C+1;j++){
			if(j<c[i]){
				T[i][j]=-DBL_MAX;
				S[i][j]=0;
				R[i][j]=0;
			}
			else{
				maxm=DBL_MIN;
				for(k=1;k<=j/c[i];k++){
					wo_rep=T[i-1][j-k*c[i]]+profit(k, p[i], s[i], t[i]);
					w_rep=T[i-1][j-k*c[i]]+profit(k, q[i], s[i], t[i])-k*r[i];
					if(max(w_rep,wo_rep)>maxm){ //Checkin conditions and filling table
						if(w_rep>wo_rep){
							maxm=w_rep;
							R[i][j]=1;
						}
						else{
							maxm=wo_rep;
							R[i][j]=0;
						}
						
						S[i][j]=k;
					}
				}
				T[i][j]=maxm;
			}
		}
	}

	//making array for printing
	int *arr=(int*)malloc(n*sizeof(int)), *rep=(int*)malloc(n*sizeof(int)), temp=C;
	for(i=n-1;i>=0;i--){
		arr[i]=S[i][temp];
		rep[i]=R[i][temp];
		temp-=S[i][temp]*c[i];
	}
	temp=0;

	//output
	printf("+++ Part 2: Best buying option\n");
	for(i=0;i<n;i++){
		printf("\tMachine %2d: %3d copies, cost = %5d %s\n", i, arr[i], arr[i]*c[i], rep[i]?"[Maintenance needed]":"[Maintenance not needed]");
		temp+=arr[i]*c[i];
	}
	printf("--- Total buying cost            = %5d\n", temp);
	printf("--- Expected total profit        = %5lf\n", T[n-1][C]);
}

int  main(){
	int i, n, C, *c;
	double *s, *t, *r, *p, *q;
	cin>>n;
	cin>>C;

	c=(int*)malloc(n*sizeof(int));
	s=(double*)malloc(n*sizeof(double));
	t=(double*)malloc(n*sizeof(double));
	r=(double*)malloc(n*sizeof(double));
	p=(double*)malloc(n*sizeof(double));
	q=(double*)malloc(n*sizeof(double));

	//Taking input
	for(i=0;i<n;i++){
		cin>>c[i];
	}
	for(i=0;i<n;i++){
		cin>>s[i];
	}
	for(i=0;i<n;i++){
		cin>>t[i];
	}
	for(i=0;i<n;i++){
		cin>>r[i];
	}
	for(i=0;i<n;i++){
		cin>>p[i];
	}
	for(i=0;i<n;i++){
		cin>>q[i];
	}

	//CAlling both functions
	optimalbuy1(n, C, c, s, t, p);
	optimalbuy2(n, C, c, s, t, r, p, q);
	return 0;
}