// #include "Site_table.h"
#include "Site.h"
#include "utils.hpp"
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstring>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdlib>
#include <ctime>
#include <math.h>
// #include <istream>
// #include <streambuf>
// #include <sstream>
class Distributor
{
private:
    string _Input_demand = "/data/demand.csv";
    string _Input_bandwidth = "/data/site_bandwidth.csv";
    string _Input_qos = "/data/qos.csv";
    string _Input_config = "/data/config.ini";
    string _Output = "/output/solution.txt";

    int qos_constraint;
    int base_cost;
    bool is_first_time;
    double average_demand;
    int max_demand = 0;
    int expected_cost;
    unordered_map<string, int> site_map;
    float xishu = 1.0;
    int best_covered_site_id0;
    int best_covered_site_id1;

    ofstream ofs;
    vector<Site> sites;
    vector<string> more_ten_sites;
    vector<vector<int>> table;
    vector<vector<int>> var_qoss;
    vector<vector<bool>> status;
    vector<string> customer_ids;
    vector<string> site_names;
    vector<string> times;
    vector<vector<Demans>> demands;
    vector<vector<Demans>> distirbute_table;
    vector<vector<vector<Demans>>> all_distirbute_table;
    vector<int> eveytime_demand;
    vector<int> site_average;
    vector<int> custormer_num_of_sites;
    // vector<int> sort_custormer_num_of_sites_id;
    vector<int> sort_site_id;
    vector<int> free_cost;
    vector<int> sort_free_id;
    vector<int> sort_demand_id;
    vector<vector<int>> every_time_full_id;
    vector<int> full_used_time;

public:
    /**
     * @brief Construct a new Distributor object
     *
     * @param Input_demand path to demand.csv
     * @param Input_bandwidth path to bandwidth.csv
     * @param Input_qos path to qos.csv
     * @param Input_config path to config.ini
     * @param Output path to save solution.txt
     */
    Distributor(string Input_demand,
                string Input_bandwidth,
                string Input_qos,
                string Input_config,
                string Output);
    /**
     * @brief read data
     *
     */
    void read_data();

    /**
     * @brief read config file
     *
     */
    void read_config();

    /**
     * @brief read site bandwidth
     *
     */
    void read_bandwidth();

    void BubbleSort(vector<int> &p, int length, int *ind_diff);

    /**
     * @brief read qos
     *
     */
    void read_qos();

    /**
     * @brief read demand file
     *
     */
    void read_demand();

    /**
     * @brief include distribute, check and output
     *
     */
    void distribute();

    /**
     * @brief to distribute with less cost
     *
     */
    void do_distribute(int dis);

    void do_distribute_average(int dis);
    void do_distribute_weight(int dis);
    void do_distribute_softmax(int dis);
    void do_distribute_minsite(int dis);
    void do_distribute_lesscost(int dis);

    void init_distribute_table();
    void get_information_demand();
    /**
     * @brief check is there any customer site do not have enough bandwidth
     * if there is some customer sites do not have enough bandwidth, try to repair
     *
     */
    void final_check(int dis);

    /**
     * @brief do optimize to get less cost
     *
     */
    void optimize(int dis);

    void optimaize_by_id(int dis);

    /**
     * @brief to make the distribute table be average between site[max_id]
     * and site[min_id]
     *
     * @param max_id the id of site which has more cost add
     * @param min_id the id of site which has less cost add
     * @param k the id of customer suport change
     * @param cost_add the vector which record the cost add of each site
     */

    void move_stream_to_full_site(int best_site_id,int tmp_sort[],vector<int> &cost_add);

    void do_average_line(int max_id, int min_id, vector<int> &cost_add);

    void do_average(int max_id, int min_id, int k, vector<int> &cost_add);


    /**
     * @brief output the result as result.txt to the right directory
     *
     */

    void softmax(vector<double> x);

    /**
     * @brief output the result as result.txt to the right directory
     *
     */
    void output(int dis);

    /**
     * @brief Destroy the Distributor object
     *
     */
    ~Distributor();
};