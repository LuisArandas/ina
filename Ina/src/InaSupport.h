//
//  InaSupport.h
//  Ina
//
//  Created by Luis Arandas on 01/09/2019.
//

#ifndef InaSupport_h
#define InaSupport_h


#endif /* InaSupport_h */
#pragma once

using namespace ci;
using namespace std;


//Scaling
int a; //int with a value between 1-100;
int b; //int with a value between 100-1000;
int c = scale(a,1,100,b,100,1000);
//scale a up to b
//the bigger the value of a, the bigger the returned value, to a maximum of 1000
//Ex: if a is 100, returns 1000
//the returned value is beween 100-1000

//Scales a float to an interval
float scale(float A, float A1, float A2, float Min, float Max)
{
    long double percentage = (A-A1)/(A1-A2);
    return (percentage) * (Min-Max)+Min;
}
