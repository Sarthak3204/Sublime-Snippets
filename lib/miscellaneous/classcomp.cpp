struct classcomp {
    bool operator() (const pair<int, int> & a, const pair<int, int> & b) const {
        if (a.first != b.first) return a.first < b.first;
        else return a.second > b.second;
    }
};
