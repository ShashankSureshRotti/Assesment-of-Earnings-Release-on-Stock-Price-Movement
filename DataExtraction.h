#pragma once
#include <map>
#include <vector>
#include "Stocks.h"
namespace fre{
    
     struct MemoryStruct 
    {
    	char* memory;
    	size_t size;
    };
    
    string formatDate(string Date);
    
    void LoadEarningsData(map<string, Stocks> &stockData);
    
    int write_data(void* ptr, size_t size, size_t nmemb, void* data);
    
    void FetchStockData(map<string, Stocks> &stockData, String tickers);
    
    void FetchIWVData(map<string, Stocks> &IWVData, String &datesList);
    
    

}