
#include <iostream>
#include <cstdint>
#include <cmath>

#include "../include/sharc_ap/sharc_ap.h"


typedef sharc_ap_lib::sharc_ap_fixed_16_8 F_TYPE;

int main()
{   

    float a = 2.4231;
    float b = 6.5878;
    F_TYPE a_fpx = a;
    F_TYPE b_fpx = b;

    F_TYPE c = a_fpx + b_fpx;

    printf("x = %f\n", float(c));

    return 0;
}