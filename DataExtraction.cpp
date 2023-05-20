#include "DataExtraction.h"
#include "Stocks.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>
#include <fstream>
#include <locale>
#include <vector>
#include "Calculation.h"
#include <map>
#include "curl/curl.h"

typedef vector<string> String;
namespace fre{
    //Make sure to rename the file to this title
    const char* SymbolsFile = "Russell3000EarningsAnnouncements.csv"; 
    //const char* SymbolsFile = "Russell3000_Steven_test.csv";
    
    //Function to format the date in the form YYYY-MM-DD (2022-03-18)
    string formatDate(string Date)
    {
        map<string, string> months = { { "JAN", "01" },{ "FEB", "02" },{ "MAR", "03" },{ "APR", "04" }, { "MAY", "05" },{ "JUN", "06" },{ "JUL", "07" },{ "AUG", "08" },
		{ "SEP", "09" },{ "OCT", "10" },{ "NOV", "11" },{ "DEC", "12" } };
	
		string day = Date.substr(0, 2);
		string month = Date.substr(3, 3);
		for (int i = 0; i < 3; i++)
		{
			month[i] = toupper(month[i]);
		}
		month = months[month];
		string year = Date.substr(9, 2);
	
		string result = "20" + year + "-" + month + "-" + day;
		return result;
    }
    
    //Loading the data from Russell Earnings csv file into a map containing ticker as key and Stock object as values
    void LoadEarningsData(map<string, Stocks> &stockData)
    {
        ifstream fin;
        
        //reading from csv file
		fin.open(SymbolsFile, ios::in);
		
		//declaring variables to store the data from csv file
		string line, ticker, earnings_date, period_ending, estimated_earnings, reported_earnings, surprise_earnings, surprise_percent;
		
		getline(fin, line);
		while (getline(fin, line))
		{
		    // getline(fin, line);
			stringstream sin(line);
			getline(sin, ticker, ',');
			getline(sin, earnings_date, ',');
			getline(sin, period_ending, ',');
			getline(sin, estimated_earnings, ',');
			getline(sin, reported_earnings, ',');
			getline(sin, surprise_earnings, ',');
			getline(sin, surprise_percent);
			
			//create a temporary stock object to store in the map
			Stocks stock;
			
			stock.SetTicker(ticker);
			stock.SetEarningsDate(formatDate(earnings_date));
			stock.SetEarningsPeriod(period_ending);
			stock.SetEstimatedEarnings(stod(estimated_earnings));
			stock.SetReportedEarnings(stod(reported_earnings));
			stock.SetSurpriseEarnings(stod(surprise_earnings));
			stock.SetSurprisePercent(stod(surprise_percent));
			
			
			//store the stock object in the map for Earnings Announcement Data
			stockData[ticker] = stock;
		}
    }
    
	//function to update the Stocks object with the data from EOD
    void updateStock(stringstream &sData, Stocks* stock)
	{
		String Date;
		Vector Open_price;
		Vector High_price;
		Vector Low_price;
		Vector Close_price;
		Vector Adjusted_close;
		Vector volume;
		
		string line, sDate, sOpen, sHigh, sLow, sClose, sAdjClose, sVolume;
		
		while (getline(sData, line)) {
			size_t found = line.find('-');
			if (found != std::string::npos)
			{
				//Date, Open, High, Low, Close, Adjusted_close, Volume
				stringstream sin(line);
				getline(sin, sDate, ',');
				Date.push_back(sDate);
				
				getline(sin, sOpen, ',');
				Open_price.push_back(stod(sOpen));
				
				getline(sin, sHigh, ',');
				High_price.push_back(stod(sHigh));
				
				getline(sin, sLow, ',');
				Low_price.push_back(stod(sLow));
				
				getline(sin, sClose, ',');
				Close_price.push_back(stod(sClose));
				
				getline(sin, sAdjClose, ',');
				Adjusted_close.push_back(stod(sAdjClose));
				
				getline(sin, sVolume);
				volume.push_back(stod(sVolume));
				
			}
		}
		
		stock->SetDate(Date);
		stock->SetOP(Open_price);
		stock->SetHP(High_price);
		stock->SetLP(Low_price);
		stock->SetCP(Close_price);
		stock->SetACP(Adjusted_close);
		stock->SetVol(volume);
		stock->CalculateReturns();
		stock->CalculateCumulativeReturns();
		
	}
	
	
	void* myrealloc(void* ptr, size_t size)
	{
		if (ptr)
			return realloc(ptr, size);
		else
			return malloc(size);
	}
	
	int write_data(void* ptr, size_t size, size_t nmemb, void* data)
	{
		size_t realsize = size * nmemb;
		struct MemoryStruct* mem = (struct MemoryStruct*)data;
		mem->memory = (char*)myrealloc(mem->memory, mem->size + realsize + 1);
		if (mem->memory) {
			memcpy(&(mem->memory[mem->size]), ptr, realsize);
			mem->size += realsize;
			mem->memory[mem->size] = 0;
		}
		return realsize;
	}
    
    //Getting stock data from EODHistoricalData
	void FetchStockData(map<string, Stocks> &stockData, String group_tickers)
	{
		//Declaring vector containing all the tickers
		String symbolList;
		
		// declaration of an object CURL 
		CURL* handle;
		CURLcode result;
	
		// set up the program environment that libcurl needs 
		curl_global_init(CURL_GLOBAL_ALL);
		
		// curl_easy_init() returns a CURL easy handle 
		handle = curl_easy_init();
		
		// if everything's all right with the easy handle... 
		if (handle)
		{
			string url_common = "https://eodhistoricaldata.com/api/eod/";
			// string start_date = "2022-01-01";
			// string end_date = "2022-12-31";
			string api_token = "6446c908805b55.15842892";  // Our API token
			
			for(int i = 0; i < (int)group_tickers.size(); i++) 
			{	
				
				if(group_tickers[i].length()==0)                          
				{
					continue;
				}
				
				string start_date = stockData[group_tickers[i]].GetStartDate();
				string end_date = stockData[group_tickers[i]].GetEndDate();
				struct MemoryStruct data;
				data.memory = NULL;
				data.size = 0;
				
				string symbol = group_tickers[i];
				
				string url_request = url_common + symbol + ".US?" + "from=" + start_date + "&to=" + end_date + "&api_token=" + api_token + "&period=d";
				
				//set the url to call the data from
				curl_easy_setopt(handle, CURLOPT_URL, url_request.c_str());
		
				//adding a user agent
				curl_easy_setopt(handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:74.0) Gecko/20100101 Firefox/74.0");
				curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
				curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
				
				//create a temporary pointer used to access the stock object values corresponding to the ticker
				Stocks *ticker = &stockData[symbol];
				
				//store the data in the Stock class using write_data function
				curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
				curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);
		
				result = curl_easy_perform(handle);	
				if (result != CURLE_OK)
				{
				    // Error Handling
					fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
					cout<<"\n\n\nError in reading data!\n\n\n";
				}
				
				stringstream sData;
				sData.str(data.memory);
				//Updating the stock object for the corresponding ticker
				updateStock(sData, ticker);
				free(data.memory);
			}
			
		}	
	
		else
		{
			fprintf(stderr, "Curl init failed!\n");
			cout << "\n\n\n Curl initialization failed! \n\n\n";
		}
	
		
		// cleanup since you've used curl_easy_init  
		curl_easy_cleanup(handle);
	
		// release resources acquired by curl_global_init() 
		curl_global_cleanup();
	}
	
	void FetchIWVData(map<string, Stocks> &IWVData, String &datesList)
	{
		
		CURL* handle;
		CURLcode result;

		curl_global_init(CURL_GLOBAL_ALL);

		handle = curl_easy_init();

		// if everything's all right with the easy handle...
		if (handle)
		{
			string url_common = "https://eodhistoricaldata.com/api/eod/";
			string start_date = "2022-03-01";
			string end_date = "2023-03-31";
			string api_token = "6446c908805b55.15842892";  // You must replace this API token with yours

			struct MemoryStruct data;
			data.memory = NULL;
			data.size = 0;

			string SYMBOL = "IWV";

			if (SYMBOL.back() == '\r') {
				SYMBOL.pop_back();
			}

			string url_request = url_common + SYMBOL + ".US?" + "from=" + start_date + "&to=" + end_date + "&api_token=" + api_token + "&period=d";
			curl_easy_setopt(handle, CURLOPT_URL, url_request.c_str());

			//adding a user agent
			curl_easy_setopt(handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:74.0) Gecko/20100101 Firefox/74.0");
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
			curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);

			curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
			curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);

			result = curl_easy_perform(handle);

			// check for errors
			if (result != CURLE_OK) {
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
				//return -1; Need to STOP EXECUTION HERE;
				return;
			}


			stringstream sData;
			sData.str(data.memory);
			string sValue, sDate;
			double dValue = 0;
			Vector Adjusted_close;
			string line;
			while (getline(sData, line))
			{
				size_t found = line.find('-');
				if (found != std::string::npos)
				{
					sDate = line.substr(0, line.find_first_of(','));
					line.erase(line.find_last_of(','));
					sValue = line.substr(line.find_last_of(',') + 1);
					dValue = strtod(sValue.c_str(), NULL);
					Adjusted_close.push_back(dValue);
					datesList.push_back(sDate);
				}
			}
			
			Stocks temp;
			
			temp.SetACP(Adjusted_close);
			temp.SetDate(datesList);
			
			IWVData[SYMBOL] = temp;
			
			free(data.memory);
			data.size = 0;
		}
		else
		{
			fprintf(stderr, "Curl init failed!\n");
			return;
		}
		// cleanup since you've used curl_easy_init
		curl_easy_cleanup(handle);

		// release resources acquired by curl_global_init()
		curl_global_cleanup();

	}
	

}