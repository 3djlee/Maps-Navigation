// Main program for testing navigation algorithms

// !!! DO NOT MODIFY THIS FILE !!!

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "navigator.h"
#include "RoadMap.h"
#include "time.h"

struct query {
	int fromNode;
	int toNode;
	struct time time;
};

static RoadMap readRoadMap(char *filename);
static struct query readRouteQuery(char *filename);

static void displayRoute(struct route r);
static void printTimeInterval(struct time t1, struct time t2,
                              int duration);
static void printInterval(struct time t1, struct time t2);
static void printTime(struct time t);
static void printDuration(int minutes);

int main(int argc, char *argv[]) {
	if (argc != 3) {
		fprintf(stderr, "usage: %s <road map file> <route request file>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	RoadMap map = readRoadMap(argv[1]);
	struct query q = readRouteQuery(argv[2]);

	struct route r = navigate(map, q.fromNode, q.toNode, q.time);

	displayRoute(r);

	free(r.legs);
	RoadMapFree(map);
}

////////////////////////////////////////////////////////////////////////

static RoadMap readRoadMap(char *filename) {
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "error: failed to open '%s' for reading\n",
		        filename);
		exit(EXIT_FAILURE);
	}

	int numNodes = 0;
	int numRoads = 0;
	if (fscanf(fp, "%d %d", &numNodes, &numRoads) != 2) {
		fprintf(stderr, "error: failed to read number of nodes/roads\n");
		exit(EXIT_FAILURE);
	}

	RoadMap map = RoadMapNew(numNodes);
	for (int i = 0; i < numRoads; i++) {
		int node1;
		int node2;
		int oneOrTwoWay;
		int minutes;
		if (fscanf(fp, "%d %d %d %d", &node1, &node2, &oneOrTwoWay, &minutes) != 4) {
			fprintf(stderr, "error: failed to read road\n");
			exit(EXIT_FAILURE);
		}

		RoadMapAddRoad(map, node1, node2, oneOrTwoWay == 2, minutes);
	}

	int numTrafficLights = 0;
	if (fscanf(fp, "%d", &numTrafficLights) != 1) {
		fprintf(stderr, "error: failed to read number of traffic lights\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < numTrafficLights; i++) {
		int node = 0;
		if (fscanf(fp, "%d", &node) != 1) {
			fprintf(stderr, "error: failed to read traffic light location\n");
			exit(EXIT_FAILURE);
		}

		RoadMapAddTrafficLights(map, node);
	}

	int numClosedRoads = 0;
	if (fscanf(fp, "%d", &numClosedRoads) == 1) {
		for (int i = 0; i < numClosedRoads; i++) {
			int from = 0;
			int to = 0;
			struct time t1;
			struct time t2;
			if (fscanf(fp, "%d %d %d:%d %d:%d", &from, &to,
					&(t1.hour), &(t1.minute), &(t2.hour), &(t2.minute)) != 6) {
				fprintf(stderr, "error: failed to read road closed time\n");
			}
			
			RoadMapSetClosedTimes(map, from, to, t1, t2);
		}
	}

	fclose(fp);
	return map;
}

static struct query readRouteQuery(char *filename) {
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "error: failed to open '%s' for reading\n", filename);
		exit(EXIT_FAILURE);
	}

	struct query q;
	if (fscanf(fp, "%d %d", &(q.fromNode), &(q.toNode)) != 2) {
		fprintf(stderr, "error: failed to read start/destination nodes\n");
		exit(EXIT_FAILURE);
	}

	if (fscanf(fp, "%d:%d", &(q.time.hour), &(q.time.minute)) != 2) {
		fprintf(stderr, "error: failed to read time\n");
		exit(EXIT_FAILURE);
	}

	fclose(fp);
	return q;
}

////////////////////////////////////////////////////////////////////////

static void displayRoute(struct route r) {
	if (r.numLegs == 0) {
		printf("No route found!\n");
		return;
	}

	printf("Route from %d to %d\n", r.fromNode, r.toNode);
	printTimeInterval(r.start, r.end, r.totalDuration);
	printf("\n\n");

	printf("Legs:\n");
	for (int i = 0; i < r.numLegs; i++) {
		struct leg l = r.legs[i];
		printf("-> From %d to %d\n", l.fromNode, l.toNode);
		printf("   ");
		printTimeInterval(l.start, l.end, l.duration);
		printf("\n");
	}
}

static void printTimeInterval(struct time t1, struct time t2,
                              int duration) {
	printInterval(t1, t2);
	printf(" (");
	printDuration(duration);
	printf(")");
}

static void printInterval(struct time t1, struct time t2) {
	printTime(t1);
	printf(" - ");
	printTime(t2);
}

static void printTime(struct time t) {
	printf("%02d:%02d", t.hour, t.minute);
}

static void printDuration(int duration) {
	int hours = duration / MINUTES_PER_HOUR;
	int minutes = duration % MINUTES_PER_HOUR;
	if (hours > 0) {
		printf("%d hour(s), %d minute(s)", hours, minutes);
	} else {
		printf("%d minute(s)", minutes);
	}
}

