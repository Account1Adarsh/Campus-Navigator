#include <bits/stdc++.h>
using namespace std;

struct Edge {
    string to;
    int dist;
    string type; // "walk", "stairs"
};

struct Position {
    int x, y, z;  // z = floor number
};

class CampusNavigator {
    map<string, vector<Edge>> graph;
    map<string, Position> coord;

  public:
    void addNode(const string &name, int x, int y, int z) {
        coord[name] = {x,y,z};
        graph[name];
    }

    void addWalkEdge(const string &a, const string &b, int dist) {
        graph[a].push_back({b, dist, "walk"});
        graph[b].push_back({a, dist, "walk"});
    }

    void addStairEdge(const string &a, const string &b) {
        graph[a].push_back({b, 0, "stairs"});
        graph[b].push_back({a, 0, "stairs"});
    }

    void showPath(const string &src, const string &dst) {
        unordered_map<string,int> dist;
        unordered_map<string,string> parent;
        for(auto &p : graph) dist[p.first] = INT_MAX;
        dist[src] = 0;
        parent[src] = src;

        using P = pair<int,string>;
        priority_queue<P, vector<P>, greater<>> pq;
        pq.push({0, src});

        while(!pq.empty()) {
            auto p = pq.top(); pq.pop();
            auto d=p.first;
            auto u=p.second;
            if (d > dist[u]) continue;
            for(auto &e : graph[u]) {
                if (d + e.dist < dist[e.to]) {
                    dist[e.to] = d + e.dist;
                    parent[e.to] = u;
                    pq.push({dist[e.to], e.to});
                }
            }
        }

        if (dist[dst] == INT_MAX) {
            cout << "No route from " << src << " to " << dst << "\n";
            return;
        }

        // Reconstruct path
        vector<string> path;
        string cur = dst;
        while (true) {
            path.push_back(cur);
            if (cur == src) break;
            cur = parent[cur];
        }
        reverse(path.begin(), path.end());

        // Output turn-by-turn
        cout << "\nDirections from " << src << " to " << dst << ":\n";
        for (size_t i = 0; i + 1 < path.size(); ++i) {
            string u = path[i], v = path[i + 1];
            // find the edge u->v
            Edge eUV;
            for (auto &e : graph[u]) {
                if (e.to == v) { eUV = e; break; }
            }

            int fu = coord[u].z, fv = coord[v].z;
            // vertical movement via stairs
            if (eUV.type == "stairs") {
                if (fv > fu)
                    cout << "- At " << u << ": Go upstairs to Floor " << fv << "\n";
                else
                    cout << "- At " << u << ": Go downstairs to Floor " << fv << "\n";
                continue;
            }

            // horizontal movement
            if (i == 0) {
                // first segment, no previous orientation
                cout << "- Start at " << u
                     << " -> proceed to " << v
                     << " (" << eUV.dist << "steps)\n";
            } else {
                string w = path[i - 1];
                cout << "- Proceed from " << u
                     << " to " << v
                     << " (" << eUV.dist << "steps)  -> "
                     << computeTurn(w, u, v) << "\n";
            }
        }

        cout << "**You have arrived at " << dst << "**\n"
             << "Total distance: " << dist[dst] << " STEPS\n";
    }

  private:
    string computeTurn(const string &prev, const string &cur, const string &nxt) {
        auto p1 = coord[prev], p2 = coord[cur], p3 = coord[nxt];
        int dx1 = p2.x - p1.x, dy1 = p2.y - p1.y;
        int dx2 = p3.x - p2.x, dy2 = p3.y - p2.y;
        // double dot = dx1*dx2 + dy1*dy2;
        // double mag1 = sqrt(dx1*dx1 + dy1*dy1), mag2 = sqrt(dx2*dx2 + dy2*dy2);
        // double angle = acos(dot / (mag1*mag2)) * 180.0 / M_PI;
        // if (angle < 15)       return "Go Straight";
        // else if (angle < 90)  /* slight turn */;
        // else                  /* sharp turn */;

        int cross = dx1*dy2 - dy1*dx2;
        if (abs(cross) < 5) return "Go Straight";
        return (cross > 0) ? "Turn Left" : "Turn Right";
    }
};

int main() {
    CampusNavigator nav;

    // Nodes: name, x, y, floor
    //ground floor:
    nav.addNode("Main Gate",  0,   0, 0);
    nav.addNode("Reception",  0,   70, 0);
    nav.addNode("Corridor Recp",  0,   82, 0);
    //right wing
    nav.addNode("G14",  8,   82, 0);
    nav.addNode("G1",  12,   82, 0);
    nav.addNode("G13", 15,   82, 0);
    nav.addNode("G12", 17,   82, 0);

    nav.addNode("Node T R0", 36,   82, 0);

    nav.addNode("Washroom T R0", 32,   82, 0);
    nav.addNode("G3", 42,   82, 0);
    nav.addNode("G2", 40,   82, 0);
    nav.addNode("G4", 53,   82, 0);
    nav.addNode("G5", 64,   82, 0);
    nav.addNode("AV EXIT", 70,   82, 0);

    nav.addNode("T R0 Staff Washroom", 36, 95, 0);
    nav.addNode("TR01 Stairs", 36, 100, 0);
    nav.addNode("G7", 36, 102, 0);
    nav.addNode("G11", 36, 102, 0);
    nav.addNode("G10", 36, 140, 0);
    nav.addNode("G8", 36, 140, 0);
    nav.addNode("G9", 36, 160, 0);
    nav.addNode("R01 Stairs", 38, 160, 0);
    nav.addNode("EXIT Basketball court", 36, 170, 0);

    //left wing
    nav.addNode("C01 Stairs",-10, 82, 0);
    nav.addNode("G15",-10, 82, 0);
    nav.addNode("G28",-30, 82, 0);
    nav.addNode("G16",-14, 82, 0);
    nav.addNode("G17",-16, 82, 0);

    nav.addNode("Node T L0",-36, 82, 0);

    nav.addNode("Washroom T L0", -32,   82, 0);
    nav.addNode("G27", -40,   82, 0);
    nav.addNode("G26", -60,   82, 0);
    nav.addNode("G25", -64,   82, 0);
    nav.addNode("G24", -64,   82, 0);
    nav.addNode("EXIT Library", -70,   82, 0);

    nav.addNode("L R0 Staff Washroom", -36, 95, 0);
    nav.addNode("TL01 Stairs", -36, 100, 0);
    nav.addNode("G23", -36, 100, 0);
    nav.addNode("G22", -36, 104, 0);
    nav.addNode("G18", -36, 104, 0);
    nav.addNode("G21", -36, 140, 0);
    nav.addNode("G19", -36, 140, 0);
    nav.addNode("G20", -36, 160, 0);
    nav.addNode("L01 Stairs", -38, 164, 0);
    nav.addNode("EXIT Basketball court", -36, 170, 0);


    // Horizontal edges


    nav.addWalkEdge("Main Gate", "Reception", 70);
    nav.addWalkEdge( "Reception","Corridor Recp", 12);
    //Right wing
    nav.addWalkEdge("Corridor Recp", "G14", 8);
    nav.addWalkEdge("G1", "G14", 4);
    nav.addWalkEdge("G1", "G13", 3);
    nav.addWalkEdge("G12", "G13", 2);
    nav.addWalkEdge("G12", "Node T R0", 19);

    nav.addWalkEdge("Washroom T R0", "Node T R0", 4);
    nav.addWalkEdge("G2", "Node T R0", 4);
    nav.addWalkEdge("G3", "G2", 2);
    nav.addWalkEdge("G3", "G4", 11);
    nav.addWalkEdge("G4", "G5", 11);
    nav.addWalkEdge("AV EXIT", "G5", 6);

    nav.addWalkEdge("T R0 Staff Washroom", "Node T R0", 13);
    nav.addWalkEdge("T R0 Staff Washroom", "TR01 Stairs", 5);
    nav.addWalkEdge("G7", "TR01 Stairs", 2);
    nav.addWalkEdge("G11", "TR01 Stairs", 2);
    nav.addWalkEdge("G7", "G11", 0);
    nav.addWalkEdge("G7", "G10", 38);
    nav.addWalkEdge("G7", "G8", 38);
    nav.addWalkEdge("G10", "G8", 0);
    nav.addWalkEdge("G10", "G9", 20);
    nav.addWalkEdge("R01 Stairs", "G9", 2);
    nav.addWalkEdge("R01 Stairs", "G9", 2);
    nav.addWalkEdge("EXIT Basketball courtR01 Stairs", "G9", 10);
    
    //left wing
    nav.addWalkEdge("Corridor Recp", "C01 Stairs", 10);
    nav.addWalkEdge("G15", "C01 Stairs", 0);
    nav.addWalkEdge("G16", "C01 Stairs", 4);
    nav.addWalkEdge("G16", "G17", 2);
    nav.addWalkEdge("G28", "G17", 14);
    nav.addWalkEdge("G28", "Node T L0", 6);
    nav.addWalkEdge("G28", "Washroom T L0", 2);

    nav.addWalkEdge("Washroom T L0", "Node T L0", 4);
    nav.addWalkEdge("G27", "Node T L0", 4);
    nav.addWalkEdge("G27", "G26", 20);
    nav.addWalkEdge("G25", "G26", 4);
    nav.addWalkEdge("G25", "G24", 0);
    nav.addWalkEdge("G25", "EXIT Library", 6);

    nav.addWalkEdge("L R0 Staff Washroom", "Node T L0", 13);
    nav.addWalkEdge("L R0 Staff Washroom", "TL01 Stairs", 5);
    nav.addWalkEdge("G23", "TL01 Stairs", 0);
    nav.addWalkEdge("G23", "G22", 4);
    nav.addWalkEdge("G18", "G22", 0);
    nav.addWalkEdge("G18", "G21", 36);
    nav.addWalkEdge("G19", "G21", 0);
    nav.addWalkEdge("G19", "G20", 20);
    nav.addWalkEdge("L01 Stairs", "G20", 4);
    nav.addWalkEdge("EXIT Basketball court", "G20", 10);

    // Vertical stairs
    nav.addStairEdge("Stairs A (G)", "Stairs A (1)");

    // User I/O
    string start, dest;
    int n;
    cin>>n;
    while(n--){
        cout << "Enter start node exactly: "; getline(cin, start);
        cout << "Enter destination node exactly: "; getline(cin, dest);
        nav.showPath(start, dest);
    }
    

    
    return 0;
}
