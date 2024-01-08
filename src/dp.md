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

> 第四步：确定遍历顺序

递推公式dp[i][j] = dp[i - 1][j] + dp[i][j - 1]，得出dp[i][j]都是从其上方和左方推导而来，所以正常的从左到右遍历即可。

> 第五步：举例推导dp数组

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

举栗，`dp[4]=dp[3]*dp[0] + dp[2]*dp[1] + dp[1]*dp[2] + dp[0]*dp[3] = 5*1 + 2*1 + 1*2 + 1*5 = 14`

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

# 01背包

有n件物品和一个容量为w的背包。第i件物品的重量是weight[i]，得到的价值是value[i] 。每件物品只能用一次，求解将哪些物品装入背包里物品价值总和最大？

## 二维dp数组解法

暴力的解法是指数级别的时间复杂度O(2^n，n表示物品数量)，用动态规划可以降低时间复杂度

> 第一步：确定dp数组和下标的含义

dp[i][j]表示为从下标为[0-i]的物品里任意取，放到j个容量的背包里，所获得的最大价值

> 第二步：确定递推公式

有两个方向可以获得dp[i][j]

- 不放物品i：dp[i][j]表示为背包容量为j时，不放物品i的最大价值，此时dp[i][j]就是dp[i - 1][j]。也可以理解成物品i无法放进背包中，所以背包内的价值依然和前面相同。

- 放物品i：dp[i - 1][j - weight[i]] 为背包容量为j - weight[i]的时候不放物品i的最大价值，那么dp[i - 1][j - weight[i]] + value[i]，就是背包放物品i得到的最大价值。容量为啥是 j - weight[i]呢，因为在放物品i之前，要腾出空间给物品i，这样才有空间给到它装。

所以递推公式为 `dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - weight[i]] + value[i]);`

> 第三步：dp数组的初始化

如果背包容量j为0的话，即dp[i][0]，无论是选哪些物品，反正也装不下，价值总和一定为0。

看递推公式，i是i-1推导而来，所以dp[0][j]一定要初始化。而dp[0][j]表示为存放0号的物品时，各种容量的背包所能存放的最大价值。

- 只要当j < weight[0]时，即这些背包装不下物品，所以dp[0][j] = 0

- 当背包装得下weight[0]时，反正也是只装第一个物品，所以最大价值dp[0][j] = value[0];

```cpp
//如果把dp数组预先初始化为0，第一个循环就可以省略，但不利于理解
for(int j = 0;j < weight[0]; j++){
    dp[0][j] = 0;
}
for(int j = weight[0]; j <= bagweight; j++){
    dp[0][j] = value[0];
}
```

> 第四步：确定遍历顺序

对于01背包而言，先遍历物品或先遍历背包都可以

```cpp
//注意weight数组的大小，就是物品的个数
for(int i = 1; i < weight.size(); i++){ //遍历物品
    for(int j = 0;j <= bagweight; j++){ //遍历背包容量
        if(j < weight[i]){
            //背包塞不下物品i，所以价值dp数组为没塞物品i前的价值
            dp[i][j] = dp[i - 1][j];
        }else{
            //塞得下的情况，要比较一下不塞和塞两种情况哪个价值来的更大
            dp[i][j] = max(dp[i - 1][j],dp[i - 1][j - weight[i]] + value[i]);
        }
    }
}
```

对于本题而言，先遍历背包再遍历物品也是可以的

```cpp
for(int j = 0; j <= bagweight; j++) { //遍历背包容量
    for(int i = 1; i < weight.size(); i++) { //遍历物品
        if (j < weight[i]) {
            dp[i][j] = dp[i - 1][j]; //这一步对应一维dp的j的起始位置 
        }
        else {
            dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - weight[i]] + value[i]);
        }
    }
}
```

为什么两种遍历方向都是可以的呢？这需要理解递归的本质和递推的方向

从递推公式可以看出，dp[i][j]是靠dp[i-1][j]和dp[i - 1][j - weight[i]]推导而来的，dp[i-1][j]和dp[i-1][j-weight[i]]都在dp[i][j]的左上方，所以先遍历物品再遍历背包的过程如下图所示：

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/resource/dp/01bag.jpg" style="width:70%;">
</p>

再来看先遍历背包再物品的流程图，如上图所示。可以看到，虽然两个for循环遍历的次序不同，但是dp[i][j]所需要的数据就是左上角，根本不影响dp[i][j]公式的推导！

> 第五步：举例推导

举个例子，背包最大容量为4，物品信息如下：

|       | 重量 | 价值 |
| ----- | ---- | ---- |
| 物品0 | 1    | 15   |
| 物品1 | 3    | 20   |
| 物品2 | 4    | 30   |

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/resource/dp/bag.png" style="width:30%;">
</p>

最终的答案就在dp数组的右下角

## 一维dp数组解法

对于背包问题其实状态都是可以压缩的，就是把dp[i][j]中i的维度去掉。

> 第一步：确定dp数组和下标的含义

之前的dp[i][j]表示从下标为[0-i]的物品里任意取，放进容量为j的背包，价值总和最大是多少。

现在的一维dp[j]表示容量为j的背包所容纳的最大价值。

> 第二步：确定递推公式

现在dp[j]有两个选择，一是取自己dp[j]，相当于二维dp数组中的dp[i-1][j]，即不放物品i。

二是取dp[j - weight[i]] + value[i]，即放物品i。它表示容量为j - weight[i]的背包所背的最大价值，因为腾出了空间，所以现在可以把物品i放入包中，那么最大价值为dp[j - weight[i]] + value[i]。

所以递归公式为：`dp[j] = max(dp[j], dp[j - weight[i]] + value[i]);`

> 第三步：dp数组的初始化

dp[0]为0，因为背包容量为0所背的物品的最大价值就是0，其他下标也初始为0就可以了（很重要）。

> 第四步：确定遍历顺序

```cpp
for(int i = 0; i < weight.size(); i++) { // 遍历物品
	//遍历背包容量，j >= weight[i]是要保证背包能装下weight[i]，不然价值无从谈起
    for(int j = bagWeight; j >= weight[i]; j--) {
        dp[j] = max(dp[j], dp[j - weight[i]] + value[i]);
    }
}
```

背包倒序遍历是因为保证物品i只被放入一次，一旦正序遍历，物品i会被重复放入多次，举个例子，物品0的重量weight[0] = 1，价值value[0] = 15，

如果正序遍历：

```cpp
//dp[1]容量为1的背包最大的价值，容量正好可以装下物品0
dp[1] = dp[1 - weight[0]] + value[0] = 15;
dp[2] = dp[2 - weight[0]] + value[0] = dp[1] + value[0] = 30;
```

dp[2]是30，可见物品0被重复放入了两次。其实如果顺序遍历，左边的数会更新影响后面的数字

倒序遍历可以保证物品只放入一次：

```cpp
//所以说为什么全部初始化为0很重要
dp[2] = dp[2 - weight[0]] + value[0] = 0 + 15 = 15;
dp[1] = dp[1 - weight[0]] + value[0] = 15;
```

先遍历背包再遍历物品也是不行的，这会导致每次都只放入一个物品。

```cpp
for(int j = bagWeight; j >= weight[i]; j--) { // 遍历背包容量
	for(int i = 0; i < weight.size(); i++) { // 遍历物品
        dp[j] = max(dp[j], dp[j - weight[i]] + value[i]);
    }
}
```

> 第五步：举例推导

例题：背包最大重量为4，物品信息如下：

|       | 重量 | 价值 |
| ----- | ---- | ---- |
| 物品0 | 1    | 15   |
| 物品1 | 3    | 20   |
| 物品2 | 4    | 30   |

牢记dp[j]是j个容量的背包所背的最大价值

用物品0去遍历背包：      [0, 15, 15, 15, 15]

用物品0、1去遍历背包：   [0, 15, 15, 20, 35]

用物品0、1、2去遍历背包：[0, 15, 15, 20, 35]

dp[j]最大为最后一个。

# 416 分割等和子集

给定一个只包含正整数的非空数组。是否可以将这个数组分割成两个子集，使得两个子集的元素和相等。

输入: [1, 5, 11, 5] 输出: true 解释: 数组可以分割成 [1, 5, 5] 和 [11]

输入: [1, 2, 3, 5] 输出: false 解释: 数组不能分割成两个元素和相等的子集

一个商品可以多次重复放入是完全背包，只能放入一次是01背包

> 第一步：确定dp数组和下标的含义

dp[j]表示容量为j的背包最大可凑成j的子集总和

> 第二步：确定递推公式

一维递推公式为：`dp[j] = max(dp[j], dp[j - weight[i]] + value[i]);`

本题中，背包内放入数值，那么物品i的重量为nums[i]，价值也是nums[i]，

所以本题递推公式为：`dp[j] = max(dp[j], dp[j - nums[i]] + nums[i]);`

> 第三步：dp数组的初始化

首先dp[0]肯定是0

如果题目给的价值都是正整数那么非0下标都初始化为0就可以了，如果题目给的价值有负数，那么非0下标就要初始化为负无穷。这样才能让dp数组在递归公式的过程中取的最大的价值，而不是被初始值覆盖了。

```cpp
//题目中说：每个数组中的元素不会超过100，数组的大小不会超过200
//总和不会大于20000，背包最大只需要其中一半，所以10001大小就可以了
vector<int> dp(10001, 0);
```

> 第四步：确定遍历顺序

一维dp，外层是遍历物品，遍历背包嵌套在内，且内层for循环倒序遍历

```cpp
for(int i = 0; i < nums.size(); i++){
    for(int j = target; j >= nums[i]; j--){
        //每一个元素都不可重复放入，所以从大到小遍历
        dp[j] = max(dp[j], dp[j - nums[i]] + nums[i]);
    }
}
```

> 第五步：举例推导

若dp[j] == j 就说明，集合中的子集总和刚好凑成总和j

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/resource/dp/target.png" style="width:30%;">
</p>

最后dp[11] == 11，说明可以将这个数组分割成两个子集，使得两个子集的和相等

```cpp
bool canPartition(std::vector<int>& nums){
	int sum = 0;//定义sum是计划算出总和
	//题目中说：每个数组中的元素不会超过 100，数组的大小不会超过 200
	//总和不会大于20000，背包最大只需要其中一半，所以10001大小就可以了
	std::vector<int> dp(10001,0);
	for(int i = 0;i < nums.size();i++){
		sum = sum + nums[i];//计算集合中数字的总和
	}
	if(sum % 2 == 1)
		return false;
	int target = sum / 2;
	//01背包开始
	for(int i = 0;i < nums.size();i++){ //先遍历物品
		//j >= nums[i]是要保证背包能装的下，这些数字既是体积也是价值
		for(int j = target; j >= nums[i]; j--){ //再遍历背包
			//每一个元素一定是不可重复放入，所以从大到小遍历
			dp[j] = max(dp[j], dp[j - nums[i]] + nums[i]);
		}
	}
	//集合中的元素正好可以凑成总和target
	if(dp[target] == target)
		return true;
	return false;
}
```

# 1049 最后一块石头的重量Ⅱ

每块石头的重量都是正整数，最多30块石头，每块最大1000kg。每次选出两块石头相撞。假设石头的重量分别为x和y，如果重量相等，两块石头都被粉碎，如果不相等，重量小的被完全粉碎，重量大的石头新重量为y - x。最后，最多只会剩下一块石头。返回此石头最小的可能重量。如果没有石头剩下，就返回0

输入：[2,7,4,1,8,1] 输出：1

解释：2和4相撞，得到2，所以数组转化为 [2,7,1,8,1]，7和8相撞，得到1，数组转化为 [2,1,1,1]，2和1相撞得到1，所以数组转化为 [1,1,1]，1和1相撞都毁灭，数组转化为 [1]，这就是最优解

> 第一步：确定dp数组和下标的含义

本题中石头价值为stone[i]，对应商品的价值value[i]；体积为stone[i]，对应商品的重量weight[i]。

dp[j]表示容量为j的背包，最多可以背dp[j]这么重的石头

> 第二步：确定递推公式

```cpp
01背包：dp[j] = max(dp[j], dp[j - weight[i]] + value[i])；
本题：  dp[j] = max(dp[j], dp[j - stone[i]] + stone[i]);
```

> 第三步：dp数组的初始化

既然dp[j]中的j表示背包容量，那么最大容量就是所有石头的重量和，题中给出最多30块石头每块最大1000kg，所以最大重量就是30000kg。因为重量都不会是负数，所以dp[j]都初始化为0就可以了，这样在一维递归公式dp[j] = max(dp[j], dp[j - stones[i]] + stones[i])中，从右往左遍历dp[j]才不会初始值所覆盖

> 第四步：确定遍历顺序

一维dp数组，物品遍历的for循环放在外层，遍历背包的for循环放在内层，且内层for循环倒序遍历

> 第五步：举例推导

输入：[2,4,1,1]，此时target = (2 + 4 + 1 + 1)/2 = 4，dp数组如下

- 用stone[0]遍历背包：dp[5] = [0, 0, 2, 2, 2]
- 用stone[1]遍历背包：dp[5] = [0, 0, 2, 2, 4]
- 用stone[2]遍历背包：dp[5] = [0, 1, 2, 3, 4]
- 用stone[3]遍历背包：dp[5] = [0, 1, 2, 3, 4]

最后dp[target]里是容量为target的背包所能背的最大重量。相撞后剩下的最小石头重量就是 (sum - dp[target]) - dp[target]。

```cpp
int lastStoneWeightII(vector<int>& stones) {
	std::vector<int> dp(15001, 0); //初始化
	int sum = 0;
	for (int i = 0; i < stones.size(); i++) sum += stones[i];
	int target = sum / 2; //背包最大容量
	for (int i = 0; i < stones.size(); i++) { //遍历物品
		for (int j = target; j >= stones[i]; j--) { //遍历背包
			dp[j] = max(dp[j], dp[j - stones[i]] + stones[i]);
		}
	}
	return sum - dp[target] - dp[target];
}
```

# 494 目标和

给你一个非负整数数组nums和一个整数target，返回运算结果等于target的不同表达式的数目，直接看示例。

输入：nums = [1,1,1,1,1], target = 3，输出：5

解释：一共有 5 种方法让最终目标和为3

-1 + 1 + 1 + 1 + 1 = 3<br>
+1 - 1 + 1 + 1 + 1 = 3<br>
+1 + 1 - 1 + 1 + 1 = 3<br>
+1 + 1 + 1 - 1 + 1 = 3<br>
+1 + 1 + 1 + 1 - 1 = 3<br>

假设加法的总和为x，那么减法的总和就是sum - x

- 假设加法总和为1，那么减法总和为4，相加为 -3 != 3
- 假设加法总和为2，那么减法总和为3，相加为 -1 != 3
- 假设加法总和为3，那么减法总和为2，相加为  1 != 3
- 假设加法总和为4，那么减法总和为1，相加为  3 == 3

问题演化成往容量为4的背包里装，有多少种装法？不过4是怎么来的：假设加法总和为x，那么减法总和为sum - x，要让 x - (sum - x)等于target，解得x == (target + sum) / 2，如果x是小数，肯定不行；如果所给target的绝对值比nums总和sum还大，也不行

> 第一步：确定dp数组和下标的含义

填满j容量的背包有dp[j]种方法

> 第二步：确定递推公式

直接说结论，求装满j容量的背包有多少种方法这种组合题目，递推公式为

`dp[j] += dp[j - nums[i]]`

> 第三步：dp数组的初始化

在初始化的时候dp[0] 一定要初始化为1，因为dp[0]是在公式中一切递推结果的起源，如果dp[0]是0的话，递归结果将都是0；也很好解释：装满容量为0的背包，有1种方法，就是装0件物品。

dp[j]其他下标对应的数值应该初始化为0

> 第四步：确定遍历顺序

01背包问题一维dp的遍历，nums放在外循环，target在内循环，且内循环倒序，就是先遍历物品再遍历背包

> 第五步：举例推导

略

```cpp
class Solution{
public:
	int findTargetSumWays(std::vector<int>& nums,int target){
        int sum = 0;
		for(int i = 0;i < nums.size();i++){
			sum += nums[i];
		}
		//假设加法的总和为x，那么减法对应的总和应该为sum - x
		//所以要求为x - (sum - x) = target
		//x = (target + sum) / 2

		//(target + sum) / 2 不为整数，即01背包容量不为整数，这是不可能的
		if((target + sum) % 2 == 1)
			return 0;
		//如果target的绝对值比sum还大，也是不可能的,返回0种方案
		if(abs(target) > sum)
			return 0;
		int bagsize = (sum + target) / 2; //背包容量的大小
        std::vector<int> dp(bagsize + 1, 0);//定义dp数组
		dp[0] = 1;//初始化dp数组
		for(int i = 0;i < nums.size();i++){
			for(int j = bagsize;j >= nums[i];j--){
				dp[j] += dp[j - nums[i]];
			}
		}
		return dp[bagsize];
	}
};
```

# 01背包总结

零一背包有二维和一维数组之分，二维数组遍历物品和遍历背包的顺序可以颠倒，一维数组不可以颠倒，一定要先遍历物品再遍历背包，并且背包的遍历顺序上要从右往左。

往j容量的背包中装入数字，一共有多少种装法这类问题，递推公式是固定的：`dp[j] += dp[j - nums[i]]`，并且dp[0]要初始化为1。

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/resource/dp/summary.jpg" style="width:60%;">
</p>
