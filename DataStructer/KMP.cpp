//
// Created by gjk on 2020/3/5.
//
#include <vector>
#include <string>
using namespace std;

//在s中找到p第一次出现的位置，使用KMP算法，时间复杂度O(m+n)
//失败返回-1
int strStr(const string& s, const string& p){
    int m = s.length(), n = p.length();
    if(!m || !n)return -1;
    //next数组
    vector<unsigned char> next(n, 0);
    //求next数组
    for(int i = 1, j = 0; i<n;){
        if(p[i] == p[j])next[i++] = ++j;    //相等时，两个都向前走一格
        else if(j>0)j = next[j-1];          //不相等了，就利用j之前的next数组
        else i++;                           //j等于0了，那么next[j]也是0了，把i向前移
    }

    //进行匹配
    for(int i = 0, j = 0; i<m; ){
        if(s[i] == p[j])++i, ++j;   //相等则双指针向前
        else if(j>0)j = next[j-1];  //不相等，当时还有next数组可以用，j移到前面去
        else i++;                   //不相等且j=0，则没有next数组可以用了，只能把主串指针往前移了

        if(j==n){
            return i-j;
        }
    }
    return -1;
}


int main(){
    printf("%d\n", strStr("hello", "ll"));  //2
    return 0;
}