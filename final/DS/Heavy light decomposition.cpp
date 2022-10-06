int value[N],Tree[N];
int parent[N], depth[N], heavy[N], head[N], pos[N];
int cur_pos,n;
vector<int > adj[N];
int dfs(int v) {
    int size = 1,max_c_size = 0;
    for (int c : adj[v]) {
        if (c != parent[v]) {
            parent[c] = v, depth[c] = depth[v] + 1;
            int c_size = dfs(c);
            size += c_size;
            if (c_size > max_c_size)
                max_c_size = c_size, heavy[v] = c;
        }
    }
    return size;
}
void update(int idx, int x,int nn){
	while(idx<=nn){
		Tree[idx]+=x; idx+=(idx&-idx);
	}
}
void decompose(int v, int h) {
    head[v] = h, pos[v] = cur_pos;
    update(cur_pos,value[v],n+1);
    cur_pos++;
    if (heavy[v] != -1) decompose(heavy[v], h);
    for (int c : adj[v]) {
        if (c != parent[v] && c != heavy[v])
            decompose(c, c);
    }
}
int query_bit(int idx){
	int sum=0;
	while(idx>0){
		sum+=Tree[idx]; idx-=(idx&-idx);
	}
	return sum;
}
void init_hld(int root,int n){
    memset(Tree, 0,sizeof Tree);
    memset(heavy,-1,sizeof heavy);
    cur_pos = 1; parent[root]=-1;
    assert(dfs(root)==n);
    decompose(root, root);
}
int segment_tree_query(int x,int y){
    if(y<x) swap(x,y);
    return query_bit(y)-query_bit(x-1);
}
int query_hld(int a, int b) {
    int res = 0;
    for (; head[a] != head[b]; b = parent[head[b]]) {
        if (depth[head[a]] > depth[head[b]])
            swap(a, b);
        int cur_heavy_path_max = segment_tree_query(pos[head[b]], pos[b]);
        res += cur_heavy_path_max;
    }
    if (depth[a] > depth[b])
        swap(a, b);
    /// now a is the lca or quert(a,b)
    int last_heavy_path_max = segment_tree_query(pos[a], pos[b]);
    res +=  last_heavy_path_max;
    return res;
}