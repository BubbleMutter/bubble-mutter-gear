/**
 * @file 给定双向容器, 如 vector
 *       以 给定步进n 遍历这个容器
 */
void step(vector<string> &m, int step) {
    cout << "post step: " << step << endl;
    for (int i = m.size(); i > 0; i -= step) {
        vector<string> n((i-step) > 0 ? m.begin()+(i-step): m.begin(), m.begin()+i);
        cout << "n: " << n << endl;
    }
}

int main(void) {
    vector<string> m;
    for (int i = 0; i < 30; i++)
        m.push_back(to_string(i));
    cout << "m: " << m << endl;

    step(m, 30);
    step(m, 29);
    step(m, 7);
    step(m, 5);
    step(m, 6);
    return 0;
}
