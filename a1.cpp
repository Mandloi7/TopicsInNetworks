#include <bits/stdc++.h>
using namespace std;
#define F first
#define S second


string pathsFile("PATHS_TABLE.txt");
ifstream fin,cfin;
ofstream fout;
bool opti = true;
string flag; 
int MAXLEN_RT = 0;
int max2 = INT_MAX/2;


class edge {
    public:
    int u, v, cost, LabelCounter;
    bool isActive;
    double bandwidth, remaining_bandwidth;
    public:
    edge (int uu, int vv, int costy, double bandwidthh){
        u = uu;
        v = vv;
        cost = costy;
        isActive = true;
        LabelCounter = 0;
        bandwidth = bandwidthh;
        remaining_bandwidth = bandwidthh;
    }
};

class Node {
    public:
    int id;
    map<pair<int,int>, pair<int,int>> forwardingTable;

    public:
    Node(int iid){
        id=iid;
    }
};


class Connection {
    public:
    int u, v, path;
    double b_min, b_avg, b_max;
    vector<int> Labels;
    public:
    Connection(int uu, int vv, double b_minn, double b_avgg, double b_maxx){
        u = uu;
        v = vv;
        b_min = b_minn;
        b_avg = b_avgg;
        b_max = b_maxx;
        path = 0;
    }
};

string Sizify(int size, string s){
    while(s.size()<size)s+=" ";
    return s;
}

double GetBandwidth(Connection *req) {
    if (opti) {
        double b_max = req->b_max;
        double b_avg = req->b_avg;
        double b_min = req->b_min;
        return min(1.0*b_max, 1.0*b_avg + 0.25 * (b_max*1.0 - b_min*1.0));
    }
    else {
        return req->b_max*1.0;
    }
}


void calculateShortestPaths(int n, vector<vector<edge*>> &adj, map<pair<int,int>, vector<edge*>> &Shortest_Path) {
    int opt[n][n][n];
    int par[n][n][n];
    
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) par[i][j][0] = -1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j)opt[i][j][0] = max2;
            else opt[i][j][0] = 0;
        }
    }
    for (int k = 1; k < n; k++) {
        int i=0;
        while(i<n) {
            for (int j = 0; j < n; j++) {
                int mini=INT_MAX;
                int v=0;
                par[i][j][k] = par[i][j][k-1];
                opt[i][j][k] = opt[i][j][k-1];
                while (v<n) {
                    if (adj[v][j] != NULL){
                        int newCost;
                        newCost = opt[i][v][k-1];
                        if(flag!="hop")newCost+=adj[v][j]->cost;
                        else newCost++;
                        if (opt[i][j][k] > newCost) {
                            opt[i][j][k] = newCost;
                            par[i][j][k] = v;
                        }
                    }
                    v++;
                }
            }
            i++;
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j){
                int p = j;
                int k=n-1;
                vector<edge*> path;
                while(k--){
                    int np = par[i][p][k];
                    if (np == -1) break;
                    if (np == p) continue;
                    path.push_back(adj[np][p]);
                    p = np; 
                }
                vector<edge*> temppath = path;
                reverse(temppath.begin(), temppath.end());
                Shortest_Path[{i, j}] = temppath;
            }
        }
    }
}

void CalculateAlternatePaths(int n, int u, int v, vector<vector<edge*>> &adj, vector<edge*> &path, map<pair<int,int>, vector<edge*>> &altPath) {
    
    
    multiset<tuple<int,int,int>> pq;
    vector<int> dist(n, INT_MAX);
    vector<int> vis(n, 0);
    vector<int> par(n, -1);
    for (auto link : path) link->isActive = false;
    vis[u] = 1;
    dist[u] = 0;
    par[u] = -1;
    pq.insert(make_tuple(0, u, -1));
   

    while (!pq.empty()) {
        tuple<int,int,int> t= *pq.begin();
        pq.erase(pq.begin());
        int cost = get<0> (t);
        int i = get<1> (t);
        int p = get<2> (t);

        if (cost <= dist[i]){
            dist[i] = cost;
            par[i] = p;
            for (int j = 0; j < n; j++) {
                if (!(adj[i][j] == NULL || !adj[i][j]->isActive || vis[j])){
                    int x=0, newCost=0;
                    if(flag=="dist"){
                        x+=adj[i][j]->cost;
                    }else x++;
                    newCost += cost + x;
                    if (newCost < dist[j]) pq.insert({newCost, j, i});
                }
            }
            vis[i] = 1;
        } 
    }
    
    int np = par[v];
    vector<edge*> newPath;
    int p = v;
    while (np != -1) {
        edge* next=adj[np][p];
        newPath.push_back(next);
        p = np;
        np = par[np];
    }
    vector<edge*> temppath = newPath;
    reverse(temppath.begin(), temppath.end());
    altPath[{u, v}] = temppath;
    for (auto link : path) link->isActive = true;
}



int isValidPath(Connection *req, vector<edge*> &path) {
    double bdwth = GetBandwidth(req);
    for (int i=0;i<path.size();i++) {
        if (path[i]->remaining_bandwidth < bdwth) return 0;
    }
    return 1;
}



void PrintPath(int print, ofstream &fout, int source, int destination, vector<edge*> &path, int &pathCost) {
    if(print!=2)fout<<Sizify(12,to_string(source))<<"";
    fout<<Sizify(12,to_string(destination))<<"";
    string pathString="";
    for (int i=0;i<path.size();i++) {
        pathString+=to_string(path[i]->u) + "->";
       
        if(flag=="hop")pathCost++;
        else pathCost+=path[i]->cost;
       
    }
   
    pathString+=to_string(destination);
    MAXLEN_RT = max(MAXLEN_RT, (int)pathString.size());
    if(print)fout<<Sizify(60,pathString)<<"";
}


int AssignPaths(Connection *req,map<pair<int,int>, vector<edge*>> &Shortest_Path,map<pair<int,int>, vector<edge*>> &altPath,vector<Node*> &nodes) {
    int u = req->u, v = req->v;
    int prevLabel = -1, curLabel;
    int prevNode = -1;
    vector<edge*> path;
    if (isValidPath(req, Shortest_Path[{u, v}])) {
        req->path = 1;
        path = Shortest_Path[{u, v}];
    }
    else if (isValidPath(req, altPath[{u, v}])) {
        req->path = 2;
        path = altPath[{u, v}];
    }
    else {
        return 0;
    }
    int b = GetBandwidth(req);
    for (auto link : path) {
        curLabel = link->LabelCounter;
        int u = link->u;
        req->Labels.push_back(curLabel);
        pair<pair<int,int>, pair<int,int>> ft_entry;
        ft_entry = make_pair(make_pair(prevNode, prevLabel), make_pair(link->v, curLabel));
        nodes[u]->forwardingTable.insert(ft_entry);
        link->LabelCounter++;
        prevLabel = curLabel;
        prevNode = u;
        link->remaining_bandwidth -= b;
    }
    pair<pair<int,int>, pair<int,int>> ft_entry;
    ft_entry = make_pair(make_pair(prevNode, prevLabel), make_pair(-1, -1));
    nodes[req->v]->forwardingTable.insert(ft_entry);
    return req->path;
}

void printRoutingTable(int n, map<pair<int,int>, vector<edge*>> &Shortest_Path, map<pair<int,int>, vector<edge*>> &altPath) {
    
    
    int  pathCost;
    for (int i = 0; i < n; i++) {
        string curRoutingTableFile = "RT_Node"+to_string(i)+".txt";
        fout.open(curRoutingTableFile);

       
        fout<<Sizify(50, "Routing Table for Source Node : "+to_string(i) )<<"\n\n";
        fout<<Sizify(12, "Destination")<<"";
        fout<<Sizify(60, "Path")<<"";
        
        fout<<Sizify(12, "PathCost")<<"\n";
        for (int j = 0; j < n; j++) {
            vector<edge*>path1,path2;
            if (i == j) continue;
            int sht_sz = Shortest_Path[{i, j}].size();
            int alt_sz = Shortest_Path[{i, j}].size();
            int f1=0, f2=0;
            if (sht_sz > 0) {
                path1  = Shortest_Path[{i, j}];
                f1=1;
            }
            if (alt_sz > 0) {
                path2 = altPath[{i, j}];
                f2=1;
            }
           
            pathCost = 0;
            if(f1){
                
                PrintPath(2,fout, i, j, path1, pathCost);
                
                fout<<Sizify(12, to_string(pathCost))<<"\n";
            }
            
            pathCost = 0;
            if(f2){
               
                PrintPath(2,fout, i, j, path2, pathCost);
               
                fout<<Sizify(12, to_string(pathCost))<<"\n";
            }
            
        }
        fout.close();
    }

    fout.close();
} 

void printFowardingTable(int n, vector<Node*> &nodes) {
    
    for (int i = 0; i < n; i++) {
        string curForwardingTable = "FT_Node"+to_string(i)+".txt";
        fout.open(curForwardingTable);
        fout<<"Forwarding Table For Node "<<i<<":\n\n";
       
        fout<<Sizify(20, "Interface In ")<<"";
        fout<<Sizify(20, "Label In ")<<"";
        fout<<Sizify(20, "Interface Out")<<"";
        fout<<Sizify(20, "Label Out")<<"\n";
       
        for (auto x : nodes[i]->forwardingTable) {
           
            fout<<Sizify(20, to_string(x.F.F))<<"";
            fout<<Sizify(20,to_string(x.F.S))<<"";
            fout<<Sizify(20,to_string(x.S.F))<<"";
            fout<<Sizify(20,to_string(x.S.S))<<"\n";
        }
        fout.close();
    }

    
}

void PrintPathsFile(vector<Connection*> &requests,map<pair<int,int>, vector<edge*>> &Shortest_Path,map<pair<int,int>, vector<edge*>> &altPath,int path1c, int path2c, int noPathc) {
   
    ofstream fout;
    fout.open(pathsFile);
   
    
    fout << " Number of connections using the Shortest Path = " <<path1c << endl;
    fout << " Number of connections using the Alternate Path = " <<path2c<< endl;
    fout << " Number of connections that were not admitted = " <<noPathc<< endl;
    
    fout << "\n\n" << endl;
    fout<<Sizify(12, "Conn-Id")<<"";
    fout<<Sizify(12, "Source")<<"";
    fout<<Sizify(12, "Destination")<<"";
  
    fout<<Sizify(60, "Label list")<<"";
    fout<<Sizify(12, "Path Cost")<<"\n";

    int connectionsSetup = 0;
    for (int i = 0; i < requests.size(); i++) {
        int pathtype = requests[i]->path;
        if (pathtype == 0) continue;
        connectionsSetup++;
        vector<edge*>path;
        int u = requests[i]->u;
        int v = requests[i]->v;
        int pathCost = 0;
       
        fout << Sizify(12,to_string(i)) << "";
        if (pathtype == 1)path = Shortest_Path[{u, v}];
        if(pathtype == 2)path = altPath[{u, v}];
        PrintPath(0, fout, u, v, path, pathCost);

        int pathsize=requests[i]->Labels.size();
        string LabelString="";
        for (int j = 0; j < pathsize; j++) {
            LabelString+=to_string( requests[i]->Labels[j]);
           
            if(j + 1 != pathsize)LabelString+="->";
        }
        fout<<Sizify(60, LabelString)<<"";
        fout << Sizify(12, to_string(pathCost)) << endl;
    }

    fout.close();
}


vector<int> Split(string s){
    vector<int>ans;
    string temp="";
    for(int i=0;i<s.size();i++){
        if(s[i]==' '||s[i]=='\n'){
            if(temp.size()){
                
                ans.push_back(stoi(temp));
                temp="";
            }
        }else{
            temp+=s[i];
        }
    }
    if(temp.size()){
      
        ans.push_back(stoi(temp));
        temp="";
    }

    return ans;
}

vector<Node*> InitializeNodes(int n){
    vector<Node*> nodes;
    for (int i = 0; i < n; i++) {
        nodes.push_back(new Node(i));
    }
    return nodes;
}

vector<vector<edge*>>  CreateAdjacencyMatrix(int n, int m){
    vector<vector<edge*>> adj;
    for(int i=0;i<n;i++){
        vector<edge*> x;
        for(int j=0;j<n;j++){
            x.push_back(NULL);
        }
        adj.push_back(x);
    }

    
    for (int ii = 0; ii < m; ii++) {
        int u, v, d, b;
        float r;
        string str;
        getline(fin,str);
        int i=0;
        while(!isdigit(str[i]))i++;
       
        str=str.substr(i,str.size()-i);
        vector<int> inp = Split(str);
       
        adj[inp[0]][inp[1]] = new edge(inp[0], inp[1], inp[2], inp[3]);
        adj[inp[1]][inp[0]] = new edge(inp[1], inp[0], inp[2], inp[3]);
      
    }
    return adj;
}

vector<Connection*> CreateConnections(int c){
    
    vector<Connection*> connections;
    for (int ii = 0; ii < c; ii++) {
        int u,v;
    int b_min, b_avg, b_max;
        string str;
        getline(cfin,str);
        int i=0;
        while(!isdigit(str[i]))i++;
      
        str=str.substr(i,str.size()-i);
        vector<int> inp = Split(str);
       
        connections.push_back(new Connection(inp[0], inp[1], inp[2], inp[3], inp[4]));
    }
    return connections;
}

void SecondShortest(vector<vector<edge*>> &adj,map<pair<int,int>, vector<edge*>> &Shortest_Path, map<pair<int,int>, vector<edge*>> &altPath,int n, int m){
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            CalculateAlternatePaths(n, i, j, adj, Shortest_Path[{i, j}], altPath);
        }
    }
}

int main(int argc, char **argv) {
   
    if(argc!=9){
        cout<<"Invalid Arguments\n";
        cout<<"Correct Usage:";
        cout<<"./routing -top <topologyfilename> -conn <connectionfilename> -flag <flag hop/dist> -p <0/1>"<<endl;
        return 0;
    }
    map<string,string> mp;
    for(int i=1;i<8;i+=2){
        
        if(argv[i]=="-top"){
            mp["top"]=argv[i+1];
        }else if(argv[i]=="-conn"){
            mp["conn"]=argv[i+1];
        }else if(argv[i]=="-flag"){
            mp["flag"]=argv[i+1];
        }else if(argv[i]=="-p"){
            mp["p"]=argv[i+1];
        }
        
    }
    string topologyFile = argv[2];
    string connectionsFile = argv[4];
    flag = argv[6];
    string pessi = argv[8];
    opti = (pessi == "0" ? true : false);
    

    fin.open(topologyFile);

    string str;
    getline(fin,str);
    int i=0;
    while(!isdigit(str[i]))i++;
    
    str=str.substr(i,str.size()-i);
    vector<int> inp = Split(str);
    int n, m;
    n=inp[0];
    m=inp[1];
   
    vector<Node*> nodes = InitializeNodes(n);
    vector<vector<edge*>> adj = CreateAdjacencyMatrix(n,m);
    fin.close();
    map<pair<int,int>, vector<edge*>> Shortest_Path, altPath;
    calculateShortestPaths(n,adj, Shortest_Path);
    SecondShortest(adj,Shortest_Path, altPath,n,m);
    printRoutingTable(n, Shortest_Path, altPath);

    
    cfin.open(connectionsFile);
    

    getline(cfin,str);
    i=0;
    while(!isdigit(str[i]))i++;
    
    str=str.substr(i,str.size()-i);
    inp = Split(str);
    int c = inp[0];

    vector<Connection*> connections = CreateConnections(c);
    
    int path1 = 0, path2 = 0, noPath = 0; 
    for (int i = 0; i < c; i++) {
        int result = AssignPaths(connections[i], Shortest_Path, altPath, nodes);
        path1+=(result==1)?1:0;
        path2+=(result==2)?1:0;
        noPath+=(result==0)?1:0;
    } 

    cout << " Number of connections using the Shortest Path = " <<path1 << endl;
    cout << " Number of connections using the Alternate Path = " <<path2<< endl;
    cout << " Number of connections that were not admitted = " <<noPath<< endl;
    printFowardingTable(n, nodes);
    PrintPathsFile(connections, Shortest_Path, altPath, path1, path2, noPath);
    fin.close();
}