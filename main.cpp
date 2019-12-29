#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <queue>
#include <time.h>
#include <wchar.h>
#include <locale.h>
#include <unordered_map>
using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        unordered_map<string, vector<string>> mp;
        for(auto s:strs){
            string ss = s;
            sort(ss.begin(), ss.end());
            mp[ss].push_back(s);
        }
        vector<vector<string>> ans;
        for(const auto& item:mp){
            ans.push_back(item.second);
        }
        return ans;
    }
};
int main() {
    vector<int> v = {1,3,3};
    Solution s;

    return 0;
}