#include <cmath>
#include <vector>

using namespace std;

// simplified type
typedef vector<double> Vector;
typedef vector<Vector> Matrix;
typedef vector<Matrix> MatrixList;

Vector operator-(const Vector& v1, const Vector& v2);
Vector& operator+=(Vector& v1, const Vector& v2);
Vector& operator/=(Vector& v, int divisor);

Vector cal_DailyRet(Vector &prices);
// 3.c + 3.d    calculate the daily return from price series

Vector cal_ABRet(Vector &stock_prices, Vector &IWV_prices);
// 3.e  calculate abnormal return given the stock series and IWV price series

Vector cal_CumulativeRet(Vector &prices);
// 3.e  calculate the cumulative return from price series

Vector cal_AAR(Matrix &matrix);
// 3.f  calculate AAR series from given abnormal return matrix

Vector cal_CAAR(Vector &arr);
// 3.g  calculate CAAR value from given abnormal return matrix, return all cumulating vector

Vector cal_avg_AAR(MatrixList &matrix_list);
// 3.h.i  calculate average AAR series from given abnormal return matrix list

Vector cal_avg_CAAR(MatrixList &matrix_list);
// 3.h.i  calculate average CAAR value from given abnormal return matrix list, return all cumulating vector

Vector cal_std_AAR(MatrixList &matrix_list);
// 3.h.ii   calculate std AAR series from given abnormal return matrix list

Vector cal_std_CAAR(MatrixList &matrix_list);
// 3.h.ii   calculate std CAAR value from given abnormal return matrix list, return all cumulating vector

