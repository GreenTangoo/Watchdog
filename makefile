BUIID_DIR=../build-WideProtectSIEM

all:
	g++ -c -g -Wall utility_module/strings/string_manager.cpp -o $(BUIID_DIR)/string_manager.o
	g++ -c -g -Wall utility_module/time/date_time.cpp -o $(BUIID_DIR)/date_time.o
	g++ -c -g -Wall main.cpp -o $(BUIID_DIR)/main.o
	g++ -g $(BUIID_DIR)/string_manager.o $(BUIID_DIR)/date_time.o $(BUIID_DIR)/main.o -o $(BUIID_DIR)/main