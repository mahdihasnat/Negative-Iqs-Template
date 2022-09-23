class SuffixAutomaton
{
    bool complete;
    int last;
    set<char> alphabet;
    struct state
    {
        int len, link, endpos, first_pos,shortest_non_appearing_string,height;
        long long substrings,length_of_substrings;
        bool is_clone;
        map<char, int> next;
        vector<int> inv_link;
        state(int leng=0,int li=0)
        {
            len=leng;
            link=li;
            first_pos=-1;
            substrings=0;
            length_of_substrings=0;
            endpos=1;
            shortest_non_appearing_string=0;
            is_clone=false;
            height=0;
        }
    };
    vector<state> st;
    void process(int node)
    {
        map<char, int> ::iterator mit;
        st[node].substrings=1;
        st[node].shortest_non_appearing_string=st.size();
        if((int) st[node].next.size()<(int) alphabet.size()) 
            st[node].shortest_non_appearing_string=1;
        for(mit=st[node].next.begin(); mit!=st[node].next.end(); ++mit)
        {
            if(st[mit->second].substrings==0) process(mit->second);
            st[node].height=max(st[node].height,1+st[mit->second].height);
            st[node].substrings=st[node].substrings+st[mit->second].substrings;
            st[node].length_of_substrings=st[node].length_of_substrings
            +st[mit->second].length_of_substrings+st[mit->second].substrings;
            st[node].shortest_non_appearing_string=min(st[node].shortest_non_appearing_string,
                                        1+st[mit->second].shortest_non_appearing_string);
        }
        if(st[node].link!=-1)
        {
            st[st[node].link].inv_link.push_back(node);
        }
    }
    void set_suffix_links(int node)
    {
        int i;
        for(i=0; i<st[node].inv_link.size(); i++)
        {
            set_suffix_links(st[node].inv_link[i]);
            st[node].endpos=st[node].endpos+st[st[node].inv_link[i]].endpos;
        }
    }
    void output_all_occurrences(int v, int P_length,vector<int> &pos)
    {
        if (!st[v].is_clone)
            pos.push_back(st[v].first_pos - P_length + 1);
        for (int u : st[v].inv_link)
            output_all_occurrences(u, P_length, pos);
    }
    void kth_smallest(int node,int k,vector<char> &str)
    {
        if(k==0) return;
        map<char, int> ::iterator mit;
        for(mit=st[node].next.begin(); mit!=st[node].next.end(); ++mit)
        {
            if(st[mit->second].substrings<k) k=k-st[mit->second].substrings;
            else
            {
                str.push_back(mit->first);
                kth_smallest(mit->second,k-1,str);
                return;
            }
        }
    }
    int find_occurrence_index(int node,int index,vector<char> &str)
    {
        if(index==str.size()) return node;
        if(!st[node].next.count(str[index])) return -1;
        else return find_occurrence_index(st[node].next[str[index]],index+1,str);
    }
    void klen_smallest(int node,int k,vector<char> &str)
    {
        if(k==0) return;
        map<char, int> ::iterator mit;
        for(mit=st[node].next.begin(); mit!=st[node].next.end(); ++mit)
        {
            if(st[mit->second].height>=k-1)
            {
                str.push_back(mit->first);
                klen_smallest(mit->second,k-1,str);
                return;
            }
        }
    }
    void minimum_non_existing_string(int node,vector<char> &str)
    {
        map<char, int> ::iterator mit;
        set<char>::iterator sit;
        for(mit=st[node].next.begin(),sit=alphabet.begin(); sit!=alphabet.end(); ++sit,++mit)
        {
            if(mit==st[node].next.end()||mit->first!=(*sit))
            {
                str.push_back(*sit);
                return;
            }
            else if(st[node].shortest_non_appearing_string==1+st[mit->second].shortest_non_appearing_string)
            {
                str.push_back(*sit);
                minimum_non_existing_string(mit->second,str);
                return;
            }
        }
    }
    void find_substrings(int node,int index,vector<char> &str,vector<pair<long long,long long> > &sub_info)
    {
        sub_info.push_back(make_pair(st[node].substrings,st[node].length_of_substrings+st[node].substrings*index));
        if(index==str.size()) return;
        if(st[node].next.count(str[index]))
        {
            find_substrings(st[node].next[str[index]],index+1,str,sub_info);
            return;
        }
        else
        {
            sub_info.push_back(make_pair(0,0));
        }
    }
    void check()
    {
        if(!complete)
        {
            process(0);
            set_suffix_links(0);
            int i;
            complete=true;
        }
    }
public:
    SuffixAutomaton(set<char> &alpha)
    {
        st.push_back(state(0,-1));
        last=0;
        complete=false;
        set<char>::iterator sit;
        for(sit=alpha.begin(); sit!=alpha.end(); sit++)
        {
            alphabet.insert(*sit);
        }
        st[0].endpos=0;
    }
    void sa_extend(char c)
    {
        int cur = st.size();
        st.push_back(state(st[last].len + 1));
        st[cur].first_pos=st[cur].len-1;
        int p = last;
        while (p != -1 && !st[p].next.count(c))
        {
            st[p].next[c] = cur;
            p = st[p].link;
        }
        if (p == -1)
        {
            st[cur].link = 0;
        }
        else
        {
            int q = st[p].next[c];
            if (st[p].len + 1 == st[q].len)
            {
                st[cur].link = q;
            }
            else
            {
                int clone = st.size();
                st.push_back(state(st[p].len + 1,st[q].link));
                st[clone].next = st[q].next;
                st[clone].is_clone=true;
                st[clone].endpos=0;
                st[clone].first_pos=st[q].first_pos;
                while (p != -1 && st[p].next[c] == q)
                {
                    st[p].next[c] = clone;
                    p = st[p].link;
                }
                st[q].link = st[cur].link = clone;
            }
        }
        last = cur;
        complete=false;
    }
    ~SuffixAutomaton()
    {
        int i;
        for(i=0; i<st.size(); i++)
        {
            st[i].next.clear();
            st[i].inv_link.clear();
        }
        st.clear();
        alphabet.clear();
    }
    void kth_smallest(int k,vector<char> &str)
    {
        check();
        kth_smallest(0,k,str);
    }
    int FindFirstOccurrenceIndex(vector<char> &str)
    {
        check();
        int ind=find_occurrence_index(0,0,str);
        if(ind==0) return -1;
        else if(ind==-1) return st.size();
        else return st[ind].first_pos+1-(int) str.size();
    }
    void FindAllOccurrenceIndex(vector<char> &str,vector<int> &pos)
    {
        check();
        int ind=find_occurrence_index(0,0,str);
        if(ind!=-1) output_all_occurrences(ind,str.size(),pos);
    }
    int Occurrences(vector<char> &str)
    {
        check();
        int ind=find_occurrence_index(0,0,str);
        if(ind==0) return 1;
        else if(ind==-1) return 0;
        else return st[ind].endpos;
    }
    void klen_smallest(int k,vector<char> &str)
    {
        check();
        if(st[0].height>=k) klen_smallest(0,k,str);
    }
    void minimum_non_existing_string(vector<char> &str)
    {
        check();
        int ind=find_occurrence_index(0,0,str);
        if(ind!=-1) minimum_non_existing_string(ind,str);
    }
    long long cyclic_occurrence(vector<char> &str)
    {
        check();
        int i,j,len;
        long long ans=0;
        int n=str.size();
        set<int> S;
        set<int>::iterator it;
        for(i=0,j=0,len=0; i<n*2-1; i++)
        {
            if(st[j].next.count(str[i%n]))
            {
                len++;
                j=st[j].next[str[i%n]];
            }
            else
            {
                while(j!=-1&&(!st[j].next.count(str[i%n])))
                {
                    j=st[j].link;
                }
                if(j!=-1)
                {
                    len=st[j].len+1;
                    j=st[j].next[str[i%n]];
                }
                else
                {
                    len=0;
                    j=0;
                }
            }
            while(st[j].link!=-1&&st[st[j].link].len>=n)
            {
                 j=st[j].link;
                 len=st[j].len;
            }
            if(len>=n) S.insert(j);
        }
        for(it=S.begin();it!=S.end();++it)
        {
            ans=ans+st[*it].endpos;
        }
        return ans;
    }
};
