/*
 * @lc app=leetcode id=860 lang=cpp
 *
 * [860] Lemonade Change
 *
 * https://leetcode.com/problems/lemonade-change/description/
 *
 * algorithms
 * Easy (52.28%)
 * Total Accepted:    90.3K
 * Total Submissions: 172.7K
 * Testcase Example:  '[5,5,5,10,20]'
 *
 * At a lemonade stand, each lemonade costs $5. Customers are standing in a
 * queue to buy from you and order one at a time (in the order specified by
 * bills). Each customer will only buy one lemonade and pay with either a $5,
 * $10, or $20 bill. You must provide the correct change to each customer so
 * that the net transaction is that the customer pays $5.
 *
 * Note that you do not have any change in hand at first.
 *
 * Given an integer array bills where bills[i] is the bill the i^th customer
 * pays, return true if you can provide every customer with the correct change,
 * or false otherwise.
 *
 *
 * Example 1:
 * 
 * 
 * Input: bills = [5,5,5,10,20]
 * Output: true
 * Explanation: 
 * From the first 3 customers, we collect three $5 bills in order.
 * From the fourth customer, we collect a $10 bill and give back a $5.
 * From the fifth customer, we give a $10 bill and a $5 bill.
 * Since all customers got correct change, we output true.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: bills = [5,5,10,10,20]
 * Output: false
 * Explanation: 
 * From the first two customers in order, we collect two $5 bills.
 * For the next two customers in order, we collect a $10 bill and give back a
 * $5 bill.
 * For the last customer, we can not give the change of $15 back because we
 * only have two $10 bills.
 * Since not every customer received the correct change, the answer is
 * false.
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * 1 <= bills.length <= 10^5
 * bills[i] is either 5, 10, or 20.
 * 
 * 
 */
class Solution {
public:
    bool lemonadeChange(vector<int>& bills) {
        vector<int> packets(2, 0);
        for (const auto& bill: bills) {
            switch (bill) {
                case 5:
                    packets[0]++; // 收获1张5
                    break;
                case 10:
                    if (packets[0] > 0) { // 找零1张5
                        packets[0]--;
                    } else {
                        return false;
                    }
                    packets[1]++; // 收获1张10
                    break;
                case 20:
                    if (packets[0] > 0 && packets[1] > 0) {
                        // 找零 1张5 和 1张 10
                        packets[0]--;
                        packets[1]--;
                    } else if (packets[0] > 2) {
                        // 找零 3张5
                        packets[0] -= 3;
                    } else {
                        return false;
                    }
                    break;
            }
        }
        return true;
    }
};
