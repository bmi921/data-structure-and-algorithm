#include <stdlib.h>
#include <stdio.h>
#include "MSP.h"

/*int型の最小値*/
#define INTMIN -2147483648

/* input data */
extern unsigned num;
extern int data[];
int calltime = 0;

/*Calculate Leftside*/
int CL(unsigned from, unsigned num)
{

	int sumL[num];
	sumL[0] = data[from + num - 1];
	for (int i = 0; i < num - 1; i++)
	{
		sumL[i + 1] = sumL[i] + data[from + num - i - 2];
	}

	int max = INTMIN;

	for (int i = 0; i < num; i++)
	{
		if (max < sumL[i])
			max = sumL[i];
	}

	return max;
}

/*Calculate Rightside */
int CR(unsigned from, unsigned num)
{
	int sumR[num];
	sumR[0] = data[from];
	for (int i = 0; i < num - 1; i++)
	{
		sumR[i + 1] = sumR[i] + data[from + i + 1];
	}

	int max = INTMIN;

	for (int i = 0; i < num; i++)
	{
		if (max < sumR[i])
			max = sumR[i];
	}

	return max;
}

/*最大部分配列を求める関数*/
int maxSubarray(unsigned from, unsigned num)
{
	/* stop recursive call */
	// num=1の場合
	if (num == 1)
		return data[from];

	// num=2の場合
	if (num == 2)
	{
		int max = INTMIN;
		if (max < data[from])
			max = data[from];
		if (max < data[from + 1])
			max = data[from + 1];
		if (max < (data[from] + data[from + 1]))
			max = (data[from] + data[from + 1]);

		return (max);
	}

	if (num >= 3)
	{
		/* divide */

		unsigned left_start = from;
		unsigned left_end = num / 2;
		unsigned right_start = from + num / 2;
		unsigned right_end = (num - num / 2);

		/* conquer */
		int maxL = 0;
		int maxR = 0;
		maxL = maxSubarray(left_start, left_end);
		maxR = maxSubarray(right_start, right_end);

		int L, R, M;
		L = CL(left_start, left_end);
		R = CR(right_start, right_end);
		M = L + R;

		/* combine */
		int max = INTMIN;

		if (max < maxL)
			max = maxL;
		if (max < maxR)
			max = maxR;
		if (max < L)
			max = L;
		if (max < R)
			max = R;
		if (max < M)
			max = M;

		return max;
	}
}

int main()
{
	printf("%d\n", maxSubarray(0, num));
}