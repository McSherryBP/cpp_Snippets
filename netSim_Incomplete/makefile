CC=g++
EXECUTABLE_PROGRAM=netSimB
SIM_CLASS=netsim
NODE_CLASS=Bnode
PACKET_CLASS=Bpacket
RUN=./

all: netSimCompile generateNodeFile

netSimCompile:
	$(CC) $(PACKET_CLASS).cpp $(NODE_CLASS).cpp $(SIM_CLASS).cpp -o $(EXECUTABLE_PROGRAM)

generateNodeFile:
	@echo "Generating Node file"
	$(RUN)$(EXECUTABLE_PROGRAM)
	@echo "* Random connected network of nodes initialized and written to nodefile.txt"
	@echo "* The following command will run the simulator using the generated collection of nodes **"
	@echo "./netSimB seedValue nodefile.txt   // Where 'seedValue' is an unsigned int "
	@echo " "
	@echo "* Running netSimB with 0 arguments wll generate a new node file **"
	@echo "./netSimB"
	@echo " "
	@echo "* Generated files are always connected"
	@echo "* Passing a disconnected graph will result in feedback recognizing the disconnect"
	@echo "* Disconnected graphs are not simulated and the program will end. "
	@echo "./netSimB (yourNodeFileHere)"
	@echo ""
	@echo "Build Complete."
	

