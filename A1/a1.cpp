// Written by Hardik Tibrewal
// 18EC10020

#include <bits/stdc++.h>

using namespace std;

typedef struct pair{ //Struct to return a pair of numbers
	double hn;
	double ln;
} PAIR;

double hirec(int n);  //Declaring Function prototypes
double lorec(int n);

PAIR hilorec(int n);
PAIR hiloformula(int n);

int main(){
	int n;
	cin>>n;
	double hn, ln;
	PAIR ans;

	//Taking input and calling all functions 

	cout<<"n = "<<n<<endl;
	cout<<"+++ Method 0\n";
	printf("\thi(%d) = %e, lo(%d) = %e\n\n", n, hirec(n), n, lorec(n));
	ans=hilorec(n);
	cout<<"Method 1\n";
	printf("\thi(%d) = %e, lo(%d) = %e\n\n", n, ans.hn, n, ans.ln);
	ans=hiloformula(n);
	cout<<"Method 2\n";
	printf("\thi(%d) = %e, lo(%d) = %e\n\n", n, ans.hn, n, ans.ln);
	return 0;
}

double hirec(int n){ // Simple recursive function for high energy neutrons
	if(n==0) return 1;
	else return 2*hirec(n-1) + lorec(n-1);
}

double lorec(int n){ // Simple recursive function for low energy neutrons
	if(n==0) return 0;
	else return lorec(n-1) + hirec(n-1);
}

PAIR hilorec(int n){ //Calls both functions above and returns a PAIR
	PAIR ans;
	ans.hn=hirec(n);
	ans.ln=lorec(n);
	return ans;
}

PAIR hiloformula(int n){ //Calculates the value using a given formula which can be derived by solving the recurrences
	PAIR ans;
	ans.hn = ((5+sqrt(5))/10)*pow(((3-sqrt(5))/2),n+1) + ((5-sqrt(5))/10)*pow((((3+sqrt(5))/2)),n+1);
	ans.ln = ((-5-3*sqrt(5))/10)*pow((((3-sqrt(5))/2)),n+1) + ((-5+3*sqrt(5))/10)*pow((((3+sqrt(5))/2)),n+1);
	return ans;	
}

