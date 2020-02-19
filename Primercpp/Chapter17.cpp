//
// Created by gjk on 2020/1/29.
//

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <bitset>
#include <tuple>
#include <random>
using namespace std;

tuple<int,int,int> fun(int a, int b, int c){
    return make_tuple(a,b,c);
}
void testTuple()
{
    //定义tuple
    std::tuple<int,int,int> tp(1,2,3);
    std::cout<<std::get<2>(tp)<<std::endl;
    tuple<string, vector<int>, double, list<float>> t4 =
            {"string", {1,2,3,4}, 3.1415, {1.2,2.3,3.4}};
    auto t = make_tuple("Hello World", 123, 22.3, 'a');
    //获取tuple的值
    cout<<get<0>(t)<<" "<<get<1>(t)<<" "<<get<2>(t)<<endl;
    //获取tuple的大小
    cout<<tuple_size<decltype(t4)>::value<<endl;
    //获取tuple中某个元素的类型
    tuple_element<1, decltype(t)>::type cnt = get<1>(t);
    auto cnt1 = get<1>(t);
    //比较两个tuple，两个tuple元素数量必须相等,并且对每对对应的元素使用比较运算符必须是合法的。
//    if(t4==t)cout<<"same"<<endl;
//    else cout<<"diff"<<endl;

    auto tt = fun(9, 8, 7);
}

void testBitset()
{
    bitset<32> b1(1U);
    bitset<32> b2(0xbeef);
    bitset<128> b3(~0ULL);
    bitset<32> b4("0000");
    string s = "111111111111111000000000";
    //从第三个字符开始，10个字符用于初始化，0用字符'0'表示，1用字符'1'表示
    bitset<33> b5(s, 3, 10, '0', '1');

    //是否存在1
    cout<<b4.any()<<endl;
    cout<<b5.any()<<endl;
    cout<<b5.count()<<endl;

    auto x = b5.to_ullong();
    cout<<x<<endl;
    cout<<b5.size()<<endl;
    //操作
    b5.flip(0); //反转
    cout<<b5<<endl;
    b5.set(0, 1);
    cout<<b5<<endl;
    b5[0] = 0;
    cout<<b5<<endl;
    ~b5[0];
    cout<<b5<<endl;
    //从输入读取bitset
    bitset<7> b6;
    cin >> b6;
    cout<<b6<<endl;
}

//C++应该使用STL的random。而不是rand()
//rand()生成的是均匀分布的伪随机数
//random可以指定随机数服从的分布
void testRandom()
{
    default_random_engine e(1024);
    e.seed(2048);   // 重置种子
    for(int i = 0;i<5;++i)cout<<e()<<" ";
    cout<<endl;
    cout<<e.min()<<" "<<e.max()<<endl;
    //0-9的均匀分布
    uniform_int_distribution<unsigned> u(0,9);
    for(int i = 0;i<5; ++i)cout<<u(e)<<" ";
    cout<<endl;
    uniform_real_distribution<double> ur(0,1);
//    uniform_real_distribution<> ur1(0,1); //使用默认类型
    for(int i = 0;i<5; ++i)cout<<ur(e)<<" ";
    cout<<endl;
    //正态分布
    normal_distribution<> nd(4, 1.5);
    //伯努利分布，这个不是模板，只会返回true/false 概率各0.5
    bernoulli_distribution bd;
}

int main(){


    cout<<boolalpha;    //bool
    cout<<oct;          //改进制
    cout<<hex;
    cout<<showbase;
    cout<<dec;
    cout<<noshowbase;   //
    cout.precision();   //设置或者获取当前浮点数精度
    cout<<true<<endl;
    cout<<false<<endl;


    return 0;
}