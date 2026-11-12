const int ALPHABET_SIZE = 26;

struct node {
    node *next[ALPHABET_SIZE];//array of pointers which stores the address for next node
    int WordsHere[ALPHABET_SIZE];//tells count of word starting having some prefix
    int EndOfWord;

    node() {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            next[i] = NULL;
            WordsHere[i] = 0;
        }
        EndOfWord = 0;
    }

    ~node() {// fix: the trie destructor only freed the first level
        for (int i = 0; i < ALPHABET_SIZE; i++)
            delete next[i];
    }
};

class trie {
public:
    node *root;
    int wordCount = 0;

    trie() {
        root = new node();//return type of new is pointer
    }

    ~trie() {
        delete root;
    }

    void insert(string &s) {
        wordCount++;

        node *crawl = root;
        int n = (int)s.size();

        for (int i = 0; i < n; i++) {
            int index = s[i] - 'a';
            (*crawl).WordsHere[index]++;

            if ((*crawl).next[index] == NULL)
                (*crawl).next[index] = new node();

            crawl = (*crawl).next[index];
        }

        (*crawl).EndOfWord++;
    }

    int find(string &s) {
        node *crawl = root;
        int n = (int)s.size();

        for (int i = 0; i < n; i++) {
            int index = s[i] - 'a';

            if ((*crawl).next[index] == NULL)
                return 0;

            crawl = (*crawl).next[index];
        }

        return (*crawl).EndOfWord;
    }

    void erase(string &s, bool allOccurence = false) {
        node *crawl = root;
        int n = (int)s.size();

        stack<node*> st;
        for (int i = 0; i < n; i++) {
            st.push(crawl);
            int index = s[i] - 'a';

            if ((*crawl).next[index] == NULL)
                return;

            crawl = (*crawl).next[index];
        }

        if ((*crawl).EndOfWord == 0) return;

        int del = (allOccurence ? (*crawl).EndOfWord : 1);

        wordCount -= del;
        (*crawl).EndOfWord -= del;

        int i = n - 1;
        while (!st.empty()) {
            crawl = st.top();
            st.pop();

            int index = s[i--] - 'a';
            (*crawl).WordsHere[index] -= del;
        }
    }

    //count occurence of full string / prefix among given words
    int count(string &s, bool prefix = false) {
        node *crawl = root;
        int n = (int)s.size();
        if (prefix and n == 0) return wordCount;// fix: every word has the empty prefix

        for (int i = 0; i < n; i++) {
            int index = s[i] - 'a';

            if ((*crawl).next[index] == NULL)
                return 0;

            if (prefix and i == n - 1)
                return (*crawl).WordsHere[index];

            crawl = (*crawl).next[index];
        }

        return (*crawl).EndOfWord;
    }

    //count words which are prefix of the given string
    int countPrefixWord(string &s) {
        node *crawl = root;
        int n = (int)s.size();

        int cnt = wordCount;
        for (int i = 0; i < n; i++) {
            int index = s[i] - 'a';

            for (int i = 0; i < ALPHABET_SIZE; i++) {
                if (i == index) continue;
                cnt -= (*crawl).WordsHere[i];
            }

            if ((*crawl).next[index] == NULL)
                return cnt;

            crawl = (*crawl).next[index];
        }

        for (int i = 0; i < ALPHABET_SIZE; i++)
            cnt -= (*crawl).WordsHere[i];

        return cnt;
    }

    //tells max length PrefixWord present among given words
    int maxPrefixWord(string &s) {
        node *crawl = root;
        int n = (int)s.size();

        int len = 0;
        for (int i = 0; i < n; i++) {
            int index = s[i] - 'a';

            // fix: erased words leave their nodes behind; only follow edges still used by a word
            if ((*crawl).next[index] == NULL or (*crawl).WordsHere[index] == 0)
                break;

            len++;
            crawl = (*crawl).next[index];
        }

        return len;
    }
};
