// @aliases bst_BSTIterator
// @fragment
class BSTIterator {
private:
    stack<TreeNode*> s;
    bool reverse = false;
    void pushAll(TreeNode *node) {
        TreeNode *temp = node;
        while (temp) {
            s.push(temp);
            temp = (reverse ? temp->rhs : temp->lhs);
        }
    }
public:
    BSTIterator(TreeNode* root, bool reverse) {
        this->reverse = reverse;
        pushAll(root);
    }
    
    int next() {
        TreeNode *temp = s.top();
        s.pop();
        pushAll((reverse ? temp->lhs : temp->rhs));
        return temp->val;
    }
    
    bool hasNext() {
        return !s.empty();
    }
};
