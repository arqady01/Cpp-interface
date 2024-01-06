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

# 二叉搜索树的下一个节点

给定一棵二叉搜索树和它的一个节点p，请找出比p的值更大的下一个节点。假设二叉搜索树中节点的值都是唯一的

```cpp
TreeNode* inorderSuccessor(TreeNode* root, TreeNode* p) {
    TreeNode* cur = root;
    TreeNode* ans = nullptr; //记录答案，防止因为回退问题太麻烦
    while (cur != NULL) {
        if (cur->val > p->val) {
            ans = cur;
            cur = cur->left;
        } else {
            cur = cur->right;
        }
    }
    return ans;
}
```

# LCR054 把二叉搜索树转换为累加树

给定一个二叉搜索树，请将它的每个节点的值替换成树中大于或者等于 该节点值的所有节点的值之和

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/resource/BinaryTree/LCR54.png" style="width:60%;">
</p>

自然想到中序遍历，值从小到大，也就是当遍历到某个节点时，比该节点的值小的节点都已经遍历过，因此也就知道了所有比该节点的值小的所有节点，相加得到sum。可是题目要求把每个节点的值替换成大于或等于该节点的值的所有节点的值之和，而不是小的。因此，可以先遍历一遍求出所有节点值的总和total，再用total减去sum即可。时间复杂度O(n2)。

能不能从右向左从大到小遍历，每当遍历到某节点时，就已经得到比自己大的所有节点呢，就能控制时间复杂度在O(n)了

```cpp
TreeNode* convertBST(TreeNode* root) {
    TreeNode* cur = root;
    int sum = 0;
    std::stack<TreeNode*> s;
    while (!s.empty() || cur != nullptr) {
        while (cur != NULL) {
            s.push(cur);
            cur = cur->right;
        }
        cur = s.top();
        
        sum = sum + cur->val;
        cur->val = sum;

        s.pop();
        cur = cur->left;
    }
    return root;
}
```

与常规的中序遍历相比，上述代码的不同点在于左右交换，这样可以得到一个从右向左的序列，加上二叉搜索树的中序遍历特性，就能做到从大到小遍历了

# 二叉搜索树的迭代器BSTIterator

请实现二叉搜索树的迭代器BSTIterator，它主要有如下3个函数：

- 构造函数：输入二叉搜索树的根节点初始化该迭代器。
- 函数next：返回二叉搜索树中下一个最小的节点的值。
- 函数hasNext：返回二叉搜索树是否还有下一个节点。

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/resource/BinaryTree/BSTIterator.png" style="width:60%;">
</p>

```cpp
class BSTIterator {
public:
    BSTIterator(TreeNode* root) {
        this->cur = root;
    }
    
    int next() {
        while (cur != nullptr) {
            s.push(cur);
            cur = cur->left;
        }
        cur = s.top();
        int val = cur->val;
        s.pop();
        cur = cur->right;
        return val;
    }
    
    bool hasNext() {
        return cur != nullptr || !s.empty();
    }
private:
    TreeNode* cur = nullptr;
    std::stack<TreeNode*> s;
};
```

基于二叉树中序遍历实现

# 二叉搜索树中两个节点的值之和

给定一棵二叉搜索树和一个值k，判断该二叉搜索树中是否存在值之和等于k的两个节点。假设二叉搜索树中节点的值均唯一。

解：

哈希表，时间复杂度为O(n)，空间复杂度为O(n)

双指针，空间复杂度O(h)，h为树高

在排序数组中，给定一个数字k，判断是否存在两个和为k的数字，我们的做法是把左指针指向数组开头，把右指针指向数组末尾。如果指针所指数字之和等于k，那么就找到了。如果两个数字之和大于k，那么右指针左移，使它指向更小的数字；如果两个数字之和小于k，那么左指针右移，使它指向更大的数字。

- 法子1：中序遍历并得到一个递增的数组，然后使用之前的方法
- 法子2：利用迭代器思想，实现两组函数能让指针向更小和更大处移动，并返回TreeNode*，之后利用上面的方法来求。就不演示了，因为需要用两个栈

```cpp
class Solution {
public:
    std::vector<int> v;
    //主函数
    bool findTarget(TreeNode* root, int k) {
        create_vector(root);
        int left = 0;
        int right = v.size() - 1;
        while (left < right) {
            if (v[left] + v[right] > k) {
                right--;
            } else if (v[left] + v[right] < k){
                left++;
            } else {
                return true;
            }
        }
        return false;
    }
    //构建中序序列
    void create_vector(TreeNode* root) {
        if (root == nullptr) return;
        create_vector(root->left);
        v.push_back(root->val);
        create_vector(root->right);
    }
};
```
