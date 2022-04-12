#include "Distributor.h"

Distributor::Distributor(string Input_demand,
                         string Input_bandwidth,
                         string Input_qos,
                         string Input_config,
                         string Output)
{
    this->_Input_demand = Input_demand;
    this->_Input_bandwidth = Input_bandwidth;
    this->_Input_qos = Input_qos;
    this->_Input_config = Input_config;
    this->_Output = Output;
    this->is_first_time = true;
}
void Distributor::read_data()
{
    read_config();
    read_bandwidth();
    read_qos();
    read_demand();
}

void Distributor::read_config()
{
    // vector<vector<int>> test_size;
    // for(int i=0;i<10;i++){
    //     vector<int>tep;
    //     test_size.push_back(tep);

    // }
    //         cout<<"size:"<<endl;
    //     cout<<test_size.size()<<endl;
    //     cout<<test_size[0].size()<<endl;

    ifstream ifs;
    ifs.open(_Input_config, ios::in);

    if (!ifs.is_open())
    {
        cout << "failed to open " << _Input_config << endl;
        return;
    }

    string buf;
    getline(ifs, buf);
    getline(ifs, buf);
    char *r = nullptr;

    r = strtok((char *)buf.data(), "=");
    r = strtok(NULL, "=");
    this->qos_constraint = atoi(r);

    cout << "qos_constraint " << qos_constraint << endl;
    while (getline(ifs, buf))
    {

        r = strtok((char *)buf.data(), "=");
        r = strtok(NULL, "=");
        this->base_cost = atoi(r);

        cout << "base_cost " << base_cost << endl;
    }
    ifs.close();
}
void Distributor::read_bandwidth()
{
    ifstream ifs;
    ifs.open(_Input_bandwidth, ios::in);

    if (!ifs.is_open())
    {
        cout << "failed to open " << _Input_bandwidth << endl;
        return;
    }

    string buf;
    getline(ifs, buf);
    while (getline(ifs, buf))
    {

        char *r = nullptr;

        r = strtok((char *)buf.data(), ",");
        string site_name = r;
        r = strtok(NULL, ",");
        int band = atoi(r);
        site_map.emplace(site_name, band);
        // cout<<"!!"<<endl;
        // Site site(site_name, band);
        // sites.push_back(site);
    }
    // for (int i = 0; i < sites.size(); i++)
    // {
    //     cout << "边缘节点 name " << sites[i]._name << endl;
    //     cout << "边缘节点 可用带宽 " << sites[i].available() << endl;
    // }
    ifs.close();
}
void Distributor::read_qos()
{
    ifstream ifs;
    ifs.open(_Input_qos, ios::in);

    if (!ifs.is_open())
    {
        cout << "failed to open " << _Input_qos << endl;
        return;
    }

    string buf;
    char *r = nullptr;
    getline(ifs, buf);

    r = strtok((char *)buf.data(), ",");

    r = strtok(NULL, ",");
    while (r != NULL)
    {
        string customer_id = r;
        // if(customer_id.find("\r\n")!=-1){
        //     customer_id = strtok((char *)customer_id.data(), "\r\n");
        //     cout<<"!!!"<<endl;
        // }

        customer_ids.push_back(customer_id);
        r = strtok(NULL, ",");
    }

    customer_ids[customer_ids.size() - 1].erase(customer_ids[customer_ids.size() - 1].size() - 1);

    while (getline(ifs, buf))
    {

        r = strtok((char *)buf.data(), ",");
        string site_name = r;
        site_names.push_back(site_name);
        r = strtok(NULL, ",");
        vector<int> rows;
        vector<bool> statu;
        int cus_num = 0;
        while (r != NULL)
        {
            int qos = atoi(r);
            if (qos < qos_constraint)
            {
                // rows.push_back(sites[site_names.size()]._max_bandwidth);
                rows.push_back(site_map[site_name]);
                statu.push_back(true);
                cus_num++;
            }
            else
            {
                rows.push_back(0);
                statu.push_back(false);
            }
            r = strtok(NULL, ",");
        }
        table.push_back(rows);
        status.push_back(statu);
        custormer_num_of_sites.push_back(cus_num);
        if (cus_num != 0)
        {
            // free_cost.push_back(sites[status.size() - 1]._available_bandwidth);
            free_cost.push_back(site_map[site_name]);
        }
        else
        {
            free_cost.push_back(0);
        }
    }
    for (uint i = 0; i < site_names.size(); i++)
    {
        Site site(site_names[i], site_map[site_names[i]]);
        sites.push_back(site);
        // cout<<site_names[i]<<site_map[site_names[i]]<<endl;
    }

    int tep_sort_free_id[free_cost.size()] = {0};
    vector<int> tep_free_cost(free_cost.begin(), free_cost.end());
    BubbleSort(tep_free_cost, free_cost.size(), tep_sort_free_id);
    //    for (int i = 0; i < free_cost.size(); i++)
    // {
    //     cout << "value: " << free_cost[i] << "   Index: " << tep_sort_free_id[i] << endl;

    // }
    for (uint i = 0; i < free_cost.size(); i++)
    {
        sort_free_id.push_back(tep_sort_free_id[free_cost.size() - i - 1]);
    }
    //        for (int i = 0; i < free_cost.size(); i++)
    // {
    //     cout << "value: " << free_cost[sort_free_id[i]] << "   Index: " << sort_free_id[i] << endl;

    // }

    // for (int i = 0; i < customer_ids.size(); i++)
    // {
    //     cout << "客户节点id " << customer_ids[i] << endl;
    // }

    // for (int i = 0; i < site_names.size(); i++)
    // {
    //     cout << "边缘节点id " << site_names[i] << endl;
    // }
    // for (int i = 0; i < table.size(); i++)
    // {
    //     cout << sites[i]._name << ":";
    //     // cout << "table " << ",";
    //     for (int j = 0; j < table[0].size(); j++)
    //     {
    //         cout << table[i][j] << ",";
    //     }
    //     cout << endl;
    // }
    // for (int i = 0; i < status.size(); i++)
    // {
    //     cout << "状态 "
    //          << ":";
    //     for (int j = 0; j < status[0].size(); j++)
    //     {
    //         cout << status[i][j] << ",";
    //     }
    //     cout << endl;
    // }
    ifs.close();
    for (uint i = 0; i <= customer_ids.size(); i++)
    {
        for (uint j = 0; j < site_names.size(); j++)
        {
            if (custormer_num_of_sites[j] == int(customer_ids.size() - i))
            {
                sort_site_id.push_back(j);
            }
        }
    }
    //     for (int i=0;i<sort_site_id.size();i++){
    //     cout << sort_site_id[i]<<":"<<custormer_num_of_sites[sort_site_id[i]] << endl;

    // }
}

void Distributor::read_demand()
{
    ifstream ifs;
    ifs.open(_Input_demand, ios::in);

    if (!ifs.is_open())
    {
        cout << "failed to open " << _Input_demand << endl;
        return;
    }

    string buf;
    char *r = nullptr;
    getline(ifs, buf);
    // getline(ifs, buf);

    r = strtok((char *)buf.data(), ",");

    r = strtok(NULL, ",");
    r = strtok(NULL, ",");
    vector<string> tem_cus_id;
    while (r != NULL)
    {
        string customer_id = r;

        tem_cus_id.push_back(customer_id);
        r = strtok(NULL, ",");
    }

    tem_cus_id[tem_cus_id.size() - 1].erase(tem_cus_id[tem_cus_id.size() - 1].size() - 1);
    unordered_map<string, int> unomap;
    for (uint i = 0; i < tem_cus_id.size(); i++)
    {
        unomap.emplace(tem_cus_id[i], i);
    }
    string tep_time_stamp;
    vector<Demans> rows;
    vector<Demans> new_rows;

    while (getline(ifs, buf))
    {

        r = strtok((char *)buf.data(), ",");
        string time_name = r;

        if (tep_time_stamp == time_name)
        {
            r = strtok(NULL, ",");
            string stream_id = r;

            r = strtok(NULL, ",");
            int i = 0;
            while (r != NULL)
            {

                int stream = atoi(r);
                Deman demand;
                demand.stream_id = stream_id;
                demand.stream = stream;
                rows[i].demans.push_back(demand);

                r = strtok(NULL, ",");
                i++;
            }
        }
        else
        {
            if (tep_time_stamp != "")
            {
                for (uint i = 0; i < rows.size(); i++)
                {

                    new_rows.push_back(rows[unomap[customer_ids[i]]]);
                }
                demands.push_back(new_rows);
            }
            rows.resize(0);
            new_rows.resize(0);
            tep_time_stamp = time_name;

            times.push_back(time_name);
            r = strtok(NULL, ",");
            string stream_id = r;

            r = strtok(NULL, ",");

            int i = 0;

            while (r != NULL)
            {
                Demans tmp;
                rows.push_back(tmp);

                int stream = atoi(r);

                Deman demand;
                demand.stream_id = stream_id;
                demand.stream = stream;
                rows[i].demans.push_back(demand);

                r = strtok(NULL, ",");
                i++;
            }
        }
    }

    for (uint i = 0; i < rows.size(); i++)
    {

        new_rows.push_back(rows[unomap[customer_ids[i]]]);
        // cout<<unomap[customer_ids[i]]<<endl;
    }
    demands.push_back(new_rows);
    // demands[0][9].mysort();
    // for(int i=0;i<demands[0][9].demans.size();i++){
    //     cout<<demands[0][9].demans[i].stream<<endl;
    // }

    // for (int i = 0; i < 1; i++)
    // {
    //     // cout << times[i];
    //     for (int j = 0; j < demands[0].size(); j++)
    //     {
    //         cout << demands[i][j].demans[0].stream << ",";
    //     }
    //     cout << endl;
    // }
    // cout << "demands_size:" << demands.size() << endl;
    // cout << "times_size:" << times.size() << endl;

    ifs.close();
    get_information_demand();
}

void Distributor::final_check(int dis)
{
    // cout<<"check"<<endl;
    vector<int> sites_used;
    vector<int> customers_get;
    vector<int> sites_available;
    vector<int> customer_nums; // the number of customer every site can distribute
    for (uint i = 0; i < site_names.size(); i++)
    {
        int tem_used = 0;
        int tem_num = 0;
        for (uint j = 0; j < customer_ids.size(); j++)
        {
            // int tem_used =0;
            for (uint k = 0; k < distirbute_table[i][j].demans.size(); k++)
            {
                tem_used += distirbute_table[i][j].demans[k].stream;
            }

            if (status[i][j])
            {
                tem_num++;
            }
        }
        sites_used.push_back(tem_used);
        sites_available.push_back(sites[i]._max_bandwidth - tem_used);
        customer_nums.push_back(tem_num);
    }
    for (uint i = 0; i < site_names.size(); i++)
    {
        if (sites_available[i] < 0)
        {
            cout << "!!!!!" << endl;
        }
    }
    // //  cout<<"check"<<endl;
    // for (int i = 0; i < customer_ids.size(); i++)
    // {
    //     int tem_get = 0;
    //     for (int j = 0; j < site_names.size(); j++)
    //     {
    //         // int tem_used =0;
    //         tem_get += distirbute_table[j][i];
    //     }
    //     customers_get.push_back(tem_get);
    // }
    // //  cout<<"check"<<endl;
    // for (int i = 0; i < customer_ids.size(); i++)
    // {

    //     if (demands[dis][i] != customers_get[i])
    //     {
    //         cout << "distribute false!!!" << endl;
    //         cout << "denmand:" << demands[dis][i] << endl;
    //         cout << "get:" << customers_get[i] << endl;
    //     }
    //     else
    //     {
    //         // cout<<"sucsess"<<endl;
    //     }
    // }
    bool flag = false;
    for (uint i = 0; i < site_names.size(); i++)
    {
        // if (sites_available[i] < 0)
        // {
        //     int max_id = 0, max_num = 0, cus_id = 0;
        //     cout << i << sites_available[i] << endl;
        //     for (uint j = 0; j < customer_ids.size(); j++)
        //     {
        //         if (status[i][j])
        //         {
        //             for (uint k = 0; k < site_names.size(); k++)
        //             {

        //                 if (status[k][j] && sites_available[k] > 0)
        //                 {
        //                     if (max_num < customer_nums[k])
        //                     {
        //                         max_id = k;
        //                         cus_id = j;
        //                         max_num = customer_nums[k];
        //                     }

        //                     // distirbute_table[k][j] += sites_available[k];
        //                     // distirbute_table[i][j] -= sites_available[k];
        //                     // sites_available[i] += sites_available[k];
        //                     // sites_available[k] = 0;
        //                 }
        //                 if (sites_available[i] >= 0)
        //                 {
        //                     break;
        //                 }
        //             }
        //         }
        //         if (sites_available[i] >= 0)
        //         {
        //             break;
        //         }
        //     }
        //     if (sites_available[i] < 0)
        //     {
        //         // flag = true;
        //         // distirbute_table[max_id][cus_id] -= sites_available[i];
        //         // distirbute_table[i][cus_id] += sites_available[i];
        //         // sites_available[max_id] += sites_available[i];
        //         // sites_available[i] = 0;
        //     }
        // }
        // else
        // {
        //     // cout<<sites_available[i]<<endl;
        // }
    }
    if (flag)
    {
        final_check(dis);
    }
}

void Distributor::optimaize_by_id(int dis)
{

    vector<int> site_used;
    vector<int> cost_add;
    for (uint i = 0; i < site_names.size(); i++)
    {
        int tem_used = 0;

        for (uint j = 0; j < customer_ids.size(); j++)
        {

            // int tem_used =0;
            tem_used += distirbute_table[i][j].total();
        }
        site_used.push_back(tem_used);

        cost_add.push_back(0);
    }

    for (uint i = 0; i < site_names.size(); i++)
    {
        if (!sites[i].full_dised)
        {
            if (custormer_num_of_sites[i] == 0)
            {
                // cost_add.push_back(0);
                cost_add[i] = 0;
                continue;
            }
            if (site_average[i] < 0)
            {
                cost_add[i] = site_used[i] - base_cost;
            }
            else
            {
                cost_add[i] = site_used[i] - base_cost - site_average[i];
            }
            // cost_add[i] = site_used[i] - base_cost - site_average[i];
            // cost_add[i] = site_used[i] - 800;
            // cost_add[i] = site_used[i] - 1.0 * site_average[i];
            // cout<<site_average[i]<<endl;

            // cost_add[i] = site_used[i] - 1500;
            // cost_add[i] = site_used[i] -average_demand/site_names.size();
        }
    }

    vector<int> tep_cost_add(cost_add.begin(), cost_add.end());
    int tep_sort_cost_add_id[cost_add.size()] = {0};
    BubbleSort(tep_cost_add, cost_add.size(), tep_sort_cost_add_id);
    int size = every_time_full_id[dis].size();
    // for (int i = 0; i < every_time_full_id[dis].size(); i++)
    for (int i = 0; i < size; i++)
    {
        // cout<<every_time_full_id[dis].size()<<endl;

        int id = every_time_full_id[dis][i];

        // cost_add[id] = site_used[id] - sites[id]._max_bandwidth;

        // sites[id]._full_distribute++;
        // sites[id].full_dised = true;
        int best_site_id = id;
        cost_add[best_site_id] = -sites[best_site_id]._available_bandwidth;

        sites[best_site_id]._full_distribute++;
        // cout << "_full_distribute" << sites[best_site_id]._full_distribute << endl;
        sites[best_site_id].full_dised = true;
        move_stream_to_full_site(best_site_id, tep_sort_cost_add_id, cost_add);
    }

    int num = 5;
    while (num > 0)
    // while (false)
    {
        num--;
        vector<int> tep_cost_add(cost_add.begin(), cost_add.end());
        int tep_sort_cost_add_id[cost_add.size()] = {0};
        BubbleSort(tep_cost_add, cost_add.size(), tep_sort_cost_add_id);

        for (uint i = 0; i < cost_add.size(); i++)
        {
            int max_id = tep_sort_cost_add_id[cost_add.size() - 1 - i];

            if (cost_add[max_id] <= 0)
            {
                break;
            }
            for (uint j = 0; j < cost_add.size(); j++)
            {
                int min_id = tep_sort_cost_add_id[j];
                if (cost_add[max_id] <= cost_add[min_id])
                {
                    continue;
                }
                if (custormer_num_of_sites[min_id] != 0)
                // if(true)
                {
                    for (uint k = 0; k < customer_ids.size(); k++)
                    {
                        // if (cost_add[max_id] <= cost_add[min_id])
                        if (cost_add[max_id] <= 0)
                        {
                            break;
                        }
                        if (distirbute_table[max_id][k].total() == 0)
                        {
                            continue;
                        }
                        if (status[max_id][k] && status[min_id][k])
                        {
                            do_average(max_id, min_id, k, cost_add);
                        }
                    }
                }
            }
        }
    }
}

void Distributor::optimize(int dis)
{

    // vector<int> site_used;
    vector<int> cost_add;
    for (uint i = 0; i < site_names.size(); i++)
    {
        // int tem_used = 0;

        // for (uint j = 0; j < customer_ids.size(); j++)
        // {

        //     // int tem_used =0;
        //     tem_used += distirbute_table[i][j].total();
        // }
        // site_used.push_back(tem_used);

        cost_add.push_back(0);
    }

    // change the cost_add of some full distribute sites

    // int size = every_time_full_id[dis].size();
    // for (int i = 0; i < every_time_full_id[dis].size(); i++)
    // for (int i = 0; i < size; i++)
    // {
    //     // cout<<every_time_full_id[dis].size()<<endl;

    //     int id = every_time_full_id[dis][i];

    //     cost_add[id] = site_used[id] - sites[id]._max_bandwidth;

    //     // sites[id]._full_distribute++;
    //     sites[id].full_dised = true;
    // }

    for (uint i = 0; i < site_names.size(); i++)
    {
        if (!sites[i].full_dised)
        {
            if (custormer_num_of_sites[i] == 0)
            {
                // cost_add.push_back(0);
                cost_add[i] = 0;
                continue;
            }
            if (site_average[i] < 0)
            {
                // cost_add[i] = site_used[i] - base_cost;
                cost_add[i] = sites[i]._max_bandwidth - sites[i]._available_bandwidth - base_cost;
            }
            else
            {
                // cost_add[i] = site_used[i] - base_cost - site_average[i];
                // cost_add[i] = sites[i]._max_bandwidth - sites[i]._available_bandwidth - base_cost - site_average[i];
                if (sites[i].base_cost < sites[i].new_base_cost && sites[i].overflow_time<times.size() / 30)
                // if (true)
                // if(false)
                {
                    cost_add[i] = sites[i]._max_bandwidth - sites[i]._available_bandwidth - sites[i].new_base_cost;
                }
                else
                {
                    cost_add[i] = sites[i]._max_bandwidth - sites[i]._available_bandwidth - base_cost - site_average[i];
                }
            }
            // cost_add[i] = site_used[i] - base_cost - site_average[i];
            // cost_add[i] = site_used[i] - 800;
            // cost_add[i] = site_used[i] - 1.0 * site_average[i];
            // cout<<site_average[i]<<endl;

            // cost_add[i] = site_used[i] - 1500;
            // cost_add[i] = site_used[i] -average_demand/site_names.size();
        }
    }

    // int tmp_demand = eveytime_demand[dis];
    // while (tmp_demand > 1.1 * expected_cost)
    while (true)
    {
        int tmp_op_num = 4;
        // cout<<dis<<endl;
        while (tmp_op_num > 0)
        // while (false)
        {
            tmp_op_num--;
            vector<int> tep_cost_add(cost_add.begin(), cost_add.end());
            int tep_sort_cost_add_id[cost_add.size()] = {0};
            BubbleSort(tep_cost_add, cost_add.size(), tep_sort_cost_add_id);

            for (uint i = 0; i < cost_add.size(); i++)
            {
                int max_id = tep_sort_cost_add_id[cost_add.size() - 1 - i];

                if (cost_add[max_id] <= 0)
                {
                    break;
                }

                // find the max customer
                //  int max_cus_id=-1;
                //  int max_cus_stream=0;
                vector<int> max_total;
                for (uint tmp = 0; tmp < customer_ids.size(); tmp++)
                {
                    max_total.push_back(distirbute_table[max_id][tmp].total());
                }
                int sort_max_id[customer_ids.size()] = {0};

                BubbleSort(max_total, customer_ids.size(), sort_max_id);
                for (uint tmp_i = 0; tmp_i < customer_ids.size(); tmp_i++)
                {
                    int tmp_id = sort_max_id[customer_ids.size() - tmp_i - 1];
                    // if(distirbute_table[max_id][tmp_id].total()>max_cus_stream){
                    //     max_cus_id=tmp_id;
                    //     max_cus_stream=distirbute_table[max_id][tmp_id].total();
                    // }
                    if (distirbute_table[max_id][tmp_id].total() != 0)
                    {

                        for (uint j = 0; j < cost_add.size(); j++)
                        {
                            int min_id = tep_sort_cost_add_id[j];
                            if (cost_add[max_id] <= cost_add[min_id])
                            {
                                continue;
                            }

                            if (status[max_id][tmp_id] && status[min_id][tmp_id])
                            {
                                do_average(max_id, min_id, tmp_id, cost_add);
                                // do_average_line(max_id, min_id, cost_add);
                                // break;
                            }
                        }
                    }
                }
                // if(max_cus_id==-1){
                //     continue;
                // }
            }
        }
        vector<int> tep_cost_add(cost_add.begin(), cost_add.end());
        int tep_sort_cost_add_id[cost_add.size()] = {0};
        BubbleSort(tep_cost_add, cost_add.size(), tep_sort_cost_add_id);
        int total_cost_add = 0;
        int max_id = tep_sort_cost_add_id[cost_add.size() - 1];
        vector<int> total;
        for (uint i = 0; i < customer_ids.size(); i++)
        {
            total.push_back(0);
        }
        for (uint i = 0; i < site_names.size(); i++)
        // for (uint i = 0; i < 7; i++)
        {

            int max_id_tmp = tep_sort_cost_add_id[cost_add.size() - i - 1];
            if (cost_add[max_id_tmp] > 10)
            {
                total_cost_add += cost_add[max_id_tmp];

                int max_cus_id = -1;
                int max_cus_stream = 0;
                for (uint j = 0; j < customer_ids.size(); j++)
                {
                    // distirbute_table[max_id_tmp][j].mysort();

                    // for (uint m = 0; m < distirbute_table[max_id_tmp][j].demans.size(); m++)
                    // {
                    //     if (distirbute_table[max_id_tmp][j].demans[m].stream > 1.1 * (base_cost + site_average[max_id_tmp]))
                    //     {
                    //         total[j] += distirbute_table[max_id_tmp][j].demans[m].stream;
                    //     }
                    // }

                    total[j] += distirbute_table[max_id_tmp][j].total();
                    if (distirbute_table[max_id_tmp][j].total() > max_cus_stream)
                    {
                        max_cus_id = j;
                        max_cus_stream = distirbute_table[max_id_tmp][j].total();
                    }
                }
                // if (i == 0)
                // if(true)
                // {
                //     total[max_cus_id] += min(cost_add[max_id_tmp], max_cus_stream);
                // }

                // total[max_cus_id] += max_cus_stream;
                // if(i==0&&every_time_full_id[dis].size()>0){
                //     break;
                // }

                // for (auto dem : distirbute_table[max_id])
                // {
                //     total[i] += dem.total();
                //     // cout<<dem.total()<<endl;
                // }
            }
        }
        // if(every_time_full_id[dis].size()<=0){
        //     break;
        // }

        // if (cost_add[max_id] < 2000)
        if (cost_add[max_id] <= 100)
        // if (cost_add[max_id] < 1.3*(base_cost+site_average[max_id]))
        {
            break;
        }

        int best_site_id = -1;
        int best_site_stream = 0;
        double best_rate = 0.0;
        for (uint i_num = 0; i_num < site_names.size(); i_num++)
        {
            int i = sort_site_id[i_num];
            // int i = sort_free_id[i_num];
            int tmp_stream = 0;
            if (!sites[i].full_dised)
            {
                // if (sites[i]._full_distribute < int(times.size() / 20 - 3) || (sites[i].one_of_ten_sites && sites[i]._full_distribute < int(times.size() / 10 - 3)))
                if (sites[i]._full_distribute < int(times.size() / 20 ) )
                {
                    for (uint j = 0; j < customer_ids.size(); j++)
                    {
                        if (status[i][j])
                        {

                            tmp_stream += total[j];
                        }
                    }
                    int can_get_stream = min(tmp_stream, sites[i]._max_bandwidth);

                    if (can_get_stream > best_site_stream)
                    {
                        best_site_id = i;
                        best_site_stream = can_get_stream;
                    }
                    if (best_site_id != -1)
                    {
                        // if (can_get_stream == best_site_stream && custormer_num_of_sites[i] < custormer_num_of_sites[best_site_id])
                        if (can_get_stream > 0.85 * best_site_stream && custormer_num_of_sites[i] < custormer_num_of_sites[best_site_id])
                        {
                            best_site_id = i;
                            best_site_stream = can_get_stream;
                        }
                    }
                }
            }
            // if (!sites[i].full_dised && sites[i]._full_distribute < int(times.size() / 20-3))
            // {
            //     for (uint j = 0; j < customer_ids.size(); j++)
            //     {
            //         if (status[i][j])
            //         {

            //             tmp_stream += total[j];
            //         }
            //     }
            //     int can_get_stream = min(tmp_stream, sites[i]._max_bandwidth);

            //     if (can_get_stream > best_site_stream)
            //     {
            //         best_site_id = i;
            //         best_site_stream = can_get_stream;
            //     }
            //     if (best_site_id != -1)
            //     {
            //         // if (can_get_stream == best_site_stream && custormer_num_of_sites[i] < custormer_num_of_sites[best_site_id])
            //         if (can_get_stream > 0.85 * best_site_stream && custormer_num_of_sites[i] < custormer_num_of_sites[best_site_id])
            //         {
            //             best_site_id = i;
            //             best_site_stream = can_get_stream;
            //         }
            //     }
            //     // if (total_cost_add > 1.5 * sites[i]._available_bandwidth&&total_cost_add > 1.5 *sites[best_site_id]._available_bandwidth)
            //     // if (tmp_stream > 0)

            //     // double rate = 0.0;
            //     // if (tmp_stream >= sites[i]._max_bandwidth)
            //     // {
            //     //     rate = 1.0;
            //     // }
            //     // else
            //     // {
            //     //     rate = tmp_stream / sites[i]._max_bandwidth;
            //     // }

            //     // if (tmp_stream > 0 && best_site_id == -1)
            //     // {
            //     //     best_site_id = i;
            //     //     best_site_stream = tmp_stream;
            //     //     best_rate = rate;
            //     //     // break;
            //     // }
            //     // if (rate > best_rate)
            //     // {
            //     //     best_site_id = i;
            //     //     best_site_stream = tmp_stream;
            //     //     best_rate = rate;
            //     // }
            //     // if (tmp_stream > 0 && rate > 0.5 && custormer_num_of_sites[i] < custormer_num_of_sites[best_site_id])
            //     // // if (tmp_stream > 0 && rate > 0.5)
            //     // {
            //     //     best_site_id = i;
            //     //     best_site_stream = tmp_stream;
            //     //     best_rate = rate;
            //     //     // break;
            //     // }

            //     // if (best_site_id == -1)
            //     // {
            //     //     best_site_id = i;
            //     //     best_site_stream = tmp_stream;
            //     // }
            //     //  if (can_get_stream == best_site_stream && custormer_num_of_sites[i] < custormer_num_of_sites[best_site_id])
            //     // if (custormer_num_of_sites[i] < custormer_num_of_sites[best_site_id])
            //     // {
            //     //     best_site_id = i;
            //     // }
            //     // else if ( sites[i]._available_bandwidth > sites[best_site_id]._available_bandwidth&&tmp_stream>sites[i]._available_bandwidth )
            //     // {
            //     //     best_site_id = i;
            //     //     best_site_stream = tmp_stream;
            //     // }

            //     // best_site_id = i;
            //     // best_site_stream=can_get_stream;

            //     // if (best_site_id != -1 && !sites[i].high_value)
            //     // {
            //     //     continue;
            //     // }

            //     // if (!sites[best_site_id].high_value)
            //     // {
            //     //     best_site_id = i;
            //     //     best_site_stream = can_get_stream;
            //     // }

            //     // if (can_get_stream > best_site_stream)
            //     // {
            //     //     best_site_id = i;
            //     //     best_site_stream = can_get_stream;
            //     // }
            //     // else if (can_get_stream == best_site_stream && custormer_num_of_sites[i] < custormer_num_of_sites[best_site_id])
            //     // {
            //     //     best_site_id = i;
            //     // }

            //     // else
            //     // {
            //     //     if (tmp_stream > best_site_stream)
            //     //     {
            //     //         best_site_id = i;
            //     //         best_site_stream = tmp_stream;
            //     //     }
            //     //     if(best_site_stream!=0&&tmp_stream == best_site_stream&&sites[i]._available_bandwidth< sites[best_site_id]._available_bandwidth){
            //     //         best_site_id = i;
            //     //         best_site_stream = tmp_stream;

            //     //     }
            //     // }

            //     // if (tmp_stream > 0)
            //     // {
            //     //     if(best_site_stream<cost_add[max_id]&&tmp_stream>best_site_stream){
            //     //         best_site_id = i;
            //     //         best_site_stream = tmp_stream;

            //     //     }

            //     //     if (tmp_stream > cost_add[max_id] && best_site_stream > tmp_stream)
            //     //     {
            //     //         best_site_id = i;
            //     //         best_site_stream = tmp_stream;
            //     //     }
            //     //     // if (best_site_stream < tmp_stream && tmp_stream < cost_add[max_id])
            //     //     // {
            //     //     //     best_site_id = i;
            //     //     //     best_site_stream = tmp_stream;
            //     //     // }
            //     // }
            // }
        }
        // cout<<best_site_stream<<endl;

        // int max_cus_id = -1;
        // int max_cus_total = 0;
        // for (int i = 0; i < customer_ids.size(); i++)
        // {
        //     if (distirbute_table[max_id][i].total() > max_cus_total)
        //     {
        //         max_cus_id = i;
        //         max_cus_total = distirbute_table[max_id][i].total();
        //     }
        // }
        // if (max_cus_id == -1)
        // {
        //     cout << "~!!!!!" << endl;
        //     break;
        // }
        // int best_site_id = -1;
        // int best_site_stream = 0;
        // for (int i = 0; i < site_names.size(); i++)
        // {
        //     int tmp_stream = 0;
        //     if (!sites[i].full_dised && sites[i]._full_distribute < times.size() / 20)
        //     {

        //         if (status[i][max_cus_id])
        //         {
        //             // tmp_stream =max_cus_total;
        //             if (best_site_id == -1)
        //             {
        //                 best_site_id = i;
        //             }
        //             else
        //             {
        //                 // if (every_time_full_id[dis].size()>8&&custormer_num_of_sites[best_site_id] > custormer_num_of_sites[i])
        //                 // {
        //                 //     best_site_id = i;
        //                 // }
        //                 if (total_cost_add > 4 * sites[i]._available_bandwidth && custormer_num_of_sites[best_site_id] < custormer_num_of_sites[i])
        //                 {
        //                     best_site_id = i;
        //                 }
        //             }
        //         }

        //         // if (tmp_stream > 0)
        //         // {
        //         //     if(best_site_stream<cost_add[max_id]&&tmp_stream>best_site_stream){
        //         //         best_site_id = i;
        //         //         best_site_stream = tmp_stream;

        //         //     }

        //         //     if (tmp_stream > cost_add[max_id] && best_site_stream > tmp_stream)
        //         //     {
        //         //         best_site_id = i;
        //         //         best_site_stream = tmp_stream;
        //         //     }
        //         //     // if (best_site_stream < tmp_stream && tmp_stream < cost_add[max_id])
        //         //     // {
        //         //     //     best_site_id = i;
        //         //     //     best_site_stream = tmp_stream;
        //         //     // }
        //         // }
        //     }
        // }
        // cout<<best_site_stream<<endl;

        // for (int i = 0; i < site_names.size(); i++)
        // {
        //     if (sites[i].full_dised && sites[i].high_value && sites[i]._available_bandwidth > sites[i]._max_bandwidth * 0.5)
        //     {
        //         sites[i]._full_distribute--;
        //         sites[i].full_dised = false;
        //         sites[i].high_value = false;
        //         cost_add[i] = sites[i]._max_bandwidth - sites[i]._available_bandwidth - base_cost - site_average[i];
        //     }
        // }
        if (best_site_id != -1)
        {
            // cout<<custormer_num_of_sites[best_site_id]<<endl;
            cost_add[best_site_id] = -sites[best_site_id]._available_bandwidth;

            sites[best_site_id]._full_distribute++;
            // cout << "_full_distribute" << sites[best_site_id]._full_distribute << endl;
            sites[best_site_id].full_dised = true;
            move_stream_to_full_site(best_site_id, tep_sort_cost_add_id, cost_add);
            // tmp_demand -= sites[best_site_id]._max_bandwidth * 0.2;
            // cout<<cost_add[best_site_id]<<endl;
        }
        else
        {
            // cout << "can not found" << endl;
            break;
        }
    }

    // if (true)
    // {
    //     for (int i = 0; i < site_names.size(); i++)
    //     {
    //         //    int i=sort_site_id[id];
    //         int run_time = 50;
    //         while (cost_add[i] < 0 && run_time > 0)
    //         {
    //             run_time--;
    //             int max_cost_add = 0;
    //             int max_id = -1;
    //             int can_add = 0;
    //             for (int j = 0; j < site_names.size(); j++)
    //             {

    //                 if (cost_add[j] > max_cost_add)
    //                 {
    //                     for (int m = 0; m < customer_ids.size(); m++)
    //                     {
    //                         if (status[i][m] && status[j][m] && distirbute_table[j][m].total() != 0)
    //                         {
    //                             max_cost_add = cost_add[j];
    //                             max_id = j;
    //                         }
    //                     }
    //                 }
    //             }
    //             if (max_id != -1)
    //             {
    //                 int j = max_id;
    //                 for (int k = 0; k < customer_ids.size(); k++)
    //                 {
    //                     can_add = 0;
    //                     if (status[i][k] && status[j][k])
    //                     {
    //                         do_average(j, i, k, cost_add);
    //                     }
    //                 }
    //             }
    //             if (max_id == -1)
    //             {
    //                 break;
    //             }
    //         }
    //     }
    // }

    int num = 1;
    while (num > 0)
    // while (false)
    {
        num--;
        vector<int> tep_cost_add(cost_add.begin(), cost_add.end());
        int tep_sort_cost_add_id[cost_add.size()] = {0};
        BubbleSort(tep_cost_add, cost_add.size(), tep_sort_cost_add_id);

        for (uint i = 0; i < cost_add.size(); i++)
        {
            int max_id = tep_sort_cost_add_id[cost_add.size() - 1 - i];

            if (cost_add[max_id] <= 0)
            {
                break;
            }
            for (uint j = 0; j < cost_add.size(); j++)
            {
                int min_id = tep_sort_cost_add_id[j];
                if (cost_add[max_id] <= cost_add[min_id])
                {
                    continue;
                }
                if (custormer_num_of_sites[min_id] != 0)
                // if(true)
                {
                    for (uint k = 0; k < customer_ids.size(); k++)
                    {
                        // if (cost_add[max_id] <= cost_add[min_id])
                        // if (cost_add[max_id] <=0)
                        // {
                        //     break;
                        // }
                        if (distirbute_table[max_id][k].total() == 0)
                        {
                            continue;
                        }
                        if (status[max_id][k] && status[min_id][k])
                        {
                            do_average(max_id, min_id, k, cost_add);
                            // do_average_line(max_id,min_id,cost_add);
                            // break;
                        }
                    }
                }
            }
        }
    }

    // sort(cost_add.begin(), cost_add.end());
    // if(cost_add[cost_add.size() - 1]>1000)
    // // if (cost_add[0] < -10000)
    // {
    //     cout << "demand: " << eveytime_demand[dis] << ", ";
    //     cout << "max: " << cost_add[cost_add.size() - 1] << ", ";

    //     cout << "min: " << cost_add[0] << endl;

    //     // cout << dis << ",size:" << every_time_full_id[dis].size() << endl;
    //     // cout << cost_add[cost_add.size() - 1] << endl;
    //     // cout << cost_add[0] << endl;
    // }
    for (int i = 0; i < site_names.size(); i++)
    {
        int tmp = 0;
        // for(int j=0;j<customer_ids.size();j++){
        //     tmp+=distirbute_table[i][j].total();

        // }
        tmp = sites[i]._max_bandwidth - sites[i]._available_bandwidth;
        sites[i].final_get(tmp);
    }
}

void Distributor::move_stream_to_full_site(int best_site_id, int tmp_sort[], vector<int> &cost_add)
{

    int can_move = sites[best_site_id]._available_bandwidth;
    for (uint i = 0; i < site_names.size(); i++)
    {
        int max_id = tmp_sort[site_names.size() - i - 1];
        if (sites[max_id].full_dised || cost_add[max_id <= 0])
        {
            continue;
        }
        for (uint j = 0; j < customer_ids.size(); j++)
        {
            if (max_id != best_site_id && status[max_id][j] && status[best_site_id][j])
            {

                // distirbute_table[max_id][j].mysort_inverse();
                int size = distirbute_table[max_id][j].demans.size();
                // if (size == 0)
                // {
                //     continue;
                // }
                // // cout<<"!!!"<<endl;
                // if (distirbute_table[max_id][j].demans[size - 1].stream > can_move)
                // {
                //     // cout<<"!!!"<<endl;
                //     break;
                // }
                // else
                // {
                //     // cout<<"!!!"<<endl;
                //     distirbute_table[best_site_id][j].demans.push_back(distirbute_table[max_id][j].demans[size - 1]);

                //     // distirbute_table[max_id].line[k].demans.resize(distirbute_table[max_id].line[k].demans.size()-1);

                //     cost_add[max_id] -= distirbute_table[max_id][j].demans[size - 1].stream;
                //     // if(cost_add[max_id]<0){
                //     //     cout<<"something wrong"<<endl;
                //     // }
                //     cost_add[best_site_id] += distirbute_table[max_id][j].demans[size - 1].stream;
                //     can_move -= distirbute_table[max_id][j].demans[size - 1].stream;
                //     // cout<<"do something"<<endl;
                //     sites[best_site_id]._available_bandwidth -= distirbute_table[max_id][j].demans[size - 1].stream;
                //     sites[max_id]._available_bandwidth += distirbute_table[max_id][j].demans[size - 1].stream;
                //     distirbute_table[max_id][j].demans.erase(distirbute_table[max_id][j].demans.begin() + size - 1);
                // }

                // cout<<"size"<<size<<endl;

                for (int i = 0; i < size; i++)

                {

                    if (distirbute_table[max_id][j].demans[size - i - 1].stream > can_move || distirbute_table[max_id][j].demans[size - i - 1].stream < 0.8 * (base_cost + site_average[j]))
                    {
                        continue;
                    }
                    else
                    {
                        distirbute_table[best_site_id][j].demans.push_back(distirbute_table[max_id][j].demans[size - i - 1]);

                        // distirbute_table[max_id].line[k].demans.resize(distirbute_table[max_id].line[k].demans.size()-1);

                        cost_add[max_id] -= distirbute_table[max_id][j].demans[size - i - 1].stream;
                        // if(cost_add[max_id]<0){
                        //     cout<<"something wrong"<<endl;
                        // }
                        cost_add[best_site_id] += distirbute_table[max_id][j].demans[size - i - 1].stream;
                        can_move -= distirbute_table[max_id][j].demans[size - i - 1].stream;
                        // cout<<"do something"<<endl;
                        sites[best_site_id]._available_bandwidth -= distirbute_table[max_id][j].demans[size - i - 1].stream;
                        sites[max_id]._available_bandwidth += distirbute_table[max_id][j].demans[size - i - 1].stream;
                        distirbute_table[max_id][j].demans.erase(distirbute_table[max_id][j].demans.begin() + size - i - 1);
                    }
                }
            }
        }
    }
}

void Distributor::do_average_line(int max_id, int min_id, vector<int> &cost_add)
{
    vector<int> conecet_id;
    int destination_num;
    destination_num = cost_add[max_id] - (cost_add[max_id] - cost_add[min_id]) / 2;
    for (uint k = 0; k < customer_ids.size(); k++)
    {

        if (status[max_id][k] && status[min_id][k])
        {
            if (sites[min_id]._available_bandwidth - distirbute_table[max_id][k].total() < 0)
            {
                continue;
            }
            conecet_id.push_back(k);
            distirbute_table[min_id][k].demans.insert(distirbute_table[min_id][k].demans.end(), distirbute_table[max_id][k].demans.begin(), distirbute_table[max_id][k].demans.end());
            cost_add[max_id] -= distirbute_table[max_id][k].total();
            cost_add[min_id] += distirbute_table[max_id][k].total();
            sites[min_id]._available_bandwidth -= distirbute_table[max_id][k].total();
            sites[max_id]._available_bandwidth += distirbute_table[max_id][k].total();
            distirbute_table[max_id][k].demans.resize(0);
            distirbute_table[min_id][k].mysort();
        }
    }
    int j = -1;

    while (true)
    {
        j++;
        bool more_time = false;
        for (uint i = 0; i < conecet_id.size(); i++)
        {
            int cus_id = conecet_id[i];
            if (j < distirbute_table[min_id][cus_id].demans.size())
            {
                more_time = true;
                if (cost_add[max_id] + distirbute_table[min_id][cus_id].demans[j].stream < destination_num)
                {
                    distirbute_table[max_id][cus_id].demans.push_back(distirbute_table[min_id][cus_id].demans[j]);

                    // distirbute_table[max_id].line[k].demans.resize(distirbute_table[max_id].line[k].demans.size()-1);

                    cost_add[max_id] += distirbute_table[min_id][cus_id].demans[j].stream;
                    // if(cost_add[max_id]<0){
                    //     cout<<"something wrong"<<endl;
                    // }
                    cost_add[min_id] -= distirbute_table[min_id][cus_id].demans[j].stream;
                    // change -= distirbute_table[max_id][k].demans[size - i - 1].stream;
                    // cout<<"do something"<<endl;
                    sites[min_id]._available_bandwidth += distirbute_table[min_id][cus_id].demans[j].stream;
                    sites[max_id]._available_bandwidth -= distirbute_table[min_id][cus_id].demans[j].stream;
                    distirbute_table[min_id][cus_id].demans.erase(distirbute_table[min_id][cus_id].demans.begin() + j);
                }
            }
        }
        if (!more_time)
        {
            break;
        }
    }
}
void Distributor::do_average(int max_id, int min_id, int k, vector<int> &cost_add)
{
    if (cost_add[max_id] < cost_add[min_id])
    {
        int tmp = max_id;
        max_id = min_id;
        min_id = tmp;
        // return;
        // cout << "some thing wrong " << endl;
    }
    // // bool show=false;
    // // if(cost_add[max_id]>2000){
    // //     cout<<"cost_add_max:"<<cost_add[max_id]<<endl;
    // // cout<<"cost_add_min:"<<cost_add[min_id]<<endl;
    // // show =true;

    // // }

    // int want_change = (cost_add[max_id] - cost_add[min_id]) * 0.5;

    // int destination_cost_add = cost_add[max_id] - min(want_change, sites[min_id]._available_bandwidth);
    // //         if(show){
    // //     cout<<"want_change:"<<want_change<<endl;
    // // cout<<"destination_cost_add:"<<destination_cost_add<<endl;

    // // }
    // distirbute_table[min_id][k].demans.insert(distirbute_table[min_id][k].demans.end(), distirbute_table[max_id][k].demans.begin(), distirbute_table[max_id][k].demans.end());
    // cost_add[max_id] -= distirbute_table [max_id][k].total();
    // cost_add[min_id] += distirbute_table[max_id][k].total();
    // sites[min_id]._available_bandwidth -= distirbute_table[max_id][k].total();
    // sites[max_id]._available_bandwidth += distirbute_table[max_id][k].total();
    // distirbute_table[max_id][k].demans.resize(0);
    // distirbute_table[min_id][k].mysort();
    // for (int i = 0; i < distirbute_table[min_id][k].demans.size(); i++)
    // {
    //     if (cost_add[max_id] + distirbute_table[min_id][k].demans[i].stream < destination_cost_add)
    //     {
    //         distirbute_table[max_id][k].demans.push_back(distirbute_table[min_id][k].demans[i]);

    //         // distirbute_table[max_id].line[k].demans.resize(distirbute_table[max_id].line[k].demans.size()-1);

    //         cost_add[max_id] += distirbute_table[min_id][k].demans[i].stream;
    //         // if(cost_add[max_id]<0){
    //         //     cout<<"something wrong"<<endl;
    //         // }
    //         cost_add[min_id] -= distirbute_table[min_id][k].demans[i].stream;
    //         // change -= distirbute_table[max_id][k].demans[size - i - 1].stream;
    //         // cout<<"do something"<<endl;
    //         sites[min_id]._available_bandwidth += distirbute_table[min_id][k].demans[i].stream;
    //         sites[max_id]._available_bandwidth -= distirbute_table[min_id][k].demans[i].stream;
    //         distirbute_table[min_id][k].demans.erase(distirbute_table[min_id][k].demans.begin() + i);
    //     }
    // }
    // //     if(show){
    // //     cout<<"cost_add_max_new:"<<cost_add[max_id]<<endl;
    // // cout<<"cost_add_min_new:"<<cost_add[min_id]<<endl;

    // // }

    // int want_change = (cost_add[max_id] - cost_add[min_id]) * 1.5;
    int want_change = (cost_add[max_id] - cost_add[min_id]) * 1.0;
    // int want_change;
    // if(cost_add[min_id]<0){
    //     want_change=-cost_add[min_id];
    // }else{
    //     want_change=(cost_add[max_id] - cost_add[min_id]) * 0.55;
    // }
    // if(sites[min_id].full_dised){
    //     want_change=__INT_MAX__;
    // }
    // int want_change = (cost_add[max_id] - cost_add[min_id]);
    int can_change = sites[min_id]._available_bandwidth;
    // for (int i = 0; i < customer_ids.size(); i++)
    // {
    //     can_change -= distirbute_table[min_id][i].total();
    // }
    // if(can_change!=sites[min_id]._available_bandwidth){
    //     cout<<"can_change"<<can_change;
    // cout<<"avalible_change"<< sites[min_id]._available_bandwidth<<endl;
    // }

    int change = min(want_change, can_change);

    // move the small stream from min_id to max_id
    distirbute_table[max_id][k].mysort();
    // distirbute_table[min_id][k].mysort();
    if (distirbute_table[max_id][k].demans.size() == 0)
    {
        return;
    }
    int min_size = distirbute_table[min_id][k].demans.size();
    for (int i = 0; i < min_size; i++)
    {
        if (distirbute_table[max_id][k].demans[0].stream > distirbute_table[min_id][k].demans[min_size - i - 1].stream && distirbute_table[min_id][k].demans[min_size - i - 1].stream <= sites[max_id]._available_bandwidth)
        {
            distirbute_table[max_id][k].demans.push_back(distirbute_table[min_id][k].demans[min_size - i - 1]);

            // distirbute_table[max_id].line[k].demans.resize(distirbute_table[max_id].line[k].demans.size()-1);

            cost_add[max_id] += distirbute_table[min_id][k].demans[min_size - i - 1].stream;
            // if(cost_add[max_id]<0){
            //     cout<<"something wrong"<<endl;
            // }
            cost_add[min_id] -= distirbute_table[min_id][k].demans[min_size - i - 1].stream;
            change += distirbute_table[min_id][k].demans[min_size - i - 1].stream;
            // cout<<"do something"<<endl;
            sites[min_id]._available_bandwidth += distirbute_table[min_id][k].demans[min_size - i - 1].stream;
            sites[max_id]._available_bandwidth -= distirbute_table[min_id][k].demans[min_size - i - 1].stream;
            distirbute_table[min_id][k].demans.erase(distirbute_table[min_id][k].demans.begin() + min_size - i - 1);
        }
        // else
        // {
        //     break;
        // }
    }

    // if (distirbute_table[max_id][k].demans[0].stream <= sites[min_id]._available_bandwidth)
    // {
    //     distirbute_table[min_id][k].demans.push_back(distirbute_table[max_id][k].demans[0]);

    //     // distirbute_table[max_id].line[k].demans.resize(distirbute_table[max_id].line[k].demans.size()-1);

    //     cost_add[max_id] -= distirbute_table[max_id][k].demans[0].stream;
    //     // if(cost_add[max_id]<0){
    //     //     cout<<"something wrong"<<endl;
    //     // }
    //     cost_add[min_id] += distirbute_table[max_id][k].demans[0].stream;
    //     change -= distirbute_table[max_id][k].demans[0].stream;
    //     // cout<<"do something"<<endl;
    //     sites[min_id]._available_bandwidth -= distirbute_table[max_id][k].demans[0].stream;
    //     sites[max_id]._available_bandwidth += distirbute_table[max_id][k].demans[0].stream;
    //     distirbute_table[max_id][k].demans.erase(distirbute_table[max_id][k].demans.begin() + 0);
    // }

    // if (change < 0)
    // {
    //     cout << "change" << change << endl;
    // }
    // distirbute_table[max_id].line[k].mysort();
    // distirbute_table[max_id].line[k].demans.reserve(distirbute_table[max_id].line[k].begin(),distirbute_table[max_id].line[k].demans.end());
    // distirbute_table[max_id][k].mysort_inverse();
    int size = distirbute_table[max_id][k].demans.size();
    // cout<<"size"<<size<<endl;
    for (int i = 0; i < size; i++)

    {
        if (cost_add[max_id] <= cost_add[min_id])
        {
            return;
        }
        if (distirbute_table[max_id][k].demans[size - i - 1].stream > change)
        {
            continue;
        }
        else
        {
            distirbute_table[min_id][k].demans.push_back(distirbute_table[max_id][k].demans[size - i - 1]);

            // distirbute_table[max_id].line[k].demans.resize(distirbute_table[max_id].line[k].demans.size()-1);

            cost_add[max_id] -= distirbute_table[max_id][k].demans[size - i - 1].stream;
            // if(cost_add[max_id]<0){
            //     cout<<"something wrong"<<endl;
            // }
            cost_add[min_id] += distirbute_table[max_id][k].demans[size - i - 1].stream;
            change -= distirbute_table[max_id][k].demans[size - i - 1].stream;
            // cout<<"do something"<<endl;
            sites[min_id]._available_bandwidth -= distirbute_table[max_id][k].demans[size - i - 1].stream;
            sites[max_id]._available_bandwidth += distirbute_table[max_id][k].demans[size - i - 1].stream;
            distirbute_table[max_id][k].demans.erase(distirbute_table[max_id][k].demans.begin() + size - i - 1);
        }
    }
}

void Distributor::distribute()
{
    // for (int i = 0; i < 10; i++)
    // {
    //     // int id = tep_sort_overflow_id[site_names.size() - i - 1];
    //     // int id = tep_sort_overflow_id[i];
    //     // int id =sort_free_id[site_names.size()-i-1];
    //     int id = sort_free_id[i];
    //     cout << free_cost[id] << endl;
    //     // cout << cost_overflow[id] << endl;
    //     // cout << id << endl;
    //     more_ten_sites.push_back(site_names[id]);
    //     sites[id].one_of_ten_sites = true;
    // }
    for (uint time_id = 0; time_id < times.size(); time_id++)
    {
        int dis = sort_demand_id[time_id];

        do_distribute(dis);

        // final_check(dis);
        optimize(dis);
        // optimaize_by_id(dis);
        all_distirbute_table[dis] = distirbute_table;
        // cout << eveytime_demand[dis] << endl;
    }
    vector<int> cost_overflow;

    for (int i = 0; i < site_names.size(); i++)
    {
        int tmp_over = sites[i].new_base_cost - sites[i].base_cost;
        cost_overflow.push_back(tmp_over);
    }
    int tep_sort_overflow_id[site_names.size()] = {0};
    vector<int> tep_cost_overflow(cost_overflow.begin(), cost_overflow.end());
    BubbleSort(tep_cost_overflow, site_names.size(), tep_sort_overflow_id);
    for (int i = 0; i < 10; i++)
    {
        int id = tep_sort_overflow_id[site_names.size() - i - 1];
        // int id = tep_sort_overflow_id[i];
        // int id =sort_free_id[site_names.size()-i-1];
        // int id = sort_free_id[i];
        // cout << free_cost[id] << endl;
        cout << cost_overflow[id] << endl;
        // cout << id << endl;
        more_ten_sites.push_back(site_names[id]);
        sites[id].one_of_ten_sites = true;
    }

    string dir = "./output"; //文件夹路径

    // int flag = mkdir(dir.c_str(), S_IRWXU);  //Linux创建文件夹
    ofs.open(_Output, ios::trunc);
    for (int i = 0; i < more_ten_sites.size(); i++)
    {
        if (i != more_ten_sites.size() - 1)
        {
            ofs << more_ten_sites[i] << ",";
        }
        else
        {
            ofs << more_ten_sites[i] << endl;
        }
    }
    for (uint dis = 0; dis < this->times.size(); dis++)
    {

        output(dis);
        // cout<<sites[0]._max_bandwidth*0.95<<endl;
    }
    ofs.close();
    // for (int i = 0; i < site_names.size(); i++)
    // {
    //     if (sites[i]._full_distribute > times.size() / 20)
    //     {
    //         cout << "full distribute too much !" << endl;
    //     }
    //     else
    //     {
    //         cout << sites[i]._full_distribute << endl;
    //     }
    // }
}
void Distributor::do_distribute(int dis)
{

    do_distribute_lesscost(dis);
}

void Distributor::output(int dis)
{

    for (uint i = 0; i < customer_ids.size(); i++)
    {
        bool is_first = true;

        ofs << customer_ids[i] << ":";

        for (uint j = 0; j < distirbute_table.size(); j++)
        {
            if (all_distirbute_table[dis][j][i].demans.size() != 0)
            {
                if (is_first)
                {
                    is_first = false;
                }
                else
                {
                    ofs << ",";
                }
                ofs << "<" << site_names[j];
                for (uint k = 0; k < all_distirbute_table[dis][j][i].demans.size(); k++)
                {
                    ofs << "," << all_distirbute_table[dis][j][i].demans[k].stream_id;
                }
                ofs << ">";
            }
        }

        ofs << endl;
    }
}

Distributor::~Distributor()
{
}