#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <array>
#include <cmath>
#include "Iris.h"

using std::array;
using std::string;
using std::ifstream;
using std::cout;
using std::endl;
using std::vector;
using std::min;

vector<Iris> data;
const int ITERATIONS = 20;


void readCSV()
{
    string fileName = "iris.csv";
    ifstream file(fileName);
    string temp;
    getline(file, temp);
    int id = 0;
    while(!file.eof())
    {
        getline(file, temp, ',');

        getline(file, temp, ',');
        double sLength = stod(temp);

        getline(file, temp, ',');
        double sWidth = stod(temp);
        
        getline(file, temp, ',');
        double pLength = stod(temp);

        getline(file, temp, ',');
        double pWidth = stod(temp);

        getline(file, temp, '\n');

        data.push_back(Iris(id++, {sLength, sWidth, pLength, pWidth}, temp));
    }
}

array<vector<double>, 3>  initClusters()
{
    srand(time(NULL));
    int first = rand() % data.size();
    int second = rand() % data.size();
    int third = rand() % data.size();
    while(second == first || third == first || third == second)
    {
        second = rand() % data.size();
        third = rand() % data.size();
    }
    return {data[first].getVals(), data[second].getVals(), data[third].getVals()};
}

double euclideanDist(vector<double> left, vector<double> right)
{
    double sum = 0;
    for (int i = 0; i < 4; i++)
    {
         sum += pow(left[i] - right[i], 2);
    }
    
    return sqrt(sum);
}

double euclideanDist_modified(vector<double> left, vector<double> right)
{
    double sum = 0;
    for (int i = 0; i < 4; i++)
    {
         sum += pow(left[i]*left[i] - right[i]*right[i], 2);
    }
    
    return sqrt(sum);
}

double manhattanDist(vector<double> left, vector<double> right)
{
    double sum = 0;
    for (int i = 0; i < 4; i++)
    {
         sum += abs(left[i] - right[i]);
    }
    
    return sum;
}

void updateCluster(Iris& iris, array<vector<double>, 3> centroids, int metric)
{
    double dist[3];
    for (size_t i = 0; i < 3; i++)
    {   
        if (metric == 0)
            dist[i] = euclideanDist(iris.getVals(), centroids[i]);
     
        else if(metric == 1)
            dist[i] = euclideanDist_modified(iris.getVals(), centroids[i]);

        else
            dist[i] = manhattanDist(iris.getVals(), centroids[i]);
    }
    iris.setCluster(1);
    for (size_t i = 1; i < 3; i++)
    {
        if(dist[i] < dist[iris.getCluster()-1]) iris.setCluster(i+1);
    }
}

array<vector<double>, 3> updateCentroids()
{
    array<vector<double>, 3> centroids;
    centroids[0] = {0,0,0,0};
    centroids[1] = {0,0,0,0};
    centroids[2] = {0,0,0,0};
    int counter[3] = {0};
    for(Iris p : data)
    {
        for (size_t i = 0; i < 4; i++)
        {
            centroids[p.getCluster() - 1][i] += p[i];
        }
        counter[p.getCluster() - 1]++;
    }

    for (size_t i = 0; i < 4; i++)
    {
        centroids[0][i] = centroids[0][i]/counter[0];
        centroids[1][i] = centroids[1][i]/counter[1];
        centroids[2][i] = centroids[2][i]/counter[2];
        
    }
    return centroids;
}

void printInfo()
{
    int count[3] = {0};
    cout << "Printing each item's cluster and class: \n";
    for (Iris p : data)
    {
        cout << p.getCluster() << ' ' << p.getClass() << endl;
        count[p.getCluster()-1]++;
    }
    cout << "Cluster 1: " <<count[0] << " items\n" 
         << "Cluster 2: " <<count[1] << " items\n"
         << "Cluster 3: " <<count[2] << " items\n";
    
}

void kMeans(int metric)
{
    array<vector<double>, 3> centroids = initClusters();
    bool done = false;
    int iteration = 0;
    while(!done || iteration > ITERATIONS)
    {
        for(Iris &p : data)
        {
            updateCluster(p, centroids, metric);
        }
        array<vector<double>, 3> newCentroids = updateCentroids();
        if(newCentroids == centroids) done = true;
        else centroids = newCentroids;

        iteration++;
    }
    cout << "Number of iterations: " << iteration << endl;
    cout << "Metric: ";
    switch (metric)
    {
    case 1:
        cout << "modified";
    case 0:
        cout << " Euclidean" << endl;
        break;
    default:
        cout << "Manhattan" << endl;
    }
}

int main(int argc, char* argv[])
{
    readCSV();
    kMeans(atoi(argv[1]));
    printInfo();

    return 0;
}