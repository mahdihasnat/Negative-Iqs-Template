class HungarianAlgorithm{
    int N,inf,n,max_match;
    int *lx,*ly,*xy,*yx,*slack,*slackx,*prev;
    int **cost;
    bool *S,*T;
    void init_labels(){
        for(int x=0;x<n;x++) lx[x]=0;
        for(int y=0;y<n;y++) ly[y]=0;
        for (int x = 0; x < n; x++)
            for (int y = 0; y < n; y++)
                lx[x] = max(lx[x], cost[x][y]);
    }
    void update_labels(){
        int x, y, delta = inf; //init delta as infinity
        for (y = 0; y < n; y++) //calculate delta using slack
            if (!T[y])
                delta = min(delta, slack[y]);
        for (x = 0; x < n; x++) //update X labels
            if (S[x]) lx[x] -= delta;
        for (y = 0; y < n; y++) //update Y labels
            if (T[y]) ly[y] += delta;
        for (y = 0; y < n; y++) //update slack array
            if (!T[y])
                slack[y] -= delta;
    }
    void add_to_tree(int x, int prevx)
//x - current vertex,prevx - vertex from X before x in the alternating path,
//so we add edges (prevx, xy[x]), (xy[x], x){
        S[x] = true; //add x to S
        prev[x] = prevx; //we need this when augmenting
        for (int y = 0; y < n; y++) //update slacks, because we add new vertex to S
            if (lx[x] + ly[y] - cost[x][y] < slack[y]){
                slack[y] = lx[x] + ly[y] - cost[x][y];
                slackx[y] = x;
            }
    }
    void augment() //main function of the algorithm{
        if (max_match == n) return; //check wether matching is already perfect
        int x, y, root; //just counters and root vertex
        int q[N], wr = 0, rd = 0; //q - queue for bfs, wr,rd - write and read
//pos in queue
        //memset(S, false, sizeof(S)); //init set S
        for(int i=0;i<n;i++) S[i]=false;
        //memset(T, false, sizeof(T)); //init set T
        for(int i=0;i<n;i++) T[i]=false;
        //memset(prev, -1, sizeof(prev)); //init set prev - for the alternating tree
        for(int i=0;i<n;i++) prev[i]=-1;
        for (x = 0; x < n; x++) //finding root of the tree{
            if (xy[x] == -1){
                q[wr++] = root = x;
                prev[x] = -2;
                S[x] = true;
                break;
            }
        }
        for (y = 0; y < n; y++) //initializing slack array{
            slack[y] = lx[root] + ly[y] - cost[root][y];
            slackx[y] = root;
        }
        while (true) //main cycle{
            while (rd < wr) //building tree with bfs cycle{
                x = q[rd++]; //current vertex from X part
                for (y = 0; y < n; y++) //iterate through all edges in equality graph{
                    if (cost[x][y] == lx[x] + ly[y] && !T[y]){
                        if (yx[y] == -1) break; //an exposed vertex in Y found, so
//augmenting path exists!
                        T[y] = true; //else just add y to T,
                        q[wr++] = yx[y]; //add vertex yx[y], which is matched
//with y, to the queue
                        add_to_tree(yx[y], x); //add edges (x,y) and (y,yx[y]) to the tree
                    }
                }
                if (y < n) break; //augmenting path found!
            }
            if (y < n) break; //augmenting path found!
            update_labels(); //augmenting path not found, so improve labeling
            wr = rd = 0;
            for (y = 0; y < n; y++){
                //in this cycle we add edges that were added to the equality graph as a
//result of improving the labeling, we add edge (slackx[y], y) to the tree if
//and only if !T[y] && slack[y] == 0, also with this edge we add another one
//(y, yx[y]) or augment the matching, if y was exposed
                if (!T[y] && slack[y] == 0){
                    if (yx[y] == -1) //exposed vertex in Y found - augmenting path exists!{
                        x = slackx[y];
                        break;
                    }
                    else{
                        T[y] = true; //else just add y to T,
                        if (!S[yx[y]]){
                            q[wr++] = yx[y]; //add vertex yx[y], which is matched with
//y, to the queue
                            add_to_tree(yx[y], slackx[y]); //and add edges (x,y) and (y,
//yx[y]) to the tree
                        }
                    }
                }
            }
            if (y < n) break; //augmenting path found!
        }
        if (y < n) //we found augmenting path!{
            max_match++; //increment matching
//in this cycle we inverse edges along augmenting path
            for (int cx = x, cy = y, ty; cx != -2; cx = prev[cx], cy = ty){
                ty = xy[cx];
                yx[cy] = cx;
                xy[cx] = cy;
            }
            augment(); //recall function, go to step 1 of the algorithm
        }
    }//end of augment() function
public:
    HungarianAlgorithm(int vv,int inf=1000000000){
        N=vv;
        n=N;
        max_match=0;
        this->inf=inf;
        lx=new int[N];
        ly=new int[N];//labels of X and Y parts
        xy=new int[N];//xy[x] - vertex that is matched with x,
        yx=new int[N];//yx[y] - vertex that is matched with y
        slack=new int[N];//as in the algorithm description
        slackx=new int[N];//slackx[y] such a vertex, that l(slackx[y]) + l(y) - w(slackx[y],y) = slack[y]
        prev=new int[N];//array for memorizing alternating paths
        S=new bool[N];
        T=new bool[N];//sets S and T in algorithm
        cost=new int*[N];//cost matrix
        for(int i=0; i<N; i++){
            cost[i]=new int[N];
        }
    }
    ~HungarianAlgorithm(){
        delete []lx;
        delete []ly;
        delete []xy;
        delete []yx;
        delete []slack;
        delete []slackx;
        delete []prev;
        delete []S;
        delete []T;
        int i;
        for(i=0; i<N; i++){
            delete [](cost[i]);
        }
        delete []cost;
    }
    void setCost(int i,int j,int c){
        cost[i][j]=c;
    }
    int* matching(bool first=true){
        int *ans;
        ans=new int[N];
        for(int i=0;i<N;i++){
            if(first) ans[i]=xy[i];
            else ans[i]=yx[i];
        }
        return ans;
    }
    int hungarian(){
        int ret = 0; //weight of the optimal matching
        max_match = 0; //number of vertices in current matching
        for(int x=0;x<n;x++) xy[x]=-1;
        for(int y=0;y<n;y++) yx[y]=-1;
        init_labels(); //step 0
        augment(); //steps 1-3
        for (int x = 0; x < n; x++) //forming answer there
            ret += cost[x][xy[x]];
        return ret;
    }
};
