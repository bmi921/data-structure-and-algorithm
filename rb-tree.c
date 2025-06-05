#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int global_key = 0;
typedef struct Node
{
    // black:0 red:1
    int color;
    int data;
    struct Node *parent;
    struct Node *left;
    struct Node *right;
} Node;

// プロトタイプ宣言
Node *left(Node *root);
Node *right(Node *root);
void printTreeGUI(Node *root, int level);
int isrbt(Node *root);
void numbering(Node *node);

Node *change1(Node *a)
{
    Node *b, *c;
    b = a->left;
    c = a->parent;

    a = right(c);
    b->color = 0;

    return a;
}
Node *change2(Node *a)
{
    Node *b, *c;
    b = a->left;
    c = a->parent;

    b = right(a);
    c->right = b;
    b->parent = c;

    b = left(c);
    a->color = 0;

    return b;
}
Node *change3(Node *a)
{
    Node *b, *c;
    b = a->right;
    c = a->parent;

    b = left(a);
    c->left = b;
    b->parent = c;

    b = right(c);
    a->color = 0;

    return b;
}

Node *change4(Node *a)
{
    Node *b, *c;
    b = a->right;
    c = a->parent;

    a = left(c);
    b->color = 0;
    return a;
}

Node *correctTree(Node *n)
{
    int det = 0;
    int data = 0;

    while (1)
    {
        if (n->parent == NULL)
        {
            return n;
        }

        //  赤ノードの親が赤ノードだった場合
        if (det == 1 && n->color == 1)
        {
            int a, b, c;
            b = data;
            a = n->data;

            // nの親が根ノードなら、黒にして終了
            if ((n->parent)->parent == NULL)
            {
                // cの親を保存
                Node *parent;
                parent = (n)->parent;
                c = (parent)->data;
                // 変形した木をどっち側にくっつける？
                int isRight = 0;
                // left:0 rigth:1
                if (parent->right == n)
                {
                    isRight = 1;
                }

                // パターンの判定
                // それぞれのパターンに対して木を変形
                if (b < a && a < c)
                {
                    n = change1(n);
                }
                if (c < b && b < a)
                {
                    n = change2(n);
                }
                if (a < b && b < c)
                {
                    n = change3(n);
                }
                if (c < a && a < b)
                {
                    n = change4(n);
                }
                n->color = 0;
                return (n);
            }
            c = (n->parent)->data;

            // cの親を保存
            Node *parent;
            parent = (n->parent)->parent;

            // 変形した木をどっち側にくっつける？
            int isRight = 0;
            // left:0 rigth:1
            if (parent->right == n->parent)
            {
                isRight = 1;
            }

            // パターンの判定
            // それぞれのパターンに対して木を変形
            if (b < a && a < c)
            {
                n = change1(n);

                // 回転した木をくっつける
                n->parent = parent;
                if (isRight == 1)
                    parent->right = n;
                else
                    parent->left = n;
            }
            if (c < b && b < a)
            {
                n = change2(n);
                // 回転した木をくっつける
                n->parent = parent;
                if (isRight == 1)
                    parent->right = n;
                else
                    parent->left = n;
            }
            if (a < b && b < c)
            {
                n = change3(n);

                // 回転した木をくっつける
                n->parent = parent;
                if (isRight == 1)
                    parent->right = n;
                else
                    parent->left = n;
            }
            if (c < a && a < b)
            {
                n = change4(n);

                // 回転した木をくっつける
                n->parent = parent;
                if (isRight == 1)
                    parent->right = n;
                else
                    parent->left = n;
            }
        }
        // det, dataの更新
        det = n->color;
        data = n->data;

        // nを根方向に進める
        if (n->parent != NULL)
        {
            n = n->parent;
        }
    }
}

void initNode(Node *root, int data)
{
    root->color = 1;
    root->data = data;
    root->left = NULL;
    root->right = NULL;
    root->parent = NULL;
}

Node *addNode(Node *root, int data)
{
    // 最初
    if (global_key == 0)
    {
        initNode(root, data);
        // 最初のノードの色は黒に変更される
        root->color = 0;
        global_key++;
        return root;
    }

    // 2番目以降
    Node *p;
    p = root;

    // 重複した場合
    if (p->data == data)
    {
        printf("%dはあります。\n", data);
        while (p->parent != NULL)
        {
            p = p->parent;
        }
        return p;
    }

    // 左に移動
    if (p->data > data)
    {
        Node *parent;
        parent = p;
        p = p->left;

        // 左に挿入
        if (p == NULL)
        {
            Node *n = (Node *)malloc(sizeof(Node));
            initNode(n, data);

            n->parent = parent;
            parent->left = n;

            // 挿入した赤ノードの親が黒ノードなら条件を満たす
            if (parent->color == 0)
            {
                while (n->parent != NULL)
                    n = n->parent;

                return n;
            }
            else
            {
                // 挿入した赤ノードから根に向かって、修正
                Node *ret;
                ret = correctTree(n);
                return ret;
            }
        }
        else
            return addNode(p, data);
    }

    // 右側
    if (p->data < data)
    {
        Node *parent;
        parent = p;
        p = p->right;

        if (p == NULL)
        {
            Node *n = (Node *)malloc(sizeof(Node));
            initNode(n, data);

            n->parent = parent;
            parent->right = n;

            // 挿入した赤ノードの親が黒ノードなら条件を満たす
            if (parent->color == 0)
            {
                while (n->parent != NULL)
                {
                    n = n->parent;
                }
                return n;
            }
            else
            {
                // 挿入した赤ノードから根に向かって、修正
                Node *ret;
                ret = correctTree(n);
                return ret;
            }
        }
        else
            return addNode(p, data);
    }
}

Node *right(Node *root)
{
    Node *a, *b;
    a = root;
    b = a->left;
    if (b->right != NULL)
    {
        // T2 について
        a->left = b->right;
        if (b->right != NULL)
            (b->right)->parent = a;

        // bについて
        b->right = a;
        b->parent = a->parent;

        // aについて
        a->parent = b;

        return b;
    }
    else
    // T2が存在しない
    {
        a->left = NULL;

        // bについて
        b->right = a;
        b->parent = a->parent;

        // aについて
        a->parent = b;

        return b;
    }
}

Node *left(Node *root)
{
    Node *a, *b;
    a = root;
    b = a->right;
    if (b->left != NULL)
    {
        a->right = b->left;
        if (b->left != NULL)
            (b->left)->parent = a;

        b->left = a;
        b->parent = a->parent;

        a->parent = b;

        return b;
    }
    else
    {
        a->right = NULL;

        b->left = a;
        b->parent = a->parent;

        a->parent = b;

        return b;
    }
}

void printTreeGUI(Node *root, int level)
{
    if (root == NULL)
        return;

    printTreeGUI(root->right, level + 1);

    // 出力開始
    for (int i = 0; i < level; i++)
        printf("         ");

    if (level > 0)
        printf("|----");

    if (root->color == 0)
    {
        printf("\x1b[44m");
        printf("%d", root->data);
        printf("\x1b[0m");
    }
    else
    {
        printf("\x1b[41m");
        printf("%d", root->data);
        printf("\x1b[0m");
    }

    printf("[");
    if (root->parent != NULL)
    {
        printf("P%d", (root->parent)->data);
    }
    if (root->left != NULL)
    {
        printf("L%d", (root->left)->data);
    }
    if (root->right != NULL)
    {
        printf("R%d", (root->right)->data);
    }
    printf("]\n");
    // 出力終了

    printTreeGUI(root->left, level + 1);
}

Node *searchNode(Node *root, int data)
{
    while (1)
    {
        // 探索成功
        if (root->data == data)
        {
            return root;
        }

        int isStep = 0;

        // rootを根方向に進める
        if (root->left != NULL && root->data > data)
        {
            isStep++;
            root = root->left;
        }
        if (root->right != NULL && root->data < data)
        {
            isStep++;
            root = root->right;
        }

        // 探索失敗
        if (isStep == 0)
        {
            return NULL;
        }
    }
}

void search(Node *root)
{
    while (1)
    {

        int input = 0;
        printf("search>>");
        scanf("%d", &input);
        // 終了条件
        if (input == 0)
            break;

        if (searchNode(root, input) == NULL)
            printf("探索失敗\n");
        else
        {
            printf("%dの探索成功\n", searchNode(root, input)->data);
        }
    }
}

Node *maxleft(Node *x)
{
    Node *p;
    p = x->left;
    while (p->right != NULL)
        p = p->right;
    return p;
}

Node *returnRoot(Node *r)
{
    while (r->parent != NULL)
        r = r->parent;

    return r;
}

typedef enum Dir
{
    dirleft,
    dirright
} dir;

Node *delfix(Node *r, dir direction)
{
    puts("delfix: now tree");
    printTreeGUI(returnRoot(r), 0);

    // 部分木が左にある
    if (direction == dirleft)
    {
        // Node a,b,c,dを定義
        Node *a = r;
        Node *b = r->right;
        Node *c = r->right->left;
        Node *d = r->right->right;

        // a,b,c,d の色によって定義
        int a_color = a->color;
        int b_color;
        int c_color;
        int d_color;

        // NULLは黒として扱い、色で場合分けするため代入
        if (b != NULL)
            b_color = b->color;
        else
            b_color = 0;

        if (c != NULL)
            c_color = c->color;
        else
            c_color = 0;

        if (d != NULL)
            d_color = d->color;
        else
            d_color = 0;

        // cが存在しない:-1 存在する:0 1ノードを持つ:1 2ノードを持つ:2
        int c_num = -1;
        // c,dの子供の数を数える
        if (c != NULL)
        {
            c_num = 0;
            if (c->left != NULL)
                c_num++;
            if (c->right != NULL)
                c_num++;
        }

        // dも同様に扱う
        int d_num = -1;
        if (d != NULL)
        {
            d_num = 0;
            if (d->left != NULL)
                d_num++;
            if (d->right != NULL)
                d_num++;
        }

        // ちゃんと反映されてる？
        printf("left r:%d\n", a->data);
        printf("a%d b%d c%d d%d | c%d d%d\n", a_color, b_color, c_color, d_color, c_num, d_num);

        // 4パターンに分類
        // 1パターン目
        if (b_color == 0 && c_color == 1 && d_color == 0)
        {
            puts("del 1pattern");
            Node *p = right(b);
            p = left(p->parent);
            p->left->color = 0;
            return returnRoot(a);
        }
        // 2パターン目
        else if (b_color == 0 && c_color == 0 && d_color == 1)
        {
            puts("del 2pattern");
            Node *a_parent;
            if (a->parent != NULL)
                a_parent = a->parent; //

            Node *p = left(a);
            p->color = 1;
            p->left->color = 0;
            p->right->color = 0;
            if (a->parent != NULL)
                a_parent->right = p; //
            return returnRoot(a);
        }
        // 1,2 のどちらか
        else if (b_color == 0 && c_color == 1 && d_color == 1)
        {
            // 1パターン目
            if (c_num == 2)
            {
                puts("del 1pattern");
                Node *p = right(b);
                p = left(p->parent);
                p->left->color = 0;
                return returnRoot(a);
            }
            // 2パターン目
            else if (d_num == 2)
            {
                puts("del 2pattern");
                Node *a_parent;
                if (a->parent != NULL)
                    a_parent = a->parent; //
                Node *p = left(a);
                p->color = 1;
                p->left->color = 0;
                p->right->color = 0;
                if (a->parent != NULL)
                    a_parent->right = p; //
                return returnRoot(a);
            }
        }
        // 3パターン目
        else if (a_color == 1 && b_color == 0 && c_color == 0 && d_color == 0)
        {
            puts("del 3pattern");
            a->color = 0;
            b->color = 1;
            return returnRoot(a);
        }
        // 4パターン目
        else if (a_color == 0 && b_color == 0 && c_color == 0 && d_color == 0)
        {
            puts("del 4pattern");
            b->color = 1;
            // aの親があるならば
            if (a->parent != NULL)
            {
                // aの親から見て、aは右左どっち？
                dir dir_from_a_parent;
                if (a->parent->left == a)
                    dir_from_a_parent = dirleft;
                else
                    dir_from_a_parent = dirright;

                // 根方向へ再帰的に
                delfix(a->parent, dir_from_a_parent);
            }
            return returnRoot(a);
        }
    }
    // 部分木が右にある
    else if (direction == dirright)
    {
        puts("dirright");
        // Node a,b,c,dを定義
        Node *a = r;
        Node *b = r->left;
        Node *c = r->left->right;
        Node *d = r->left->left;

        // a,b,c,d の色によって定義
        int a_color = a->color;
        int b_color;
        int c_color;
        int d_color;

        // NULLは黒として扱い、色で場合分けするため代入
        if (b != NULL)
            b_color = b->color;
        else
            b_color = 0;

        if (c != NULL)
            c_color = c->color;
        else
            c_color = 0;

        if (d != NULL)
            d_color = d->color;
        else
            d_color = 0;

        // cが存在しない:-1 存在する:0 1ノードを持つ:1 2ノードを持つ:2
        int c_num = -1;
        // c,dの子供の数を数える
        if (c != NULL)
        {
            c_num = 0;
            if (c->left != NULL)
                c_num++;
            if (c->right != NULL)
                c_num++;
        }

        // dも同様に扱う
        int d_num = -1;
        if (d != NULL)
        {
            d_num = 0;
            if (d->left != NULL)
                d_num++;
            if (d->right != NULL)
                d_num++;
        }

        // ちゃんと反映されてる？
        printf("right r:%d\n", a->data);
        printf("a%d b%d c%d d%d | c%d d%d\n", a_color, b_color, c_color, d_color, c_num, d_num);

        // 4パターンに分類
        // 1パターン目
        if (b_color == 0 && c_color == 1 && d_color == 0)
        {
            puts("del 1pattern");
            Node *p = left(b);
            p = right(p->parent);
            p->right->color = 0;
            return returnRoot(a);
        }
        // 2パターン目
        else if (b_color == 0 && c_color == 0 && d_color == 1)
        {
            puts("del 2pattern");
            Node *a_parent;
            if (a->parent != NULL)
                a_parent = a->parent; //

            Node *p = right(a);
            p->color = 1;
            p->right->color = 0;
            p->left->color = 0;
            if (a->parent != NULL)
                a_parent->left = p; //
            return returnRoot(a);
        }
        // 1,2 のどちらか
        else if (b_color == 0 && c_color == 1 && d_color == 1)
        {
            // 1パターン目
            if (c_num == 2)
            {
                puts("del 1pattern");
                Node *p = left(b);
                p = right(p->parent);
                p->right->color = 0;
                return returnRoot(a);
            }
            // 2パターン目
            else if (d_num == 2)
            {
                puts("del 2pattern");
                Node *a_parent;
                if (a->parent != NULL)
                    a_parent = a->parent; //

                Node *p = right(a);
                p->color = 1;
                p->right->color = 0;
                p->left->color = 0;
                if (a->parent != NULL)
                    a_parent->left = p; //
                return returnRoot(a);
            }
        }
        // 3パターン目
        else if (a_color == 1 && b_color == 0 && c_color == 0 && d_color == 0)
        {
            puts("del 3pattern");
            a->color = 0;
            b->color = 1;
            return returnRoot(a);
        }
        // 4パターン目
        else if (a_color == 0 && b_color == 0 && c_color == 0 && d_color == 0)
        {
            puts("del 4pattern");
            b->color = 1;
            // aの親があるならば
            if (a->parent != NULL)
            {
                // aの親から見て、aは右左どっち？
                dir dir_from_a_parent;
                if (a->parent->left == a)
                    dir_from_a_parent = dirleft;
                else
                    dir_from_a_parent = dirright;

                // 根方向へ再帰的に
                delfix(a->parent, dir_from_a_parent);
            }
            return returnRoot(a);
        }
    }
}

Node *deleteNode(Node *root, int data)
{
    // 削除対象をxとする
    Node *x;

    // xを探索し、探索成功したら終了
    x = searchNode(root, data);
    if (x == NULL)
        return root;

    // xの子の数を数える
    int childNum = 0;
    if (x->right != NULL)
        childNum++;
    if (x->left != NULL)
        childNum++;

    // xの子の個数で場合わけ

    // xが葉の場合
    if (childNum == 0)
    {
        // xが根ノードかつ葉ノードの場合
        if (x->parent == NULL)
            return NULL;

        // xの親, xが親から見てdirを保存
        Node *root = x->parent;
        dir d;

        // xはrootから見て左？右にいる？
        if ((x->parent)->left == x)
        {
            (x->parent)->left = NULL;
            d = dirleft;
        }
        else if ((x->parent)->right == x)
        {
            (x->parent)->right = NULL;
            d = dirright;
        }

        // 修正が必要かどうかのフラグ
        int isRBT = isrbt(returnRoot(root));

        // 修正 xを削除しているので、黒が一つ足りない木はxの場所
        if (isRBT == 0)
        {
            // xを部分木として、変形を行う
            delfix(root, d);
        }

        // 変形されたら根ノードも変わるので、根っこを特定してから返す
        return returnRoot(root);
    }

    // xが一つの子を持つ場合
    if (childNum == 1)
    {
        int isRight = 0;
        if (x->right != NULL)
            isRight = 1;

        // 左に子を持つ
        if (isRight == 0)
        {
            // x が根ノードの場合
            if (x->parent == NULL)
            {
                Node *left;
                left = x->left;
                left->parent = NULL;

                //  根は必ず黒でなければならない
                left->color = 0;
                return left;
            }

            // xが親を持つ場合
            if (x->parent != NULL)
            {
                // 左の子ノードを覚えさせておく
                Node *leftNode = x->left;

                // 場合わけで使う色を定義しておく
                int x_left_color;

                if (x->left->color == 0)
                    x_left_color = 0;
                else
                    x_left_color = 1;

                // xの子の親を引き継ぐ
                (x->left)->parent = x->parent;

                dir dir_from_x_parent;
                // xの親が、xを左にもつ？xを右に持つ？
                if ((x->parent)->right == x)
                {
                    (x->parent)->right = x->left;
                    dir_from_x_parent = dirright;
                }
                if ((x->parent)->left == x)
                {
                    (x->parent)->left = x->left;
                    dir_from_x_parent = dirleft;
                }

                // 修正の場合分け
                if (x_left_color == 0)
                {
                    // xが黒ノードが一つ足りない部分木になる
                    delfix(x->parent, dir_from_x_parent);
                }
                else if (x_left_color == 1)
                {
                    // 昇格した後、色をred->blackに変更
                    leftNode->color = 0;
                }
            }
        }

        // 右に子を持つ
        if (isRight == 1)
        {
            // x が根ノードの場合
            if (x->parent == NULL)
            {
                Node *right;
                right = x->right;
                right->parent = NULL;

                // 根ノードは必ず黒ノードである
                right->color = 0;
                return right;
            }

            // xが親を持つ場合
            if (x->parent != NULL)
            {
                // 右の子ノードを覚えさせておく
                Node *rightNode = x->right;
                // 場合わけで使う色を定義しておく
                int x_right_color;

                if (x->right->color == 0)
                    x_right_color = 0;
                else
                    x_right_color = 1;

                // xの子の親を引き継ぐ
                (x->right)->parent = x->parent;

                dir dir_from_x_parent;
                // xの親が、xを左にもつ？xを右に持つ？
                if ((x->parent)->right == x)
                {
                    (x->parent)->right = x->right;
                    dir_from_x_parent = dirright;
                }
                if ((x->parent)->left == x)
                {
                    (x->parent)->left = x->right;
                    dir_from_x_parent = dirleft;
                }

                // 修正の場合分け
                if (x_right_color == 0)
                {
                    // xが黒ノードが一つ足りない部分木になる
                    delfix(x->parent, dir_from_x_parent);
                }
                else if (x_right_color == 1)
                {
                    // 昇格した後、色をred->blackに変更
                    rightNode->color = 0;
                }
            }
        }
        // xが親を持つ場合、グラフの根を返す
        while (x->parent != NULL)
            x = x->parent;

        return x;
    }

    // xが2の子を持つ場合
    if (childNum == 2)
    {
        Node *c;
        // xと入れ替えるノード cの特定
        if (x->left != NULL)
            c = maxleft(x);

        // 入れ替えるノードの表示
        printf("translation Node: %d\n", c->data);

        // cの親の場所を後で使う
        // c->parentは必ず存在
        // 削除対象x, 入れ替えるノードcの情報を記録->分岐
        Node *c_parent;
        if (c->parent != x)
            c_parent = c->parent;
        else if (c->parent == x)
            c_parent = c;

        printf("fix root Node: %d\n", c_parent->data);

        // cは親から見てどっちにある？
        dir dir_from_c_parent;
        if (c->parent->left == c)
            dir_from_c_parent = dirleft;
        else if (c->parent->right == c)
            dir_from_c_parent = dirright;

        // dirの出力
        if (dir_from_c_parent == dirleft)
            printf("dirleft\n");
        else
            printf("dirright\n");

        // x,c の色の保存
        int x_color = x->color;
        int c_color = c->color;

        // どの変形がおこなわれる？
        printf("x%d c%d \n", x_color, c_color);

        // xは根ノードである
        if (x->parent == NULL)
        {

            // x->right, x->left は必ず存在
            (x->left)->parent = c;
            (x->right)->parent = c;

            // 追加
            if (c->parent != x && c->left != NULL)
                c->parent->right = c->left;
            else if (c->parent != x)
                c->parent->right = NULL;

            c->parent = NULL;
            c->right = x->right;
            if (x->left != c)
                c->left = x->left;

            // 削除ノードxは必ず黒なのでチェックする必要はない
            if (c_color == 0)
            {
                // cが黒ノードが足りない部分木となる,
                delfix(c_parent, dir_from_c_parent);
            }
            else if (c_color == 1)
            {
                // 根ノードの色はかならず黒出なければならない
                c->color = 0;
            }

            return returnRoot(c);
        }

        // xが親を持つ
        if (x->parent != NULL)
        {
            // x->right, x->left は必ず存在
            (x->left)->parent = c;
            (x->right)->parent = c;

            // 追加
            if (c->parent != x && c->left != NULL)
                c->parent->right = c->left;
            else if (c->parent != x)
                c->parent->right = NULL;

            c->parent = x->parent;
            c->right = x->right;
            if (x->left != c)
                c->left = x->left;

            if (x->parent->left == x)
                x->parent->left = c;
            else if (x->parent->right == x)
                x->parent->right = c;

            // 変形
            if (x_color == 1 && c_color == 1)
            {
                // 何もしないで終わり
            }
            else if (x_color == 1 && c_color == 0)
            {
                // xが赤だったように、同じくcも赤にする
                // cの場所で一つ足りなくなる
                c->color = 1;
                delfix(c_parent, dir_from_c_parent);
            }
            else if (x_color == 0 && c_color == 0)
            {
                // // cの場所で一つ足りなくなる
                delfix(c_parent, dir_from_c_parent);
            }
            else if (x_color == 0 && c_color == 1)
            {
                // xが黒だったように、cを黒にして終わり
                c->color = 0;
            }

            return returnRoot(c);
        }
    }
}

// 黒の要素数が格納される配列、添え字
int black[256];
int num;

void numbering(Node *node)
{
    int b = 0;
    // 葉ノードの色は黒？
    if (node->color == 0)
        b++;

    // 内部ノードの色は黒？
    while (node->parent != NULL)
    {
        node = node->parent;
        if (node->color == 0)
            b++;
    }
    // グローバル変数に書き込み
    black[num] = b;
    num++;
}

void numblack(Node *node)
{
    // 左だけNULL
    if (node->left == NULL && node->right != NULL)
    {
        numbering(node);
        numblack(node->right);
    }
    // 右だけNULL
    else if (node->left != NULL && node->right == NULL)
    {
        numbering(node);
        numblack(node->left);
    }
    // 両方NULL
    else if (node->left == NULL && node->right == NULL)
    {
        numbering(node);
    }
    // 両方NULLではない
    else if (node->left != NULL && node->right != NULL)
    {
        numblack(node->right);
        numblack(node->left);
    }
}

int isrbt(Node *root)
{
    // 初期化
    for (int i = 0; i < 256; i++)
        black[i] = 0;
    num = 0;

    // 黒の要素数を数え上げる
    numblack(root);

    int isrbt = 1;

    // isrbtかチェック
    for (int i = 0; i < 256; i++)
    {
        if (black[i + 1] == 0)
            break;
        else if (black[i] != black[i + 1])
        {
            isrbt = 0;
            break;
        }
    }

    // 出力
    if (isrbt == 0)
        printf("rbt木ではありません。\n");
    else
        printf("rbt木です。\n");

    // 初期化
    for (int i = 0; i < 256; i++)
        black[i] = 0;
    num = 0;

    return isrbt;
}

void delete(Node *root)
{
    while (1)
    {
        int input = 0;
        printf("delete>>");
        scanf("%d", &input);

        if (input == 0)
            break;

        root = deleteNode(root, input);
        if (root == NULL)
        {
            printf("NULLが帰ってきたため終了\n");
            return;
        }

        printTreeGUI(root, 0);
        isrbt(root);
    }

    return;
}

Node *add(Node *root)
{
    while (1)
    {
        int input = 0;
        printf("add>>");
        scanf("%d", &input);
        // 終了条件
        if (input == 0)
            return root;
        else
        {
            root = addNode(root, input);
            printTreeGUI(root, 0);
            isrbt(root);
        }
    }
}

Node *randominput(Node *root)
{
    printf("insert number>>");
    int insertnum = 0;
    scanf("%d", &insertnum);

    for (int i = 0; i < insertnum; i++)
    {
        // sleep(0.1);
        // srand((unsigned int)time(NULL));
        srand((unsigned int)i);
        int insert_num = rand() % 100 + 1;
        root = addNode(root, insert_num);
        printTreeGUI(root, 0);
        isrbt(root);
    }

    return root;
}

int main()
{
    Node Tree;
    Node *root;
    root = &Tree;

    int d = 1;
    while (d)
    {
        printf("0:exit 1:add 2:search 3:delete 4:show 5:randominput\n");
        printf("input>>");
        scanf("%d", &d);
        switch (d)
        {
        case 0:
            d = 0;
            break;
        case 1:
            root = add(root);
            break;
        case 2:
            search(root);
            break;
        case 3:
            delete (root);
            break;
        case 4:
            printTreeGUI(root, 0);
            isrbt(root);
            break;
        case 5:
            root = randominput(root);
            break;

        default:
            d = 0;
            break;
        }
    }
    return 0;
}