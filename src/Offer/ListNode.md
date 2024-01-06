# LCR23 相交链表

给定两个单链表的头节点 headA 和 headB ，请找出并返回两个单链表相交的起始节点。如果两个链表没有交点，返回 null 。

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/resource/listnode/LCR23.png" style="width:70%;">
</p>

```cpp
class Solution {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
		//计算链表A和链表B的长度
        int count_A = get_count(headA);
        int count_B = get_count(headB);
		//让指针longer指向长链表头部，指针shorter指向短链表头部
        ListNode* longer = (count_A > count_B) ? headA : headB;
        ListNode* shorter = (count_A > count_B) ? headB : headA;
		//需要保证链表个数相减是正数
        int diff = (count_A > count_B) ? (count_A - count_B) : (count_B - count_A);
        for (int i = 0; i < diff; i++)
            longer = longer->next; //让长链表的指针移动diff位，使其和短链表开头齐平
        while (longer != nullptr) {
            if (longer == shorter) { //这个==肯定是重载过的，但是不用管
                return longer;
            }
            longer = longer->next;
            shorter = shorter->next;
        }
        return nullptr;
    }
    //统计链表中节点个数
    int get_count(ListNode* head) {
        int count = 0;
        while (head != nullptr) {
            count++;
            head = head->next;
        }
        return count;
    }

};
```

解法二：利用环形链表

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/resource/listnode/LCR23_1.jpg" style="width:70%;">
</p>

# LCR25 链表中的数字相加

给定两个非负链表l1和l2。数字最高位位于链表开始位置。它们的每个节点只存储一位数字。将这两数相加会返回一个新的链表

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/resource/listnode/LCR25.png" style="width:70%;">
</p>

注意不能根据链表求出整数，然后将两个整数相加，最后把结果用链表表示。因为这样存在整数溢出的问题

同时也要注意，当两个链表长度不等时，应该把尾部对齐在相加而不是直接从头部开始相加，

但是两个尾节点相加后，在单向链表中却无法前进到倒数第二个节点，因为单向链表是next节点而不含last节点。

所以应该将链表反转。反转之后的链表的头节点表示个位数，尾节点表示最高位数。此时从两个链表的头节点开始相加，就相当于从整数的个位数开始相加。

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/resource/listnode/25.png" style="width:70%;">
</p>

```cpp
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode* head1 = reverse(l1); //反转链表l1
        ListNode* head2 = reverse(l2); //反转链表l2
        ListNode* ans = rebuild(head1, head2);
	    return reverse(ans); //再反转过来
    }
    //反转链表
    ListNode* reverse(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* cur = head;
        while (cur != nullptr) {
            ListNode* p = cur->next; //防止断链
            cur->next = prev;
            prev = cur; //向后移动
            cur = p; //向后移动
        }
        return prev;
    }
    //重建链表
    ListNode* rebuild(ListNode* head1, ListNode* head2) {
        ListNode* dummy = new ListNode(0);
        ListNode* cur = dummy; //操作指针
        int carry = 0; //进位，如果和超过10，进位就是1，否则不需要进位，进位就是0
        while (head1 != nullptr || head2 != nullptr) {
            int sum = ((head1 == nullptr) ? 0 : head1->val) + ((head2 == nullptr) ? 0 : head2->val) + carry;
            carry = (sum > 9) ? 1 : 0;
            int number = (sum > 9) ? (sum - 10) : sum;
            ListNode* newNode = new ListNode(number);
            cur->next = newNode;
            cur = newNode; //移动操作指针，cur = cur->next也可以，因为cur的next已经打通
            head1 = (head1 == nullptr) ? nullptr : head1->next;
            head2 = (head2 == nullptr) ? nullptr : head2->next;
        }
        //如果最后一次进位为1，就需要开个小灶
        if (carry == 1) {
            cur->next = new ListNode(1);
        }
        return dummy->next;
    }
};
```

# 26：重排链表

给定一个链表，链表中节点的顺序是 L0->L1->L2->…->Ln-1->Ln，请重排链表使节点的顺序变成 L0->Ln->L1->Ln-1->L2->Ln-2->…

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/resource/listnode/26.png" style="width:70%;">
</p>

我们先可以找到链表的中间节点，然后把链表分成两半，然后把后半段链表反转，最后从前半段链表和后半段链表的头节点开始，逐个把它们的节点连接起来形成一个新链表

寻找链表中间节点：双指针

一快一慢指针同时从链表的头节点出发，快指针一次向前走两步，而慢指针一次走一步，那么当快指针走到链表尾节点时慢指针刚好走到链表中间。

先把前半段链表和后半段链表的头，节点1和6连接起来，再把节点2和5连接起来，最后把两个尾节点3和4连接起来，因此在新的链表中节点的顺序是1、6、2、5、3、4

```cpp
class Solution {
public:
    //主函数
    void reorderList(ListNode* head) {
        if (head == nullptr) return;
        ListNode* middle = get_middle(head); //得到中间节点middle
        ListNode* l1 = head;
        ListNode* l2 = middle->next; //防止分开后找不到第二段链表
        middle->next = nullptr; //让中间节点的next指向空，即从中间断开
        l2 = reverse(l2); //反转第二段链表
        mergeList(l1, l2);
    }
    //寻找链表中间节点
    ListNode* get_middle(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;
        while (fast->next != nullptr && fast->next->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow;
    }
    //反转链表
    ListNode* reverse(ListNode* head) {
        ListNode* prev = nullptr;
        ListNode* cur = head;
        while (cur != nullptr) {
            ListNode* p = cur->next; //防止断链
            cur->next = prev;
            prev = cur; //向后移动
            cur = p; //向后移动
        }
        return prev;
    }
    //1->2->3->4->nullptr
    //6->7->8->9->nullptr
    //需要再多两个操作指针，记为p1和p2
    void mergeList(ListNode* node1, ListNode* node2) {
        ListNode* p1;
        ListNode* p2;
        while (node1 != nullptr && node2 != nullptr) {
            p1 = node1->next;
            p2 = node2->next;

            node1->next = node2;
            node2->next = p1;

            node1 = p1;
            node2 = p2;
        }
    }
};
```

# 回文链表

请判断一个链表是否回文（链表的节点序列从前往后和从后往前看是相同的）

回文链表的一个特性是对称性，如果是偶数个节点，前半部分反转后和后半部分相同就是回文链表；假如是奇数个节点，比如1、2、k、2、1，前半部分反转之后是2、1，后面也是2、1，不管中间节点是什么该链表都是回文链表。

```cpp
class Solution {
public:
    bool isPalindrome(ListNode* head) {
        ListNode* l1 = head;
        ListNode* mid = get_middle(head);
        ListNode* l2 = mid->next;
        mid->next = nullptr; //至此，一个链表从中间一分为二
        l2 = reverse(l2);
        while (l2 != nullptr) {
            if (l1->val != l2->val) return false;
            l1 = l1->next;
            l2 = l2->next;
        }
        return true;
    }
    //寻找中间节点
    ListNode* get_middle(ListNode* head) {
        ListNode* fast = head;
        ListNode* slow = head;
        while (fast->next != nullptr && fast->next->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow;
    }
    //反转链表
    ListNode* reverse(ListNode* head) {
        ListNode* cur = head;
        ListNode* prev = nullptr;
        while (cur != nullptr) {
            ListNode* p = cur->next;
            cur->next = prev;
            prev = cur;
            cur = p;
        }
        return prev;
    }
};
```
