/*
 * @lc app=leetcode id=155 lang=cpp
 *
 * [155] Min Stack
 *
 * https://leetcode.com/problems/min-stack/description/
 *
 * algorithms
 * Easy (46.20%)
 * Total Accepted:    681K
 * Total Submissions: 1.5M
 * Testcase Example:  '["MinStack","push","push","push","getMin","pop","top","getMin"]\n[[],[-2],[0],[-3],[],[],[],[]]'
 *
 * Design a stack that supports push, pop, top, and retrieving the minimum
 * element in constant time.
 * 
 * 
 * push(x) -- Push element x onto stack.
 * pop() -- Removes the element on top of the stack.
 * top() -- Get the top element.
 * getMin() -- Retrieve the minimum element in the stack.
 * 
 * 
 * 
 * Example 1:
 * 
 * 
 * Input
 * ["MinStack","push","push","push","getMin","pop","top","getMin"]
 * [[],[-2],[0],[-3],[],[],[],[]]
 * 
 * Output
 * [null,null,null,null,-3,null,0,-2]
 * 
 * Explanation
 * MinStack minStack = new MinStack();
 * minStack.push(-2);
 * minStack.push(0);
 * minStack.push(-3);
 * minStack.getMin(); // return -3
 * minStack.pop();
 * minStack.top();    // return 0
 * minStack.getMin(); // return -2
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * Methods pop, top and getMin operations will always be called on non-empty
 * stacks.
 * 
 * 
 */
class MinStack {
    vector<int> stk;
    int mmin;
public:
    /** initialize your data structure here. */
    MinStack() { mmin = numeric_limits<int>::max(); }

    void push(int x) { stk.push_back(x); mmin = min(mmin, x); }

    void pop() {
        if (stk.back() == mmin) {
            stk.pop_back();
            mmin = numeric_limits<int>::max();
            for (int i = 0; i < stk.size(); i++)
                mmin = min(stk[i], mmin);
        } else {
            stk.pop_back();
        }
    }

    int top() { return stk.back(); }

    int getMin() { return mmin; }
};

/**
 * Your MinStack object will be instantiated and called as such:
 * MinStack* obj = new MinStack();
 * obj->push(x);
 * obj->pop();
 * int param_3 = obj->top();
 * int param_4 = obj->getMin();
 */
