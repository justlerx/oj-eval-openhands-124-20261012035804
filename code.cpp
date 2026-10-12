#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int x, k, p;
    scanf("%d %d %d", &x, &k, &p);
    
    // Create an array to track the number of seats booked on each segment
    // segments[i] represents the number of seats booked on the segment from station i to i+1
    vector<int> segments(x, 0);  // for x stations, we have x-1 segments, but we'll use x for simplicity
    
    for (int i = 0; i < p; i++) {
        int u, v, n;
        scanf("%d %d %d", &u, &v, &n);
        
        // If u >= v, the booking is invalid (can't book from a station to itself or backwards)
        if (u >= v) {
            printf("N\n");
            continue;
        }
        
        // If x == 1, no segments exist, so booking is invalid
        if (x == 1) {
            printf("N\n");
            continue;
        }
        
        u--; v--; // convert to 0-indexed
        
        // Check if we have enough seats on all segments from u to v-1
        int max_seats = 0;
        for (int j = u; j < v; j++) {
            if (segments[j] > max_seats) {
                max_seats = segments[j];
            }
        }
        
        if (max_seats + n <= k) {
            // Can fulfill the booking - update all segments from u to v-1
            for (int j = u; j < v; j++) {
                segments[j] += n;
            }
            printf("T\n");
        } else {
            printf("N\n");
        }
    }
    
    return 0;
}