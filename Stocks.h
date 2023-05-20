#pragma once
#include <vector>
#include <cmath>
#include <string>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <map>
#include <iomanip>
#include <sstream>
#include "Calculation.h"

using namespace std;
using namespace std::chrono;

namespace fre{

typedef vector<string> String;

class Stocks {
    
    private:
        string ticker;
        string earnings_date;
        string earnings_period;
        String Date;
        double estimated_earnings;
        double reported_earnings;
        double surprise_earnings;
        double surprise_percent;
        Vector Open_price;
        Vector High_price;
    	Vector Low_price;
    	Vector Close_price;
    	Vector Adjusted_close;
    	Vector volume;
    	
    	Vector return_vector;
    	Vector cumulative_return_vector;
 
    	
    	string start_date;
    	string end_date;
    
    public:
        
        Stocks():
            ticker("NULL"),
            earnings_date(""),
            estimated_earnings(0.0),
            reported_earnings(0.0),
            surprise_earnings(0.0),
            surprise_percent(0.0)
        {
        }
    
        void SetTicker(const std::string& ticker_) {
            ticker= ticker_ ;
        }
    
        void SetEarningsDate(const string& earnings_date_) {
            earnings_date = earnings_date_;
        }
    
        void SetEstimatedEarnings(double estimated_earnings_) {
            estimated_earnings = estimated_earnings_;
        }
    
        void SetReportedEarnings(double reported_earnings_) {
            reported_earnings = reported_earnings_ ;
        }
    
        void SetSurpriseEarnings(double surprise_earnings_) {
            surprise_earnings = surprise_earnings_;
        }
    
        void SetSurprisePercent(double surprise_percent_) {
            surprise_percent = surprise_percent_;
        }
        
        void SetEarningsPeriod(const string& earnings_period_) {
            earnings_period = earnings_period_;
        }
        
    
        string GetTicker() const {
            return ticker;
        }
    
        string GetEarningsDate() const {
            return earnings_date;
        }
        
        string GetEarningsPeriod() const {
            return earnings_period;
        }
    
        double GetEstimatedEarnings() const {
            return estimated_earnings;
        }
    
        double GetReportedEarnings() const {
            return reported_earnings;
        }
    
        double GetSurpriseEarnings() const {
            return surprise_earnings;
        }
    
        double GetSurprisePercent() const {
            return surprise_percent;
        }
        
        void SetStartDate(const string& start_date_){
            start_date = start_date_;
        }
        
        void SetEndDate(const string& end_date_){
            end_date = end_date_;
        }
        void SetOP(Vector Open_price_){
            Open_price = Open_price_;
        }
    
        void SetHP(Vector High_price_){
            High_price = High_price_;
        }
        
        void SetLP(Vector Low_price_){
            Low_price = Low_price_;
        }
        
        void SetCP(Vector Close_price_){
            Close_price = Close_price_;
        }
        
        void SetACP(Vector Adjusted_close_){
            Adjusted_close = Adjusted_close_;
        }
        
        void SetVol(Vector volume_){
            volume = volume_;
        }
    
        void SetDate(String Date_){
            Date = Date_;
        }
        
        map<string, double> GetPrices() const {
            map<string, double> prices;
            for (size_t i = 0; i < Date.size(); ++i) {
                prices[Date[i]] = Adjusted_close[i];
            }
            return prices;
        }

        
        void CalculateReturns(){
            return_vector = cal_DailyRet(Adjusted_close);
        }
        
        void CalculateCumulativeReturns(){
            cumulative_return_vector = cal_CumulativeRet(Adjusted_close);
        }
        
        Vector Get_return_vector() const{
            return return_vector;
        }
        
        Vector Get_cumulative_return_vector() const{
            return cumulative_return_vector;
        }
        string GetStartDate() const{
            return start_date;
        }
        
        string GetEndDate() const{
            return end_date;
        }
        String GetDate() const{
            return Date;
        }
        Vector GetACP() const{
            return Adjusted_close;
        }

    };
}
