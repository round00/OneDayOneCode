#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <new>
#include <map>
#include <string>
using namespace std;

//struct TreeNode {
//    int val;
//    TreeNode *left;
//    TreeNode *right;
//    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
//};
//
//TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
//    if(!root || root==p ||root==q){
//        return root;
//    }
//
//    auto l = lowestCommonAncestor(root->left, p, q);
//    auto r = lowestCommonAncestor(root->right, p, q);
//
//    if(l && r)return root;
//    return l?l:r;
//}

class Solution1 {
public:
    static bool cmp(int a,int b){
        string aa = to_string(a),bb = to_string(b);
        string sa = aa+bb, sb = bb+aa;
        if(sa<sb)return false;
        else return true;
    }
    string largestNumber(vector<int>& nums) {
        sort(nums.begin(), nums.end(), cmp);
        string ans;
        for(auto x:nums){
            ans += to_string(x);
        }
        if(ans[0]=='0')return "0";
        return ans;
    }
};
int main() {

//    vector<int> v;
//    for(int i = 0;i<100; ++i)v.push_back(0);
    Solution1 s;
    s.cmp(30, 34);
//    s.largestNumber(v);
    cout<<123<<endl;
    return 0;
}