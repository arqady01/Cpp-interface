# 递归法

### 104.二叉树的最大深度

给定一个二叉树 root ，返回其最大深度（根节点到最远叶子节点的最长路径上的节点数）

```cpp
//求root节点的最大深度
int maxDepth(TreeNode* root) {
    if (root == nullptr) //递归终止条件：当所求
        return 0; //节点是空节点时，最大深度为0

    //root节点的左节点的最大深度为leftMax
    int leftMax = maxDepth(root->left);
    //root节点的右节点的最大深度为rightMax
    int rightMax = maxDepth(root->right);
    return max(leftMax, rightMax) + 1;
}
```

### 111. 二叉树的最小深度

给定一个二叉树，找出其最小深度(根节点到最近叶子节点的最短路径上的节点数量)

输入：`root = [2,null,3,null,4,null,5,null,6]`，输出：5

```cpp
//求root节点的最小深度
int minDepth(TreeNode* root) {
    //终止条件：当root为空时，最小深度为0
    if (root == nullptr) return 0;

    //右子树为空，就去左子树中递归
    if (root->right == nullptr) {
        return minDepth(root->left) + 1;
    }
    //左子树为空，就去右子树中递归
    if (root->left == nullptr) {
        return minDepth(root->right) + 1;
    }

    int leftMin = minDepth(root->left);
    int rightMin = minDepth(root->right);
    return min(leftMin, rightMin) + 1;
}
```

### 226.反转二叉树

Google面试题

输入：`root = [4,2,7,1,3,6,9]`，输出：`[4,7,2,9,6,3,1]`

```cpp
TreeNode* invertTree(TreeNode* root) {
    if (root == nullptr) return nullptr; //如果是空节点就直接输出nullptr节点

    //invertTree(root->left)就是反转root->left树，最后挂到root的左边
    root->left = invertTree(root->left);
    root->right = invertTree(root->right); //同理
    std::swap(root->left, root->right); //交换root的左/右节点
    
    return root;
}
```

### 100.相同的树

给你两棵二叉树的根节点 p 和 q ，从结构和值上判断两棵树是否相同

可能不止有一个循环终止条件！

```cpp
bool isSameTree(TreeNode* p, TreeNode* q) {
    if (p == nullptr && q == nullptr)
        return true;
    else if (p == nullptr || q == nullptr)
        return false;

    if (p->val != q->val)
        return false;

    bool leftTree = isSameTree(p->left, q->left);
    bool rightTree = isSameTree(p->right, q->right);
    return leftTree && rightTree;
}
```

### 110.平衡二叉树

给定一个二叉树，判断它是否是平衡二叉树

```cpp
//辅助函数height计算*root的高度
int height(TreeNode* root) {
    if (root == nullptr) return 0;

    int leftH = height(root->left);
    int rightH = height(root->right);
    return std::max(leftH, rightH) + 1;
}
//主函数
bool isBalanced(TreeNode* root) {
    if (root == nullptr) return true;
    
    if (std::abs(height(root->left) - height(root->right)) > 1) {
        return false;
    }

    //应对这种情况：[1,2,2,3,null,null,3,4,null,null,4]
    //即平衡二叉树不止站在根节点看，要求树中的每一个节点都是平衡的
    return isBalanced(root->left) && isBalanced(root->right);
}
```

### 二叉树的所有路径

给你一个二叉树的根节点 root ，按任意顺序 ，返回所有从根节点到叶子节点的路径

输入：`[1,2,3,7,5]`，输出：`["1->2->7","1->2->5","1->3"]`

```cpp
vector<string> binaryTreePaths(TreeNode* root) {
    //递归终止条件Ⅰ
    std::vector<std::string> ans;
    if (root == nullptr) return ans;
    
    //递归终止条件Ⅱ
    if (root->left == nullptr && root->right == nullptr) {
        ans.push_back(to_string(root->val));
        return ans;
    }
    
    //root的左子节点组成的字符串
    std::vector<string> leftString = binaryTreePaths(root->left);
    for (int i = 0; i < leftString.size(); i++) {
        ans.push_back(to_string(root->val) + "->" + leftString[i]);
    }
    //root的右子节点组成的字符串
    std::vector<string> rightString = binaryTreePaths(root->right);
    for (int i = 0; i < rightString.size(); i++) {
        ans.push_back(to_string(root->val) + "->" + rightString[i]);
    }

    return ans;
}
```

# 回溯法

### 77 组合

给定两个整数 n 和 k，返回 1 ... n 中所有可能的k个数的组合？

示例: 输入: n = 4, k = 2 输出: [ [2,4], [3,4], [2,3], [1,2], [1,3], [1,4], ]

<p align="center"> 
    <img src="/resource/backtravel/77.jpg" style="width:66%;">
</p>

引入startIndex的目的是防止出现重复的组合，因为用过的数字就不能再用了，下一层递归要怎么知道从哪里开始就需要靠startIndex。

vector<vector<int>> ans; //存放符合条件的所有的结果的集合

vector<int> path; //符合条件的单一数组，存放根节点到叶子节点的路径

> 回溯函数的终止条件该如何选择呢？

> 只要看图中，path（根节点到叶子节点的路径数组）的大小和k数相同，那就说明到达叶子节点，然后把path保存进ans集合，终止本层递归。

> 单层递归的逻辑？

> 假设取1后，处理节点就是把1纳入path这个单一数组，此时在这个节点下，我们要遍历剩下来的集合{2、3、4}，毫无疑问这是一个递归的过程，继续调用递归函数，至于回溯，是因为path是单一数组，如果不撤销处理的节点，会出现诸如{1、2、3}的path出现，只有当path为{1、2}，回溯撤销，删除2，才能加入3构成{1、3}。

```cpp
class Solution {
private:
    vector<vector<int>> result;
    vector<int> path;
    void backtracking(int n, int k, int startIndex) {
        //终止条件
        if (path.size() == k) {
            result.push_back(path);
            return;
        }
        //单层递归逻辑
        for (int i = startIndex; i <= n; i++) {
            path.push_back(i); // 处理节点 
            backtracking(n, k, i + 1); // 递归
            path.pop_back(); // 回溯，撤销处理的节点
        }
    }
public: //主函数
    vector<vector<int>> combine(int n, int k) {
        backtracking(n, k, 1); //是1而不是0，因为是从1~n的数字
        return result;
    }
};
```

### 216 组合总和 III

找出所有相加之和为 n 的 k 个数的组合。组合中只允许含有 1 - 9 的正整数，并且每种组合中不存在重复的数字。

示例 1: 输入: k = 3, n = 7 输出: [[1,2,4]]

示例 2: 输入: k = 3, n = 9 输出: [[1,2,6], [1,3,5], [2,3,4]]

<p align="center"> 
    <img src="/resource/backtravel/216.jpg" style="width:80%;">
</p>

```cpp
class Solution {
public:
    vector<vector<int>> combinationSum3(int k, int n) {
        fx(k, n, 1, 0);
        return ans;
    }
    void fx(int k, int n, int startIndex, int sum) {
        //终止条件
        if (sum == n && path.size() == k) {
            ans.push_back(path);
            return;
        }
        //单层递归的逻辑
        for (int i = startIndex; i <= 9; i++) {
            path.push_back(i);
            sum += i; //sum是已收集元素和，即path的和
            fx(k, n, i + 1, sum);
            sum -= i; //回溯；处理过程和回溯过程是对应的，处理加，回溯就要减
            path.pop_back(); //path也要回退
        }
    }
private:
    std::vector<std::vector<int>> ans; //存放结果集合
    std::vector<int> path; //符合条件的结果
};
```

### 17 电话号码的组合

<p align="center"> 
    <img src="/resource/backtravel/LCR17_que.png" style="width:70%;">
</p>

<p align="center"> 
    <img src="/resource/backtravel/LCR17.jpg" style="width:60%;">
</p>

> 终止条件

用例"23"，两个数字，那么根节点往下递归两层就可以了，叶子节点就是要收集的结果集

那么终止条件就是如果startIndex等于输入的数字个数（digits.size）了（本来startIndex就是用来遍历digits的）

然后收集结果，结束本层递归

> 单层遍历的逻辑

取startIndex指向的数字，并找到对应的字符集。然后for循环来处理这个字符集，注意这个for循环是按照字符集的情况做循环的。

```cpp
class Solution {
public:
    //主函数
    vector<string> letterCombinations(string digits) {
        if (digits.size() == 0) {
            return ans;
        }
        fx(digits, 0);
        return ans;
    }
    //startIndex就是看当前取哪个数字的，比如digits=“23”、startIndex=2时，数字取3
    //同时startIndex还有一个作用，当startIndex等于path大小时，说明符合终止条件了
    void fx(string digits, int startIndex) { //startIndex一次次自增，直到走完字典
        if (startIndex == digits.size()) {
            ans.push_back(path);
            return;
        }
        int num = digits[startIndex] - '0'; //比如从“23”中找到第二个字符3并转为int型
        std::string ch = letterMap[num]; //取出对应字符集，比如3对应“def”
        for (int i = 0; i < ch.size(); i++) { //单层递归逻辑
            path.push_back(ch[i]); //处理
            fx(digits, startIndex + 1); //递归
            path.pop_back(); //回溯，即回退
        }
    }
    std::vector<string> ans;
    std::string path;
private:
    const string letterMap[10] = {
        "", // 0
        "", // 1
        "abc", // 2
        "def", // 3
        "ghi", // 4
        "jkl", // 5
        "mno", // 6
        "pqrs", // 7
        "tuv", // 8
        "wxyz", // 9
    };
};
```

### 39 组合总和

给定一个无重复元素的数组candidates和一个目标数target，找出candidates中所有可以使数字和为target的组合，注意candidates中的数字可以无限制重复选取。

输入：candidates = [2,3,5], target = 8

输出： [ [2,2,2,2], [2,3,3], [3,5] ]

<p align="center"> 
    <img src="/resource/backtravel/39ans.png" style="width:70%;">
</p>

> 单层递归的逻辑

单层for循环依然是从startIndex开始，搜索candidates集合；不过本题是可以重复选取的，反应在代码注释处。

```cpp
class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        fx(candidates, target, 0, 0);
        return ans;
    }
    void fx(std::vector<int> candidates, int target, int startIndex, int sum) {
        //递归终止条件Ⅰ：sum大于target时，条件不成立直接返回
        if (sum > target) return;
        //递归终止条件Ⅱ：sum等于target时，收集结果后返回
        if (sum == target) {
            ans.push_back(path);
            return;
        }
        
        for (int i = startIndex; i < candidates.size(); i++) {
            sum += candidates[i];
            path.push_back(candidates[i]);
            //i不用++，因为i传给startIndex
            //如果i+1，下次for循环会跳过自己没法重复选取
            fx(candidates, target, i, sum);
            sum -= candidates[i];
            path.pop_back();
        }
    }
    std::vector<std::vector<int>> ans;
    std::vector<int> path;
};
```

### 40 组合总和II

给定一个数组candidates和正数target，找出candidates中所有可以让数字和为target的组合。注意candidates每个数字只能使用一次。

输入: candidates = [10,1,2,7,6,1,5], target = 8

输出：[ [1, 7], [1, 2, 5], [2, 6], [1, 1, 6] ]

<p align="center"> 
    <img src="/resource/backtravel/40ans.jpg" style="width:70%;">
</p>

> 问：去重是树枝去重还是树层去重呢？

答：树层去重。candidates数组有几个元素，也就最多有多少层，最多candidates每个元素都取一次；假设candidates=[1,1,7]，target=8，第一次取0号位的1，接下来7符合，答案加上{1，7}，第二次依旧会取1号位上的1，然后{1，7}，重复！

> 问：同一树层上的使用过数字需要去重，如何判断同一树层上元素（相同的元素）是否使用过了呢？

答：首先排序后，如果candidates[i] == candidates[i - 1]，就说明和之前的元素相同。

当used[i - 1] == true，说明同一树枝下candidates[i - 1]是使用过的，不管向下发展了几层都是如此；因为向上回溯的原因，原本true的位置会被回设为false，

当used[i - 1] == false，说明同一层之前的元素candidates[i - 1]被使用过，因为虽然它是0，但是for循环往后走了一次，i - 1 处必然是被用过的

```cpp
class Solution {
public:
    std::vector<std::vector<int>> ans;
    std::vector<int> path;
    vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
        sort(candidates.begin(), candidates.end());
        std::vector<bool> used(candidates.size(), false);
        fx(candidates, target, used, 0, 0);
        return ans;
    }
    void fx(vector<int> candidates, int target, vector<bool> used, int startIndex, int sum) {
        if (sum == target) {
            ans.push_back(path);
            return;
        }
        //sum + candidates[i] <= target为剪枝操作
        for (int i = startIndex; i < candidates.size() && sum + candidates[i] <= target; i++) {
            //i - 1可能越界，要保证i>0；同时去重发生在树层上
            if (i > 0 && candidates[i] == candidates[i - 1] && used[i - 1] == false) continue;
            sum += candidates[i];
            path.push_back(candidates[i]);
            used[i] = true;
            fx(candidates, target, used, i + 1, sum);
            sum -= candidates[i];
            path.pop_back();
            used[i] = false;
            
        }
    }
};
```

### 131 分割回文串

给你一个字符串s，将s分割成一些子串，使每个子串都是回文串（正反读都一样的字符串）。返回所有的分割方案。

输入：s = "aab"，

输出：[["a","a","b"],["aa","b"]]

<p align="center"> 
    <img src="/resource/backtravel/131ans.png" style="width:80%;">
</p>

```cpp
class Solution {
public:
    vector<vector<string>> partition(string s) {
        fx(s, 0);
        return ans;
    }
    void fx(string s, int startIndex) {
        if (startIndex == s.size()) {
            ans.push_back(path);
            return;
        }
        for (int i = startIndex; i < s.size(); i++) {
            if (isPalindrome(s, startIndex, i)) {
                string str = s.substr(startIndex, i - startIndex + 1);
                path.push_back(str);
            } else {
                continue;
            }
            fx(s, i + 1);
            path.pop_back();
        }
    }
    //判断字符串是否是回文的
    bool isPalindrome(const string& s, int start, int end) {
        for (int i = start, j = end; i < j; i++, j--) {
            if (s[i] != s[j]) {
                return false;
            }
        }
        return true;
    }
    std::vector<std::vector<string>> ans;
    std::vector<string> path;
};
```

### 78 求子集

整数数组nums中，元素互不相同。返回该数组所有可能的子集。

输入：nums = [1,2,3]

输出：[[],[1],[2],[1,2],[3],[1,3],[2,3],[1,2,3]]

<p align="center"> 
    <img src="/resource/backtravel/78ans.png" style="width:80%;">
</p>

```cpp
class Solution {
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        ans.push_back(path); //先加一个空的进去满足题目要求
        fx(nums, 0);
        return ans;
    }
    void fx(std::vector<int> nums, int startIndex) {
        if (startIndex >= nums.size()) return;
        for (int i = startIndex; i < nums.size(); i++) {
            path.push_back(nums[i]); //处理
            ans.push_back(path); //每个节点都要纳入结果集
            fx(nums, i + 1);
            path.pop_back(); //回溯
        }
    }
    std::vector<std::vector<int>> ans;
    std::vector<int> path;
};
```

### 90 子集Ⅱ

整数数组nums中，元素可以相同。返回该数组所有可能的子集。

输入：nums = [1,2,2]，

输出：[[],[1],[1,2],[1,2,2],[2],[2,2]]

<p align="center"> 
    <img src="/resource/backtravel/90ans.png" style="width:80%;">
</p>

本题中出现重复元素，而且求取的子集要去重，所以是树层剪枝而非树枝剪枝。

参考之前两道题的解法，融合一下。

```cpp
class Solution {
public:
    vector<vector<int>> subsetsWithDup(vector<int>& nums) {
        ans.push_back(path);
        std::vector<bool> used(nums.size(), false);
        sort(nums.begin(), nums.end()); //去重需要排序
        fx(nums, 0, used);
        return ans;
    }
    void fx(std::vector<int> nums, int startIndex, std::vector<bool> used) {
        if (startIndex >= nums.size()) return;
        for (int i = startIndex; i < nums.size(); i++) {
            //used[i - 1] == true，说明同一树枝candidates[i - 1]使用过
            //used[i - 1] == false，说明同一树层candidates[i - 1]使用过
            //要对同一树层使用过的元素进行跳过
            if (i > 0 && nums[i] == nums[i - 1] && used[i - 1] == false) {
                continue;    
            }
            path.push_back(nums[i]);
            ans.push_back(path);
            used[i] = true;
            fx(nums, i + 1, used);
            path.pop_back();
            used[i] = false;
        }
    }
    std::vector<std::vector<int>> ans;
    std::vector<int> path;
};
```

### 491 递增子序列

一个整数数组nums，返回所有该数组中不同的递增子序列，递增子序列中至少有两个元素。

输入：nums = [4,7,6,7]

输出：[[4,7],[4,7,7],[4,6],[4,6,7],[7,7],[6,7]]

```cpp
class Solution {
public:
    vector<vector<int>> findSubsequences(vector<int>& nums) {
        fx(nums, 0);
        return ans;
    }
    void fx(std::vector<int> nums, int startIndex) {
        std::unordered_set<int> us;
        if (path.size() >= 2) {
            ans.push_back(path);
            //不要加return
        }
        for (int i = startIndex; i < nums.size(); i++) {
            //在同一层中，不能重复使用元素，但是又不能像之前排序后在做，所以借助unordered_set
            //用过的元素加入容器中，如果使用过就会在容器中找到。如果重复使用，直接跳过
            //如果不满足递增条件也直接跳过，path不为空是为了防止调用back()函数段错误
            if (!path.empty() && nums[i] < path.back() || us.find(nums[i]) != us.end()) {
                continue;
            }
            path.push_back(nums[i]);
            us.insert(nums[i]); //标记次元素在树层用过了，本层后面不能再用
            fx(nums, i + 1);
            path.pop_back();
        }
    }
    std::vector<std::vector<int>> ans;
    std::vector<int> path;
};
```

<p align="center"> 
    <img src="/resource/backtravel/491ans.jpg" style="width:80%;">
</p>

### 46 全排列

给定一个不含重复数字的数组nums，返回所有可能的全排列。

输入：nums = [1,2,3]

输出：[[1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]]

洒洒水啦

```cpp
class Solution {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        //代表nums中数字都没有使用过
        std::vector<bool> used(nums.size(), false);
        fx(nums, used);
        return ans;
    }
    void fx(std::vector<int>& nums, std::vector<bool>& used) {
        if (path.size() == nums.size()) {
            ans.push_back(path);
            return;
        }
        for (int i = 0; i < nums.size(); i++) {
            if (used[i] == true) {
                //使用过的元素就跳过
                continue;
            }
            path.push_back(nums[i]);
            used[i] = true;
            fx(nums, used);
            path.pop_back();
            used[i] = false;
        }
    }
    std::vector<std::vector<int>> ans;
    std::vector<int> path;
};
```

### 47 全排列Ⅱ

给定一个可包含重复数字的序列nums，按任意顺序返回所有不重复的全排列

输入：nums = [1,1,2]

输出：[ [1,1,2], [1,2,1], [2,1,1] ]

洒洒水啦

不需要用到startIndex变量，但是要先排序

```cpp
class Solution {
public:
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        std::vector<bool> used(nums.size(), false);
        fx(nums, used);
        return ans;
    }
    void fx(std::vector<int>& nums, std::vector<bool>& used) {
        if (path.size() == nums.size()) {
            ans.push_back(path);
            return;
        }
        for(int i = 0; i < nums.size(); i++) {
            //树层去重，树枝也要做判断
            if (i > 0 && nums[i - 1] == nums[i] && used[i - 1] == false || used[i] == true) {
                continue;
            }
            path.push_back(nums[i]);
            used[i] = true;
            fx(nums, used);
            path.pop_back();
            used[i] = false;
        }
    }
    std::vector<std::vector<int>> ans;
    std::vector<int> path;
};
```

### 51 N皇后

将n个皇后放置在 n×n 的棋盘上，并且皇后之间不能互相攻击（上下左右斜向攻击）

输入：n = 4

输出：[[".Q..","...Q","Q...","..Q."],["..Q.","Q...","...Q",".Q.."]]

<p align="center"> 
    <img src="/resource/backtravel/51ans.PNG" style="width:80%;">
</p>

```cpp
class Solution {
public:
    vector<vector<string>> solveNQueens(int n) {
        std::vector<std::string> path(n, std::string(n, '.'));
        fx(n, 0, path);
        return ans;
    }
    //row是第几行
    void fx(int n, int row, std::vector<std::string>& path) {
        if (n == row) {
            ans.push_back(path);
            return;
        }
        //col是列的意思，不过还是用i来表示
        for (int i = 0; i < n; i++) {
            if (isValid(path, row, i, n) == false) {
                continue;
            }
            path[row][i] = 'Q';
            fx(n, row + 1, path);
            path[row][i] = '.';
        } 
    }
    std::vector<std::vector<string>> ans;
private:
    //下面的判断我当时没有写出来，尤其注意
    bool isValid(vector<string>& chessboard, int row, int col,  int n) {
        // 行不需要检查，看图中的说明
        // 检查列
        for (int i = 0; i < row; i++) { // 这是一个剪枝
            if (chessboard[i][col] == 'Q') {
                return false;
            }
        }
        // 检查 45度角是否有皇后
        for (int i = row - 1, j = col - 1; i >=0 && j >= 0; i--, j--) {
            if (chessboard[i][j] == 'Q') {
                return false;
            }
        }
        // 检查 135度角是否有皇后
        for(int i = row - 1, j = col + 1; i >= 0 && j < n; i--, j++) {
            if (chessboard[i][j] == 'Q') {
                return false;
            }
        }
        return true;
    }
};
```
