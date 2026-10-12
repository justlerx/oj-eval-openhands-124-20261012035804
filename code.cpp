#include <cstdio>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

class SegmentTree {
private:
    vector<int> max_tree;  // maximum value in each segment
    vector<int> lazy;      // lazy propagation
    int n;

    void push(int v) {
        if (lazy[v] != 0) {
            max_tree[v*2] += lazy[v];
            max_tree[v*2+1] += lazy[v];
            lazy[v*2] += lazy[v];
            lazy[v*2+1] += lazy[v];
            lazy[v] = 0;
        }
    }

    void update(int v, int tl, int tr, int l, int r, int addend) {
        if (l > r) return;
        if (l == tl && tr == r) {
            max_tree[v] += addend;
            lazy[v] += addend;
        } else {
            push(v);
            int tm = (tl + tr) / 2;
            update(v*2, tl, tm, l, min(r, tm), addend);
            update(v*2+1, tm+1, tr, max(l, tm+1), r, addend);
            max_tree[v] = max(max_tree[v*2], max_tree[v*2+1]);
        }
    }

    int query(int v, int tl, int tr, int l, int r) {
        if (l > r) return 0;
        if (l <= tl && tr <= r) {
            return max_tree[v];
        }
        push(v);
        int tm = (tl + tr) / 2;
        return max(query(v*2, tl, tm, l, min(r, tm)), 
                   query(v*2+1, tm+1, tr, max(l, tm+1), r));
    }

public:
    SegmentTree(int size) {
        n = size;
        max_tree.resize(4 * n, 0);
        lazy.resize(4 * n, 0);
    }

    int query_range(int l, int r) {
        return query(1, 0, n-1, l, r);
    }
    
    void update_range(int l, int r, int addend) {
        update(1, 0, n-1, l, r, addend);
    }
};

int main() {
    int x, k, p;
    scanf("%d %d %d", &x, &k, &p);
    
    // Handle edge case where there's only one station
    if (x == 1) {
        for (int i = 0; i < p; i++) {
            int u, v, n;
            scanf("%d %d %d", &u, &v, &n);
            printf("N\n");
        }
        return 0;
    }
    
    SegmentTree st(x-1);
    
    for (int i = 0; i < p; i++) {
        int u, v, n;
        scanf("%d %d %d", &u, &v, &n);
        
        // If u >= v, the booking is invalid
        if (u >= v) {
            printf("N\n");
            continue;
        }
        
        u--; v--; // convert to 0-indexed
        
        // Query the maximum number of seats in the range [u, v-1]
        int max_seats = st.query_range(u, v-1);
        
        if (max_seats + n <= k) {
            // Can fulfill the booking
            printf("T\n");
            // Update the range [u, v-1] by adding n seats
            st.update_range(u, v-1, n);
        } else {
            printf("N\n");
        }
    }
    
    return 0;
}