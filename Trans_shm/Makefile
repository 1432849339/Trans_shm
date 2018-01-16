CC = g++
CFLAGS += -g -O0 -std=c++11 

INCLUDE += -I./include

LIBDIR += -L./lib
LIBS += -lpthread -lukey -lgflags -lglog
LIBS += -lmysqlcppconn -lshm
LIBS += -lboost_system -lboost_filesystem

LDFLAGS += 
BIN = shmTokline
SUFFIX = cpp

SRCS = $(wildcard *.$(SUFFIX))
OBJS = $(SRCS:%.$(SUFFIX)=%.o)

$(BIN): $(OBJS)
	$(CC) -o $@ $^ $(LIBDIR) $(LIBS) $(LDFLAGS)

$(OBJS): %.o:%.$(SUFFIX)
	$(CC) $(INCLUDE) $(CFLAGS) -c $<

clean:
	rm -f $(BIN) *.o