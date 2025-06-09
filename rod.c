#include <stdio.h>
#include <stdlib.h>

// 長さi で切る -> 金額 price[i] (1から始まる)
int p[11] = {0, 1, 5, 8, 9, 10, 17, 17, 20, 24, 30};

// 2値を返すための構造体
typedef struct Result
{
    int *bestPrice;
    int *cutData;
} result;

result *cutRod(int n)
{
    // 切らない場合　自明
    if (n == 0)
        return 0;

    // 返す値を定義
    result *r = (result *)malloc(sizeof(result));

    // bestPriceおよびcutDataメンバーに対してメモリを割り当てる
    r->bestPrice = (int *)malloc(sizeof(int) * (n + 1));
    r->cutData = (int *)malloc(sizeof(int) * (n + 1));

    // i=0
    r->bestPrice[0] = 0;

    // 初期化
    for (int i = 0; i <= n; i++)
    {
        r->bestPrice[i] = 0;
        r->cutData[i] = 0;
    }

    int array[n + 1];

    // 長さi、一番大きな切り方をした時の価格
    // 長さi のロッドと仮定 -> 1から順番に大きくして最終的にnを求める
    for (int i = 1; i <= n; i++)
    {
        // 長さiの最大の価格
        int maxPrice = 0;
        for (int j = 1; j <= i; j++)
        {
            // 切り方によって価格が大きくなるほうをとる
            if (maxPrice < p[j] + r->bestPrice[i - j])
            {
                maxPrice = p[j] + r->bestPrice[i - j];

                // 長さiのロッドを長さj切ったことを記録
                r->cutData[i] = j;
            }
        }
        // 記録
        r->bestPrice[i] = maxPrice;
    }

    // bestprice, ,maxPrice 返す
    return r;
}

void printRod(int n)
{
    // 実行
    result *r = cutRod(n);

    // 表示
    printf("price: %d\n", r->bestPrice[n]);

    // nは残り長さ
    // n=0 は自明なので含まなくてよい
    while (n > 0)
    {
        // 切らないほうがよい ->　再帰終了
        if (r->cutData[n] == n)
        {
            printf("no cut-> rest: %d  \n", r->cutData[n]);
            break;
        } // 切った場合
        else
        {
            printf("%d cut->  rest: %d\n", r->cutData[n], n - r->cutData[n]);

            // カットした分だけ引く -> 残りの長さ
            n = n - r->cutData[n];
        }
    }
}

int main()
{
    // n の入力
    int n;
    printf("n>>");
    scanf("%d", &n);

    // 実行 -> 結果の表示
    printRod(n);

    return 0;
}