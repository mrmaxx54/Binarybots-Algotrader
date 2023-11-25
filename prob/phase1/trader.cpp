#include "receiver.h"
#include <string>
#include <vector>
#include <sstream>

using namespace std;
void calcSubset(vector<pair<vector<pair<string, int>>, pair<int, int>>> &A, vector<pair<vector<pair<string, int>>, pair<int, int>>> &subset, int index, vector<vector<pair<vector<pair<string, int>>, pair<int, int>>>> &res)
{
    res.push_back(subset);
    // Algorithm for choosing the best subset
    for (int i = index; i < A.size(); i++)
    {
        subset.push_back(A[i]);
        calcSubset(A, subset, i + 1, res);
        subset.pop_back();
    }
}
vector<vector<pair<vector<pair<string, int>>, pair<int, int>>>> subsets(vector<pair<vector<pair<string, int>>, pair<int, int>>> &A)
{
    vector<pair<vector<pair<string, int>>, pair<int, int>>> subset;
    vector<vector<pair<vector<pair<string, int>>, pair<int, int>>>> res;
    int index = 0;
    calcSubset(A, subset, index, res);
    return res;
}

void calcSubsetWithQuantity(vector<pair<pair<vector<pair<string, int>>, pair<int, int>>, int>> &tuples, vector<vector<pair<vector<pair<string, int>>, pair<int, int>>>> &res,
                            vector<pair<vector<pair<string, int>>, pair<int, int>>> &subset, int index)
{
    // Add the current subset to the result list
    res.push_back(subset);

    // Generate subsets by recursively including and excluding elements
    for (int i = index; i < tuples.size(); i++)
    {
        for (int j = 1; j <= tuples[i].second; j++)
        {
            // Include the current element in the subset 'j' times
            subset.push_back(tuples[i].first);

            // Recursively generate subsets with the current element included 'j' times
            calcSubsetWithQuantity(tuples, res, subset, i + 1);

            // Exclude the current element from the subset 'j' times (backtracking)
            subset.pop_back();
        }
    }
}

vector<vector<pair<vector<pair<string, int>>, pair<int, int>>>> subsetsWithQuantity(vector<pair<pair<vector<pair<string, int>>, pair<int, int>>, int>> &tuples)
{
    vector<pair<vector<pair<string, int>>, pair<int, int>>> subset;
    vector<vector<pair<vector<pair<string, int>>, pair<int, int>>>> res;
    int index = 0;
    calcSubsetWithQuantity(tuples, res, subset, index);
    return res;
}

bool isNumber(string s)
{
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] == '-')
            continue;
        // std::cout<<s[i]<<endl;
        if (isdigit(s[i]) == false)
            return false;
    }

    return true;
}

bool isequal(auto first1, auto last1, auto first2)
{
    while (first1 != last1)
    {
        if (!(*first1 == *first2)) // or: if (!pred(*first1,*first2)), for version 2
            return false;
        ++first1;
        ++first2;
    }
    return true;
}
bool allof(auto first, auto last, int element)
{
    while (first != last)
    {
        if (*first != element)
        {
            return false;
        }
        first++;
    }
    return true;
}

void part1()
{
    Receiver rcv;
    vector<pair<string, int>> stock_prices; // To keep track of the best prices
    bool foundDollar = false;
    string line;
    vector<pair<string, int>> buy_update;
    vector<pair<string, int>> sell_update;
    while (!foundDollar)
    {
        string input_file = rcv.readIML();
        if (input_file.find('$') != string::npos)
        {
            rcv.terminate();
            foundDollar = true;
        }
        input_file.pop_back(); // remove the $ at the end of the input_file
        istringstream stream(input_file);

        while (getline(stream, line, '#'))
        {
            string stock_name;
            string p;
            int price;
            char act;
            istringstream stream(line);
            stream >> stock_name >> p >> act;
            price = stoi(p);

            // if case  for buy ******************************************************

            if (act == 'b')
            {
                bool ftest = false;
                for (int i = 0; i < buy_update.size(); i++)
                {
                    if (buy_update[i].first == stock_name)
                    {
                        if (buy_update[i].second >= price)
                        {
                            cout << "No Trade" << endl;
                            ftest = true;
                        }
                        else if (buy_update[i].second < price)
                        {
                            buy_update.erase(buy_update.begin() + i);
                        }
                        break;
                    }
                }
                if (ftest == false)
                {
                    bool check = false;
                    for (int i = 0; i < sell_update.size(); i++)
                    {
                        if (sell_update[i].first == stock_name)
                        {
                            if (sell_update[i].second == price)
                            {
                                sell_update.erase(sell_update.begin() + i);
                                cout << "No Trade" << endl;
                                check = true;
                                break;
                            }
                        }
                    }
                    if (check == false)
                    {
                        bool check1 = false;
                        for (int i = 0; i < stock_prices.size(); i++)
                        {
                            if (stock_prices[i].first == stock_name)
                            {
                                if (stock_prices[i].second < price)
                                {
                                    stock_prices[i].second = price;
                                    cout << stock_name << " " << price << " s" << endl;
                                }
                                else
                                {
                                    bool c = false;
                                    for (int i = 0; i < buy_update.size(); i++)
                                    {
                                        if (buy_update[i].first == stock_name)
                                        {
                                            if (buy_update[i].second < price)
                                            {
                                                buy_update[i].second = price;
                                            }
                                            c = true;
                                            break;
                                        }
                                    }
                                    if (c == false)
                                    {
                                        buy_update.push_back(make_pair(stock_name, price));
                                    }
                                    cout << "No Trade" << endl;
                                }
                                check1 = true;
                                break;
                            }
                        }
                        if (check1 == false)
                        {
                            stock_prices.push_back(make_pair(stock_name, price));
                            cout << stock_name << " " << price << " s" << endl;
                        }
                    }
                }
            }

            // next if case  for sell ******************************************************

            if (act == 's')
            {
                bool ftest = false;
                for (int i = 0; i < sell_update.size(); i++)
                {
                    if (sell_update[i].first == stock_name)
                    {
                        if (sell_update[i].second <= price)
                        {
                            cout << "No Trade" << endl;
                            ftest = true;
                        }
                        else if (sell_update[i].second > price)
                        {
                            sell_update.erase(sell_update.begin() + i);
                        }
                        break;
                    }
                }
                if (ftest == false)
                {
                    bool check = false;
                    for (int i = 0; i < buy_update.size(); i++)
                    {
                        if (buy_update[i].first == stock_name)
                        {
                            if (buy_update[i].second == price)
                            {
                                buy_update.erase(buy_update.begin() + i);
                                cout << "No Trade" << endl;
                                check = true;
                                break;
                            }
                        }
                    }
                    if (check == false)
                    {
                        bool check1 = false;
                        for (int i = 0; i < stock_prices.size(); i++)
                        {
                            if (stock_prices[i].first == stock_name)
                            {
                                if (stock_prices[i].second > price)
                                {
                                    stock_prices[i].second = price;
                                    cout << stock_name << " " << price << " b" << endl;
                                }
                                else
                                {
                                    bool c = false;
                                    for (int i = 0; i < sell_update.size(); i++)
                                    {
                                        if (sell_update[i].first == stock_name)
                                        {
                                            if (sell_update[i].second > price)
                                            {
                                                sell_update[i].second = price;
                                            }
                                            c = true;
                                            break;
                                        }
                                    }
                                    if (c == false)
                                    {
                                        sell_update.push_back(make_pair(stock_name, price));
                                    }
                                    cout << "No Trade" << endl;
                                }
                                check1 = true;
                                break;
                            }
                        }
                        if (check1 == false)
                        {
                            stock_prices.push_back(make_pair(stock_name, price));
                            cout << stock_name << " " << price << " b" << endl;
                        }
                    }
                }
            }
        }
    }
}

void part2()
{
    Receiver rcv;
    vector<pair<string, int>> stock_prices; // To keep track of the best prices
    string input_file = rcv.readIML();
    input_file.pop_back(); // remove the $ at the end of the input_file
    string line;
    int total_profit = 0;
    // vector<pair<string, int>> buy_update;
    // vector<pair<string, int>> sell_update;
    istringstream stream(input_file);
    // A vector would be used to store all the inputs of part2 type till now
    vector<pair<vector<pair<string, int>>, pair<int, int>>> overall_stock_info;
    // vector<pair<int, int>> price_and_act_for_allstocks;
    vector<pair<string, int>> stock_name_code;
    // Input strings are taken just like part1
    int j = 0;
    while (getline(stream, line, '#'))
    {
        vector<pair<string, int>> current_stock_info; // Here vector<LC,price-signal pair>, LC:vector<stock_code,quantity>
        // overall_stock_info.push_back(current_stock_info);
        int price;
        char act;
        string line1 = line;
        istringstream iss(line);
        line1.erase(line1.size() - 1);
        current_stock_info.push_back(make_pair(line1, j + 1)); // first element is 1 means the stock has not been used for arbitrage and j+1 is printing which line of input it is
        j++;

        int no_of_stocks = 0; // for number of stocks in the LC
        vector<pair<int, int>> k_values_and_quantity;
        while (iss)
        {
            int k = -1; // variable to be used for stock_name_code
            string stock_name;
            string q;
            iss >> stock_name >> q;
            // no_of_stocks++;
            if (q == "b" || q == "s")
            {
                if (q == "b")
                    act = 'b';
                else
                    act = 's';
                // std::cout << stock_name << endl;
                price = stoi(stock_name);
                // std::cout << price << endl;
                break;
            }
            int quantity = stoi(q);
            for (int i = 0; i < stock_name_code.size(); i++)
            {
                if (stock_name_code[i].first == stock_name)
                {
                    k = i;
                    break;
                }
            }
            if (k == -1)
            {
                stock_name_code.push_back(make_pair(stock_name, stock_name_code.size()));
                k = stock_name_code.size() - 1;
            }
            k_values_and_quantity.push_back(make_pair(k, quantity));
        }
        for (int i = 0; i < stock_name_code.size(); i++)
        {                                                                         // This loop is for creating space inside the vector current_stock_info
            current_stock_info.push_back(make_pair(stock_name_code[i].first, 0)); // This means every stock code for a particular stock name is initialised to -1(that means stock is not there)
            // current_stock_info.push_back(0);// stock not there hence quantity is zero:)
        }
        for (int i = 0; i < k_values_and_quantity.size(); i++)
        {
            current_stock_info[k_values_and_quantity[i].first + 1] = make_pair(stock_name_code[k_values_and_quantity[i].first].first, k_values_and_quantity[i].second);
            // current_stock_info[2*k_values_and_quantity[i].first+2]=k_values_and_quantity[i].second;
        }
        // std::cout << price << "Yo" << endl;
        if (act == 'b')
            overall_stock_info.push_back(make_pair(current_stock_info, make_pair(price, 1))); // 1 for buy
        else
            overall_stock_info.push_back(make_pair(current_stock_info, make_pair(price, -1))); // -1 for sell
        // overall_stock_info.push_back(current_stock_info);
        // algorithm to do the trade

        int brk = 0;
        for (int i = 0; i < overall_stock_info.size() - 1; i++)
        {
            // std::cout << overall_stock_info[i].second.first << "Hello" << endl;
            if (overall_stock_info[i].first.size() < overall_stock_info[overall_stock_info.size() - 1].first.size())
            {
                int p = overall_stock_info[i].first.size();
                while (overall_stock_info[i].first.size() != overall_stock_info[overall_stock_info.size() - 1].first.size())
                {
                    overall_stock_info[i].first.push_back(make_pair(stock_name_code[p - 1].first, 0));
                    p++;
                }
            }
        }
        for (int i = 0; i < overall_stock_info.size() - 1; i++)
        {
            if (isequal(overall_stock_info[i].first.begin() + 1, overall_stock_info[i].first.end(), overall_stock_info[overall_stock_info.size() - 1].first.begin() + 1))
            {
                if (overall_stock_info[i].second.second == overall_stock_info[overall_stock_info.size() - 1].second.second && overall_stock_info[i].second.second == 1 && overall_stock_info[i].second.first > overall_stock_info[overall_stock_info.size() - 1].second.first)
                { // better buy price and the current order cancelled
                    // overall_stock_info.erase(overall_stock_info.begin()+i);
                    overall_stock_info.erase(overall_stock_info.begin() + overall_stock_info.size() - 1);
                    brk = 1;
                    break;
                }
                else if (overall_stock_info[i].second.second == overall_stock_info[overall_stock_info.size() - 1].second.second && overall_stock_info[i].second.second == 1 && overall_stock_info[i].second.first <= overall_stock_info[overall_stock_info.size() - 1].second.first)
                { // better buy price and the previous order cancelled
                    overall_stock_info.erase(overall_stock_info.begin() + i);
                    break;
                    // overall_stock_info.erase(overall_stock_info.begin()+overall_stock_info.size()-1);
                }
                else if (overall_stock_info[i].second.second == overall_stock_info[overall_stock_info.size() - 1].second.second && overall_stock_info[i].second.second == -1 && overall_stock_info[i].second.first < overall_stock_info[overall_stock_info.size() - 1].second.first)
                { // better sell price and the current order cancelled
                    // overall_stock_info.erase(overall_stock_info.begin()+i);
                    overall_stock_info.erase(overall_stock_info.begin() + overall_stock_info.size() - 1);
                    brk = 1;
                    break;
                }
                else if (overall_stock_info[i].second.second == overall_stock_info[overall_stock_info.size() - 1].second.second && overall_stock_info[i].second.second == -1 && overall_stock_info[i].second.first >= overall_stock_info[overall_stock_info.size() - 1].second.first)
                { // better sell price and the previous order cancelled
                    overall_stock_info.erase(overall_stock_info.begin() + i);
                    break;
                    // overall_stock_info.erase(overall_stock_info.begin()+overall_stock_info.size()-1);
                }
            }
        }
        if (brk == 0)
        {
            for (int i = 0; i < overall_stock_info.size() - 1; i++)
            {
                if (isequal(overall_stock_info[i].first.begin() + 1, overall_stock_info[i].first.end(), overall_stock_info[overall_stock_info.size() - 1].first.begin() + 1))
                {
                    if (overall_stock_info[i].second.second == -overall_stock_info[overall_stock_info.size() - 1].second.second && overall_stock_info[i].second.first == overall_stock_info[overall_stock_info.size() - 1].second.first)
                    { // buy-sell cancellation of same LC with same price
                        overall_stock_info.erase(overall_stock_info.begin() + i);
                        overall_stock_info.erase(overall_stock_info.begin() + overall_stock_info.size() - 1);
                        brk = 1;
                        break;
                    }
                }
            }
        }
        if (brk == 1)
        {
            cout << "No Trade" << endl;
        }
        else
        {
            int max_profit;
            vector<int> max_profit_subset;
            // max_profit_subset.clear();
            max_profit = 0;
            vector<vector<pair<vector<pair<string, int>>, pair<int, int>>>> subsets_of_the_orders = subsets(overall_stock_info); // my vector containing all the subsets of the stock
            for (int i = 0; i < subsets_of_the_orders.size(); i++)
            {
                vector<pair<vector<pair<string, int>>, pair<int, int>>> subset = subsets_of_the_orders[i];
                if (!subset.empty())
                {
                    vector<int> sum_of_quantities(subset[0].first.size(), 0);
                    int profit = 0;
                    for (int j = 0; j < subset.size(); j++)
                    {
                        for (int k = 0; k < subset[0].first.size() - 1; k++)
                        {                                                                                        // subset[0].first contains one more (stock order name, stock order number) pair hence size-1
                            sum_of_quantities[k + 1] += subset[j].first[k + 1].second * subset[j].second.second; // k+1, since first pair is (string, stock_number)
                        }
                        profit += subset[j].second.first * subset[j].second.second;
                    }
                    bool allZeros = allof(sum_of_quantities.begin(), sum_of_quantities.end(), 0); // To Check whether all elements are zero or not
                    if (allZeros)
                    {
                        if (max_profit < profit)
                        {
                            max_profit = profit;
                            max_profit_subset.clear(); // to clear previous data
                            for (int z = 0; z < subset.size(); z++)
                            {
                                max_profit_subset.push_back(subset[z].first[0].second);
                            }
                        }
                    }
                }
            }
            if (max_profit == 0)
            {
                cout << "No Trade" << endl;
            }
            else
            {
                int iter = max_profit_subset.size() - 1;
                total_profit = total_profit + max_profit;
                // int size_fixed=overall_stock_info.size();
                for (int i = overall_stock_info.size() - 1; i >= 0; i--)
                {
                    if (iter == max_profit_subset.size())
                    {
                        break;
                    }
                    if (max_profit_subset[iter] == overall_stock_info[i].first[0].second)
                    {
                        iter--;
                        if (overall_stock_info[i].second.second == 1)
                        {
                            cout << overall_stock_info[i].first[0].first << "s#" << endl;
                        }
                        else if (overall_stock_info[i].second.second == -1)
                        {
                            cout << overall_stock_info[i].first[0].first << "b#" << endl;
                        }
                        overall_stock_info.erase(overall_stock_info.begin() + i);
                        i++;
                    }
                }
            }
        }
    }
    cout << total_profit << endl;
}

void part3()
{
    Receiver rcv;
    vector<pair<string, int>> stock_prices; // To keep track of the best prices
    string input_file = rcv.readIML();
    input_file.pop_back(); // remove the $ at the end of the input_file
    string line;
    int total_profit=0;
    // vector<pair<string, int>> buy_update;
    // vector<pair<string, int>> sell_update;
    istringstream stream(input_file);
    vector<pair<pair<vector<pair<string,int>>,pair<int,int>>,int>> overall_stock_info_part_3;
    //vector<pair<int, int>> price_and_act_for_allstocks;  
    vector<pair<string, int>> stock_name_code;
    //Input strings are taken just like part1
    int j=0;
    while(getline(stream, line, '#'))
    {   
        vector<pair<string,int>> current_stock_info;//Here vector<LC,price-signal pair>, LC:vector<stock_code,quantity>
        //overall_stock_info.push_back(current_stock_info);
        int price;
        int net_quantity;
        char act; 
        istringstream stream(line);
        //std::cout<<line <<std::endl;
        current_stock_info.push_back(make_pair(line.substr(0,line.length()-4),j+1));//first element is 1 means the stock has not been used for arbitrage and j+1 is printing which line of input it is
        j++;
        
        int no_of_stocks=0;// for number of stocks in the LC 
        vector<pair<int, int>> k_values_and_quantity; 
        while(stream){
            int k=-1; //variable to be used for stock_name_code
            string stock_name;
            string q;
            stream >> stock_name >> q;
           
            if(isNumber(stock_name)){
                //std::cout << stock_name << endl;
                price=stoi(stock_name);
                net_quantity=stoi(q);
                stream >> act;
                break;
            }
            // no_of_stocks++;
            // if(q=="b"||q=="s"){
            //     if(q=="b") act='b';
            //     else act='s';
            //     //std::cout << stock_name << endl;
            //     price=stoi(stock_name);
            //     //std::cout << price << endl;
            //     break;
            // }
            //std::cout << current_stock_info[0].first << endl;
            int quantity=stoi(q);
            for(int i=0;i<stock_name_code.size();i++){
                if(stock_name_code[i].first==stock_name){
                    k=i;
                    break;
                }
            }
            if(k==-1){
                stock_name_code.push_back(make_pair(stock_name,stock_name_code.size()));
                k=stock_name_code.size()-1;
            }
            k_values_and_quantity.push_back(make_pair(k,quantity));
        }
        for(int i=0;i<stock_name_code.size();i++){ //This loop is for creating space inside the vector current_stock_info
            current_stock_info.push_back(make_pair(stock_name_code[i].first,0)); //This means every stock code for a particular stock name is initialised to -1(that means stock is not there)
            //current_stock_info.push_back(0);// stock not there hence quantity is zero:)
        }
        for(int i=0;i<k_values_and_quantity.size();i++){
            current_stock_info[k_values_and_quantity[i].first+1]=make_pair(stock_name_code[k_values_and_quantity[i].first].first,k_values_and_quantity[i].second);
            //current_stock_info[2*k_values_and_quantity[i].first+2]=k_values_and_quantity[i].second;
        }
        //std::cout << price << "Yo" << endl;
        if(act=='b') overall_stock_info_part_3.push_back(make_pair(make_pair(current_stock_info,make_pair(price,1)),net_quantity));// 1 for buy
        else overall_stock_info_part_3.push_back(make_pair(make_pair(current_stock_info,make_pair(price,-1)),net_quantity));// -1 for sell
        //overall_stock_info.push_back(current_stock_info);
        //algorithm to do the trade


        for(int i=0;i<current_stock_info.size();i++){
            //std::cout << current_stock_info[i].first << current_stock_info[i].second << std::endl;
        }
        
        int brk=0;
        for(int i=0;i<overall_stock_info_part_3.size()-1;i++){
            //std::cout << overall_stock_info[i].second.first << "Hello" << endl;
            if(overall_stock_info_part_3[i].first.first.size()<overall_stock_info_part_3[overall_stock_info_part_3.size()-1].first.first.size()){
                int p=overall_stock_info_part_3[i].first.first.size();
                while(overall_stock_info_part_3[i].first.first.size()!=overall_stock_info_part_3[overall_stock_info_part_3.size()-1].first.first.size()){
                    overall_stock_info_part_3[i].first.first.push_back(make_pair(stock_name_code[p-1].first,0));
                    p++;
                }
            }
        }
        for(int i=0;i<overall_stock_info_part_3.size()-1;i++){
            if(isequal(overall_stock_info_part_3[i].first.first.begin()+1,overall_stock_info_part_3[i].first.first.end(),overall_stock_info_part_3[overall_stock_info_part_3.size()-1].first.first.begin()+1)){
                if(overall_stock_info_part_3[i].first.second.second==overall_stock_info_part_3[overall_stock_info_part_3.size()-1].first.second.second && overall_stock_info_part_3[i].first.second.second==1 && overall_stock_info_part_3[i].first.second.first==overall_stock_info_part_3[overall_stock_info_part_3.size()-1].first.second.first){ //same type of buy order quantities added
                    //overall_stock_info.erase(overall_stock_info.begin()+i);
                    overall_stock_info_part_3[overall_stock_info_part_3.size()-1].second+= overall_stock_info_part_3[i].second;
                    overall_stock_info_part_3.erase(overall_stock_info_part_3.begin()+i);
                    brk=0;
                    break;
                }
                else if(overall_stock_info_part_3[i].first.second.second==overall_stock_info_part_3[overall_stock_info_part_3.size()-1].first.second.second && overall_stock_info_part_3[i].first.second.second==-1 && overall_stock_info_part_3[i].first.second.first==overall_stock_info_part_3[overall_stock_info_part_3.size()-1].first.second.first){ //same type of sell order quantities added 
                    //overall_stock_info.erase(overall_stock_info.begin()+i);
                    overall_stock_info_part_3[overall_stock_info_part_3.size()-1].second+= overall_stock_info_part_3[i].second;
                    overall_stock_info_part_3.erase(overall_stock_info_part_3.begin()+i);
                    brk=0;
                    break;
                }
                else if(overall_stock_info_part_3[i].first.second.second==-overall_stock_info_part_3[overall_stock_info_part_3.size()-1].first.second.second && overall_stock_info_part_3[i].first.second.first==overall_stock_info_part_3[overall_stock_info_part_3.size()-1].first.second.first){ //buy-sell cancellation and new quantity generation of same LC with same price
                    if(overall_stock_info_part_3[overall_stock_info_part_3.size()-1].second>=overall_stock_info_part_3[i].second){
                        overall_stock_info_part_3[overall_stock_info_part_3.size()-1].second-= overall_stock_info_part_3[i].second;
                        overall_stock_info_part_3.erase(overall_stock_info_part_3.begin()+i);
                    }
                    else if(overall_stock_info_part_3[overall_stock_info_part_3.size()-1].second<overall_stock_info_part_3[i].second){
                        overall_stock_info_part_3[i].second-= overall_stock_info_part_3[overall_stock_info_part_3.size()-1].second;
                        overall_stock_info_part_3.erase(overall_stock_info_part_3.begin()+overall_stock_info_part_3.size()-1);
                    }
                    brk=0;
                    if(overall_stock_info_part_3[overall_stock_info_part_3.size()-1].second==0){
                        brk=1;
                        overall_stock_info_part_3.erase(overall_stock_info_part_3.begin()+overall_stock_info_part_3.size()-1);
                    }
                    break;
                }  
            }
        }
        if(brk==1){
            cout << "No Trade" << endl;
        }
        else{
            int max_profit;
            vector<pair<int,int>> max_profit_subset;//(line number, quantity)
            //max_profit_subset.clear();
            max_profit=0;
            //vector<pair<vector<pair<string,int>>,pair<int,int>>> overall_stock_info_part_3_order_splits;//for splitting orders 
            //for(int q=0;q<overall_stock_info_part_3.size();q++){
            //    for(int r=0;r<overall_stock_info_part_3[q].second;r++){
            //        overall_stock_info_part_3_order_splits.push_back(overall_stock_info_part_3[q].first);
            //    }
            //}
            vector<vector<pair<vector<pair<string,int>>,pair<int,int>>>> subsets_of_the_orders=subsetsWithQuantity(overall_stock_info_part_3);//my vector containing all the subsets of the stock
            for(int i=0;i<subsets_of_the_orders.size();i++){
                vector<pair<vector<pair<string,int>>,pair<int,int>>>subset=subsets_of_the_orders[i];
                if(!subset.empty()){
                    vector<int> sum_of_quantities(subset[0].first.size(),0);
                    //cout << max_profit << endl;
                    //max_profit++;
                    int profit=0;
                        //subset[0].first contains one more (stock order name, stock order number) pair hence size-1
                        //cout << "subset priniting started" << endl;
                    for(int j=0; j<subset.size();j++){
                        for(int k=0; k<subset[0].first.size()-1;k++){
                            //std::cout << subset[j].first[0].first << endl;//for debugging purposes whether stock orders are being taken in subsets or not
                            //std::cout << profit << std::endl;
                            //std::cout << k << std::endl;
                            //std::cout << j << std::endl;
                            sum_of_quantities[k+1]+=subset[j].first[k+1].second*subset[j].second.second;// k+1, since first pair is (string, stock_number)
                            //std::cout << subset[j].second.first<< endl;
                            //std::cout << subset[j].second.second << endl;
                            //std::cout << subset[j].second.first*subset[j].second.second << endl;
                        }
                            profit+=subset[j].second.first*subset[j].second.second;
                        
                    }
                    //for(int o=0;o<sum_of_quantities.size();o++){
                        //cout << sum_of_quantities[o] << endl;
                    //    if(o==sum_of_quantities.size()-1 && allZeros && profit>0){
                    //        cout << o << " " << profit << " subset priniting ended" << endl;
                    //    }
                    //}
                    //To Check whether all elements are zero or not
                    bool allZeros = std::all_of(sum_of_quantities.begin(), sum_of_quantities.end(), [](int element) {
                    return element == 0;});
                    if(allZeros){
                        if(max_profit<profit){
                            max_profit=profit;
                            max_profit_subset.clear();//to clear previous data
                            for(int z=0;z<subset.size();z++){
                                int max_profit_quantity=1;
                                while(subset[z]==subset[z+1]){
                                    max_profit_quantity++;
                                    z++;
                                }
                                max_profit_subset.push_back(make_pair(subset[z].first[0].second,max_profit_quantity));
                                //std::cout << max_profit << endl;
                                //std::cout << subset[z].first[0].first << max_profit_quantity << subset.size() << endl;
                            }
                            //cout << "Hello"<<endl;
                            //int x=0;
                            //for(int j=0; j<subset.size();j++){
                            //    x+=subset[j].second.first*subset[j].second.second;
                            //    cout << x << subset[j].second.first*subset[j].second.second << endl;
                        //}
                            
                        }
                    }
                }
            }
            if(max_profit==0){
                cout << "No Trade" << endl;
            }
            else{
                int iter=max_profit_subset.size()-1;
                //int size_fixed=overall_stock_info.size();
                for(int i=overall_stock_info_part_3.size()-1;i>=0;i--){
                    if(iter==-1){
                        break;
                    }
                    if(max_profit_subset[iter].first==overall_stock_info_part_3[i].first.first[0].second){
                        //cout<< overall_stock_info_part_3[i].first.second.second << endl;
                        if(overall_stock_info_part_3[i].first.second.second==1){
                            cout << overall_stock_info_part_3[i].first.first[0].first << " " << max_profit_subset[iter].second << " s#" << endl;
                        }
                        else if(overall_stock_info_part_3[i].first.second.second==-1){
                            cout << overall_stock_info_part_3[i].first.first[0].first << " " << max_profit_subset[iter].second << " b#" << endl;
                        }
                        overall_stock_info_part_3[i].second-=max_profit_subset[iter].second;
                        iter--;
                        //cout << endl;
                        if(overall_stock_info_part_3[i].second==0){
                            overall_stock_info_part_3.erase(overall_stock_info_part_3.begin()+i);
                            i++;
                        } 
                    }
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{

    int partNumber = stoi(argv[1]);

    switch (partNumber)
    {
    case 1:
        part1();
        break;
    case 2:
        part2();
        break;
    case 3:
        part3();
        break;
    default:
        cout << "Invalid part number\n";
        return 1;
    }

    return 0;
}