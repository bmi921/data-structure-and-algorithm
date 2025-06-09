#include <stdlib.h>
#include <stdio.h>
#include "Knapsack.h"

/* test data */
extern unsigned num;
extern OBJ object[];
extern float knapsack;

int main()
{
	// コスパを求める
	for (unsigned i = 0; i < num; i++)
		object[i].performance = object[i].value / object[i].weight;

	// 品物をコスパでソートする
	sortObject(object, num);

	// ナップサックの中身を表す変数を定義
	char *content_name[num];
	float content_weight[num];

	// 初期化
	for (int i = 0; i < num; i++)
	{
		content_name[i] = NULL;
		content_weight[i] = 0;
	}

	// コスパが良い品物から1つずつ選ぶ
	for (int i = 0; i < num; i++)
	{
		if (knapsack == 0)
		{
			break;
		}
		if (object[i].weight < knapsack)
		{
			content_name[i] = object[i].name;
			content_weight[i] = object[i].weight;
			knapsack = knapsack - object[i].weight;
		}
		if (object[i].weight > knapsack)
		{
			content_name[i] = object[i].name;
			content_weight[i] = knapsack;
			knapsack = 0;
		}
	}

	for (int i = 0; i < num; i++)
	{
		if (content_name[i] != NULL)
			printf("[%d] name:%s weight: %f\n", i + 1, content_name[i], content_weight[i]);
	}
}

// ・・
void showOBJ(OBJ *obj, unsigned num)
{
	for (int i = 0; i < num; i++)
		printf("%s %f %f %f\n", obj[i].name, obj[i].weight, obj[i].value, obj[i].performance);

	printf("\n");
}

// OBJ型の配列を交換する関数
void changeOBJ(OBJ *obj1, OBJ *obj2)
{
	OBJ tmp = *obj1;
	*obj1 = *obj2;
	*obj2 = tmp;
}

// コスパ順にバブルソートする関数
void sortObject(OBJ obj[], unsigned n)
{
	/* make it! */
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n - i; j++)
		{
			if (obj[j].performance < obj[j + 1].performance)
				changeOBJ(&obj[j], &obj[j + 1]);
		}
	}
}