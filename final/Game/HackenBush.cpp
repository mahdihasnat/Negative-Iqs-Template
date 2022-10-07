/* tree case:  g[u] = for all v : XOR[  g[v] + 1]
 lose if no moves available
1. Colon Principle: Grundy number of a tree is the
          xor of Grundy number of child subtrees.
2. Fusion Principle: Consider a pair of adjacent
    vertices u, v that has another path (i.e.,
    they are in a cycle). Then,we can contract u
    and v without changing Grundy number.
We first decompose graph into two-edge connected
components.Then, by contracting each components by
using Fusion Principle,we obtain a tree (and many
self loops) that has the same Grundy number to the
original graph. By using Colon Principle, we can
compute the Grundy number. O(m + n). */
struct hackenbush {
	int n;  vector<vector<int>> adj;
	hackenbush(int n) : n(n), adj(n) { }
	void add_edge(int u, int v) {
		adj[u].push_back(v);
		if(u!=v) adj[v].push_back(u);
	}
  int grundy(int r) {
		vector<int> num(n), low(n); int t = 0;
		function<int(int, int)> dfs=[&](int p,int u) {
			num[u] = low[u] = ++t;  int ans = 0;
			for (int v : adj[u]) {
				if (v == p) { p += 2 * n; continue; }
				if (num[v] == 0) {
					int res = dfs(u, v);
					low[u] = min(low[u], low[v]);
					if (low[v] > num[u]) ans ^= (1+res)^1;
					else ans ^= res;           // non bridge
				}
        else low[u] = min(low[u], num[v]);
			}
			if (p > n) p -= 2 * n;
			for (int v : adj[u])
				if (v != p && num[u] <= num[v]) ans ^= 1;
			return ans;
		};
		return dfs(-1, r);
	}
};