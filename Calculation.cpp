#include "Calculation.h"
#include <vector>
#include <numeric>
#include <stdexcept>
#include <iostream>

using namespace std;

Vector operator-(const Vector& v1, const Vector& v2) 
{
    if (v1.size() != v2.size()) {
        throw std::invalid_argument("Vector size mismatch.");
    }
    Vector result(v1.size());
    for (unsigned int i = 0; i < v1.size(); i++) {
        result[i] = v1[i] - v2[i];
    }
    return result;
}

Vector& operator+=(Vector& v1, const Vector& v2) {
    if (v1.size() != v2.size()) {
        throw std::invalid_argument("Vector size mismatch.");
    }
    for (unsigned int i = 0; i < v1.size(); i++) {
        v1[i] += v2[i];
    }
    return v1;
}

Vector& operator/=(Vector& v, int divisor) {
    if (divisor == 0) {
        throw std::invalid_argument("Division by zero.");
    }
    for (unsigned int i = 0; i < v.size(); i++) {
        v[i] /= divisor;
    }
    return v;
}


Vector cal_DailyRet(Vector &prices)
{
    // 3.c + 3.d
    // input: prices series (Vector) 
    // important to get extra day for return calculation
    // output: Daily Return Series (Vector)  t = -60, -59,…-1, 0, 1,…, 59, 60
    
    Vector daily_ret;
    double returns = 0.0;
    int n = prices.size();
    for (int i=0; i <= n-2; i++)
    {
        returns = (prices[i+1] - prices[i])/ prices[i];
        daily_ret.push_back(returns);
    }

    return daily_ret;
}

Vector cal_CumulativeRet(Vector &prices)
{
    Vector daily_ret = cal_DailyRet(prices);
    Vector cum_ret;
    double returns = 1.0;
    int n = daily_ret.size();
    for (int i=0; i < n; i++)
    {
        returns = (daily_ret[i]+1)*returns;
        cum_ret.push_back(returns);
    }
    
    return cum_ret;
}

Vector cal_ABRet(Vector &stock_prices, Vector &IWV_prices)
{
    // 3.e
    // input: stockPrices [-N-1,N+1] stock price series; IWVPrices [-N-1,N+1] IWV price series.
    // output: abnormal price series.
    
	Vector stock_ret=cal_DailyRet(stock_prices);                // get daily return
	Vector IWV_ret=cal_DailyRet(IWV_prices);                    // get IWV return
	Vector abnormal_ret = stock_ret - IWV_ret;                  // #vector = vector - vector !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    
	return abnormal_ret;
}


// ============================================================================================================



Vector cal_AAR(Matrix &matrix)
{
    // 3.f
    // input: matrix is the matrix of 1 group of abnormal return
    //          with every row is a abnormal return vector of one stock
    //          There is 80 stocks in a group, so there is 80 rows
    // output: the average ARR for this group (80 stocks)
    int M = static_cast<int>(matrix.size());    // The number of stock in one group (80)
    Vector arr = matrix[0];                    // initialize the result vector
    
    for(int i = 1; i<M ; i ++)
    {
        arr += matrix[i];     //#vector += vector  !!!!!!!!!!!!!!!
    }
    arr /= M;                  //#vector/=int !!!!!!!!!!!!!!!!!!
    return arr;     // vector [2N]
}


Vector cal_CAAR(Vector &arr)
{
    // 3.g
    // input: the average ARR vector for one group (80 stocks)
    // output: the carr vector for this group
    int length = static_cast<int>(arr.size());     // The length of aar vector
    Vector carr = {};
    double result = 0.0;
    for (int i = 0; i < length; i++)
    {
        result += arr[i];
        carr.push_back(result);
    }
    
    return carr;     // vector [2N]
}



// ============================================================================================================


Vector cal_avg_AAR(MatrixList &matrix_list)
{
    // 3.h.i
    // input: a vector of matrix, each maxtrix is from one sampling
    //          The matrix is the same with the input of cal_ARR
    // output: average ARR vector for 40 samplings
    int sampling_num = static_cast<int>(matrix_list.size());        // The number of samplings (40)
    Vector avg_arr = cal_AAR(matrix_list[0]);
    for(int i = 1; i < sampling_num; i++)
    {
        // calculate current_arr for the current sampling
        Vector current_arr = cal_AAR(matrix_list[i]);
        // add current_arr as a new row to avg_arr
        avg_arr += current_arr;
    }
    avg_arr /= sampling_num;
    return avg_arr;     // vector [2N] 
}


Vector cal_avg_CAAR(MatrixList &matrix_list)
{
    // 3.h.i
    // input: a vector of matrix, each maxtrix is from one sampling
    //          The matrix is the same with the input of cal_ARR
    // output: average CARR for 40 samplings
    
    int sampling_num = static_cast<int>(matrix_list.size());        // The number of samplings (40)
    Vector avg_arr = cal_AAR(matrix_list[0]); 
    Vector avg_carr = cal_CAAR(avg_arr);
    Vector current_arr;
    Vector current_carr;
    for(int i = 1; i < sampling_num; i++)
    {
        // calculate current_carr for the current sampling
        current_arr = cal_AAR(matrix_list[i]);
        current_carr = cal_CAAR(current_arr);
        // add current_carr as a new row to avg_arr
        avg_carr += current_carr;
    }
    avg_carr /= sampling_num;
    
    return avg_carr;     // vector [2N]
}



// ============================================================================================================

Vector cal_std_AAR(MatrixList &matrix_list)
{
    // 3.h.ii
    // input: a vector of matrix, each maxtrix is from one sampling
    //          The matrix is the same with the input of cal_AAR
    //          T is the end day of cumm
    // output: std ARR vector for 40 samplings
    
    int sampling_num = static_cast<int>(matrix_list.size());        // The number of samplings (40)
    int arr_length = static_cast<int>(matrix_list[0][0].size());    // The length of AAR vector (2N)
    
    Matrix result_matrix;  // initialize an empty matrix to store each sampling's AAR vector
    Matrix current_matrix;
    Vector current_arr;
    for(int i = 0; i < sampling_num; i++)
    {
        // calculate current_arr for the current sampling
        current_matrix = matrix_list[i];
        current_arr = cal_AAR(current_matrix);
        
        // add current_arr as a new row to result_matrix
        result_matrix.push_back(current_arr);
    }
    
    Vector std_arr(arr_length); // initialize an empty vector to store the standard deviation of each column
    
    for(int j = 0; j < arr_length; j++)
    {
        // get the j-th column of result_matrix
        Vector col_j;
        for(int i = 0; i < sampling_num; i++)
        {
            col_j.push_back(result_matrix[i][j]);
        }
        // calculate the standard deviation of the j-th column and store it in std_arr
        double mean = accumulate(col_j.begin(), col_j.end(), 0.0) / sampling_num;
        double variance = 0.0;
        for (int i = 0; i < sampling_num; ++i) 
        {
            variance += pow(col_j[i] - mean, 2);
        }
        variance /= sampling_num;
        double stddev = sqrt(variance);
        std_arr[j] = stddev;
    }
    
    return std_arr;
}


Vector cal_std_CAAR(MatrixList &matrix_list)
{
    int sampling_num = static_cast<int>(matrix_list.size());        // The number of samplings (40)
    int arr_length = static_cast<int>(matrix_list[0][0].size());    // The length of AAR vector (2N)
    
    Matrix result_matrix;  // initialize an empty matrix to store each sampling's AAR vector
    Matrix current_matrix;
    Vector current_arr;
    Vector current_carr;
    for(int i = 0; i < sampling_num; i++)
    {
        // calculate current_arr for the current sampling
        current_matrix = matrix_list[i];
        current_arr = cal_AAR(current_matrix);
        current_carr = cal_CAAR(current_arr);
        
        // add current_arr as a new row to result_matrix
        result_matrix.push_back(current_carr);
    }
    
    Vector std_carr(arr_length); // initialize an empty vector to store the standard deviation of each column
    
    for(int j = 0; j < arr_length; j++)
    {
        // get the j-th column of result_matrix
        Vector col_j;
        for(int i = 0; i < sampling_num; i++)
        {
            col_j.push_back(result_matrix[i][j]);
        }
        // calculate the standard deviation of the j-th column and store it in std_arr
        double mean = accumulate(col_j.begin(), col_j.end(), 0.0) / sampling_num;
        double variance = 0.0;
        for (int i = 0; i < sampling_num; ++i) 
        {
            variance += pow(col_j[i] - mean, 2);
        }
        variance /= sampling_num;
        double stddev = sqrt(variance);
        std_carr[j] = stddev;
    }
    
    return std_carr;
}
