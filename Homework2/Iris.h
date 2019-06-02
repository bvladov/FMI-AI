#include <vector>
#include <string>
using std::vector;
using std::string;

class Iris{

private:
    int id, cluster;
    vector<double> vals;
    string classAttr;

public:
    Iris(int id, vector<double> vals, string classAttr): id(id), vals(vals), cluster(0), classAttr(classAttr)
    {

    }

    int getId()
    {
        return id;
    }

    int getCluster()
    {
        return cluster;
    }

    void setCluster(int val)
    {
        cluster = val;
    }

    vector<double> getVals()
    {
        return vals;
    }

    string getClass()
    {
        return classAttr;
    }
    double operator[](int index)
    {
        return vals[index];
    }
};