// 环形数组的 左移 右移
int ring_buffer[1024];
int curr_cursor;

void ring_buff_forward(int step) {
    curr_cursor = (curr_cursor + step) % array_count(ring_buff);
}
void ring_buff_backward(int step) {
    // 逆向步进 = 步进一圈 再反向走
    // 因为直接方向走 会 导致索引变为负数; 负数取模的出现负数索引
    curr_cursor = (curr_cursor + array_count(ring_buff) - step) % array_count(ring_buff);
}

// % is a slow operation,
// when array_count base'2 exponent
// use & to optimize
void ring_buff_forward(int step) {
    curr_cursor =  (curr_cursor + step) & (array_count(ring_buff)-1);
}
void ring_buff_backward(int step) {
    // 逆向步进 = 步进一圈 再反向走
    // 因为直接方向走 会 导致索引变为负数; 负数取模的出现负数索引
    curr_cursor = (curr_cursor + array_count(ring_buff) - step) & (array_count(ring_buff)-1);
}

// ---------------