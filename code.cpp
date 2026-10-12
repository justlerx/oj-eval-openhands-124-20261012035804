#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

class SegmentTree {
private:
    vector<int> tree;  // stores maximum value in each segment
    vector<int> lazy;  // lazy propagation for range updates
    int n;

    void push(int v) {
        if (lazy[v] != 0) {
            tree[v*2] += lazy[v];
            tree[v*2+1] += lazy[v];
            lazy[v*2] += lazy[v];
            lazy[v*2+1] += lazy[v];
            lazy[v] = 0;
        }
    }

    void update(int v, int tl, int tr, int l, int r, int addend) {
        if (l > r) 
            return;
        if (l == tl && tr == r) {
            tree[v] += addend;
            lazy[v] += addend;
        } else {
            push(v);
            int tm = (tl + tr) / 2;
            update(v*2, tl, tm, l, min(r, tm), addend);
            update(v*2+1, tm+1, tr, max(l, tm+1), r, addend);
            tree[v] = max(tree[v*2], tree[v*2+1]);
        }
    }

    int query(int v, int tl, int tr, int l, int r) {
        if (l > r)
            return 0;  // 0 is the minimum possible value
        if (l <= tl && tr <= r)
            return tree[v];
        push(v);
        int tm = (tl + tr) / 2;
        return max(query(v*2, tl, tm, l, min(r, tm)), 
                   query(v*2+1, tm+1, tr, max(l, tm+1), r));
    }

public:
    SegmentTree(int size) {
        n = size;
        tree.resize(4 * n, 0);
        lazy.resize(4 * n, 0);
    }

    void update_range(int l, int r, int addend) {
        update(1, 0, n-1, l, r, addend);
    }

    int query_range(int l, int r) {
        return query(1, 0, n-1, l, r);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int x, k, p;
    cin >> x >> k >> p;
    
    // Create segment tree for x-1 segments (between x stations)
    SegmentTree st(x-1);
    
    for (int i = 0; i < p; i++) {
        int u, v, n;
        cin >> u >> v >> n;
        
        // If u >= v, the booking is invalid (can't book from a station to itself or backwards)
        if (u >= v) {
            cout << "N\n";
            continue;
        }
        
        u--; v--; // convert to 0-indexed
        
        // Check the maximum number of seats already booked in the range [u, v-1]
        int max_seats = st.query_range(u, v-1);
        
        if (max_seats + n <= k) {
            // Can fulfill the booking - after adding n seats, we won't exceed k on any segment
            cout << "T\n";
            // Update the segment tree - add n to the range [u, v-1]
            st.update_range(u, v-1, n);
        } else {
            cout << "N\n";
        }
    }
    
    return 0;
}