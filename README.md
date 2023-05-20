# Assesment-of-Earnings-Release-on-Stock-Price-Movement
Built a stock analysis tool in C++ to retrieve data using Libcurl API, analyze performance based on quarterly financial results, and use Gnuplot to plot for visualization.

## Introduction
In this project, we retrieve EOD historical data of Russell 3000 stocks using the LibCurl API. The data is processed and analyzed to perform various calculations, including Average Abnormal Returns (AAR), Cumulative Average Abnormal Return (CAAR), and Average Standard Deviation for different groups of stocks.

## Features
1. Retrieve EOD historical data of Russell 3000 stocks using the LibCurl API.
2. Utilize multi-threading in C++ to improve runtime by 50%.
3. Store EPS price information using STL maps with stock ticker as the key and stock object as the value.
4. Group stocks as Beat/Meet/Miss based on the Earnings Surprise Ratio.
5. Implement a Bootstrapping method to simulate random 80 stocks from the group of 3.
6. Calculate Average Abnormal Returns (AAR), Cumulative Average Abnormal Return (CAAR), and Average Standard Deviation for each group.
7. Build a user interface (UI) for easy interaction.
8. Allow user input of N days for fetching data, creating groups, and performing calculations.
9. Display detailed data about a stock, group, and plot graphs for AAR and CAAR for all the groups.

## Getting Started
To get started with the project, follow these steps:

Clone the repository: git clone [https://github.com/username/project.git](https://github.com/ShashankSureshRotti/Assesment-of-Earnings-Release-on-Stock-Price-Movement.git)
Install the required dependencies: Install GNU Plot plug-in
Set up the necessary API credentials.
Compile and run the code.

## Usage
To use the project, follow these steps:

Open the UI interface.
Enter the desired number of days for fetching data.
Press appropriate option to perform necessary calculations such as creating groups, calculating AAR, CAAR, and Standard Deviation.
View the results, including detailed stock information and plotted graphs.

