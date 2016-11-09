/*
 * k-means.h - The K-means class.
 *
 * Author: WeeSan Lee <weesan@weesan.com>
 */

#ifndef K_MEANS_H
#define K_MEANS_H

#include <vector>
#ifndef INT32_MAX
#define INT32_MAX              (2147483647)
#endif

using namespace std;

// T can be int, float and double
// N is 2 for 2D vector, 3 for 3D vector and so on.  Default is 2.
template <class T, int N = 2>
class KmeansData : public vector<T> {
private:
    int iCluster;
    
public:
    KmeansData(void) :
	vector<T>(N, 0),
	iCluster(0) {
    }
    int cluster(void) {
	return (iCluster);
    }
    void cluster(int _iCluster) {
	iCluster = _iCluster;
    }
};

template <class T, int N = 2>
class Kmeans : public vector<KmeansData<T, N> > {
private:
    int iSteps;
    vector<KmeansData<T, N> > cClusterHeads;

public:
    Kmeans(int iClusters) :
	iSteps(0) {
	cClusterHeads.resize(iClusters);
    }
    int steps(void) const {
	return (iSteps);
    }
    vector<KmeansData<T, N> > &clusterHeads(void) {
	return (cClusterHeads);
    }
    bool nextStep(void) {
	int iSize = vector<KmeansData<T, N> >::size();
	// Randomly pick some cluster heads initially
	if (iSteps++ == 0) {
	    for (int i = 0; i < cClusterHeads.size(); i++) {
		cClusterHeads[i] = (*this)[random() % iSize];
		//cClusterHeads[i] = (*this)[i];
	    }
	}
	// Cluster each data point to the nearest cluster head
	for (int i = 0; i < iSize; i++) {
	    int iCluster = 0;
	    T tMin = INT32_MAX;
	    for (int j = 0; j < cClusterHeads.size(); j++) {
		T tDist = 0;
		for (int k = 0; k < (*this)[i].size(); k++) {
		    T tDiff = (*this)[i][k] - cClusterHeads[j][k];
		    tDist += tDiff * tDiff;
		}
		// No need to call sqrt() to save computation
		if (tDist < tMin) {
		    tMin = tDist;
		    iCluster = j;
		}
	    }
	    (*this)[i].cluster(iCluster);
	}
	// Keep the old cluster heads and reset the current
	vector<KmeansData<T, N> > cOldClusterHeads(cClusterHeads.size());
	for (int j = 0; j < cClusterHeads.size(); j++) {
	    for (int k = 0; k < cClusterHeads[j].size(); k++) {
		cOldClusterHeads[j][k] = cClusterHeads[j][k];
		cClusterHeads[j][k] = 0;
	    }
	}
	// Re-compute the new cluster heads based on the average
	// of all the data points from the same cluster
	vector<int> cCount(cClusterHeads.size(), 0);
	for (int i = 0; i < iSize; i++) {
	    int iCluster = (*this)[i].cluster();
	    cCount[iCluster]++;
	    for (int k = 0; k < cClusterHeads[iCluster].size(); k++) {
		cClusterHeads[iCluster][k] += (*this)[i][k];
	    }
	}
	for (int j = 0; j < cClusterHeads.size(); j++) {
	    for (int k = 0; k < cClusterHeads[j].size(); k++) {
		if (cCount[j]) {
		    cClusterHeads[j][k] /= cCount[j];
		}
	    }
	}
	// Check if the new heads is same as the old ones
	for (int j = 0; j < cClusterHeads.size(); j++) {
	    for (int k = 0; k < cClusterHeads[j].size(); k++) {
		if (cClusterHeads[j][k] != cOldClusterHeads[j][k]) {
		    return (false);
		}
	    }
	}
	return (true);
    }
};

#endif // K_MEANS_H
