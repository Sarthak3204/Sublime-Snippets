const int N = 30;
 
struct node {
    node *l = NULL, *r = NULL;
};
 
class trie {
public:
    node* root;
    trie() {
        root = new node;
    }
 
    void insert(int x) {
        node *crawl = root;
        for (int i = N - 1; i >= 0; i--) {
            if ((x >> i) & 1) {
                if (crawl->r == NULL) crawl->r = new node;
                crawl = crawl->r;
            }
            else {
                if (crawl->l == NULL) crawl->l = new node;
                crawl = crawl->l;
            }
        }
    }
 
    int calc(int x) {
        int ans = 0;
        node *crawl = root;
        for (int i = N - 1; i >= 0; i--) {
            if ((x >> i) & 1) {
                if (crawl->l) {
                    ans |= (1 << i);
                    crawl = crawl->l;
                }
                else crawl = crawl->r;
            }
            else {
                if (crawl->r) {
                    ans |= (1 << i);
                    crawl = crawl->r;
                }
                else crawl = crawl->l;
            }
        }
        return ans;
    }
};
