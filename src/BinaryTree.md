# 18 重建二叉树

输入一棵二叉树前序遍历和中序遍历的结果，请恢复该二叉树

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/resource/Offer_Answer_images/18.png" style="width:70%;">
</p>

```cpp
class Solution {
public:
    std::map<int,int> m;//其实放入makeTree做参数也可以，但会提升时间复杂度
    //辅助函数
    TreeNode* makeTree(std::vector<int> preorder, int l1, int r1, std::vector<int> inorder, int l2, int r2) {
        if(l1 > r1 || l2 > r2) return nullptr;
        //从先序遍历中找到最左边的元素，即根节点
        std::map<int, int>::iterator iter = m.find(preorder[l1]);
        int i = iter->second; //i是根节点在中序遍历中的下标
        //开始构造
        TreeNode* root = new TreeNode(preorder[l1]); //建立根节点
        root->left = makeTree(preorder, l1 + 1, l1 + (i - l2), inorder, l2, i - 1); //递归
        root->right = makeTree(preorder, l1 + (i - l2) + 1, r1, inorder, i + 1, r2);//递归
        return root;
    }
    //主函数
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        //通过中序遍历构造出数值和下标的map，比如 [9,3,15,20,7]构建出的map为
        //{(9,1),(3,2),(15,3),(20,4),(7,5)}
        for (int i = 0; i < inorder.size(); i++)
            m.insert(std::make_pair(inorder[i], i));
        return makeTree(preorder, 0, preorder.size() - 1, inorder, 0, inorder.size() - 1);
    }
};
```

# 19 二叉树的下一个节点

给定一棵二叉树的其中一个节点，请找出中序遍历序列的下一个节点

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/resource/Offer_Answer_images/19.png" style="width:100%;">
</p>

```cpp
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode *father;
    TreeNode(int x) : val(x), left(NULL), right(NULL), father(NULL) {}
};
class Solution {
public:
    TreeNode* inorderSuccessor(TreeNode* p) {
        TreeNode* ans = nullptr;
        //case1，p有右子树，那么下一个节点就是右子树的最左叶子节点
        if (p->right != nullptr) {
            ans = p->right;
            while (ans->left != nullptr) {
                ans = ans->left;
            }
            return ans;
        }
        //case2 + 3，p没有右子树，且p是其父节点的左/右节点
        if (p->father != nullptr) {
            TreeNode* pCurrent = p;
            TreeNode* pParent = p->father;
            //1️⃣: pParent != nullptr, 2️⃣: pCurrent == pParent->right
            while (pParent != nullptr && pCurrent == pParent->right) {
                pCurrent = pParent;
                pParent = pParent->father;
            }
            ans = pParent;
        }
        return ans;
    }
};
```

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

# 124 二叉树中的最大路径和

字节、快手、虾皮、蔚来校招题目

给定一个非空二叉树，返回其最大路径和。

本题中，路径被定义为一条从树中任意节点出发，达到任意节点的序列。该路径至少包含一个节点，且不一定经过根节点。

输入：root = [1,2,3]，
输出：6，
解释：最优路径是 2 -> 1 -> 3 ，路径和为 2 + 1 + 3 = 6

输入：root = [-10,9,20,null,null,15,7]，
输出：42，
解释：最优路径是 15 -> 20 -> 7 ，路径和为 15 + 20 + 7 = 42

> 如果某个子树 dfs 结果为负，收益不增反减，所以不应走入，直接返回0（壮士断腕）

```cpp
int leftGain = max(0, dfs(root->left));
int rightGain = max(0, dfs(root->right));
```

- 从父节点延伸下来的路径，可以在子树中获取的最大收益。分三种情况
    - 左右孩子节点都是负数，取自己：root->val
    - 走左子树，从左子树获取收益：root->val + dfs(root->left)
    - 走右子树，从右子树获取收益：root->val + dfs(root->right)
- 从节点走下来的路径，不能走入左子树又掉头走右子树，即不能两头都收益

返回值就是最大收益，即三者取最大的：`return root->val + max(0, dfs(root->left), dfs(root->right));`

```cpp
class Solution {
public:
    int sum = INT_MIN; //全局变量，记录最大路径和
    int maxPathSum(TreeNode* root) { //计算任意节点的最大贡献值
        dfs(root);
        return sum;
    }
    int dfs(TreeNode* root) {
        if (root == nullptr) return 0;
        int leftGain = max(0, dfs(root->left));
        int rightGain = max(0, dfs(root->right));
        //某子树内部最大收益 = 左子树最大收益 + 根节点值 + 右子树最大收益
        //自我更新，万万不可写成int Sum = ....
        sum = max(sum, root->val + leftGain + rightGain);
        return root->val + max(leftGain, rightGain); //要么走左边要么右边，不能两边都收益
    }
};
```

# 二叉树的层序遍历

给你二叉树的根节点 root ，返回其节点值的层序遍历

输入：root = [8,17,21,18,null,null,6]

输出：[8,17,21,18,6]

下面的解法就是模板，bfs的模板，利用此模板可以快速解下面的那两道题

```cpp
vector<int> decorateRecord(TreeNode* root) {
    std::vector<int> v;
    std::queue<TreeNode*> q;
    if (root != nullptr) q.push(root);
    while (!q.empty()) {
        int length = q.size();
        while (length-- > 0) {
            TreeNode* temp = q.front();
            q.pop();
            v.push_back(temp->val);
            if (temp->left != nullptr) {
                q.push(temp->left);
            }
            if (temp->right != nullptr) {
                q.push(temp->right);
            }
        }
    }
    return v;
}
```

# 二叉树的层序遍历I

输入：root = [8,17,21,18,null,null,6]

输出：[[8],[17,21],[18,6]]

```cpp
vector<vector<int>> decorateRecord(TreeNode* root) {
    vector<vector<int>> vv;
    vector<int> stl;
    std::queue<TreeNode*> q;
    if (root != nullptr) q.push(root);
    while (!q.empty()) {
        int length = q.size();
        while (length-- > 0) {
            TreeNode* temp = q.front();
            q.pop();
            stl.push_back(temp->val);
            if (temp->left != nullptr) {
                q.push(temp->left);
            }
            if (temp->right != nullptr) {
                q.push(temp->right);
            }
        }
        vv.push_back(std::vector<int>(stl.begin(), stl.end()));
        stl.resize(0); //清空
    }
    return vv;
}
```

# 二叉树的层序遍历II

锯齿状层序遍历，第一层从左到右，第二层为从右到左，以此类推。

解：

和之前的区别在于多了一个标志位control、vector换成了deque，因为需要用到deque的两端插入特点

```cpp
vector<vector<int>> decorateRecord(TreeNode* root) {
    vector<vector<int>> vv;
    deque<int> stl;
    std::queue<TreeNode*> q;
    int control = 0;
    if (root != nullptr) q.push(root);
    while (!q.empty()) {
        int length = q.size();
        while (length-- > 0) {
            TreeNode* temp = q.front();
            q.pop();
            if (control % 2 == 0) { //偶数
                stl.push_back(temp->val);
            }
            if (control % 2 == 1) { //奇数
                stl.push_front(temp->val);
            }
            if (temp->left != nullptr) {
                q.push(temp->left);
            }
            if (temp->right != nullptr) {
                q.push(temp->right);
            }
        }
        vv.push_back(std::vector<int>(stl.begin(), stl.end()));
        control++;
        stl.resize(0); //清空
    }
    return vv;
}
```

# 验证二叉搜索树的后序遍历序列

请实现一个函数来判断整数数组 postorder 是否为二叉搜索树的后序遍历结果

```cpp
class Solution{
public:
    bool verifyPostorder(std::vector<int>& postorder){
        return assist_func(postorder, 0, postorder.size() - 1);
    }
    bool assist_func(std::vector<int> array,int left,int right){
        //若left == right，说明只剩一个节点，无需判断，若left > right，说明没有节点
        if(left >= right) return true;
        //数组的最后一个数array[right]就是二叉树的根结点值，从左往右找出第一个比根结点值大的元素
        //找出来的元素后面的数都是二叉树的右子节点(左闭右开，因为左边要包含当前值，右边要剔除根结点)
        //找出来的元素前面的数都是二叉树的左子节点
        int pos = left;
        int root = array[right]; //根结点值
        while(array[pos] < root){
            pos++;//此举是为了找到第一个比根结点值还大的值的坐标,记为pos
        }
        //找到了pos后，array[pos]前的值都是比root小的，还需要在确定array[pos]后是否还存在小于root的值
        //有就说明不是二叉搜索树，没有就继续
        int temp = pos;
        while(temp < right){
            if(array[temp] < root) return false;
            temp++; //temp++继续往后判断是否还有小于root的值
        }
        return assist_func(array,left,pos - 1) && assist_func(array,pos,right - 1);//递归
    }
};
```
