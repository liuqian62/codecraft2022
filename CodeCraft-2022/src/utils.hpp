#ifndef __UTILS_H__
#define __UTILS_H__
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
struct Deman
{
    string stream_id;
    int stream;
};
class mycompare
{
private:
    /* data */
public:
   
    bool operator()(Deman dem1,Deman dem2){
        return dem1.stream>dem2.stream;
    }
   
};

class mycompare_inverse
{
private:
    /* data */
public:
   
    bool operator()(Deman dem1,Deman dem2){
        return dem1.stream<dem2.stream;
    }
   
};


struct Demans
{
    vector<Deman> demans;

    int total(){
        int res=0;
        for(uint i=0;i<demans.size();i++){
            res+=demans[i].stream;
        }
        return res;
    }

    void mysort(){
        sort(demans.begin(),demans.end(),mycompare());
    }

    void mysort_inverse(){
        sort(demans.begin(),demans.end(),mycompare_inverse());
    }
};


#endif