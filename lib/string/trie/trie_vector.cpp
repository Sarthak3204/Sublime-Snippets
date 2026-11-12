// Trie over integer sequences with alphabet [0, N).
//   trie<10> t; t.insert(v); t.erase(v, all_occurrences); t.find(v); t.count(v, prefix);
//   t.count_prefix_word(v)  -> stored words that are a prefix of v
//   t.max_prefix_word(v)    -> longest prefix of v shared with a stored word
template <int N>
struct trie_node {
    trie_node *next[N];  // child pointers
    int words_here[N];   // words going through next[i]
    int end_of_word = 0;

    trie_node() {
        for (int i = 0; i < N; i++) next[i] = nullptr, words_here[i] = 0;
    }
    ~trie_node() {  // fix: the original only freed the first level
        for (int i = 0; i < N; i++) delete next[i];
    }
};

template <int N>
class trie {
    using node = trie_node<N>;

public:
    int word_count = 0;

    trie() : root_(new node()) {}
    ~trie() { delete root_; }
    trie(const trie &) = delete;
    trie &operator=(const trie &) = delete;

    void insert(const vector<int> &s) {
        word_count++;
        node *crawl = root_;
        for (int index : s) {
            crawl->words_here[index]++;
            if (crawl->next[index] == nullptr) crawl->next[index] = new node();
            crawl = crawl->next[index];
        }
        crawl->end_of_word++;
    }

    int find(const vector<int> &s) const {
        node *crawl = root_;
        for (int index : s) {
            if (crawl->next[index] == nullptr) return 0;
            crawl = crawl->next[index];
        }
        return crawl->end_of_word;
    }

    void erase(const vector<int> &s, bool all_occurrence = false) {
        node *crawl = root_;
        stack<node *> st;
        for (int index : s) {
            st.push(crawl);
            if (crawl->next[index] == nullptr) return;
            crawl = crawl->next[index];
        }
        if (crawl->end_of_word == 0) return;

        int del = all_occurrence ? crawl->end_of_word : 1;
        word_count -= del;
        crawl->end_of_word -= del;

        int i = (int)s.size() - 1;
        while (!st.empty()) {
            crawl = st.top();
            st.pop();
            crawl->words_here[s[i--]] -= del;
        }
    }

    // occurrences of the full vector, or (prefix = true) of words having it as a prefix
    int count(const vector<int> &s, bool prefix = false) const {
        node *crawl = root_;
        int n = s.size();
        if (prefix and n == 0) return word_count;
        for (int i = 0; i < n; i++) {
            int index = s[i];
            if (crawl->next[index] == nullptr) return 0;
            if (prefix and i == n - 1) return crawl->words_here[index];
            crawl = crawl->next[index];
        }
        return crawl->end_of_word;
    }

    int count_prefix_word(const vector<int> &s) const {
        node *crawl = root_;
        int cnt = word_count;
        for (int index : s) {
            for (int i = 0; i < N; i++)
                if (i != index) cnt -= crawl->words_here[i];
            if (crawl->next[index] == nullptr) return cnt;
            crawl = crawl->next[index];
        }
        for (int i = 0; i < N; i++) cnt -= crawl->words_here[i];
        return cnt;
    }

    int max_prefix_word(const vector<int> &s) const {
        node *crawl = root_;
        int len = 0;
        for (int index : s) {
            // fix: erased words leave their nodes behind; only follow edges still used by a word
            if (crawl->next[index] == nullptr or crawl->words_here[index] == 0) break;
            len++;
            crawl = crawl->next[index];
        }
        return len;
    }

private:
    node *root_;
};
