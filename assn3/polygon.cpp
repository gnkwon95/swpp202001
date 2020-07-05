#include <cstdint>
#include <vector>
#include <iostream>

using namespace std;

uint64_t area(vector<pair<int, int>> &points) {
  // This code has signed overflows. :)
  // Please fix this so it correctly evaluates area..!
  uint64_t total = 0;
  size_t n = points.size();
  
  int64_t sum;
  int inc = 0;
  int64_t list[n]; 
  for (unsigned i = 0; i < n; i++) {
    unsigned j = (i + 1) % n; // x coordinate of next object
    int x_i = points[i].first;
    int y_i = points[i].second;
    int x_j = points[j].first;
    int y_j = points[j].second;
    sum = ((int64_t)(x_i) * ( (int64_t)y_j - (int64_t)y_i) - ((int64_t)y_i) * ((int64_t)x_j - (int64_t)x_i));
    list[i] = sum;
  }

    while(1){
      int swapped = 0;

      for(int i=0; i<n-1; i++){
        if (list[i]>list[i+1]){
          int64_t tmp = list[i];
          list[i] = list[i+1];
          list[i+1] = tmp;
          swapped = 1;
        }
      }
      if(swapped == 0){
        break;
      }
    }
    uint64_t sum2 = 0;
    for (int i=0; i<n/2; i++){
      if (list[n-1-i]%2 == 1 || list[n-1-i]%2 == -1){
        if (list[n-1-i] >= 0) inc++; else inc--;
      }
      sum2 += list[n-1-i]/2;

      if (list[i]%2 == 1 || list[i]%2 == -1){
        if (list[i] >= 0) inc++; else inc--;
      }
      sum2 += list[i]/2;

      
    }

    if (n%2 == 1){ 
      if (list[n/2]%2 == 1 || list[n/2]%2 == -1) {
        if (list[n/2]>=0) inc++; else inc--;
      }
      sum2 += list[n/2]/2;
    }

    sum2 += inc/2;
    total = sum2;
  

  return total;
}