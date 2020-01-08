#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <queue>
#include <time.h>
#include <wchar.h>
#include <locale.h>
#include <unordered_map>
#include <memory>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};
class Solution {
public:
    int last;

};
namespace std{
    template <>
    void swap<Solution>(Solution& a, Solution&b)noexcept {

    }
}
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    if(!root || root==p ||root==q){
        return root;
    }

    auto l = lowestCommonAncestor(root->left, p, q);
    auto r = lowestCommonAncestor(root->right, p, q);

    if(l && r)return root;
    return l?l:r;
}
int main() {
    TreeNode* rt = new TreeNode(3);
    TreeNode* l = new TreeNode(5);
    TreeNode* r = new TreeNode(1);
    rt->left = l;
    rt->right = r;
    auto ret = lowestCommonAncestor(rt, l, r);


    return 0;
}