# LCR23 相交链表

给定两个单链表的头节点 headA 和 headB ，请找出并返回两个单链表相交的起始节点。如果两个链表没有交点，返回 null 。

<p align="center"> 
    <img src="https://github.com/arqady01/Cpp-interface/blob/main/resource/listnode/LCR23.png" style="width:60%;">
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
