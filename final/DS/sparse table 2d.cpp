const int N=500;
const int K = 8 ; /// k >= ceil(lg22(n)) +1
int arr[N][N];
int st[K+1][K+1][N][N];
int lg2[N+1];
void ini(){
    lg2[1] = 0;
    for (int i = 2; i <= N; i++)
        lg2[i] = lg2[i/2] + 1;
}
int f(int i,int j){
    return max(i,j);
}
void pre( int n,int m){
    for(int x=0;x<=K;x++){
        for(int y=0;y<=K;y++){
            for(int i=0;i<n;i++){
                for(int j=0;j<m;j++){
if(i+(1<<x)>n or j+(1<<y) > m) continue;
if(x>0) st[x][y][i][j] = f(st[x-1][y][i][j] , st[x-1][y][i+(1<<(x-1))][j]);
else if(y>0) st[x][y][i][j] = f(st[x][y-1][i][j] , st[x][y-1][i][j+(1<<(y-1))]);
else st[x][y][i][j] = f(arr[i][j] , arr[i][j]);
                }
            }
        }
    }
}
int getf( int R1 ,int C1 , int R2 ,  int C2){
    if(R1>R2) swap(R1,R2);
    if(C1>C2) swap(C1,C2);
    int x = lg2[R2 - R1 + 1];
    int y = lg2[C2 - C1 + 1];
    return  f(f(f(st[x][y][R1][C1],st[x][y][R2-(1<<x)+1][C1]  ), st[x][y][R1][C2 - (1<<y)+1]), st[x][y][R2-(1<<x)+1][C2-(1<<y)+1]);
}
