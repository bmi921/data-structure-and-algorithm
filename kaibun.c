#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum Dir
{
    right,
    down,
    rightdown
};

typedef struct Table
{
    int value;
    enum Dir dir;
} table;

typedef struct Result
{
    char *s;
    int length;
} result;

void showTable(table **c, int m, int n)
{
    for (int i = 0; i <= m; i++)
    {
        for (int j = 0; j <= n; j++)
        {
            char dir;
            switch (c[i][j].dir)
            {
            case right:
                dir = '>';
                break;
            case down:
                dir = '|';
                break;
            case rightdown:
                dir = '\\';
                break;
            default:
                dir = ' ';
                break;
            }
            printf("%4d%c", c[i][j].value, dir);
        }
        printf("\n");
    }
}

result *LCS(char *x, char *y, int m, int n)
{
    // 戻し値をまとめる構造体を動的に確保
    result *r = (result *)malloc(sizeof(result));

    // 動的計画法のテーブルを定義 row->行　col->列
    table **c = (table **)malloc(sizeof(table *) * (m + 1));
    if (c == NULL)
        return NULL;

    for (int i = 0; i < m + 1; i++)
    {
        c[i] = (table *)malloc(sizeof(table) * (n + 1));
        if (c[i] == NULL)
            return NULL;
    }

    // 初期化
    for (int i = 0; i <= m; i++)
        c[i][0].value = 0;
    for (int j = 0; j <= n; j++)
        c[0][j].value = 0;

    // 動的計画法の実行
    for (int i = 1; i <= m; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            // 右斜め
            if (x[i - 1] == y[j - 1])
            {
                c[i][j].value = c[i - 1][j - 1].value + 1;
                c[i][j].dir = rightdown;
            }
            // 右
            else if (c[i][j - 1].value > c[i - 1][j].value)
            {
                c[i][j].value = c[i][j - 1].value;
                c[i][j].dir = right;
            }
            // 下
            else if (c[i][j - 1].value <= c[i - 1][j].value)
            {
                c[i][j].value = c[i - 1][j].value;
                c[i][j].dir = down;
            }
        }
    }

    // テーブルの表示
    showTable(c, m, n);

    // Sの長さを結果に格納
    r->length = c[m][n].value;

    // 共通している文字の特定
    int i = m;
    int j = n;
    int common[m + 1];
    // common[0]は使用しない
    common[0] = 0;

    // 表の端につかない限りループ
    while (i != 0 && j != 0)
    {
        // 斜め
        if (c[i][j].dir == rightdown)
        {
            // フラグを立てる
            common[i] = 1;
            i--;
            j--;
        }
        // 　右
        else if (c[i][j].dir == right)
        {
            common[i] = 0;
            j--;
        }
        // 下
        else if (c[i][j].dir == down)
        {
            common[i] = 0;
            i--;
        }
    }

    // フラグから最長共通部分配列sを求める
    int t = 0;
    char str[m + n]; // 100 はﾌﾞﾗｯｸﾅﾝﾊﾞｰ

    for (int k = 1; k <= m; k++)
    {
        if (common[k] == 1)
            str[t++] = x[k - 1];
    }
    // NULL文字の挿入
    str[t] = '\0';

    // sを格納
    if (strdup(str) != NULL)
        r->s = strdup(str);

    return r;
}

// 逆から読んだ文字列yを返す関数
char *reverce(char *x, int m)
{
    char *y = (char *)malloc(sizeof(char) * (m + 1));

    for (int i = 0; i < m; i++)
    {
        y[i] = x[m - i - 1];
    }
    y[m] = '\0';

    return y;
}

int main()
{
    // 一時的に入力を受ける配列
    char input[256];
    char *in = input;

    // x文字列を動的に確保
    printf("x>> ");
    scanf("%s", in);
    int m = strlen(input);
    char *x = (char *)malloc(sizeof(char) * (m));
    strcpy(x, in);

    // x-reverce文字列を動的に確保
    char *y = reverce(x, m);
    int n = strlen(y);

    printf("y>> %s\n", y);

    // m, n ->　x,yの文字の長さ
    result *r = LCS(x, y, m, n);

    // 結果の表示
    printf("l: %d \n", r->length);
    printf("s: %s\n", r->s);

    // メモリの解放
    free(x);
    free(y);
    free(r->s);
    free(r);

    return 0;
}