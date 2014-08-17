#include "moremath.h" 
#include <cmath>

double choose(double n, double k) {
	double r = 1;
	double d;
	if (k > n) return 0;
	for (d=1; d <= k; d++) {
		r *= n--;
		r /= d;
	}
	return r;
}
double binomial_pdf(int x, double p, double n){
	return choose(n,x) * pow(p,x) * pow(1-p,n-x);
}
double binomial_cdf(int x,double p, int n){
	double sum=0;
	for(int i=0;i<x;i++){
		sum+= binomial_pdf(i,p,n);
	}
	return sum;
}
