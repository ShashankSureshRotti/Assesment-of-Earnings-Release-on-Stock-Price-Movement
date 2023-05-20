#include "gnuplot.h"

void plot::plotResult() const
{
    
    int dataSize = yData1.size();
    vector<double> xData(dataSize);
    int N = dataSize/2;
    
    for (int i = - N +1 ; i <= N; i++) {
        xData[N + i - 1] = i;
    }
    
    
    FILE *gnuplotPipe,*tempDataFile;
    const char *tempDataFileName1 = "Beat";
    const char *tempDataFileName2 = "Meet";
    const char *tempDataFileName3 = "Miss";
    double x1, y1, x2, y2, x3, y3;

    const char *xLabel = "Announcement Date";
    gnuplotPipe = popen("gnuplot -persist", "w");
    
    fprintf(gnuplotPipe, "set grid\n");
    fprintf(gnuplotPipe, "set title '%s'\n", title);
    fprintf(gnuplotPipe, "set arrow from 0,graph(0,0) to 0,graph(1,1) nohead lc rgb 'red'\n");
    fprintf(gnuplotPipe, "set xlabel '%s'\n set ylabel '%s'\n", xLabel, yLabel);
    
    if (gnuplotPipe) {
        fprintf(gnuplotPipe,"plot \"%s\" with lines, \"%s\" with lines, \"%s\" with lines\n", tempDataFileName1, tempDataFileName2, tempDataFileName3);
        fflush(gnuplotPipe);
        
        
        tempDataFile = fopen(tempDataFileName1,"w");
        for (int i=0; i < dataSize; i++) {
            x1 = xData[i];
            y1 = yData1[i];
            fprintf(tempDataFile,"%lf %lf\n",x1,y1);
        }
        fclose(tempDataFile);
        
        
        tempDataFile = fopen(tempDataFileName2,"w");
        for (int i=0; i < dataSize; i++) {
            x2 = xData[i];
            y2 = yData2[i];
            fprintf(tempDataFile,"%lf %lf\n",x2,y2);
        }
        fclose(tempDataFile);
        
        
        tempDataFile = fopen(tempDataFileName3,"w");
        for (int i=0; i < dataSize; i++) {
            x3 = xData[i];
            y3 = yData3[i];
            fprintf(tempDataFile,"%lf %lf\n",x3,y3);
        }
        
        fclose(tempDataFile);
        printf("press enter to continue...");
        getchar();
    } else {        
        printf("gnuplot not found...");    
    }
} 