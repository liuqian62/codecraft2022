#ifndef __SITE_H__
#define __SITE_H__

#include <iostream>
#include <string>
#include <vector>
// #include <algorithm>
using namespace std;

class Site
{
private:


public:
    string _name;
    int _max_bandwidth;
    int _available_bandwidth;
    int _full_distribute;
    int _num_distribute;
    bool full_dised;
    bool have_chance=true;
    bool high_value;
    vector<int> cost;
    int base_cost;
    int new_base_cost;
    int overflow_time;
    bool one_of_ten_sites;

    Site(string name, int max_bandwidth);
    int available();
    bool get(int width);
    void final_get(int width);
    bool full_able();
    int used();
    ~Site();
};

#endif