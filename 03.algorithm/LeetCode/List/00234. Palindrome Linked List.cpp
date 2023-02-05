/*
 * @lc app=leetcode id=234 lang=cpp
 *
 * [234] Palindrome Linked List
 *
 * https://leetcode.com/problems/palindrome-linked-list/description/
 *
 * algorithms
 * Easy (40.34%)
 * Total Accepted:    566.3K
 * Total Submissions: 1.4M
 * Testcase Example:  '[1,2]'
 *
 * Given a singly linked list, determine if it is a palindrome.
 * 
 * Example 1:
 * 
 * 
 * Input: 1->2
 * Output: false
 * 
 * Example 2:
 * 
 * 
 * Input: 1->2->2->1
 * Output: true
 * 
 * Follow up:
 * Could you do it in O(n) time and O(1) space?
 * 
 */
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    bool isPalindrome(ListNode* head) {
        // 1. 双指针法; slow 指针最终指向 中间节点
        //    前提是链表不成环
        // 2. 中间节点及其后面 的链表翻转
        // 3. 同时 从开头到中间节点遍历
        //         从翻转后中间节点遍历
        // 4. 其中有节点值不相等; 返回false
        // 5. 能遍历结束; 说明节点相等
        if (!head) return true;
        ListNode *fast = head->next; // 避免 [0,1]
        ListNode *slow = head;
        while (fast && fast->next) {
            fast = fast->next->next;
            slow = slow->next;
        }

        // 翻转后 两个链表最终成 Y 形状
        fast = nullptr;
        while (slow) {
            ListNode *next = slow->next;
            slow->next = fast;
            fast = slow;
            slow = next;
        }
        slow = head; // 此时 fast 指向后半翻转后的链表
        while (slow && fast) {
            if (slow->val != fast->val)
                return false;
            slow = slow->next;
            fast = fast->next;
        }
        return true;
    }
};
