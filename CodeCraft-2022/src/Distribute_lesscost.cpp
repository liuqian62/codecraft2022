#include "Distributor.h"

void Distributor::do_distribute_lesscost(int dis)
{
    init_distribute_table();
    uint j = 0;
    for (uint i = 0; i < customer_ids.size(); i++)
    {
        int steam_size = demands[dis][i].demans.size();

        while (steam_size > 0)
        {
            if (demands[dis][i].demans[steam_size - 1].stream == 0)
            {
                steam_size--;
                continue;
            }
            if (status[j][i] && sites[j].available() >= demands[dis][i].demans[steam_size - 1].stream)
            {
                distirbute_table[j][i].demans.push_back(demands[dis][i].demans[steam_size - 1]);
                sites[j]._available_bandwidth -= demands[dis][i].demans[steam_size - 1].stream;
                steam_size--;

                // if (sites[j].get(demands[dis][i].demans[steam_size - 1].stream))
                // {
                //     distirbute_table[j].line[i].demans.push_back(demands[dis][i].demans[steam_size - 1]);
                //     steam_size--;

                // }else{
                //     cout << "!!!!!!!!!!!!!!!!!!!!" << site_names[j] << endl;
                // }

                // sites[j].get(demands[dis][i].demans[steam_size-1].stream);
                // cout<<sites[j].available()<<endl;
                if (sites[j].available() < 0)
                {
                    cout << "!!!!!!!!!!!!!!!!!!!!" << site_names[j] << endl;
                    cout << sites[j].available() << endl;
                }
            }
            j++;
            if (j >= site_names.size())
            {
                j = 0;
            }
        }
    }

    // for (int i = 0; i < site_names.size(); i++)
    // {
    //     // int can_change = sites[i]._max_bandwidth;
    //     // for (int k = 0; k < customer_ids.size(); k++)
    //     // {
    //     //     can_change -= distirbute_table[i][k].total();
    //     // }
    //     // if(can_change!=sites[i]._available_bandwidth){
    //     //     cout<<"can_change"<<can_change;
    //     // cout<<"avalible_change"<< sites[i]._available_bandwidth<<endl;
    //     // }
    //     for (int j = 0; j < customer_ids.size(); j++)
    //     {
    //         distirbute_table[i][j].mysort();
    //     }
    // }
}
void Distributor::BubbleSort(vector<int> &p, int length, int *ind_diff)
{
    for (int m = 0; m < length; m++)
    {
        ind_diff[m] = m;
    }

    for (int i = 0; i < length; i++)
    {
        for (int j = 0; j < length - i - 1; j++)
        {
            if (p[j] > p[j + 1])
            {
                int temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;

                int ind_temp = ind_diff[j];
                ind_diff[j] = ind_diff[j + 1];
                ind_diff[j + 1] = ind_temp;
            }
        }
    }
}

void Distributor::get_information_demand()
{
    // double average_demand = 0.0;
    double all_demand = 0.0;
    // vector<int> eveytime_demand;
    for (uint i = 0; i < demands.size(); i++)
    {
        int tem_demand = 0;
        for (uint j = 0; j < customer_ids.size(); j++)
        {
            // for(int k=0;k<demands[i][j].demans.size();k++){
            //     tem_demand +=demands[i][j].demans[k].stream;
            //     all_demand +=demands[i][j].demans[k].stream;
            // }
            // tem_demand=0;
            tem_demand += demands[i][j].total();
            all_demand += demands[i][j].total();
        }
        eveytime_demand.push_back(tem_demand);
        if (tem_demand > max_demand)
        {
            max_demand = tem_demand;
        }
    }
    average_demand = all_demand / demands.size();
    cout << "max_demand" << max_demand << endl;
    cout << "average_demand" << average_demand << endl;
    vector<int> sort_demand(eveytime_demand.begin(), eveytime_demand.end());
    // sort(sort_demand.begin(), sort_demand.end(), greater<int>());
    vector<int> temp_max_demand;

    // int demand_id[sort_demand.size()] = {0};
    // vector<int> tep_free_cost(free_cost.begin(), free_cost.end());
    // BubbleSort(sort_demand, sort_demand.size(), demand_id);
    //    for (int i = 0; i < sort_demand.size(); i++)
    // {
    //     cout << "value: " << sort_demand[i] << "   Index: " << demand_id[i] << endl;

    // }
    int demand_id[sort_demand.size()] = {0};
    // for (int i = 0; i < free_cost.size(); i++)
    // {
    //     cout << "value: " << free_cost[sort_free_id[i]] << "   Index: " << sort_free_id[i] << endl;//from max to min
    // }

    // init every_time_full_id;
    for (uint i = 0; i < times.size(); i++)
    {
        vector<int> v;
        every_time_full_id.push_back(v);
        vector<vector<Demans>> table;
        all_distirbute_table.push_back(table);
    }
    for (uint i = 0; i < site_names.size(); i++)
    {
        full_used_time.push_back(0);
    }

    // best_covered_site_id0 = sort_site_id[0];
    // best_covered_site_id1 = sort_site_id[1];
    for (uint i = 0; i < sort_free_id.size(); i++)
    {

        int site_id = sort_free_id[sort_site_id.size()-1-i];
        // if(site_id==sort_site_id[0]){
        //     cout<<custormer_num_of_sites[site_id]<<"!!!"<<endl;
        //     continue;
        // }
        // sort demand
        if (free_cost[site_id] <=0)
        {
            continue;
        }
        vector<int> new_sort_demand(sort_demand.begin(), sort_demand.end());

        BubbleSort(new_sort_demand, new_sort_demand.size(), demand_id); // from min to max
        for (uint j = 0; j < times.size() / 20; j++)
        {
            // count how much can free
            int time_id = demand_id[sort_demand.size() - j - 1];
            int can = 0;

            for (uint k = 0; k < customer_ids.size(); k++)
            {

                if (status[site_id][k])
                {
                    can += demands[time_id][k].total();
                }
            }
            if (every_time_full_id[time_id].size() <= 0)
            {
                sort_demand[time_id] -= 0.85 * min(free_cost[site_id], can);
                // sort_demand[time_id] -= max_demand;
                every_time_full_id[time_id].push_back(site_id);
            }
            else
            {
                sort_demand[time_id] -= 0.75* min(free_cost[site_id], can);
                every_time_full_id[time_id].push_back(site_id);
            }
        }
    }

    sort(sort_demand.begin(), sort_demand.end(), greater<int>());
    this->expected_cost = sort_demand[0];
    // vector<int> time_custoermer_num;
    // for(int i=0;i<times.size();i++){
    //     int num=0;
    //     for(int j=0;j<customer_ids.size();j++){
    //         for(int k=0;k<every_time_full_id[i].size();k++){
    //             if(status[every_time_full_id[i][k]][j]){
    //                 num ++;
    //                 break;
    //             }
    //         }
    //     }
    //     time_custoermer_num.push_back(num);

    // }
    // int sort_num_id[time_custoermer_num.size()] = {0};
    //     vector<int> new_time_custoermer_num(time_custoermer_num.begin(), time_custoermer_num.end());

    //     BubbleSort(new_time_custoermer_num, time_custoermer_num.size(), sort_num_id); // from min to max

    // int num =0;
    // for(int i=0;i<times.size();i++){
    //     int id=sort_num_id[i];

    //     int thresh_hold=times.size()/20;
    //     if(every_time_full_id[id].size()>0&&num<thresh_hold){
    //         every_time_full_id[id].push_back(best_covered_site_id0);
    //         // cout<<time_custoermer_num[id]<<endl;
    //         num++;
    //     }
    //     //         if(every_time_full_id[id].size()>0&&num>thresh_hold&&num<2*thresh_hold){
    //     //     every_time_full_id[id].push_back(best_covered_site_id1);
    //     //     // cout<<time_custoermer_num[id]<<endl;
    //     //     num++;
    //     // }
    // }

    // for (int i = 0; i < times.size() / 20; i++)
    // {
    //     for (int j = 0; j < free_cost.size(); j++)
    //     {
    //         if (temp_max_demand.size() == sort_demand.size())
    //         {
    //             break;
    //         }
    //         if (temp_max_demand.size() == 0)
    //         {
    //             temp_max_demand.push_back(sort_demand[0]);
    //         }
    //         if (temp_max_demand[0] >= sort_demand[temp_max_demand.size()])
    //         {
    //             temp_max_demand[0] -= xishu * free_cost[j];
    //             sort(temp_max_demand.begin(), temp_max_demand.end(), greater<int>());
    //         }
    //         else
    //         {
    //             temp_max_demand.push_back(sort_demand[temp_max_demand.size()] - xishu * free_cost[j]);
    //             sort(temp_max_demand.begin(), temp_max_demand.end(), greater<int>());
    //         }
    //     }
    // }

    this->expected_cost = max(expected_cost, 0);
    if (this->expected_cost == 0)
    {
        this->expected_cost = 0.5 * this->average_demand;
    }
    this->expected_cost =0.9* this->expected_cost;
    cout << "expected_cost" << this->expected_cost << endl;
    // for(int i=0;i<every_time_full_id.size();i++){
    //     for(int j=0;j<every_time_full_id[i].size();j++){
    //         cout<<every_time_full_id[i][j]<<",";
    //     }
    //     cout<<endl;
    // }

    int tep_sort_demand_id[times.size()] = {0};
    vector<int> tep_every_demand(eveytime_demand.begin(), eveytime_demand.end());
    BubbleSort(tep_every_demand, times.size(), tep_sort_demand_id);
    //    for (int i = 0; i < free_cost.size(); i++)
    // {
    //     cout << "value: " << free_cost[i] << "   Index: " << tep_sort_free_id[i] << endl;

    // }
    for (uint i = 0; i < times.size(); i++)
    {
        sort_demand_id.push_back(tep_sort_demand_id[eveytime_demand.size() - i - 1]);
    }
    // for (int i = 0; i < eveytime_demand.size(); i++)
    // {
    //     cout << "demand: " << eveytime_demand[sort_demand_id[i]] << "   Index: " << sort_demand_id[i] << endl;
    // }
    // for(int i=0;i<times.size();i++){
    //     cout<<"demand at "<<times[i]<<":"<<sort_demand[i]<<endl;
    // }
    //     for(int i=0;i<site_names.size();i++){
    //     cout<<"free_cost at "<<i<<":"<<free_cost[i]<<"="<<sites[i]._max_bandwidth<<endl;
    // }

    vector<int> site_max;
    double sum_max = 0.0;
    for (uint i = 0; i < site_names.size(); i++)
    {
        for (uint j = 0; j < customer_ids.size(); j++)
        {
            if (status[i][j])
            {
                site_max.push_back(sites[i]._max_bandwidth);
                sum_max += sites[i]._max_bandwidth;
                this->expected_cost-=this->base_cost;
                break;
            }
            if (j == customer_ids.size() - 1)
            {
                site_max.push_back(0);
            }
        }
    }
    

    if(this->expected_cost<0){
        this->expected_cost=0;
    }



    for (uint i = 0; i < site_names.size(); i++)
    {
        // cout<<site_max[i]<<endl;
        double weight = site_max[i] / sum_max;
        // site_average.push_back(this->eveytime_demand[dis] * site_max[i] / sum_max);
        site_average.push_back(this->expected_cost * weight);
        sites[i].base_cost=this->base_cost+site_average[i];
        sites[i].new_base_cost=this->base_cost+site_average[i];
        // cout<<site_average[i]<<endl;
    }
}

void Distributor::init_distribute_table()
{
    // 初始化流量分配
    distirbute_table.resize(0);
    for (uint i = 0; i < site_names.size(); i++)
    {
        vector<Demans> temp_site_table;
        for (uint j = 0; j < customer_ids.size(); j++)
        {
            Demans tmp_demans;
            temp_site_table.push_back(tmp_demans);

            // temp.push_back(0);
        }
        distirbute_table.push_back(temp_site_table);
    }

    for (uint i = 0; i < site_names.size(); i++)
    {
        sites[i]._available_bandwidth = sites[i]._max_bandwidth;
        sites[i].full_dised=false;
        sites[i].high_value=true;
    }
}