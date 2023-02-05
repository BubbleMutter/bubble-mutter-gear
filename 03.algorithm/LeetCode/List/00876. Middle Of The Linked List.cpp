/*
 * @lc app=leetcode id=876 lang=cpp
 *
 * [876] Middle of the Linked List
 *
 * https://leetcode.com/problems/middle-of-the-linked-list/description/
 *
 * algorithms
 * Easy (71.94%)
 * Total Accepted:    548.1K
 * Total Submissions: 760.6K
 * Testcase Example:  '[1,2,3,4,5]'
 *
 * Given the head of a singly linked list, return the middle node of the linked
 * list.
 * 
 * If there are two middle nodes, return the second middle node.
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: head = [1,2,3,4,5]
 * Output: [3,4,5]
 * Explanation: The middle node of the list is node 3.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: head = [1,2,3,4,5,6]
 * Output: [4,5,6]
 * Explanation: Since the list has two middle nodes with values 3 and 4, we
 * return the second one.
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * The number of nodes in the list is in the range [1, 100].
 * 1 <= Node.val <= 100
 * 
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
    // 快慢指针;
    // 如果是 A B;       使得慢指针指向B 为中间节点
    // 如果是 A B C;     使得慢指针指向B 为中间节点
    // 如果是 A B C D；  使得慢指针指向C
    //   A->B->C->nullptr
    //   D->C---->nullptr
    // 如果是 A B C D E; 使得慢指针指向C
    //   A->B->C->nullptr
    //   E->D->C->nullptr

    ListNode* middleNode(ListNode* head) {
        // 给定一个带有头结点 head 的非空单链表，返回链表的中间结点。
        // 如果有两个中间结点，则返回第二个中间结点。
        ListNode *fast = head, *slow = head;
        while (fast && fast->next) {
            fast = fast->next->next;
            slow = slow->next;
        }
        return slow;
    }
};
