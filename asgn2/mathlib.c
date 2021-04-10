#include "mathlib.h"

#include <math.h>

#define EPSILON 1e-14

static inline long double Abs(long double x) {
    return x < 0 ? -x : x;
}

//returns absolute value of a number
//@param x - number you are getting the absolute value of
double Exp(double x) {
    double term = 1.0;
    double sum = term;

    for (long double k = 1.0; Abs(term) > EPSILON; k += 1.0) {
        term = x / k * term;
        sum += term;
    }

    return sum;
}

//calculates lnx for a given value of x
//@param x - the value of x in lnx
//@return - the value of lnx
double Log(double x) {
    double y = 1.0;
    double p = Exp(y);
    while (Abs(p - x) > EPSILON) {
        y = y + (x - p) / p;
        p = Exp(y);
    }
    return y;
}

//calculates sinx for a given value of x
//@param x - the value of x in sinx
//@return - the value of sinx
double Sin(double x) {
    //makes x lay between -2pi and 2pi so that it converges more quickley
    double threshold = 2 * M_PI;
    while (x < -threshold) {
        x += threshold;
    }
    while (x > threshold) {
        x -= threshold;
    }

    double sum = x;
    double numerator = x;
    double denominator = 1;
    double quotient = numerator / denominator;
    for (double k = 3; Abs(quotient) > EPSILON; k += 2) {
        numerator *= -x * x;
        denominator *= k * (k - 1.0);
        quotient = numerator / denominator;
        sum += quotient;
    }
    return sum;
}

//calculates cosx for a given value of x
//@param x - the value of x in cosx
//return - the value of cosx
double Cos(double x) {
    //makes x lay between -2pi and 2pi so that it converges more quickley
    double threshold = 2 * M_PI;
    while (x < -threshold) {
        x += threshold;
    }
    while (x > threshold) {
        x -= threshold;
    }

    double numerator = 1;
    double denominator = 1;
    double quotient = numerator / denominator;
    double sum = quotient;
    for (double k = 2; Abs(quotient) > EPSILON; k += 2) {
        numerator *= -x * x;
        denominator *= k * (k - 1.0);
        quotient = numerator / denominator;
        sum += quotient;
    }
    return sum;
}

//calculates tanx for a given value of x
//@param x - the value of x in tanx
//return - the value of tanx
double Tan(double x) {
    return Sin(x) / Cos(x);
}
