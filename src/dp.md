# 509 裴波那契数

# 70 爬楼梯

# 746 使用最小花费爬楼梯

给定一个整数数组cost，cost[i] 是从第i个台阶向上爬所需的费用。一旦你支付，可选择向上爬一个或者两个台阶。可以选择从下标0或下标1的台阶开始爬，请返回达到顶楼的最低开销

> 第一步：确定dp数组和下标的含义

dp[i]的定义为到达第i阶所需的最小费用

> 第二步：确定递推公式

到达i阶之前（楼梯号：i - 1 or i - 2）所需要的费用，加上目前所要的过路费cost[i]：

dp[i] = min(dp[i - 1] + dp[i - 2]) + cost[i]；

> 第三步：dp数组的初始化

由递推公式，dp[i]是由dp[i - 1] 和 dp[i - 2]推来的，往前追溯，只需初始化dp[0] 和 dp[1]，dp[0] = cost[0],dp[1] = cost[1];

> 第四步：确定遍历顺序

从前往后

> 第五步：举例推导dp数组

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/resource/746ans.png" style="width:70%;">
</p>

```cpp
int minCostClimbingStairs(vector<int>& cost) {
	std::vector<int> dp(cost.size(), 0);
	dp[0] = cost[0];
	dp[1] = cost[1];
	for (int i = 2; i < cost.size(); i++) {
		dp[i] = min(dp[i - 2], dp[i - 1]) + cost[i];
	}
	//最后一步可理解为不用花费，取倒数第一或者倒数第二的最小值
	return min(dp[cost.size() - 1],dp[cost.size() - 2]);
}
```

这道题不好，哪里要花费那里不要，最后一步台阶要不要也不清不楚，可跳过，这题不好

# 62 不同路径

一个机器人位于一个 m x n 网格的左上角，每次只能向下或向右移动一步。机器人试图达到网格的右下角，总共有多少条不同的路径？

输入：m = 3, n = 7, 输出：28

尝试用图论的DFS做，但是超时了。

```cpp
class Solution {
private:
    int dfs(int i, int j, int m, int n) {
        if (i > m || j > n) return 0; // 越界了
        if (i == m && j == n) return 1; // 找到一种方法，相当于找到了叶子节点
        return dfs(i + 1, j, m, n) + dfs(i, j + 1, m, n);
    }
public:
    int uniquePaths(int m, int n) {
        return dfs(1, 1, m, n);
    }
};
```

> 第一步：确定dp数组和下标的含义

dp[i][j]表示机器人从(0,0)移动到(i,j)处有dp[i][j]种不同的走法

> 第二步：确定递推公式

要想求dp[i][j]，就要从dp[i - 1][j]和dp[i][j - 1]推导而来，dp[i - 1][j]表示(0,0)到(i - 1,j)有几条路径，dp[i][j - 1]同理，于是dp[i][j] = dp[i - 1][j] + dp[i][j - 1]。

> 第三步：dp数组的初始化

二维vector的初始化，循环初始化：因为从(0,0)到(i,0)的路径只有一条，所以dp[i][0]都要初始化为1；

同理(0,0)到(0,j)的路径也只有一条，所以dp[0][j]全都要初始化为1。

第四步：确定遍历顺序

递推公式dp[i][j] = dp[i - 1][j] + dp[i][j - 1]，得出dp[i][j]都是从其上方和左方推导而来，所以正常的从左到右遍历即可。

第五步：举例推导dp数组

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/resource/62ans.png" style="width:60%;">
</p>

```cpp
int uniquePaths(int m, int n) {
	std::vector<std::vector<int>> dp(m, std::vector<int>(n, 1));
	for (int i = 1; i < m; i++) {
		for (int j = 1; j < n; j++) {
			dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
		}
	}
	return dp[m - 1][n - 1];
}
```

# 63 不同路径Ⅱ

一个机器人位于一个 m x n 网格的左上角，每次只能向下或向右移动一步。机器人试图达到网格的右下角，在考虑图中有障碍物的情况下，总共有多少条不同的路径？图中有障碍用1表示没障碍用0表示

输入：obstacleGrid = [[0,0,0],[0,1,0],[0,0,0]]，输出：2

> 第一步：确定dp数组和下标的含义

dp[i][j]表示机器人从(0,0)移动到(i,j)处有dp[i][j]种不同的走法

> 第二步：确定递推公式

依旧的dp[i][j] = dp[i - 1][j] + dp[i][j - 1]，但是有了障碍物后，需要注意一点的是，当(i,j)是障碍物时，(i,j)就是机器人不可达状态

> 第三步：dp数组的初始化

因为从(0,0)到(i,0)的路径只有一条，所以dp[i][0]都要初始化为1；

同理(0,0)到(0,j)的路径也只有一条，所以dp[0][j]全都要初始化为1。

> 第四步：确定遍历顺序

从左到右一层一层遍历

> 第五步：举例推导dp数组

```cpp
int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
	int m = obstacleGrid.size(); //行
	int n = obstacleGrid[0].size(); //列
	vector<vector<int>> ans(m, vector<int>(n, 0));
	for (int i = 0; i < m && obstacleGrid[i][0] == 0; i++) {
		ans[i][0] = 1; //第一列且没障碍物的格子全部设置为1
	}
	for (int i = 0; i < n && obstacleGrid[0][i] == 0; i++) {
		ans[0][i] = 1; //第一行且没障碍物的格子全部设置为1
	}
	for (int i = 1; i < m; i++) {
		for (int j = 1; j < n; j++) {
			if (obstacleGrid[i][j] == 1){
				//(i,j)处是障碍物，机器人不可达，dp[i][j]保持为0
				//即到达(i,j)处的路线有0条
				continue;
			}
			ans[i][j] = ans[i - 1][j] + ans[i][j - 1];
		}
	}
	return ans[m - 1][n - 1];
}
```

# 96 不同的二叉搜索树

给定一个整数 n，求以 1 ... n 为节点组成的二叉搜索树有多少种？

> 第一步：确定dp数组和下标的含义

dp[i]：i个不同元素节点组成的二叉搜索树的个数

比如dp[3]就是3个不同元素（数字1，2，3）组成的二叉搜索树的个数

> 第二步：确定递推公式

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/resource/96ans.jpg" style="width:70%;">
</p>

注意布局只是位置差异，不关心数值的差异。二叉搜索树为 “左 < 根 < 右”

假设当前节点为j，那么左子树的节点集合为[1, j - 1]，右子树的节点集合为[j + 1, n]，当n是4的时候展开就是：

- 当1为头节点时，他的右子树表现为 只有3个元素组成的布局，没有左子树因为没有比1更小的数
- 当2为头节点时，他的左子树表现为 只有1个元素组成的布局，右子树布局表现为 只有2个元素组成的布局
- 当3为头节点时，他的左子树表现为 只有2个元素组成的布局，右子树布局表现为 只有1个元素组成的布局
- 当4为头节点时，他的左子树表现为 只有3个元素组成的布局，没有右子树

再举个例子，n为3的时候：

- 当1为头节点时，他的右子树的布局只有两个元素构成的布局，肯定和n为2时一模一样
- 当3为头节点时，他的左子树只有两个元素组成，布局和n为2时一模一样
- 当2为头节点时，他的左右子树都只有一个元素，布局和n为1时只有一棵树的布局一模一样

关键点：可以通过dp[1]和dp[2]来推导dp[3]

dp[3]就是头节点为1的搜索树数量 + 头节点为2的搜索树数量 + 头节点为3的搜索树数量

- 头节点为1的搜索树数量 = 右子树有2个元素的搜索树数量 * 左子树有0个元素的搜索树数量 = dp[2]*dp[0]
- 头节点为2的搜索树数量 = 右子树有1个元素的搜索树数量 * 左子树有1个元素的搜索树数量 = dp[1]*dp[1]
- 头节点为3的搜索树数量 = 右子树有0个元素的搜索树数量 * 左子树有2个元素的搜索树数量 = dp[0]*dp[2]

dp[i] += dp[以j为头结点左子树节点数量] * dp[以j为头结点右子树节点数量]

> 第三步：dp数组的初始化

推导的基础是dp[0]，所以只需要初始化dp[0]，dp[0]虽然没有意义，但是在计算的过程中为了方便和代码完整

> 第四步：确定遍历顺序

> 第五步：举例推导dp数组

举栗，dp[4]=dp[3]*dp[0] + dp[2]*dp[1] + dp[1]*dp[2] + dp[0]*dp[3] = 5*1 + 2*1 + 1*2 + 1*5 = 14

```cpp
int numTrees(int n) {
	std::vector<int> dp(n + 1, 0);
	dp[0] = 1; //dp[0]没有实际意义，只是为了运算方便
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= i; j++) {
			dp[i] += dp[j - 1] * dp[i - j];
		}
	}
	return dp[n];
}
```
