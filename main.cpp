#include<unordered_map>
#include<unordered_set>
#include<list>
#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<vector>
#include<ctime>
#include<algorithm>
#include<cstring>
#include<set>
#include<omp.h>
using namespace std;
int total,minlength,nowlength;
const int m=100;
const int T=60000;
const double Pc=0.2;
const double Pm=0.2;
struct example{
	int a[10];
};
struct Table{
	vector<example> calc;
	int fitness;
	int length;
	
};

int a[55],s[55];
Table s1[m+5],s2[m+5];
double fitness[m+5];
unordered_set<int> f;
Table* a1;
Table* a2;
Table T1,T2;
int totalpair=0;
int nowans=-1;
void init_input(){
	cout<<"请输入参数的个数"<<endl;
	cin>>total;
	cout<<"请输入每个参数的值的取值个数"<<endl;
	for (int i=1;i<=total;i++) cin>>a[i],s[i]=s[i-1]+a[i];
	for (int i=1;i<=total;i++)
		for (int j=i+1;j<=total;j++){
			if (a[i]*a[j]>minlength) minlength=a[i]*a[j];
			totalpair+=a[i]*a[j];
		}
}
inline void calc_fitness(int now){
	
	f.clear();
	for (int i=0;i<nowlength;i++)
		for (int j=1;j<=total;j++)
			for (int k=j+1;k<=total;k++)
			f.insert((((int)a1[now].calc[i].a[j])*100)+(int)a1[now].calc[i].a[k]);
	a1[now].fitness=f.size();
	if (f.size()==totalpair){
		nowans=now;
	}
}
void init_Table(int now){
	a1[now].calc.resize(minlength);
	for (int i=0;i<minlength;i++){
		for (int j=1;j<=total;j++){
			a1[now].calc[i].a[j]=rand()%a[j]+s[j-1];
		}
	}
	calc_fitness(now);
}
void init_S(){
	for (int i=1;i<=m;i++)
		init_Table(i);
}
inline int found(double t1){
	int l=1,r=m;
	while (l<r){
		int mid=(l+r)/2;
		if (fitness[mid]<t1) l=mid+1;
		else r=mid;
	}
	return l;
}
void addexample(int now){
	f.clear();
	for (int i=0;i<nowlength;i++)
		for (int j=1;j<=total;j++)
			for (int k=j+1;k<=total;k++)
			f.insert(a1[now].calc[i].a[j]*100+a1[now].calc[i].a[k]);
	example temp;
	temp.a[1]=rand()%a[1];
	for (int i=2;i<=total;i++){
		int temp1=1e9,ans1=0;
		for (int k=s[i-1];k<s[i];k++){
			int temp2=0;
			for (int j=1;j<i;j++)
				if (f.find(k*100+temp.a[j])!=f.end()) temp2++;
			if (temp2<temp1) ans1=k,temp1=temp2;
		}
		temp.a[i]=ans1;
	}
	a1[now].calc.push_back(temp);
}
int main(){
	srand(time(0));
	a1=s1;a2=s2;
	init_input();
	nowlength=minlength;
	init_S();
	//#pragma omp parallel for
		for (int i=1;i<=m;i++)
			calc_fitness(i);
	while (true){
		int t=0;
		
		int totalfit=0;
		for (int i=1;i<=m;i++)
			totalfit+=a1[i].fitness;
		fitness[0]=0;
		for (int i=1;i<=m;i++)
			fitness[i]=fitness[i-1]+(double)a1[i].fitness/totalfit;
		while (t<T){
			//#pragma omp parallel for
			for (int tt=0;tt<m/2;tt++){
				T1=a1[found(double(rand()%RAND_MAX)/RAND_MAX)];
				T2=a1[found(double(rand()%RAND_MAX)/RAND_MAX)];
				if (double(rand()%RAND_MAX)/RAND_MAX<=Pc){//Cross
				int temp=rand()%nowlength;
				for (int i=temp;i<nowlength;i++) swap(T1.calc[i],T2.calc[i]);
				}
				if (double(rand()%RAND_MAX)/RAND_MAX<=Pm){//Mutate
				int temp=rand()%nowlength;
				int t1=rand()%total+1;
				T1.calc[temp].a[t1]=rand()%(a[t1])+s[t1-1];
				}
				if (double(rand()%RAND_MAX)/RAND_MAX<=Pm){//Mutate
				int temp=rand()%nowlength;
				int t1=rand()%total+1;
				T2.calc[temp].a[t1]=rand()%(a[t1])+s[t1-1];
				}
				a2[tt*2+1]=T1;
				a2[tt*2+2]=T2;
			}
			swap(a1,a2);
			//#pragma omp parallel for
			for (int i=1;i<=m;i++)
			calc_fitness(i);
			for (int i=1;i<=m;i++)
			fitness[i]=fitness[i-1]+(double)a1[i].fitness/totalfit;
			if (nowans!=-1){
				break;
			}
			t++;
		}
		if (nowans!=-1){
			cout<<"共有"<<nowlength<<"条测试用例"<<endl;
			for (int i=0;i<nowlength;i++){
				for (int j=1;j<=total;j++)
					cout<<(int)a1[nowans].calc[i].a[j]-s[j-1]<<' ';
				cout<<endl;
			}
			break;
		}
		nowlength++;
		for (int i=1;i<=m;i++){
			addexample(i);
		}
	}
	system("pause");
	return 0;
}