#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <queue>
using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
public:
    int kthSmallest(vector<vector<int>>& matrix, int k) {
        auto cmp = [&](pair<int,int> a, pair<int,int> b){
            return matrix[a.first][a.second] <matrix[b.first][b.second];
        };
        priority_queue<pair<int,int>, vector<pair<int,int>>, decltype(cmp)> pq(cmp);
        int n = matrix.size();
        for(int i = 0;i<n;++i)pq.push(make_pair(0, i));

        for(int i = 0;i<k-1;++i){
            auto t = pq.top();
            pq.pop();
            if(t.first<n-1)pq.push(make_pair(t.first+1, t.second));
        }
        return matrix[pq.top().first][pq.top().second];
    }
};
int f(int, float);
int main() {
    cout<<is_same<decltype(f), int(int,int)>::value<<endl;
    return 0;
}