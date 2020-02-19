//
// Created by gjk on 2020/1/8.
//

#ifndef ONEDAYONECODE_TEXTQUERY_H
#define ONEDAYONECODE_TEXTQUERY_H

#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <memory>
using namespace std;

class QueryResult;

class TextQuery{
public:
    using lineno = vector<string>::size_type ;
    explicit TextQuery(ifstream& in);
    QueryResult query(const string& s);
private:
    shared_ptr<vector<string>>  m_lines;
    map<string, shared_ptr<set<int>>> m_mpWordLines;
};


class QueryResult{
public:
    QueryResult(const string& s,
                shared_ptr<set<int>> linenos,
                shared_ptr<vector<string>> lines)
        :m_queryWord(s), m_linenos(linenos), m_lines(lines){}

private:
    friend ostream& print(ostream& out, const QueryResult& qr);

    string      m_queryWord;
    shared_ptr<set<int>>    m_linenos;
    shared_ptr<vector<string>>  m_lines;
};

void runQuery(const string& file);
#endif //ONEDAYONECODE_TEXTQUERY_H
