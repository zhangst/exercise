#include <stdio.h>
#include <math.h>
#include <assert.h>


/**
 * Function mysqrt (y, precision)
 *
 *  Returns
 *    
 *
 *  Parameters
 *    @y:          
 *    @precision:  
 * 
 *  Description
 *    求y的平方根,x*x == y
 *
 **/
static double mysqrt(double y, double precision)
{
	double x, start = 0.0, end = y;
	
	assert(y > 0.0);
	while (1) {
		x = (start + end) / 2;
		if (x*x < y)
			start = x;
		else
			end = x;

		if (fabs(x*x - y) < precision)
			break;
	}

	return x;
}

int main(void)
{
	printf("%f\n", mysqrt(1.0, 0.000001));

	return 0;
}
