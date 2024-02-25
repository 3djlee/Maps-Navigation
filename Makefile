CC = clang
CFLAGS = -Wall -Wvla -Werror -g

########################################################################

.PHONY: asan msan nosan

asan: CFLAGS += -fsanitize=address,leak,undefined
asan: all

msan: CFLAGS += -fsanitize=memory,undefined -fsanitize-memory-track-origins
msan: all

nosan: all

########################################################################

.PHONY: all
all: testRoadMap testNavigator

testRoadMap: testRoadMap.c RoadMap.c time.c
	$(CC) $(CFLAGS) -o testRoadMap testRoadMap.c RoadMap.c time.c

testNavigator: testNavigator.c navigator.c RoadMap.c time.c
	$(CC) $(CFLAGS) -o testNavigator testNavigator.c navigator.c RoadMap.c time.c

.PHONY: clean
clean:
	rm -f testRoadMap testNavigator
