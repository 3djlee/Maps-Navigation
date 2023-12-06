// Daniel Lee 
// This file is for the navigation for planet of Zephyrus
// The program can show the route, time and legs for navigation

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "RoadMap.h"
#include "navigator.h"
#include "time.h"

// Helper Func
static int *dijkstraAlg(RoadMap map, int fromNode, int toNode, int *distArray);

struct route navigate(RoadMap map, int fromNode, int toNode, struct time time) {
    struct route route = {
        .fromNode = fromNode,
        .toNode = toNode,
        .numLegs = 0,
        .legs = NULL,
        .totalDuration = 0,
    };

    // If nodes are not on same island
    if (!RoadMapOnSameIsland(map, fromNode, toNode)) {
        route.numLegs = 0;
        return route;
    }
    // Store in route for start time
    route.start.hour = time.hour;
    route.start.minute = time.minute;

    int numNodes = RoadMapNumNodes(map);
    route.legs = malloc(sizeof(struct leg) * numNodes);
    int *distArray = malloc(sizeof(int) * numNodes);

    // Get the predArray after dijkstra Algorithm
    int *predArray = dijkstraAlg(map, fromNode, toNode, distArray);
    int destination = toNode;

    // Given in reverse order for route from dijkstra's Algorithm
    struct leg *reverseLegs = malloc(sizeof(struct leg) * numNodes);
    int numLegs = 0;
    while (destination != fromNode) {
        int durations;
        // If it contains traffic minus 1 since we added 1 from dijkstra's algo
        if (RoadMapHasTrafficLights(map, predArray[destination]) &&
            predArray[destination] != fromNode) {
            durations =
                distArray[destination] - distArray[predArray[destination]] - 1;
        } else {
            durations =
                distArray[destination] - distArray[predArray[destination]];
        }
        struct leg reverseLeg = {
            .fromNode = predArray[destination],
            .toNode = destination,
            .duration = durations,
        };
        reverseLegs[numLegs] = reverseLeg;
        numLegs++;
        destination = predArray[destination];
    }

    // Change from reverse to fromNode
    for (int i = 0; i < numLegs; i++) {
        route.legs[i] = reverseLegs[numLegs - i - 1];
    }

    for (int j = 0; j < numLegs; j++) {
        // If node has traffic but not fromNode, add 1
        if (RoadMapHasTrafficLights(map, route.legs[j].fromNode) && j != 0) {
            time = timeAdd(time, 1);
        }
        // Add starting time for route
        route.legs[j].start.hour = time.hour;
        route.legs[j].start.minute = time.minute;

        time = timeAdd(time, route.legs[j].duration);
        // Add ending time for route
        route.legs[j].end.hour = time.hour;
        route.legs[j].end.minute = time.minute;
    }
    // Store in route for end time
    route.end.hour = time.hour;
    route.end.minute = time.minute;

    route.totalDuration += distArray[toNode];
    route.numLegs = numLegs;

    // Free malloc'd memory
    free(predArray);
    free(distArray);
    free(reverseLegs);
    return route;
}

// Use dijkstra's algorithm to find shortest path
static int *dijkstraAlg(RoadMap map, int fromNode, int toNode, int *distArray) {
    // Dijkstra ignores nodes on another island
    int numNodes = RoadMapNumNodes(map);
    // Dynamically allocate arrays
    int *predArray = malloc(sizeof(int) * numNodes);
    bool *visited = malloc(sizeof(bool) * numNodes);
    struct road road[MAX_ROADS_PER_NODE];

    // Create dist, pred and visited array
    for (int index = 0; index < numNodes; index++) {
        distArray[index] = INT_MAX;
        predArray[index] = -1;
        visited[index] = false;
    }

    // Making source distance 0
    distArray[fromNode] = 0;
    for (int index = 0; index < numNodes; index++) {
        // Get node with shortest travel distance
        int shortestNode = 0;
        // Road with shortest travel distance
        int shortestDist = INT_MAX;
        // Get shortest distance to a node
        for (int index1 = 0; index1 < numNodes; index1++) {
            if (!visited[index1] && distArray[index1] < shortestDist) {
                shortestDist = distArray[index1];
                shortestNode = index1;
            }
        }
        // Set visited to true
        visited[shortestNode] = true;
        // If node exists on same Island
        if (RoadMapOnSameIsland(map, fromNode, shortestNode)) {
            int travelNode = 0;
            int travel = INT_MAX;
            // Gets the nodes adjacent to fromNode
            int nodeRoads = RoadMapGetRoadsFrom(map, shortestNode, road);
            // Gets the shortest Road of the nodes adjacent
            for (int node1 = 0; node1 < nodeRoads; node1++) {
                if (!visited[node1] && road[node1].travelMinutes < travel) {
                    travel = road[node1].travelMinutes;
                    travelNode = road[node1].toNode;
                }
            }
            for (int j = 0; j < nodeRoads; j++) {
                // If it contains traffic add 1 to distance for road
                if (RoadMapHasTrafficLights(map, road[j].fromNode) &&
                    road[j].fromNode != fromNode) {
                    road[j].travelMinutes++;
                }
                struct road lowRoad = road[j];
                // Edge relaxation - If src distance to dest is shorter overall
                if (!visited[lowRoad.toNode] &&
                    distArray[lowRoad.fromNode] + lowRoad.travelMinutes <
                        distArray[lowRoad.toNode]) {
                    distArray[lowRoad.toNode] =
                        distArray[lowRoad.fromNode] + lowRoad.travelMinutes;
                    predArray[lowRoad.toNode] = lowRoad.fromNode;
                }
            }
        }
    }
    free(visited);
    return predArray;
}

struct route advancedNavigate(RoadMap map, int fromNode, int toNode,
                              struct time time) {
    struct route route = {
        .fromNode = fromNode,
        .toNode = toNode,
        .numLegs = 0,
        .legs = NULL,
    };

    return route;
}
