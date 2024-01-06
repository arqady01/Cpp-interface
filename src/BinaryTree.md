# LCR047 二叉树剪枝

给定一个二叉树根节点root ，树的每个节点的值要么是0要么是1。请剪除该二叉树中所有节点的值为0的子树。

节点node的子树为node本身，以及所有node的后代。

输入: [1,0,1,0,0,0,1]

输出: [1,null,1,null,1]

如果用先序遍历，遇到一个节点不管三七二十一就处理，那么孩子节点全都遭殃

所以确定用后续遍历，每遍历一个节点，就要确定它是否有左右子树，如果左右子树都为空，并且节点的值是0，那么也就可以删除这个节点；否则条件不满足就不能移除，返回当前节点

边界条件：当输入为空时，即可返回空

```cpp
TreeNode* pruneTree(TreeNode* root) {
    if (root == nullptr) return root; //终止条件
    root->left = pruneTree(root->left);
    root->right = pruneTree(root->right);
    //后续遍历的操作函数在递归之后：
    if ((root->left == nullptr && root->right == nullptr) && root->val == 0)
        return nullptr; //剪枝
    return root;
}
```

# LCR049 从根节点到叶节点的路径数字之和

输入：root = [4,9,0,5,1]

输出：1026

从根到叶子节点路径 4->9->5 代表数字 495

从根到叶子节点路径 4->9->1 代表数字 491

从根到叶子节点路径 4->0 代表数字 40

因此，数字总和 = 495 + 491 + 40 = 1026

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/resource/BinaryTree/LCR049.png" style="width:80%;">
</p>

每遍历到一个节点时都计算从根节点到当前节点的路径表示的数字。如果这个节点还有子节点，就接着继续遍历它的子节点。

先计算到当前节点的路径表示的数字，再计算到它的子节点的路径表示的数字，这实质上就是前序遍历。

```cpp
class Solution {
public:
    int sumNumbers(TreeNode* root) {
        return dfs(root, 0);
    }
    int dfs(TreeNode* root, int prevSum) {
        if (root == nullptr) {
            return 0;
        }
        int sum = 10 * prevSum + root->val;
        if (root->left == nullptr && root->right == nullptr) {
            return sum; //前序遍历的操作函数在循环之前，顺序不能倒
        } else {
            //看图中dfs(4,0)这个情况，明显就是左右两边之和
            return dfs(root->left, sum) + dfs(root->right, sum);
        }
    }
};
```

# 展平二叉搜索树

给定一棵二叉搜索树，请让每个节点都没有左子节点。调整之后的树看起来像一个链表，但仍然是二叉搜索树

按照递增顺序遍历二叉搜索树中的每个节点，并将节点用指向右子节点的指针连接起来。显然是中序遍历。

```cpp
TreeNode* increasingBST(TreeNode* root) {
    if (root == nullptr) return root;
    TreeNode* cur = root;
    std::stack<TreeNode*> s;
    TreeNode* prev = nullptr;
    TreeNode* first = nullptr;
    while (!s.empty() || cur != nullptr) {
        while (cur != nullptr) {
            s.push(cur);
            cur = cur->left;
        }
        cur = s.top(); //常规的步骤取出栈顶元素
        s.pop(); //常规的步骤弹出栈顶元素
        
        if (prev != nullptr) {
            prev->right = cur;
        } else {
            first = cur;
        }
        prev = cur;
        cur->left = nullptr; //让左子树为空

        cur = cur->right; //常规的步骤：走右边
    }
    return first;
}
```

上述代码只是对中序遍历稍做修改。变量prev表示前一个遍历到的节点。在遍历到当前节点cur时，把变量prev的右子节点的指针指向 cur，并将 cur 指向左子节点的指针设为nullptr。

展平之后的二叉搜索树的根节点是值最小的节点，因此也是中序遍历第一个被遍历到的节点。变量first就是第一个被遍历到的节点，在展平之后就是二叉搜索树的根节点，因此将它作为函数的返回值
