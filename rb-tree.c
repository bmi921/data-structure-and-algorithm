#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // sleepのため (randominputでコメントアウトされているが残す)

// 色の定義
#define RED 1
#define BLACK 0

typedef struct Node {
    int color;          // RED または BLACK
    int data;
    struct Node *parent;
    struct Node *left;
    struct Node *right;
} Node;

// プロトタイプ宣言 (一部変更・追加)
Node *createNode(int data);
Node *leftRotate(Node *root_of_subtree, Node **root_of_tree); // ツリー全体のrootも渡すように変更
Node *rightRotate(Node *root_of_subtree, Node **root_of_tree);
void fixAfterInsert(Node **root, Node *newNode);
void printTreeGUI(Node *node, int level);
int getBlackHeight(Node *node); // isrbt のためのヘルパー
int checkRBTProperties(Node *root); // isrbt をより包括的に
Node* bstInsert(Node* root, Node* pt); // BSTとしての挿入
Node* deleteNode(Node **rootRef, int data); // ダブルポインタでルートの変更に対応
void fixAfterDelete(Node **rootRef, Node *x, Node *x_parent);
Node* treeMinimum(Node* node);
void transplant(Node **rootRef, Node *u, Node *v); // 削除用ヘルパー

// 新しいノードを作成
Node *createNode(int data) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        perror("Failed to allocate memory for new node");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->color = RED; // 新規ノードは常に赤
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;
    return newNode;
}

Node *leftRotate(Node *x, Node **root_of_tree) {
    Node *y = x->right;
    x->right = y->left;
    if (y->left != NULL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL) {
        *root_of_tree = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
    return y; // 新しい部分木の根
}

Node *rightRotate(Node *y, Node **root_of_tree) {
    Node *x = y->left;
    y->left = x->right;
    if (x->right != NULL) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == NULL) {
        *root_of_tree = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }
    x->right = y;
    y->parent = x;
    return x; // 新しい部分木の根
}


void fixAfterInsert(Node **root, Node *z) {
    Node *parent_z = NULL;
    Node *grand_parent_z = NULL;

    while ((z != *root) && (z->color == RED) && (z->parent != NULL && z->parent->color == RED)) {
        parent_z = z->parent;
        grand_parent_z = z->parent->parent; // 親が赤なら祖父は必ず存在するはず(ルートが黒なので)

        // 親が祖父の左の子の場合
        if (parent_z == grand_parent_z->left) {
            Node *uncle_z = grand_parent_z->right;

            // Case 1: 叔父が赤色
            if (uncle_z != NULL && uncle_z->color == RED) {
                grand_parent_z->color = RED;
                parent_z->color = BLACK;
                uncle_z->color = BLACK;
                z = grand_parent_z; // 祖父を新たなzとしてチェックを続ける
            } else { // 叔父が黒色 (NULLも黒として扱う)
                // Case 2: zが右の子 (くの字型) -> 左回転で直線型にする
                if (z == parent_z->right) {
                    z = parent_z;
                    leftRotate(z, root); // zは回転前の親の位置に来る
                    parent_z = z->parent; // zの親を更新
                }
                // Case 3: zが左の子 (直線型)
                if (parent_z != NULL) parent_z->color = BLACK; // NULLチェック追加
                if (grand_parent_z != NULL) grand_parent_z->color = RED; // NULLチェック追加
                if (grand_parent_z != NULL) rightRotate(grand_parent_z, root);
                // z = parent_z; // この操作で z (またはその部分木の新しい根)が上に移動することが多い
                               // ループの条件で z は *root ではないことがチェックされるので、
                               // この時点で z が黒になればループは終了する。
                               // 通常、Case3の後は修正完了。
                               // ただし、上記のz=grand_parent_zのような伝播は発生しない。
                               // CLRSの教科書では、Case3の後はループ終了となっていることが多い。
                               // ここでは、zを更新せずにループ条件で自然に終了させるか、
                               // 明示的にループを抜けるようにしてもよい。
                               // 重要なのは、grand_parent_z の回転で構造が変わること。
                               // ループの次の反復のために z を適切に設定する必要があるが、
                               // 通常、このケースの後、木の特性は回復している。
                               // 安全のため z を再設定するなら、回転後の新しい頂点を指すようにする。
                               // しかし、教科書的な実装では、このパスの後は z が親を指すようにし、
                               // 次のループ条件で終了することが多い。
                               // ここでは、grand_parent_z が回転して下に移動するので、
                               // z や parent_z の相対関係が変わる。
                               // ループの先頭で z が *root でないか、色がREDか、親がREDかを見ている。
                               // Case 3の修正で z の親はBLACKになるので、ループは終了するはず。
            }
        }
        // 親が祖父の右の子の場合 (上記と対称)
        else {
            Node *uncle_z = grand_parent_z->left;

            // Case 1: 叔父が赤色
            if (uncle_z != NULL && uncle_z->color == RED) {
                grand_parent_z->color = RED;
                parent_z->color = BLACK;
                uncle_z->color = BLACK;
                z = grand_parent_z;
            } else {
                // Case 2: zが左の子 (くの字型) -> 右回転
                if (z == parent_z->left) {
                    z = parent_z;
                    rightRotate(z, root);
                    parent_z = z->parent;
                }
                // Case 3: zが右の子 (直線型)
                if (parent_z != NULL) parent_z->color = BLACK;
                if (grand_parent_z != NULL) grand_parent_z->color = RED;
                if (grand_parent_z != NULL) leftRotate(grand_parent_z, root);
            }
        }
    }
    (*root)->color = BLACK; // 常にルートは黒
}

Node *addNode(Node **root, int data) {
    Node *newNode = createNode(data);

    if (*root == NULL) {
        newNode->color = BLACK; // ルートノードは黒
        *root = newNode;
        return *root;
    }

    // 通常のBST挿入
    Node *current = *root;
    Node *parent = NULL;
    while (current != NULL) {
        parent = current;
        if (data < current->data) {
            current = current->left;
        } else if (data > current->data) {
            current = current->right;
        } else {
            printf("%dは既に存在します。\n", data);
            free(newNode); // 重複なので作成したノードを解放
            return *root;  // 変更なし
        }
    }

    // 新しいノードを接続
    newNode->parent = parent;
    if (data < parent->data) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }

    // RBTのプロパティを修正
    fixAfterInsert(root, newNode);
    return *root; // 新しい (かもしれない) ルートを返す
}

void printTreeGUI(Node *node, int level) {
    if (node == NULL)
        return;

    printTreeGUI(node->right, level + 1);

    for (int i = 0; i < level; i++)
        printf("         ");

    if (level > 0)
        printf("|----");

    // 色の表示 (BLACK: 青背景, RED: 赤背景)
    if (node->color == BLACK) {
        printf("\x1b[44m%d\x1b[0m", node->data); // 青背景
    } else {
        printf("\x1b[41m%d\x1b[0m", node->data); // 赤背景
    }

    printf("[");
    if (node->parent != NULL) printf("P%d", node->parent->data);
    if (node->left != NULL) printf("L%d", node->left->data);
    if (node->right != NULL) printf("R%d", node->right->data);
    printf("]\n");

    printTreeGUI(node->left, level + 1);
}

Node *searchNode(Node *root, int data) {
    Node *current = root;
    while (current != NULL) {
        if (data == current->data) {
            return current;
        } else if (data < current->data) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return NULL; // 見つからなかった
}


// --- 削除処理関連 (CLRSベースの考え方) ---
// uをvで置き換えるヘルパー関数
void transplant(Node **rootRef, Node *u, Node *v) {
    if (u->parent == NULL) {
        *rootRef = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v != NULL) {
        v->parent = u->parent;
    }
}

Node* treeMinimum(Node* node) {
    while (node != NULL && node->left != NULL) {
        node = node->left;
    }
    return node;
}


// `fixAfterDelete` は非常に複雑です。ここでは基本的な構造のみ示します。
// 完全な実装には教科書の詳細なケース分析が必要です。
void fixAfterDelete(Node **rootRef, Node *x, Node *x_parent) {
    // x: 削除されたノードの位置に入ったノード (NULLの場合もある)
    // x_parent: xの親
    // この関数は、xが二重黒 (double black) の状態を表す場合に呼ばれるイメージ
    // (実際にはxがNULLで、その位置が二重黒を意味することが多い)

    Node *sibling;
    while (x != *rootRef && (x == NULL || x->color == BLACK)) {
        if (x == x_parent->left) { // xが左の子の場合
            sibling = x_parent->right;
            if (sibling == NULL) { // 実際にはNIL葉を想定するので、兄弟がNULLはありえないはずだが、
                                  // 簡単な実装ではNULLがありうる。その場合の処理が必要。
                                  // ここでは兄弟が必ず存在すると仮定して進める。
                x = x_parent; // 問題を上に伝播 (簡易的な処理)
                if (x != *rootRef && x!=NULL) x_parent = x->parent; else break;
                continue;
            }


            // Case 1: 兄弟が赤
            if (sibling->color == RED) {
                sibling->color = BLACK;
                x_parent->color = RED;
                leftRotate(x_parent, rootRef);
                sibling = x_parent->right; // 新しい兄弟
            }
            // Case 2: 兄弟が黒、兄弟の両方の子が黒
            if ((sibling->left == NULL || sibling->left->color == BLACK) &&
                (sibling->right == NULL || sibling->right->color == BLACK)) {
                sibling->color = RED;
                x = x_parent; // 問題を親に移動
                if (x != *rootRef && x != NULL) x_parent = x->parent; else break;
            } else {
                // Case 3: 兄弟が黒、兄弟の左の子が赤、右の子が黒
                if (sibling->right == NULL || sibling->right->color == BLACK) {
                    if (sibling->left != NULL) sibling->left->color = BLACK;
                    sibling->color = RED;
                    rightRotate(sibling, rootRef);
                    sibling = x_parent->right; // 新しい兄弟
                }
                // Case 4: 兄弟が黒、兄弟の右の子が赤
                sibling->color = x_parent->color;
                x_parent->color = BLACK;
                if (sibling->right != NULL) sibling->right->color = BLACK;
                leftRotate(x_parent, rootRef);
                x = *rootRef; // 修正完了、ループを抜ける
            }
        } else { // xが右の子の場合 (上記と対称)
            sibling = x_parent->left;
            if (sibling == NULL) {
                x = x_parent;
                if (x != *rootRef && x != NULL) x_parent = x->parent; else break;
                continue;
            }

            // Case 1
            if (sibling->color == RED) {
                sibling->color = BLACK;
                x_parent->color = RED;
                rightRotate(x_parent, rootRef);
                sibling = x_parent->left;
            }
            // Case 2
            if ((sibling->left == NULL || sibling->left->color == BLACK) &&
                (sibling->right == NULL || sibling->right->color == BLACK)) {
                sibling->color = RED;
                x = x_parent;
                 if (x != *rootRef && x != NULL) x_parent = x->parent; else break;
            } else {
                // Case 3
                if (sibling->left == NULL || sibling->left->color == BLACK) {
                    if (sibling->right != NULL) sibling->right->color = BLACK;
                    sibling->color = RED;
                    leftRotate(sibling, rootRef);
                    sibling = x_parent->left;
                }
                // Case 4
                sibling->color = x_parent->color;
                x_parent->color = BLACK;
                if (sibling->left != NULL) sibling->left->color = BLACK;
                rightRotate(x_parent, rootRef);
                x = *rootRef;
            }
        }
    }
    if (x != NULL) x->color = BLACK;
}


Node* deleteNode(Node **rootRef, int data) {
    Node *z = searchNode(*rootRef, data);
    if (z == NULL) {
        printf("%d は存在しません。\n", data);
        return *rootRef;
    }

    Node *y = z; // y: 実際に削除される(または移動する)ノード
    Node *x = NULL; // x: yの位置に入るノード
    Node *x_parent = NULL; // xの親
    int y_original_color = y->color;

    if (z->left == NULL) {
        x = z->right;
        x_parent = z->parent; // transplant前に親を記録
        transplant(rootRef, z, z->right);
    } else if (z->right == NULL) {
        x = z->left;
        x_parent = z->parent; // transplant前に親を記録
        transplant(rootRef, z, z->left);
    } else {
        y = treeMinimum(z->right); // zの次節点
        y_original_color = y->color;
        x = y->right;

        if (y->parent == z) {
            if (x != NULL) x->parent = y; // xがNULLでない場合のみ親を設定
            x_parent = y; // xの親はyになる
        } else {
            x_parent = y->parent; // xの親はyの元の親
            transplant(rootRef, y, y->right);
            y->right = z->right;
            if (y->right != NULL) y->right->parent = y;
        }
        transplant(rootRef, z, y);
        y->left = z->left;
        if (y->left != NULL) y->left->parent = y;
        y->color = z->color;
    }

    free(z); // 元のzノードを解放

    if (y_original_color == BLACK) {
        // x_parent は、xがNULLの場合でも、xが元々あった場所の親を指す必要がある。
        // もしxがNULLでyがルートだった場合、x_parentもNULLになる可能性がある。
        // fixAfterDeleteが呼ばれるのは、実際に黒ノードが失われた場合。
        // x_parentがNULLになるケースは、削除されたのがルートで、かつその子がNULLの場合。
        // この場合、ツリーが空になるか、新しいルートが赤なら黒にするだけで済む。
        if (*rootRef != NULL) { // ツリーが空でなければ fixAfterDelete を考慮
            if(x_parent == NULL && x != NULL) { // xが新しいルートになった場合
                 x->color = BLACK; // 新しいルートは黒
            } else if (x_parent != NULL) { // x_parent が存在するなら修正処理
                 fixAfterDelete(rootRef, x, x_parent);
            } else if (x == NULL && x_parent == NULL && *rootRef != NULL){
                // 削除の結果ルートがNULLになったが、*rootRefはまだ何かを指している場合（これは通常起こらない）
                // もしツリーが非NULLなら、ルートは黒でなければならない。
                // *rootRef がNULLなら何もしない。
                // if (*rootRef != NULL) (*rootRef)->color = BLACK; (fixAfterDelete内で対応)
            }
        }
    }
    return *rootRef;
}

// --- RBTプロパティ検証 (簡易版) ---
// isrbtの簡易版。黒の高さが全てのパスで同じかを確認する。
// ただし、これはRBTの全てのプロパティを網羅していない。
int getBlackHeight(Node *node) {
    if (node == NULL) return 1; // NIL葉は黒と数える
    int leftHeight = getBlackHeight(node->left);
    int rightHeight = getBlackHeight(node->right);
    if (leftHeight == -1 || rightHeight == -1 || leftHeight != rightHeight) return -1; // 不整合
    return (node->color == BLACK ? 1 : 0) + leftHeight;
}

int checkRBTProperties(Node *root) {
    if (root == NULL) {
        printf("Tree is empty. (Valid RBT)\n");
        return 1;
    }
    if (root->color != BLACK) {
        printf("RBT Property violated: Root is not black.\n");
        return 0;
    }
    if (getBlackHeight(root) == -1) {
        printf("RBT Property violated: Black height mismatch or other structural issue.\n");
        return 0;
    }
    // TODO: ここにさらにプロパティ4 (赤ノードの子は黒) のチェックを追加できる
    printf("RBT properties seem to be maintained (checked root color and black height).\n");
    return 1;
}


// --- main関数とメニュー ---
void search_ui(Node *root) {
    int input = 0;
    printf("search>>");
    if (scanf("%d", &input) != 1) { // scanfの戻り値チェックを追加
        printf("不正な入力です。\n");
        // 不正な入力をクリアする処理 (オプション)
        while (getchar() != '\n'); // 改行文字まで読み飛ばす
        return;
    }

    if (input == 0) return;

    Node* foundNode = searchNode(root, input); // searchNodeの結果を変数に格納
    if (foundNode == NULL) {
        printf("探索失敗: %dは見つかりませんでした。\n", input);
    } else {
        printf("%dの探索成功\n", foundNode->data); // 見つかったノードのデータを表示
    }
}

Node *add_ui(Node *root) { // 関数名変更
    int input = 0;
    printf("add>>");
    scanf("%d", &input);
    if (input == 0) return root;

    root = addNode(&root, input); // addNodeが新しいrootを返すように
    printTreeGUI(root, 0);
    checkRBTProperties(root);
    return root;
}

Node *delete_ui(Node *root) { // 関数名変更
    int input = 0;
    printf("delete>>");
    scanf("%d", &input);
    if (input == 0) return root;

    root = deleteNode(&root, input);
    if (root == NULL) {
        printf("Tree is now empty.\n");
    } else {
        printTreeGUI(root, 0);
    }
    checkRBTProperties(root);
    return root;
}

Node *randominput_ui(Node *root) { // 関数名変更
    printf("insert number>>");
    int insertnum = 0;
    scanf("%d", &insertnum);

    for (int i = 0; i < insertnum; i++) {
        // sleep(1); // デバッグ用に遅延させる場合。time(NULL)の粒度のため
        int val = rand() % 100 + 1;
        printf("\nInserting %d:\n", val);
        root = addNode(&root, val);
        printTreeGUI(root, 0);
        checkRBTProperties(root);
    }
    return root;
}


int main() {
    Node *root = NULL; // 最初は空の木
    srand((unsigned int)time(NULL)); // プログラム開始時に一度だけsrandを呼び出す

    int choice = 1;
    while (choice != 0) {
        printf("\n0:exit 1:add 2:search 3:delete 4:show 5:randominput\n");
        printf("input>>");
        scanf("%d", &choice);
        switch (choice) {
            case 0:
                break;
            case 1:
                root = add_ui(root);
                break;
            case 2:
                if (root) search_ui(root); else printf("Tree is empty.\n");
                break;
            case 3:
                if (root) root = delete_ui(root); else printf("Tree is empty.\n");
                break;
            case 4:
                if (root) printTreeGUI(root, 0); else printf("Tree is empty.\n");
                if (root) checkRBTProperties(root); // 空でない場合のみチェック
                break;
            case 5:
                root = randominput_ui(root);
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }

    // TODO: プログラム終了時に全てのノードを解放する処理 (freeTree関数など) を追加するのが望ましい
    // freeTree(root);
    printf("Exiting.\n");
    return 0;
}