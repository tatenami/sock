SRCS=test.cpp
LIBS=sock.cpp

${SRCS:.cpp=}: ${SRCS} ${LIBS}
	g++ -o $@ $^