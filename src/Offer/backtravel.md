# 77 组合

给定两个整数 n 和 k，返回 1 ... n 中所有可能的k个数的组合？

示例: 输入: n = 4, k = 2 输出: [ [2,4], [3,4], [2,3], [1,2], [1,3], [1,4], ]

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/resource/backtravel/77.jpg" style="width:60%;">
</p>

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

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/resource/backtravel/216.jpg" style="width:80%;">
</p>

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

# 17 电话号码的组合

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/resource/backtravel/LCR17_que.png" style="width:70%;">
</p>

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/resource/backtravel/LCR17.jpg" style="width:60%;">
</p>

> 终止条件

用例"23"，两个数字，那么根节点往下递归两层就可以了，叶子节点就是要收集的结果集

那么终止条件就是如果index等于输入的数字个数（digits.size）了（本来index就是用来遍历digits的）

然后收集结果，结束本层递归

> 单层遍历的逻辑

取index指向的数字，并找到对应的字符集。然后for循环来处理这个字符集，注意这个for循环是按照字符集的情况做循环的。

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
    //index就是看当前取哪个数字的，比如digits=“23”、index=2时，数字取3
    //同时index还有一个作用，当index等于path大小时，说明符合终止条件了
    void fx(string digits, int index) { //index一次次自增，直到走完字典
        if (index == digits.size()) {
            ans.push_back(path);
            return;
        }
        int num = digits[index] - '0'; //比如从“23”中找到第二个字符3并转为int型
        std::string ch = letterMap[num]; //取出对应字符集，比如3对应“def”
        for (int i = 0; i < ch.size(); i++) { //单层递归逻辑
            path.push_back(ch[i]); //处理
            fx(digits, index + 1); //递归
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

# 39 组合总和

给定一个无重复元素的数组candidates和一个目标数target，找出candidates中所有可以使数字和为target的组合，注意candidates中的数字可以无限制重复选取。

输入：candidates = [2,3,5], target = 8

答案： [ [2,2,2,2], [2,3,3], [3,5] ]

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/resource/backtravel/39ans.png" style="width:70%;">
</p>

> 终止条件

当和sum大于target时，条件不成立直接返回；和sum等于target，收集结果后返回。

> 单层递归的逻辑

单层for循环依然是从index开始，搜索candidates集合；不过本题是可以重复选取的，反应在代码注释处。

```cpp
class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        fx(candidates, target, 0, 0);
        return ans;
    }
    void fx(std::vector<int> candidates, int target, int index, int sum) {
        if (sum > target) return;
        if (sum == target) {
            ans.push_back(path);
            return;
        }
        for (int i = index; i < candidates.size(); i++) {
            sum += candidates[i];
            path.push_back(candidates[i]);
			//i不用++，因为i是传给index参数的
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

# 40 组合总和II

给定一个数组candidates和正数target，找出candidates中所有可以让数字和为target的组合。注意candidates每个数字只能使用一次。

输入: candidates = [10,1,2,7,6,1,5], target = 8

输出：[ [1, 7], [1, 2, 5], [2, 6], [1, 1, 6] ]

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/resource/backtravel/40ans.jpg" style="width:70%;">
</p>

> 问：去重是树枝去重还是树层去重呢？

答：树层去重。candidates数组有几个元素，也就最多有多少层，最多candidates每个元素都取一次；假设candidates=[1,1,7]，target=8，第一次取0号位的1，接下来7符合，答案加上{1，7}，第二次依旧会取1号位上的1，然后{1，7}，重复！

> 问：同一树层上的使用过数字需要去重，如何判断同一树层上元素（相同的元素）是否使用过了呢？

答：首先排序后，如果candidates[i] == candidates[i - 1]，就说明和之前的元素相同。当used[i - 1] == true，说明同一树枝下candidates[i - 1]是使用过的，不管向下发展了几层都是如此；因为向上回溯的原因，原本true的位置会被回设为false，当used[i - 1] == false，说明同一层之前的元素candidates[i - 1]被使用过，因为虽然它是0，但是for循环往后走了一次，i - 1 处必然是被用过的

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
    void fx(vector<int> candidates, int target, vector<bool> used, int index, int sum) {
        if (sum == target) {
            ans.push_back(path);
            return;
        }
		//sum + candidates[i] <= target为剪枝操作
        for (int i = index; i < candidates.size() && sum + candidates[i] <= target; i++) {
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
