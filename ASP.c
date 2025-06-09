#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Activity.h"

/* input data */
extern unsigned num;
extern ACT activity[];

// activityの入れ替え
void swaps(ACT *act1, ACT *act2)
{
	ACT tmp;
	tmp = *act1;
	*act1 = *act2;
	*act2 = tmp;
}

// 終了時間が早い順番にバブルソート
void sortActivity(ACT act[], unsigned n)
{
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < n - 1 - i; j++)
		{
			if (act[j].finish > act[j + 1].finish)
				swaps(&act[j], &act[j + 1]);
		}
	}
}

// 表示
void show(ACT act[], unsigned n)
{
	printf("----------\n");
	for (int i = 0; i < n; i++)
	{
		printf("%s ", act[i].name);
		printf("%d ", act[i].start);
		printf("%d\n ", act[i].finish);
	}
	printf("----------\n");
}

int main()
{
	// 終了時間が早い順番にソート
	sortActivity(activity, num);

	// 貪欲法でtimeまで最適解を選択した
	unsigned time = 0;
	int i = 0;
	while (1)
	{
		if (activity[i].start >= time /*最適解を選ぶ条件式*/)
		{
			time = activity[i].finish;
			printf("%s ", activity[i].name);
		}
		i++;

		// 終了条件
		if (i >= num)
			break;
	}
}