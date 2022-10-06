typedef long long ll;
class LiChaoTree{
    ll L,R;
    bool minimize;
    int lines;
    struct Node{
        complex<ll> line;
        Node *children[2];
        Node(complex<ll> ln= {0,1000000000000000000}){
            line=ln;
            children[0]=0;
            children[1]=0;
        }
    } *root;
    ll dot(complex<ll> a, complex<ll> b){
        return (conj(a) * b).real();
    }
    ll f(complex<ll> a,  ll x){
        return dot(a, {x, 1});
    }
    void clear(Node* &node){
        if(node->children[0]){
            clear(node->children[0]);
        }
        if(node->children[1]){
            clear(node->children[1]);
        }
        delete node;
    }
    void add_line(complex<ll> nw, Node* &node, ll l, ll r){
        if(node==0){
            node=new Node(nw);
            return;
        }
        ll m = (l + r) / 2;
        bool lef = (f(nw, l) < f(node->line, l)&&minimize)||((!minimize)&&f(nw, l) > f(node->line, l));
        bool mid = (f(nw, m) < f(node->line, m)&&minimize)||((!minimize)&&f(nw, m) > f(node->line, m));
        if(mid){
            swap(node->line, nw);
        }
        if(r - l == 1){
            return;
        }
        else if(lef != mid){
            add_line(nw, node->children[0], l, m);
        }
        else{
            add_line(nw, node->children[1], m, r);
        }
    }
    ll get(ll x, Node* &node, ll l, ll r){
        ll m = (l + r) / 2;
        if(r - l == 1){
            return f(node->line, x);
        }
        else if(x < m){
            if(node->children[0]==0) return f(node->line, x);
            if(minimize) return min(f(node->line, x), get(x, node->children[0], l, m));
            else return max(f(node->line, x), get(x, node->children[0], l, m));
        }
        else{
            if(node->children[1]==0) return f(node->line, x);
            if(minimize) return min(f(node->line, x), get(x, node->children[1], m, r));
            else return max(f(node->line, x), get(x, node->children[1], m, r));
        }
    }
public:
    LiChaoTree(ll l=-1000000001,ll r=1000000001,bool mn=false){
        L=l;
        R=r;
        root=0;
        minimize=mn;
        lines=0;
    }
    void AddLine(pair<ll,ll> ln){
        add_line({ln.first,ln.second},root,L,R);
        lines++;
    }
    int number_of_lines(){
        return lines;
    }
    ll getOptimumValue(ll x){
        return get(x,root,L,R);
    }
    ~LiChaoTree(){
        if(root!=0) clear(root);
    }
};
