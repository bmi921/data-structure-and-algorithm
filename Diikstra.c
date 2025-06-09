#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include "Dijkstra.h"

// 親ノードに向かって距離でヒープ化->根が距離の最小値になる
void changeHeap(HEAP *heap, unsigned pos, unsigned *dist)
{
	if (pos == 0)
		return;
	unsigned posx = (pos - 1) / 2;
	unsigned posb = posx * 2 + 2;
	if (posb == pos)
	{
		posb--;
		if (dist[heap->val[pos]] < dist[heap->val[posb]])
		{
			unsigned tmp = heap->val[pos];
			heap->val[pos] = heap->val[posb];
			heap->val[posb] = tmp;
			changeHeap(heap, pos, dist);
			pos = posb;
		}

		if (dist[heap->val[pos]] < dist[heap->val[posx]])
		{
			unsigned tmp = heap->val[pos];
			heap->val[pos] = heap->val[posx];
			heap->val[posx] = tmp;
			changeHeap(heap, posx, dist);
		}
	}
}

// 末端に向かって添え字でヒープ化
void heapify(HEAP *heap, unsigned pos)
{
	// 左、右の子の位置
	unsigned posl = 2 * pos + 1;
	unsigned posr = 2 * pos + 2;

	// 親が最小値だと仮定
	unsigned min = pos;

	if (posl < heap->num && heap->val[posl] < heap->val[min])
	{
		min = posl;
	}
	if (posr < heap->num && heap->val[posr] < heap->val[min])
	{
		min = posr;
	}
	if (min != pos)
	{
		unsigned tmp = heap->val[pos];
		heap->val[pos] = heap->val[min];
		heap->val[min] = tmp;
		heapify(heap, min);
	}
}

unsigned removeRoot(HEAP *heap)
{
	// 根ノードの値をrootに格納
	unsigned root = heap->val[0];
	// 木構造の一番下の値を根ノードの値にする
	heap->val[0] = heap->val[--heap->num];

	// heap->num :木構造の一番下の値
	// pos: ヒープ化したい木構造の頂点
	// heap :ヒープ化したい対象　　としてヒープ化する
	heapify(heap, 0);

	return root;
}

HEAP *newHeap(unsigned num)
{
	HEAP *new;
	if ((new = (HEAP *)malloc(sizeof(HEAP))) == NULL)
	{
		perror("no more memory");
		exit(1);
	}
	new->num = num;
	if ((new->val = (unsigned *)calloc(num, sizeof(unsigned))) == NULL)
	{
		perror("too large");
	}
	return new;
}

void freeHeap(HEAP *heap)
{
	free(heap->val);
	free(heap);
}

void showHeap(HEAP *heap, unsigned *dist)
{
	printf("heap_num: %d\n", heap->num);
	for (int i = 0; i < heap->num; i++)
	{
		printf("[%d] val:%u  dist:%u\n", i, heap->val[i], dist[i]);
	}
}

void main()
{
	unsigned dist[NMAX];
	/* initialise dist */
	dist[0] = 0;
	for (unsigned i = 1; i < numN; i++)
		dist[i] = UINT_MAX;

	// heapの作成
	HEAP *heap = newHeap(numN);
	// 添え字の作成
	for (unsigned i = 0; i < numN; i++)
		heap->val[i] = i;

	/* greedy method */
	while (heap->num != 0)
	{

		/* get current nearest node */
		unsigned xm = removeRoot(heap);

		/* loop for further nodes */
		for (unsigned i = 0; i < heap->num; i++)
		{
			unsigned xi = heap->val[i];
			/* check connectivity */
			if (weight[xm][xi] == 0)
				continue;
			/* di = min{di, dm+w(xm,xi)} */
			int sum = dist[xm] + weight[xm][xi];
			if (sum <= dist[xi])
			{
				dist[xi] = sum;
				changeHeap(heap, xi, dist);
			}
		}
	}
	/* output results */
	for (unsigned xi = 0; xi < numN; xi++)
	{
		printf("x[%d]= %d\n", xi, dist[xi]);
	}
}