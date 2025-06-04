CXX = g++
CXXFLAGS = -Wall -Werror -pthread
BINPATH = bin

define build
	$(CXX) $(CXXFLAGS) src/$(1).cpp -o $(BINPATH)/$(1)
endef

define run
	./$(BINPATH)/$(1)
endef

# build
buildServer: createBin
	$(call build,chatServer)
buildClient: createBin
	$(call build,chatClient)

# run
runServer: buildServer
	$(call run,chatServer)
runClient: buildClient
	$(call run,chatClient)

# gdb
gdbServer: buildServer
	gdb $(BINPATH)/chatServer
gdbClient: buildClient
	gdb $(BINPATH)/chatClient

# utility
createBin:
	mkdir -p $(BINPATH)
clean:
	rm -rf $(BINPATH)
