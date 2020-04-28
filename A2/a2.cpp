// Written by Hardik Tibrewal
// 18EC10020

#include <bits/stdc++.h>

using namespace std;

typedef struct { // struct to store all the expressions for a number
	int n;
	vector<string> expr;
	vector<char> etype;
}EXPR;

string combineexpr(string s1, string s2, char e1, char e2, char op){ //comines two expressions by checking types
	string s;
	if(op=='+') s=s1+"+"+s2;
	else{
		if(e1=='+' && e2=='+') s="("+s1+")*("+s2+")";
		else if(e1=='+') s="("+s1+")*"+s2;
		else if(e2=='*') s=s1+"*("+s2+")";
		else s=s1+"*"+s2;
	}
	return s;
}


EXPR findallexpr(int n){ //finds all expressions
	vector<EXPR> ans;
	EXPR E;
	// putting base cases 0 and 1
	E.n=0;
	E.expr.push_back("");
	E.etype.push_back('+');
	ans.push_back(E);
	E.n=1;
	E.expr.clear(); E.etype.clear();
	E.expr.push_back("1");
	E.etype.push_back('+');
	ans.push_back(E);

	int i, j, k, l, ii;
	bool flag;
	string s;
	for(i=2;i<=n;i++){ //Using dynamic programming to build table from bottom up
		E.n=i; E.expr.clear(); E.etype.clear();
		for(j=1;j<=i/2;j++){
			for(k=0;k<ans[j].expr.size();k++){
				for(l=0;l<ans[i-j].expr.size();l++){ //finding all sum pairs
					s=combineexpr(ans[j].expr[k], ans[i-j].expr[l], ans[j].etype[k], ans[i-j].etype[l], '+');
					flag=false;
					for(ii=0;ii<E.expr.size();ii++){ //Checking if any copy is present
						if(s==E.expr[ii]){
							flag=true;
							break;
						}
					}
					if(flag==false){
						E.expr.push_back(s);
						E.etype.push_back('+');
					}

				}
			}
		}
		for(j=2;j<=sqrt(i);j++){ //finding all product pairs
			if(i%j==0){
				for(k=0;k<ans[j].expr.size();k++){
					for(l=0;l<ans[i/j].expr.size();l++){
						s=combineexpr(ans[j].expr[k], ans[i/j].expr[l], ans[j].etype[k], ans[i/j].etype[l], '*');
						flag=false;
						for(ii=0;ii<E.expr.size();ii++){ //Checking if any copy is present
							if(s==E.expr[ii]){
								flag=true;
								break;
							}
						}
						if(flag==false){
							E.expr.push_back(s);
							E.etype.push_back('*');
						}
					}
				}
			}
		}
		ans.push_back(E);
	}
	return ans[n];
}

void print(EXPR E){ //function to print all expressions of a number
	for(int i=0;i<E.expr.size();i++){
		cout<<E.expr[i]<<endl;
	}
	return;
}

int count1(string s){ //function to count number of ones in an expression
	int i, count=0;
	for(i=0;i<s.size();i++){
		if(s[i]=='1'){
			count++;
		}
	}
	return count;
}

bool comp(string s1, string s2){ //Comparator function for std::sort
	int cnt1=count1(s1), cnt2=count1(s2);
	if(cnt1>cnt2) return true;
	else if(cnt1==cnt2){
		return s1.size()>s2.size();
	}
	else return false;
}

int main(){
	int n;
	cin>>n;
	EXPR ANS;
	//calling function
	ANS = findallexpr(n);
	cout<<"n = "<<n<<endl<<endl;
	print(ANS);
	cout<<endl;

	//sorting and calling function
	sort(ANS.expr.begin(), ANS.expr.end(), comp);
	print(ANS);
	return 0;
}