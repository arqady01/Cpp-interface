# 77 组合

给定两个整数 n 和 k，返回 1 ... n 中所有可能的k个数的组合？

示例: 输入: n = 4, k = 2 输出: [ [2,4], [3,4], [2,3], [1,2], [1,3], [1,4], ]

==插入图片==

引入index的目的是防止出现重复的组合，因为用过的数字就不能再用了，下一层递归要怎么知道从哪里开始就需要靠index。

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
    void backtracking(int n, int k, int index) {
		//终止条件
        if (path.size() == k) {
            result.push_back(path);
            return;
        }
		//单层递归逻辑
        for (int i = index; i <= n; i++) {
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

# 216 组合总和 III

找出所有相加之和为 n 的 k 个数的组合。组合中只允许含有 1 - 9 的正整数，并且每种组合中不存在重复的数字。

示例 1: 输入: k = 3, n = 7 输出: [[1,2,4]]

示例 2: 输入: k = 3, n = 9 输出: [[1,2,6], [1,3,5], [2,3,4]]

==插入图片==

```cpp
class Solution {
public:
    vector<vector<int>> combinationSum3(int k, int n) {
        fx(k, n, 1, 0);
        return ans;
    }
    void fx(int k, int n, int index, int sum) {
        //终止条件
        if (sum == n && path.size() == k) {
            ans.push_back(path);
            return;
        }
        //单层递归的逻辑
        for (int i = index; i <= 9; i++) {
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
