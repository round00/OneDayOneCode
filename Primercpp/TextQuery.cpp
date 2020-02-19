//
// Created by gjk on 2020/1/8.
//

#include "TextQuery.h"
#include <sstream>
#include <iostream>
TextQuery::TextQuery(ifstream &in):m_lines(new vector<string>) {
    string s;
    while(getline(in, s)){
        m_lines->push_back(s);
        int n = m_lines->size()-1;
        istringstream is(s);
        string word;
        while(is>>word){
            auto it = m_mpWordLines.find(word);
            if(it == m_mpWordLines.end())
                m_mpWordLines[word] = make_shared<set<int>>();
            m_mpWordLines[word]->insert(n);
        }
    }
}

QueryResult TextQuery::query(const string& s) {
    auto it = m_mpWordLines.find(s);
    if(it == m_mpWordLines.end()){
        return QueryResult(s, make_shared<set<int>>(), m_lines);
    }

    return QueryResult(it->first, it->second, m_lines);
}

ostream& print(ostream& out, const QueryResult& qr){
    out<<qr.m_queryWord<<" occurs "<<qr.m_linenos->size()<<" times"<<endl;
    for(auto nos : *qr.m_linenos){
        out<<"(line: "<< nos+1 <<") "<<(*qr.m_lines)[nos]<<endl;
    }
    return out;
}

void runQuery(const string& file)
{
    ifstream ifs(file);
    TextQuery tq(ifs);
    while(1){
        cout<<"Enter a word to query: ";
        string s;
        if(!(cin>>s) || s=="q")break;
        print(cout, tq.query(s));
        cout<<endl;
    }
}
