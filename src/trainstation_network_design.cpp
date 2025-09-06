#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
#include <iomanip>

using namespace std;

// Custom structure for railway connections between stations
struct RailwayLink {
    int origin_station;
    int destination_station;
    double track_distance;
    
    RailwayLink(int orig, int dest, double dist) : 
        origin_station(orig), destination_station(dest), track_distance(dist) {}
};

class SouthwestRailwayNetwork {
private:
    int total_stations;
    vector<string> station_names;
    vector<vector<double>> distance_matrix;
    vector<RailwayLink> all_railway_links;
    
    // Union-Find data structure for Kruskal's MST algorithm
    vector<int> station_parent;
    vector<int> station_rank;
    
public:
    SouthwestRailwayNetwork(int num_stations) : total_stations(num_stations) {
        distance_matrix.resize(num_stations, vector<double>(num_stations, INT_MAX));
        station_parent.resize(num_stations);
        station_rank.resize(num_stations, 0);
        
        // Initialize distance matrix diagonal to 0
        for (int i = 0; i < num_stations; i++) {
            distance_matrix[i][i] = 0;
            station_parent[i] = i;
        }
    }
    
    // Add station names for Southwest Nigeria
    void setupNigerianStations() {
        station_names = {
            "Lagos_Central", "Ibadan_Main", "Abeokuta_Junction", 
            "Ilorin_Terminal", "Ogbomoso_Station", "Oshogbo_Hub",
            "Akure_Depot", "Ado_Ekiti_Stop", "Ikeja_Connect",
            "Sagamu_Point", "Oyo_Classic", "Ile_Ife_Heritage"
        };
    }
    
    // Add railway connection with bidirectional links
    void connectStations(int station_a, int station_b, double railway_km) {
        all_railway_links.push_back(RailwayLink(station_a, station_b, railway_km));
        distance_matrix[station_a][station_b] = railway_km;
        distance_matrix[station_b][station_a] = railway_km;
    }
    
    // Union-Find operations for MST
    int findStationRoot(int station) {
        if (station_parent[station] != station) {
            station_parent[station] = findStationRoot(station_parent[station]);
        }
        return station_parent[station];
    }
    
    void mergeStationGroups(int station_a, int station_b) {
        int root_a = findStationRoot(station_a);
        int root_b = findStationRoot(station_b);
        
        if (station_rank[root_a] < station_rank[root_b]) {
            station_parent[root_a] = root_b;
        } else if (station_rank[root_a] > station_rank[root_b]) {
            station_parent[root_b] = root_a;
        } else {
            station_parent[root_b] = root_a;
            station_rank[root_a]++;
        }
    }
    
    // Kruskal's algorithm to find Minimal Railway Spanning Tree
    vector<RailwayLink> buildOptimalRailwayNetwork() {
        vector<RailwayLink> mst_railway_tracks;
        
        // Sort all railway links by distance
        sort(all_railway_links.begin(), all_railway_links.end(), 
             [](const RailwayLink& a, const RailwayLink& b) {
                 return a.track_distance < b.track_distance;
             });
        
        cout << "\n=== BUILDING OPTIMAL RAILWAY NETWORK (MST) ===\n";
        double total_track_length = 0;
        
        for (const auto& link : all_railway_links) {
            int root_origin = findStationRoot(link.origin_station);
            int root_dest = findStationRoot(link.destination_station);
            
            if (root_origin != root_dest) {
                mst_railway_tracks.push_back(link);
                mergeStationGroups(link.origin_station, link.destination_station);
                total_track_length += link.track_distance;
                
                cout << "Railway Track: " << station_names[link.origin_station] 
                     << " <--> " << station_names[link.destination_station]
                     << " | Distance: " << link.track_distance << " km\n";
            }
        }
        
        cout << "\nTotal Railway Network Length: " << total_track_length << " km\n";
        return mst_railway_tracks;
    }
    
    // Floyd-Warshall algorithm for shortest paths between all stations
    void calculateShortestRoutes() {
        cout << "\n=== CALCULATING SHORTEST ROUTES BETWEEN ALL STATIONS ===\n";
        
        // Create working copy of distance matrix
        vector<vector<double>> shortest_routes = distance_matrix;
        
        // Floyd-Warshall triple nested loop
        for (int transit_station = 0; transit_station < total_stations; transit_station++) {
            for (int start_station = 0; start_station < total_stations; start_station++) {
                for (int end_station = 0; end_station < total_stations; end_station++) {
                    if (shortest_routes[start_station][transit_station] != INT_MAX &&
                        shortest_routes[transit_station][end_station] != INT_MAX) {
                        
                        double route_via_transit = shortest_routes[start_station][transit_station] + 
                                                  shortest_routes[transit_station][end_station];
                        
                        if (route_via_transit < shortest_routes[start_station][end_station]) {
                            shortest_routes[start_station][end_station] = route_via_transit;
                        }
                    }
                }
            }
        }
        
        // Display shortest distance matrix
        displayShortestDistanceTable(shortest_routes);
    }
    
    void displayShortestDistanceTable(const vector<vector<double>>& routes) {
        cout << "\nSHORTEST DISTANCES BETWEEN NIGERIAN RAILWAY STATIONS:\n";
        cout << setw(15) << "FROM \\ TO";
        
        for (int i = 0; i < total_stations; i++) {
            cout << setw(12) << station_names[i].substr(0, 10);
        }
        cout << "\n" << string(15 + 12 * total_stations, '-') << "\n";
        
        for (int from = 0; from < total_stations; from++) {
            cout << setw(15) << station_names[from].substr(0, 13);
            for (int to = 0; to < total_stations; to++) {
                if (routes[from][to] == INT_MAX) {
                    cout << setw(12) << "INF";
                } else {
                    cout << setw(12) << fixed << setprecision(1) << routes[from][to];
                }
            }
            cout << "\n";
        }
    }
    
    void displayStationNetwork() {
        cout << "SOUTHWEST NIGERIA RAILWAY STATIONS:\n";
        for (int i = 0; i < total_stations; i++) {
            cout << i << ": " << station_names[i] << "\n";
        }
    }
};

int main() {
    cout << "================================================\n";
    cout << "   SOUTHWEST NIGERIA RAILWAY NETWORK OPTIMIZER  \n";
    cout << "================================================\n";
    
    // Initialize network with 12 major stations
    SouthwestRailwayNetwork naija_railway(12);
    naija_railway.setupNigerianStations();
    naija_railway.displayStationNetwork();
    
    // Create realistic railway connections across Southwest Nigeria
    // Lagos connections (major hub)
    naija_railway.connectStations(0, 2, 64.5);  // Lagos-Abeokuta
    naija_railway.connectStations(0, 8, 23.7);  // Lagos-Ikeja
    naija_railway.connectStations(0, 9, 46.2);  // Lagos-Sagamu
    naija_railway.connectStations(0, 1, 128.2); // Lagos-Ibadan
    
    // Ibadan connections (second major hub)
    naija_railway.connectStations(1, 4, 91.3);  // Ibadan-Ogbomoso
    naija_railway.connectStations(1, 10, 54.8); // Ibadan-Oyo
    naija_railway.connectStations(1, 5, 89.7);  // Ibadan-Oshogbo
    naija_railway.connectStations(1, 2, 74.1);  // Ibadan-Abeokuta
    
    // Abeokuta connections
    naija_railway.connectStations(2, 8, 67.4);  // Abeokuta-Ikeja
    naija_railway.connectStations(2, 9, 52.9);  // Abeokuta-Sagamu
    
    // Northern connections
    naija_railway.connectStations(4, 3, 83.6);  // Ogbomoso-Ilorin
    naija_railway.connectStations(4, 5, 47.8);  // Ogbomoso-Oshogbo
    naija_railway.connectStations(5, 6, 114.2); // Oshogbo-Akure
    naija_railway.connectStations(5, 11, 76.3); // Oshogbo-Ile-Ife
    
    // Eastern connections
    naija_railway.connectStations(6, 7, 52.1);  // Akure-Ado-Ekiti
    naija_railway.connectStations(11, 7, 89.4); // Ile-Ife-Ado-Ekiti
    
    // Additional strategic connections
    naija_railway.connectStations(10, 4, 38.5); // Oyo-Ogbomoso
    naija_railway.connectStations(8, 9, 34.7);  // Ikeja-Sagamu
    naija_railway.connectStations(3, 5, 97.2);  // Ilorin-Oshogbo
    
    // Build optimal railway network using MST
    vector<RailwayLink> optimal_network = naija_railway.buildOptimalRailwayNetwork();
    
    // Calculate shortest paths between all stations
    naija_railway.calculateShortestRoutes();
    
    cout << "\n================================================\n";
    cout << "   RAILWAY NETWORK OPTIMIZATION COMPLETED!      \n";
    cout << "================================================\n";
    
    return 0;
}