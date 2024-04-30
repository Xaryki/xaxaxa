#include <string>
#include <fstream>
#include <cmath>
#include <iomanip> 
#include <vector> 
#include <map> 
#include <functional>

 void standart(int n,double *x,double &a,double &s);
 void my_sort(int n,double *x);
 //void prog1(string);

/*
void prog1(string ff) {
    string st;
    int i,n;
    double a,s,*x;
    
    // Read Data
    ifstream inp("Inp"+ff+".inp");
    ofstream out("Out"+ff+".out");

    inp>>st;
    inp>>n;
    x=new double[n];
    inp>>st;
    for(i=0;i<n;i++) { 
      inp>>x[i];
    }
    inp.close();

    my_sort(n,x);

    for(i=0;i<n;i++) { 
      out<<x[i]<<" ";
    }    


    standart(n,x,a,s);
    out<<endl;
    out<<"n="<<n<<endl;
    out<<"a="<<a<<endl;
    out<<"s="<<s<<endl;
    out.close();

}
*/


//###################################################

 void standart(int n,double *x,double &a,double &s) {

 int i;
 double s1,s2;

 s1=0;s2=0;
 for(i=0;i<n;i++) {
  s1+=x[i];
  s2+=x[i]*x[i];
 }

 a=s1/n;
 s=s2-n*a*a;
 s=sqrt(s/(n-1.));

 }


//#################################################

 void my_sort(int n,double *x) {
   int i,j;
   double s;

   for (i = 0; i < n; i++) {
    for (j = 0; j < n-1; j++) {
      if (x[j] > x[j + 1]) {
        s = x[j]; // создали дополнительную переменную
        x[j] = x[j + 1]; // меняем местами
        x[j + 1] = s; // значения элементов
      }
    }
  }
}

