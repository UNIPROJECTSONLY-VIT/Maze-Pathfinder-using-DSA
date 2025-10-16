#include <bits/stdc++.h>
using namespace std;

using P = pair<int,int>;

bool inBounds(int r, int c, int H, int W)
{
    if (r < 0) {
        return false;
    }
    if (c < 0) {
        return false;
    }
    if (r >= H) {
        return false;
    }
    if (c >= W) {
        return false;
    }
    return true;
}

static inline int costOf(char ch){
    if(ch == '#')
        return 100000;  
    else if(ch == '~')
        return 5;
    else if(ch == 'S' || ch == 'E')
        return 1;
    else
        return 1;
}



bool run_bfs(const vector<string>& maze, vector<string>& out)
{
    int H = maze.size();
    int W = maze[0].size();

    P S = {-1,-1};
    P E = {-1,-1};

    // finding start and end points
    for (int i = 0; i < H; i++) 
    {
        for (int j = 0; j < W; j++) 
        {
            if (maze[i][j] == 'S') 
                S = {i, j};
            if (maze[i][j] == 'E') 
                E = {i, j};
        }
    }

    if (S.first == -1 || E.first == -1) 
    {
        cout << "Start or End not found!\n";
        return false;
    }

    vector<vector<int>> dist(H, vector<int>(W, -1));
    vector<vector<P>> parent(H, vector<P>(W, {-1, -1}));
    queue<P> q;

    q.push(S);
    dist[S.first][S.second] = 0;

    int dr[4] = {1, -1, 0, 0};
    int dc[4] = {0, 0, 1, -1};

    while (!q.empty())
    {
        P cur = q.front();
        q.pop();

        int r = cur.first;
        int c = cur.second;

        if (r == E.first && c == E.second)
            break;

        for (int k = 0; k < 4; k++)
        {
            int nr = r + dr[k];
            int nc = c + dc[k];

            if (!inBounds(nr, nc, H, W))
                continue;
            
            if (maze[nr][nc] == '#')
                continue;

            if (dist[nr][nc] == -1)
            {
                dist[nr][nc] = dist[r][c] + 1;
                parent[nr][nc] = {r, c};
                q.push({nr, nc});
            }
        }
    }

    if (dist[E.first][E.second] == -1)
    {
        return false;
    }

    out = maze;

    P cur = E;
    while (cur != S)
    {
        if (out[cur.first][cur.second] == ' ')
            out[cur.first][cur.second] = '.';
        if (out[cur.first][cur.second] == '~')
            out[cur.first][cur.second] = '.';
        cur = parent[cur.first][cur.second];
    }

    cout << "Shortest path (BFS) length: " << dist[E.first][E.second] << endl;
    return true;
}



bool run_dijkstra(const vector<string>& maze, vector<string>& out)
{
    int H = maze.size();
    int W = maze[0].size();

    P S = {-1,-1};
    P E = {-1,-1};

    for (int i = 0; i < H; i++) 
    {
        for (int j = 0; j < W; j++) 
        {
            if (maze[i][j] == 'S') 
                S = {i, j};
            if (maze[i][j] == 'E') 
                E = {i, j};
        }
    }

    if (S.first == -1 || E.first == -1) 
    {
        cout << "Start or End not found!\n";
        return false;
    }

    const int INF = 1e9;
    vector<vector<int>> dist(H, vector<int>(W, INF));
    vector<vector<P>> parent(H, vector<P>(W, {-1, -1}));

    typedef pair<int, P> Node;
    priority_queue<Node, vector<Node>, greater<Node>> pq;

    dist[S.first][S.second] = 0;
    pq.push({0, S});

    int dr[4] = {1, -1, 0, 0};
    int dc[4] = {0, 0, 1, -1};

    while (!pq.empty())
    {
        Node top = pq.top();
        pq.pop();

        int d = top.first;
        P u = top.second;

        int r = u.first;
        int c = u.second;

        if (d != dist[r][c])
            continue;

        if (u == E)
            break;

        for (int k = 0; k < 4; k++)
        {
            int nr = r + dr[k];
            int nc = c + dc[k];

            if (!inBounds(nr, nc, H, W))
                continue;

            char ch = maze[nr][nc];
            if (ch == '#')
                continue;

            int w = costOf(ch);

            if (dist[r][c] + w < dist[nr][nc])
            {
                dist[nr][nc] = dist[r][c] + w;
                parent[nr][nc] = {r, c};
                pq.push({dist[nr][nc], {nr, nc}});
            }
        }
    }

    if (dist[E.first][E.second] == INF)
    {
        return false;
    }

    out = maze;

    
    P cur = E;
    while (cur != S)
    {
        if (out[cur.first][cur.second] == ' ')
            out[cur.first][cur.second] = '.';
        else if (out[cur.first][cur.second] == '~')
            out[cur.first][cur.second] = '*';
        cur = parent[cur.first][cur.second];
    }

    cout << "Weighted cost (Dijkstra): " << dist[E.first][E.second] << endl;
    return true;
}



int main()
{
    vector<string> maze;

    maze.push_back("#########");
    maze.push_back("#S # ~ E#");
    maze.push_back("#  # #  #");
    maze.push_back("#  ~~ # #");
    maze.push_back("#########");

    // check weight or na
    bool hasWeights = false;
    for (int i = 0; i < maze.size(); i++)
    {
        if (maze[i].find('~') != string::npos)
        {
            hasWeights = true;
            break;
        }
    }

    cout << "Choose algorithm (bfs/dijkstra) or press enter for auto: ";
    string mode;
    getline(cin, mode);

    // auto detect 
    if (mode.empty())
    {
        if (hasWeights)
            mode = "dijkstra";
        else
            mode = "bfs";

        cout << "Auto-chosen: " << mode << endl;
    }

    cout << "Solving maze using " << mode << " algorithm.." << endl << endl;

    vector<string> solved;
    bool ok = false;

    if (mode == "bfs")
    {
        ok = run_bfs(maze, solved);
    }
    else if (mode == "dijkstra")
    {
        ok = run_dijkstra(maze, solved);
    }
    else
    {
        cout << "Invalid option entered using auto-detect mode " << endl;
        if (hasWeights)
            ok = run_dijkstra(maze, solved);
        else
            ok = run_bfs(maze, solved);
    }

    if (ok)
    {
        cout << "Final maze with path: " << endl;
        for (int i = 0; i < solved.size(); i++)
        {
            cout << solved[i] << endl;
        }
    }
    else
    {
        cout << "No path found in this maze " << endl;
    }

    return 0;
}