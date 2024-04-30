#pragma once

#include <cmath>
#include <boost/math/distributions/normal.hpp>
#include <boost/math/distributions/students_t.hpp>
#include <boost/math/distributions/chi_squared.hpp>
#include <boost/math/distributions/fisher_f.hpp>
#include <boost/math/distributions/non_central_t.hpp>
#include <boost/math/distributions/non_central_chi_squared.hpp>
#include <boost/math/distributions/non_central_f.hpp>
#include <boost/math/distributions/binomial.hpp>

using namespace boost::math;

double maxrealnumber=pow(10,300);
double expm2 = 0.13533528323661269189;
double s2pi=2.50662827463100050242;

// CDF - вероятность (ИНТЕГРАЛ)
// PDF - плотность (ПОДЫИНТЫГРАЛЬНАЯ ФУНКЦИИ)
// PPF - квантиль (ПРОЦЕНТНАЯ ТОЧКА)

double norm_cdf(double);
double norm_pdf(double);
double norm_ppf(double);
double t_cdf(double,double);
double t_pdf(double,double);
double t_ppf(double,double);
double chi_cdf(double,double);
double chi_pdf(double,double);
double chi_ppf(double,double);
double f_cdf(double,double,double);
double f_pdf(double,double,double);
double f_ppf(double,double,double);
double nct_cdf(double,double,double);
double nct_pdf(double,double,double);
double nct_ppf(double,double,double);
double nchi_cdf(double,double,double);
double nchi_pdf(double,double,double);
double nchi_ppf(double,double,double);
double ncf_cdf(double,double,double,double);
double ncf_pdf(double,double,double,double);
double ncf_ppf(double,double,double,double);
double invnormaldistribution(double y0); 

//#############Normal Distribution############################

double norm_cdf(double x) {
   normal_distribution<>d(0,1);  
   return(cdf(d,x)); 
}
double norm_ppf(double p) {
   if(p<=0 || p>=1) return 0;
   normal_distribution<>d(0,1);  
   return(quantile(d,p)); 
}
double norm_pdf(double x) {
   normal_distribution<>d(0,1);  
   return(pdf(d,x)); 
}

//#############Student Distribution############################

double t_cdf(double x,double f) {
   students_t_distribution<>d(f);  
   return(cdf(d,x)); 
}
double t_ppf(double p,double f) {
   if(p<=0 || p>=1) return 0;
   students_t_distribution<>d(f); 
   return(quantile(d,p)); 
}
double t_pdf(double x,double f) {
   students_t_distribution<>d(f);  
   return(pdf(d,x)); 
}
//#############Chi-Squared Distribution############################
double chi_cdf(double x,double f) {
   chi_squared_distribution<>d(f);  
   return(cdf(d,x)); 
}
double chi_ppf(double p,double f) {
   if(p<=0 || p>=1) return 0;
   chi_squared_distribution<>d(f); 
   return(quantile(d,p)); 
}
double chi_pdf(double x,double f) {
   chi_squared_distribution<>d(f);  
   return(pdf(d,x)); 
}
//#############F-Distribution############################
double f_cdf(double x,double f1,double f2) {
   fisher_f_distribution<>d(f1,f2);  
   return(cdf(d,x)); 
}
double f_ppf(double p,double f1,double f2) {
   if(p<=0 || p>=1) return 0;
   fisher_f_distribution<>d(f1,f2); 
   return(quantile(d,p)); 
}
double f_pdf(double x,double f1,double f2) {
   fisher_f_distribution<>d(f1,f2);  
   return(pdf(d,x)); 
}
//#############Non Central t-Distribution############################

double nct_cdf(double x,double f,double delta) {
  non_central_t_distribution<>d(f,delta);
  return(cdf(d,x));
}
double nct_ppf(double p,double f,double delta) {
  if(p<=0 || p>=1) return 0;
  non_central_t_distribution<>d(f,delta);
  return(quantile(d,p));
}
double nct_pdf(double x,double f,double delta) {
  non_central_t_distribution<>d(f,delta);
  return(pdf(d,x));
}
//#############Non Central chi-squared-Distribution############################

double nchi_cdf(double x,double f,double delta) {
  non_central_chi_squared_distribution<>d(f,delta);
  return(cdf(d,x));
}
double nchi_ppf(double p,double f,double delta) {
  if(p<=0 || p>=1) return 0;
  non_central_chi_squared_distribution<>d(f,delta);
  return(quantile(d,p));
}
double nchi_pdf(double x,double f,double delta) {
  non_central_chi_squared_distribution<>d(f,delta);
  return(pdf(d,x));
}
//#############Non Central F-Distribution############################

double ncf_cdf(double x,double f1,double f2,double delta) {
  non_central_f_distribution<>d(f1,f2,delta);
  return(cdf(d,x));
}
double ncf_ppf(double p,double f1,double f2,double delta) {
  if(p<=0 || p>=1) return 0;
  non_central_f_distribution<>d(f1,f2,delta);
  return(quantile(d,p));
}
double ncf_pdf(double x,double f1,double f2,double delta) {
  non_central_f_distribution<>d(f1,f2,delta);
  return(pdf(d,x));
}


//###########################################################
double invnormaldistribution(double y0) {
	double result, x, y, z, y2, x0, x1, p0, q0, p1, q1, p2, q2, zz;
	int code;

	if (y0 <= 0) {
		result = -maxrealnumber;
		return result;
	}
	if (y0 >= 1) {
		result = maxrealnumber;
		return result;
	}
	code = 1;
	y = y0;
	if (y > 1.0 - expm2) {
		y = 1.0 - y;
		code = 0;
	}
	if (y > expm2) {
		y = y - 0.5;
		y2 = y * y;
		p0 = -59.9633501014107895267;
		p0 = 98.0010754185999661536 + y2 * p0;
		p0 = -56.6762857469070293439 + y2 * p0;
		p0 = 13.9312609387279679503 + y2 * p0;
		p0 = -1.23916583867381258016 + y2 * p0;
		q0 = 1;
		q0 = 1.95448858338141759834 + y2 * q0;
		q0 = 4.67627912898881538453 + y2 * q0;
		q0 = 86.3602421390890590575 + y2 * q0;
		q0 = -225.462687854119370527 + y2 * q0;
		q0 = 200.260212380060660359 + y2 * q0;
		q0 = -82.0372256168333339912 + y2 * q0;
		q0 = 15.9056225126211695515 + y2 * q0;
		q0 = -1.18331621121330003142 + y2 * q0;
		x = y + y * y2 * p0 / q0;
		x = x * s2pi;
		result = x;
		return result;
	}

	zz = log(y);
	x = sqrt(-(2.0 * zz));
	x0 = x - log(x) / x;
	z = 1.0 / x;

	if (x < 8.0) {
		p1 = 4.05544892305962419923;
		p1 = 31.5251094599893866154 + z * p1;
		p1 = 57.1628192246421288162 + z * p1;
		p1 = 44.0805073893200834700 + z * p1;
		p1 = 14.6849561928858024014 + z * p1;
		p1 = 2.18663306850790267539 + z * p1;
		p1 = -(1.40256079171354495875 * 0.1) + z * p1;
		p1 = -(3.50424626827848203418 * 0.01) + z * p1;
		p1 = -(8.57456785154685413611 * 0.0001) + z * p1;
		q1 = 1;
		q1 = 15.7799883256466749731 + z * q1;
		q1 = 45.3907635128879210584 + z * q1;
		q1 = 41.3172038254672030440 + z * q1;
		q1 = 15.0425385692907503408 + z * q1;
		q1 = 2.50464946208309415979 + z * q1;
		q1 = -(1.42182922854787788574 * 0.1) + z * q1;
		q1 = -(3.80806407691578277194 * 0.01) + z * q1;
		q1 = -(9.33259480895457427372 * 0.0001) + z * q1;
		x1 = z * p1 / q1;
	}
	else {
		p2 = 3.23774891776946035970;
		p2 = 6.91522889068984211695 + z * p2;
		p2 = 3.93881025292474443415 + z * p2;
		p2 = 1.33303460815807542389 + z * p2;
		p2 = 2.01485389549179081538 * 0.1 + z * p2;
		p2 = 1.23716634817820021358 * 0.01 + z * p2;
		p2 = 3.01581553508235416007 * 0.0001 + z * p2;
		p2 = 2.65806974686737550832 * 0.000001 + z * p2;
		p2 = 6.23974539184983293730 * 0.000000001 + z * p2;
		q2 = 1;
		q2 = 6.02427039364742014255 + z * q2;
		q2 = 3.67983563856160859403 + z * q2;
		q2 = 1.37702099489081330271 + z * q2;
		q2 = 2.16236993594496635890 * 0.1 + z * q2;
		q2 = 1.34204006088543189037 * 0.01 + z * q2;
		q2 = 3.28014464682127739104 * 0.0001 + z * q2;
		q2 = 2.89247864745380683936 * 0.000001 + z * q2;
		q2 = 6.79019408009981274425 * 0.000000001 + z * q2;
		x1 = z * p2 / q2;
	}
	x = x0 - x1;
	if (code != 0) x = -x;
	result = x;
	return result;
}
