# 13 找出数组中重复的数字

在一个长度为n的数组nums里的所有数字都在 0 ~ n - 1 的范围内。数组中某些数字是重复的，但不知道有几个数字重复了，也不知道每个数字重复了几次。请找出数组中任意一个重复的数字。

输入：[2, 3, 1, 0, 2, 5, 3]，输出：2或3

**哈希表**

```cpp
class Solution {
public:
    std::map<int, int> m = {};
    int duplicateInArray(vector<int>& nums) {
        for (int i = 0; i < nums.size(); ++i) {
            if (nums[i] < 0 || nums[i] > nums.size()) return -1;
        }
        for (int i = 0; i < nums.size(); ++i) {
            if (m[nums[i]] == 1) return nums[i]; //如果遇到计数器已经是1的，就说明遇到相同的元素了
            m[nums[i]] = 1; //如果是0，就将计数器改为1
        }
        return -1;
    }
};
```

**下标法（犁地法）**

哈希表是最常想到的方法，但是没有用到所有数据都在0～n-1这个条件

长度为n，都落在范围[0, n - 1]内，即如果不重复的话，就可以保证每个数字都与和下标相同，所以不断交换就可以“犁地”，如果遇到🐊，就说明这个数字重复了

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/resource/Offer_Answer_images/13.jpg" style="width:80%;">
</p>

```cpp
class Solution {
public:
    void swap(int& a, int& b) {
        int temp = a;
        a = b;
        b = temp;
    }
    int duplicateInArray(vector<int>& nums) {
        for (int i = 0; i < nums.size(); ++i) {
            while (i != nums[i]) {
                if (nums[i] == nums[nums[i]]) { 🐊
                    return nums[i];
                }
                swap(nums[i], nums[nums[i]]);
            }
        }
        return -1;
    }
};
```

# 15 二维数组中的查找

在一个二维数组中，每一行都按照从左到右递增的顺序排序，每一列都按照从上到下递增的顺序排序。现在输入一个整数，判断数组中是否含有该整数。

<p align="center">
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/resource/Offer_Answer_images/15.png" style="width:40%;">
</p>

```cpp
class Solution {
public:
    bool searchArray(vector<vector<int>> array, int target) {
        if (array.size() == 0) return false; //没数据直接返回false
        int row = 0;
        int col = 0;
        while (row < array[0].size() && col < array.size()) {
            if (target > array[array.size() - col - 1][row]) {
                row++;
            } else if (target < array[array.size() - col - 1][row]) {
                col++;
            } else {
                return true;
            }
        }
        return false;
    }
};
```

# 16 替换空格

请实现一个函数，把字符串中的每个空格替换成"%20"。例如输入："we are happy."，则输出"we%20are%20happy."


**差劲解法**

先申请足够的空间扩容后，从左至右依次遍历，遇到空格就替换成"%20"，但是最后面的部分会多次移动，所以时间复杂度O(n2)

**以空间换时间**

先申请一个空的字符串，遍历题目给的字符串，并一一写到空字符串末尾，遇到空格就追加写入"%20"

```cpp
class Solution {
public:
    string replaceSpaces(string &str) {
        std::string ans;
        for (char i : str)
            if (i == ' ')
                ans += "%20";
            else
                ans += i;
        return ans;
    }
};
```

**时间空间都最优**

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/resource/Offer_Answer_images/16.png" style="width:70%;">
</p>

```cpp
class Solution {
public:
    //计算str的长度和空格个数
    void get_length_blank(std::string str, int& x, int& y) {
        int i = 0; //指针
        while (str[i] != '\0') {
            ++x;
            if (str[i] == ' ') ++y;
            ++i; //指针后移
        }
    }
    string replaceSpaces(string &str) {
        int originLength = 0; //原始字符串长度
        int blank = 0; //原始字符串中空格个数
        get_length_blank(str, std::ref(originLength), std::ref(blank));
        int newLength = originLength + blank * 2; //将空格替换成"%20"后的长度
        str.resize(newLength); //给str扩容成合适的大小
        
        int p1 = originLength; //指针
        int p2 = newLength;
        while (p1 >= 0 && p2 > p1) {
            if (str[p1] != ' ') {
                str[p2] = str[p1];
                p1--;
                p2--;
            } else if (str[p1] == ' ') {
                str[p2--] = '0';
                str[p2--] = '2';
                str[p2--] = '%';
                p1--; //p1也要归位为下次做准备
            }
        }
        return str;
    }
};
```
