#include "Calculation.h"
#include "Group.h"
#include <random>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
using namespace fre;

class Bootstrap {
public:
    Bootstrap(const map<string, Stocks>& group_data, const map<string, Stocks>& iwv_data, int N, int n_samples)
        : group_data_(group_data), iwv_data_(iwv_data), N_(N), n_samples_(n_samples) {}

    void PerformBootstrap() {
        MatrixList samples;
        for (int i = 0; i < n_samples_; ++i) {
            Matrix sample_abnormal_returns = GetSampleAbnormalReturns();
            samples.push_back(sample_abnormal_returns);
        }
        avg_aar = cal_avg_AAR(samples);
        avg_caar = cal_avg_CAAR(samples);
        std_aar = cal_std_AAR(samples);
        std_caar = cal_std_CAAR(samples);
    }
    
    Matrix GroupMatrix() const {
        Matrix result;
        result.push_back(avg_aar);
        result.push_back(avg_caar);
        result.push_back(std_aar);
        result.push_back(std_caar);
        return result;
    }
    
    const Vector& GetAvgAAR() const { return avg_aar; }
    const Vector& GetAvgCAAR() const { return avg_caar; }
    const Vector& GetStdAAR() const { return std_aar; }
    const Vector& GetStdCAAR() const { return std_caar; }

private:
    Matrix GetSampleAbnormalReturns() {
        vector<Stocks> stocks = GetRandomStocks(80);
        Matrix sample_abnormal_returns;
    
        for (const auto& stock : stocks) {
            map<string, double> stock_prices = stock.GetPrices();
            map<string, double> IWV_prices = iwv_data_.at("IWV").GetPrices();
    
            map<string, double> filtered_IWV_prices;
            for (const auto& date_price_pair : stock_prices) {
                if (IWV_prices.find(date_price_pair.first) != IWV_prices.end()) {
                    filtered_IWV_prices[date_price_pair.first] = IWV_prices.at(date_price_pair.first);
                }
            }
            Vector stock_prices_values(stock_prices.size());
            Vector IWV_prices_values(filtered_IWV_prices.size());
            
            transform(stock_prices.begin(), stock_prices.end(), stock_prices_values.begin(), [](const pair<string, double>& p) { return p.second; });
            transform(filtered_IWV_prices.begin(), filtered_IWV_prices.end(), IWV_prices_values.begin(), [](const pair<string, double>& p) { return p.second; });
            
            Vector abnormal_returns = cal_ABRet(stock_prices_values, IWV_prices_values);
            sample_abnormal_returns.push_back(abnormal_returns);
        }
        return sample_abnormal_returns;
    }

    vector<Stocks> GetRandomStocks(int n) {
        vector<Stocks> all_stocks;
        
        transform(group_data_.begin(), group_data_.end(),
                  back_inserter(all_stocks),
                  [](const pair<string, Stocks>& p) { 
                        return p.second; 
                        }
                  );
                  
        vector<Stocks> new_all_stocks;
        for (const auto& stock : all_stocks) {
            map<string, double> stock_prices = stock.GetPrices();
            if (stock_prices.size() >= (unsigned)2*N_){
                new_all_stocks.push_back(stock);
            }
        }
        all_stocks = new_all_stocks;

        random_device rd;
        mt19937 g(rd());
        shuffle(all_stocks.begin(), all_stocks.end(), g);
        all_stocks.resize(n);
        return all_stocks;
    }

    const map<string, Stocks>& group_data_;
    const map<string, Stocks>& iwv_data_;
    unsigned N_;
    int n_samples_;
    Vector avg_aar;
    Vector avg_caar;
    Vector std_aar;
    Vector std_caar;
};

/*
int main() {
    Group group;
    // Add stocks to the group...

    int N = 60;
    int num_samples = 40;
    Bootstrap bootstrap(group, N, num_samples);
    bootstrap.PerformBootstrap();

    Vector avg_aar = bootstrap.GetAverageAAR();
    Vector avg_caar = bootstrap.GetAverageCAAR();
    Vector std_aar = bootstrap.GetStandardDeviationAAR();
    Vector std_caar = bootstrap.GetStandardDeviationCAAR();
    // Output the results
    std::cout << "Average AAR:\n";
    for (const auto& value : avg_aar) {
        std::cout << value << ' ';
    }
    std::cout << "\n\nAverage CAAR:\n";
    for (const auto& value : avg_caar) {
        std::cout << value << ' ';
    }
    std::cout << "\n\nStandard Deviation AAR:\n";
    for (const auto& value : std_aar) {
        std::cout << value << ' ';
    }
    std::cout << "\n\nStandard Deviation CAAR:\n";
    for (const auto& value : std_caar) {
        std::cout << value << ' ';
    }
    std::cout << '\n';
    
    return 0;
}
*/