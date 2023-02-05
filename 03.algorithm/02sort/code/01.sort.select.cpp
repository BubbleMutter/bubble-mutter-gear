void sort_select(vector<int> &a) {
    for (int i = 0; i < a.size(); ++i) {
        int mmin = i;
        for (int j = i+1; j < a.size(); ++j)
            if (a[j] < a[mmin])
                mmin = j;
        swap(a[i], a[mmin]);
    }
}