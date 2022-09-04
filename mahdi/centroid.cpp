// tested on https://codeforces.com/contest/342/problem/E
// require c++17
namespace centroid
{

vector< array<int,2> > g[N]; // u->{v,e}
int sz[N]; // subtree size
int nn ; // current tree size
bool vis[N]; // centroid discovered
int big[N]; // big child of u
int par[N]; // parent in centroid tree
void dfs_sz(int u,int p=-1)
{
	sz[u]=1;
	big[u]=-1;
	nn++;
	for(auto [v,e]: g[u])
	{
		if(v!=p and !vis[v])
		{
			dfs_sz(v,u);
			sz[u]+=sz[v];
			if(big[u]!=-1 or sz[v] > sz[big[u]])
			{
				big[u]=v;
			}
		}
	}
}
int get_centroid(int u)
{
	if(big[u]==-1)
		return u;
	if(sz[big[u]]>nn/2)
		return get_centroid(big[u]);
	return u;
}
void init(int n)
{
	memset(vis, 0, sizeof(vis[0]) * (n+1));
}


void decompose(int u,int p)
{
	nn=0;
	dfs_sz(u);
	int c = get_centroid(u);

	vis[c]=1;
	par[c]=p;
	
	for(auto [v,e]: g[c])
	{
		if(!vis[v])
		{
			decompose(v,c);
		}
	}
}

}