// Main program for testing the Road Map ADT

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "RoadMap.h"

static void test1_1(void);
static void test1_2(void);
static void test1_3(void);
static void test1_4(void);
static void test1_5(void);
static void test1_6(void);

static void test2_1(void);
static void test2_2(void);
static void test2_3(void);

static void test3_1(void);

static void sortRoads(struct road roads[], int numRoads);
static bool roadIsLess(struct road road1, struct road road2);
static bool roadIsEqual(struct road road1, struct road road2);

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s <task number>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	switch (atoi(argv[1])) {
		case 1:
			test1_1();
			test1_2();
			test1_3();
			test1_4();
			test1_5();
			test1_6();
			break;
		case 2:
			test2_1();
			test2_2();
			test2_3();
			break;
		case 3:
			test3_1();
			break;
		default:
			fprintf(stderr, "no tests for task '%s'\n", argv[1]);
			exit(EXIT_FAILURE);
	}
}

////////////////////////////////////////////////////////////////////////
// Task 1 tests

// Testing initial state of RoadMap
static void test1_1(void) {
	RoadMap map = RoadMapNew(5);

	assert(RoadMapNumNodes(map) == 5);
	assert(RoadMapNumRoads(map) == 0);

	RoadMapFree(map);

	printf("Test 1.1 passed!\n");
}

// Testing adding one-way roads
static void test1_2(void) {
	RoadMap map = RoadMapNew(5);

	bool added;

	added = RoadMapAddRoad(map, 0, 1, false, 2);
	assert(added); // road should be successfully added

	added = RoadMapAddRoad(map, 0, 3, false, 3);
	assert(added);

	added = RoadMapAddRoad(map, 1, 2, false, 5);
	assert(added);

	added = RoadMapAddRoad(map, 3, 4, false, 4);
	assert(added);

	added = RoadMapAddRoad(map, 2, 0, false, 3);
	assert(added);

	added = RoadMapAddRoad(map, 4, 0, false, 4);
	assert(added);

	assert(RoadMapNumRoads(map) == 6);

	RoadMapFree(map);

	printf("Test 1.2 passed!\n");
}

// Testing adding two-way roads
static void test1_3(void) {
	RoadMap map = RoadMapNew(5);

	bool added;

	added = RoadMapAddRoad(map, 0, 1, true, 3);
	assert(added);

	added = RoadMapAddRoad(map, 1, 2, true, 4);
	assert(added);

	added = RoadMapAddRoad(map, 2, 3, true, 2);
	assert(added);

	added = RoadMapAddRoad(map, 3, 4, true, 5);
	assert(added);

	added = RoadMapAddRoad(map, 4, 0, true, 3);
	assert(added);

	assert(RoadMapNumRoads(map) == 5);

	RoadMapFree(map);

	printf("Test 1.3 passed!\n");
}

// Testing adding too many roads
static void test1_4(void) {
	RoadMap map = RoadMapNew(6);

	bool added;

	added = RoadMapAddRoad(map, 0, 1, true, 3);
	assert(added);

	added = RoadMapAddRoad(map, 0, 2, false, 4);
	assert(added);

	added = RoadMapAddRoad(map, 0, 3, true, 5);
	assert(added);

	added = RoadMapAddRoad(map, 0, 4, false, 6);
	assert(added);

	added = RoadMapAddRoad(map, 0, 5, true, 7);
	assert(!added); // node 0 already has MAX_ROADS_PER_NODE roads

	RoadMapFree(map);

	printf("Test 1.4 passed!\n");
}

// Testing RoadMapGetRoadsFrom
static void test1_5(void) {
	RoadMap map = RoadMapNew(5);

	RoadMapAddRoad(map, 0, 1, false, 4);
	RoadMapAddRoad(map, 0, 2, true, 3);
	RoadMapAddRoad(map, 3, 0, false, 6);
	RoadMapAddRoad(map, 4, 0, true, 5);

	struct road roads[MAX_ROADS_PER_NODE];
	int numRoads = RoadMapGetRoadsFrom(map, 0, roads);
	assert(numRoads == 3);

	sortRoads(roads, numRoads);
	assert(roadIsEqual(roads[0], (struct road){0, 1, 4}));
	assert(roadIsEqual(roads[1], (struct road){0, 2, 3}));
	assert(roadIsEqual(roads[2], (struct road){0, 4, 5}));

	RoadMapFree(map);

	printf("Test 1.5 passed!\n");
}

// Testing RoadMapGetRoadsTo
static void test1_6(void) {
	RoadMap map = RoadMapNew(5);

	RoadMapAddRoad(map, 0, 1, false, 4);
	RoadMapAddRoad(map, 0, 2, true, 3);
	RoadMapAddRoad(map, 3, 0, false, 6);
	RoadMapAddRoad(map, 4, 0, true, 5);

	struct road roads[MAX_ROADS_PER_NODE];
	int numRoads = RoadMapGetRoadsTo(map, 0, roads);
	assert(numRoads == 3);

	sortRoads(roads, numRoads);
	assert(roadIsEqual(roads[0], (struct road){2, 0, 3}));
	assert(roadIsEqual(roads[1], (struct road){3, 0, 6}));
	assert(roadIsEqual(roads[2], (struct road){4, 0, 5}));

	RoadMapFree(map);

	printf("Test 1.6 passed!\n");
}

////////////////////////////////////////////////////////////////////////
// Task 2 tests

// Testing number of islands
static void test2_1(void) {
	RoadMap map = RoadMapNew(5);

	// travel time isn't important for task 2, so we use 1 for all
	RoadMapAddRoad(map, 0, 1, true, 1);
	RoadMapAddRoad(map, 1, 2, true, 1);
	RoadMapAddRoad(map, 3, 4, false, 1);

	RoadMapProcessIslands(map);

	assert(RoadMapNumIslands(map) == 2);

	RoadMapFree(map);

	printf("Test 2.1 passed!\n");
}

// Testing RoadMapOnSameIsland
static void test2_2(void) {
	RoadMap map = RoadMapNew(5);

	RoadMapAddRoad(map, 0, 1, true, 1);
	RoadMapAddRoad(map, 1, 2, true, 1);
	RoadMapAddRoad(map, 3, 4, false, 1);

	RoadMapProcessIslands(map);

	assert(RoadMapOnSameIsland(map, 0, 1));
	assert(RoadMapOnSameIsland(map, 3, 4));
	assert(!RoadMapOnSameIsland(map, 0, 3));

	RoadMapFree(map);

	printf("Test 2.2 passed!\n");
}

// Testing setting and getting island names
static void test2_3(void) {
	RoadMap map = RoadMapNew(9);

	RoadMapAddRoad(map, 0, 3, true, 1);
	RoadMapAddRoad(map, 0, 5, false, 1);
	RoadMapAddRoad(map, 1, 4, false, 1);
	RoadMapAddRoad(map, 2, 6, true, 1);
	RoadMapAddRoad(map, 4, 7, false, 1);
	RoadMapAddRoad(map, 4, 8, false, 1);
	RoadMapAddRoad(map, 7, 8, true, 1);

	RoadMapProcessIslands(map);

	assert(RoadMapNumIslands(map) == 3);

	assert(strcmp(RoadMapGetIslandName(map, 0), "(unnamed)") == 0);

	char islandName[100];

	// Set the name of the island that 0 is on to "Bora Bora"
	strcpy(islandName, "Bora Bora");
	RoadMapSetIslandName(map, 0, islandName);

	// Set the name of the island that 8 is on to "Huahine"
	strcpy(islandName, "Huahine");
	RoadMapSetIslandName(map, 8, islandName);

	// Set the name of the island that 2 is on to "Moorea"
	strcpy(islandName, "Moorea");
	RoadMapSetIslandName(map, 2, islandName);

	// 0, 3 and 5 are on the same island
	assert(strcmp(RoadMapGetIslandName(map, 0), "Bora Bora") == 0);
	assert(strcmp(RoadMapGetIslandName(map, 3), "Bora Bora") == 0);
	assert(strcmp(RoadMapGetIslandName(map, 5), "Bora Bora") == 0);

	// 1, 4, 7 and 8 are on the same island
	assert(strcmp(RoadMapGetIslandName(map, 1), "Huahine") == 0);
	assert(strcmp(RoadMapGetIslandName(map, 4), "Huahine") == 0);
	assert(strcmp(RoadMapGetIslandName(map, 7), "Huahine") == 0);
	assert(strcmp(RoadMapGetIslandName(map, 8), "Huahine") == 0);
	
	// 2 and 6 are on the same island
	assert(strcmp(RoadMapGetIslandName(map, 2), "Moorea") == 0);
	assert(strcmp(RoadMapGetIslandName(map, 6), "Moorea") == 0);

	RoadMapFree(map);

	printf("Test 2.3 passed!\n");
}

////////////////////////////////////////////////////////////////////////
// Task 3 tests

static void test3_1(void) {
	RoadMap map = RoadMapNew(5);

	assert(!RoadMapHasTrafficLights(map, 0));
	assert(!RoadMapHasTrafficLights(map, 1));
	assert(!RoadMapHasTrafficLights(map, 2));
	assert(!RoadMapHasTrafficLights(map, 3));
	assert(!RoadMapHasTrafficLights(map, 4));

	RoadMapAddTrafficLights(map, 0);
	RoadMapAddTrafficLights(map, 3);

	assert(RoadMapHasTrafficLights(map, 0));
	assert(!RoadMapHasTrafficLights(map, 1));
	assert(!RoadMapHasTrafficLights(map, 2));
	assert(RoadMapHasTrafficLights(map, 3));
	assert(!RoadMapHasTrafficLights(map, 4));

	RoadMapFree(map);

	printf("Test 3.1 passed!\n");
}

////////////////////////////////////////////////////////////////////////

// Sort roads by node
static void sortRoads(struct road roads[], int numRoads) {
	for (int i = 0; i < numRoads - 1; i++) {
		int min = i;
		for (int j = i + 1; j < numRoads; j++) {
			if (roadIsLess(roads[j], roads[min])) {
				min = j;
			}
		}

		struct road tmp = roads[i];
		roads[i] = roads[min];
		roads[min] = tmp;
	}
}

static bool roadIsLess(struct road road1, struct road road2) {
	return (
		road1.fromNode < road2.fromNode ||
		(
			road1.fromNode == road2.fromNode &&
			road1.toNode < road2.toNode
		)
	);
}

static bool roadIsEqual(struct road road1, struct road road2) {
	return (
		road1.fromNode == road2.fromNode &&
		road1.toNode == road2.toNode &&
		road1.travelMinutes == road2.travelMinutes
	);
}

