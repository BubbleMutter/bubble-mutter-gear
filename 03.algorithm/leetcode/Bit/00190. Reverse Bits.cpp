/*
 * @lc app=leetcode id=190 lang=cpp
 *
 * [190] Reverse Bits
 *
 * https://leetcode.com/problems/reverse-bits/description/
 *
 * algorithms
 * Easy (41.88%)
 * Total Accepted:    326.1K
 * Total Submissions: 774.2K
 * Testcase Example:  '00000010100101000001111010011100'
 *
 * Reverse bits of a given 32 bits unsigned integer.
 * 
 * Note:
 * 
 * 
 * Note that in some languages such as Java, there is no unsigned integer type.
 * In this case, both input and output will be given as a signed integer type.
 * They should not affect your implementation, as the integer's internal binary
 * representation is the same, whether it is signed or unsigned.
 * In Java, the compiler represents the signed integers using 2's complement
 * notation. Therefore, in Example 2 above, the input represents the signed
 * integer -3 and the output represents the signed integer -1073741825.
 * 
 * 
 * Follow up:
 * 
 * If this function is called many times, how would you optimize it?
 * 
 * 
 * Example 1:
 * 
 * 
 * Input: n = 00000010100101000001111010011100
 * Output:    964176192 (00111001011110000010100101000000)
 * Explanation: The input binary string 00000010100101000001111010011100
 * represents the unsigned integer 43261596, so return 964176192 which its
 * binary representation is 00111001011110000010100101000000.
 * 
 * 
 * Example 2:
 * 
 * 
 * Input: n = 11111111111111111111111111111101
 * Output:   3221225471 (10111111111111111111111111111111)
 * Explanation: The input binary string 11111111111111111111111111111101
 * represents the unsigned integer 4294967293, so return 3221225471 which its
 * binary representation is 10111111111111111111111111111111.
 * 
 * 
 * 
 * Constraints:
 * 
 * 
 * The input must be a binary string of length 32
 * 
 * 
 */
class Solution {
    // 1. 16bit 16bit 地翻转 ffff 0000 <> 0x0000 ffff
    // 2. 8bit   8bit 地翻转 ff00 <> 00ff
    // 3. 4bit   4bit 地翻转 f0 <> 0f
    // 4. 2bit   2bit 地翻转 1100 <> 0011
    // 5. 1bit   1bit 地翻转 1010 <> 0101
public:
    uint32_t reverseBits(uint32_t n) {
#if 0
        static struct {
            uint32_t mask;
            uint32_t step;
        } unit[] = {
            { .mask = 0xffff0000, .step = 16 },
            { .mask = 0xff00ff00, .step =  8 },
            { .mask = 0xf0f0f0f0, .step =  4 },
            { .mask = 0xcccccccc, .step =  2 },
            { .mask = 0xaaaaaaaa, .step =  1 },
        };
        for (int i =  0; i < sizeof(unit)/sizeof(*unit); i++)
            n = ((n & unit[i].mask) >> unit[i].step) | ((n & (unit[i].mask >> unit[i].step)) << unit[i].step);
#endif

        n = ((n & 0xffff0000) >> 16) | ((n & 0x0000ffff) << 16);
        n = ((n & 0xff00ff00) >>  8) | ((n & 0x00ff00ff) <<  8);
        n = ((n & 0xf0f0f0f0) >>  4) | ((n & 0x0f0f0f0f) <<  4);
        n = ((n & 0xcccccccc) >>  2) | ((n & 0x33333333) <<  2);
        n = ((n & 0xaaaaaaaa) >>  1) | ((n & 0x55555555) <<  1);

        return n;
    }
};
