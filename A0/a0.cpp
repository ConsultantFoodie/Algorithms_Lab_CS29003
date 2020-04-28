//By Hardik Tibrewal
//18EC10020

#include<bits/stdc++.h>

using namespace std;

typedef struct node{  //Struct for node of the linked list
	int date;
	node* hptr;
	node*vptr;
} NODE;


NODE* initcal(){  // To make a 20x21 linked list of zeros
	NODE *C, *temp_col, *temp_row, *prev_row, *prev_col;
	int i, j;
	C=new NODE;
	C->date=0;
	temp_row=temp_col=C;
	for(i=1;i<21;i++){  //Initialising first row
		temp_row->hptr=new NODE;
		temp_row=temp_row->hptr;
		temp_row->date=0;
	}
	for(i=1;i<20;i++){ //Initialising other rows and linking with rows above them
		prev_col=temp_col;
		prev_row=prev_col;
		prev_col->vptr=new NODE;
		temp_col=prev_col->vptr;
		temp_col->date=0;
		temp_row=temp_col;
		for(j=1;j<21;j++){
			temp_row->hptr=new NODE;
			temp_row=temp_row->hptr;
			temp_row->date=0;
			prev_row=prev_row->hptr;
			prev_row->vptr=temp_row;
		}
	}

	return C;
}

void printcal(NODE* C){ //Function of print the calendar neatly
	NODE* row, *col;
	row=col=C;
	int month_cnt=0, col_cnt=0, row_cnt=0;
	string arr[7] = {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"};
	string brr[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
	while(col!=NULL){
		row=col;
		if(col_cnt%5==0){
			cout<<endl;
			int len0=brr[month_cnt].size(), len1=brr[month_cnt+1].size(), len2=brr[month_cnt+2].size();
			cout<<setw((20-len0)/2 +len0+1)<<brr[month_cnt]<<setw((20-len0)/2 + len0%2+1)<<"  ";
			cout<<setw((20-len1)/2 +len1+1)<<brr[month_cnt+1]<<setw((20-len1)/2 + len0%2 + len1%2+1)<<"  ";
			cout<<setw((20-len2)/2 +len2+1)<<brr[month_cnt+2]<<endl;
			month_cnt+=3;
			for(int i=0;i<21;i++){
				if(i%7==0 && i>0) cout<<" ";
				cout<<arr[i%7]<<" ";
			}
			cout<<endl;
		}
		row_cnt=0;
		while(row!=NULL){
			if(row->date!=0) printf("%2d ", row->date);
			else printf("   ");
			row=row->hptr;
			row_cnt++;
			if(row_cnt%7==0) cout<<" ";
		}
		cout<<endl;
		col=col->vptr;
		col_cnt++;
	}
}

int storemonth(NODE* C, int month, int year, int start){ // Sets the calendar for a given month given the day on the 1st of the month
	NODE* row, *col;
	int i, j;
	int arr[]={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if((year%400)||(year%4 && !(year%100))){
		arr[1] = 29; //Checks for leap year and makes adjustments
	}
	row=C;
	int row_p = month/3, col_p = month%3; // To reach the start of the month block
	while(row_p--){
		for(i=0;i<5;i++){
			row=row->vptr;
		}
	}
	col=row;
	while(col_p--){
		for(i=0;i<7;i++){
			row=row->hptr;
		}
	}
	NODE* here = row;
	col=row;
	int day=0, wrap=0;
	for(i=0;i<start;i++){
		row=row->hptr;
		day++;
	}
	int count=1;
	while(count<=arr[month]){ // Adding the dates
		row->date=count;
		count++;
		if(day<6){
			row=row->hptr;
			day++;
		} 
		else{
			col=col->vptr;
			wrap++;
			if(wrap==5) col=here;
			row=col;
			day=0;
		}
	}
	return day; // Returning the day for the next month's first 
}

int main(){
	NODE* C;
	C = initcal();
	int year, day;
	cout<<"Year: ";
	cin>>year;
	cout<<"Which day of the week is it on 1st January, "<<year<<"(0 for Sunday, 1 for Monday, ..., 6 for Saturday): ";
	cin>>day; //Input

	for(int i=0;i<12;i++){
		day=storemonth(C, i, year, day); // Making the calendar
	}
    
    //Printing the calendar
	printcal(C);
	return 0;
}