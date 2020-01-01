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

int main() {
    vector<int> v = {2,1,5,6,2,3};
    Solution s;

    return 0;
}