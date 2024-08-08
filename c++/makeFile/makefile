# 编译器设置
CXX = g++
CXXFLAGS = -g -Wall -std=c++11

# 源文件和目标文件
OBJ = $(patsubst %.cpp, %.o, $(wildcard ./*.cpp))
TARGET = server

# 库路径和链接库
LDFLAGS = -L./src_so -L./dataBase_so -L./hashTable_so
LIBS = -ljson-c -lsqlite3 -lSoMyTree -lSoMydataBase -lSoMyHash

# 将.o文件标记为中间文件
.INTERMEDIATE: $(OBJ)

# 默认目标
all: $(TARGET)

# 使用$(TARGET)，必须要加$符号 连接目标
$(TARGET): $(OBJ)
	$(CXX) -g $^ -o $@ $(LDFLAGS) $(LIBS)

# 编译规则
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理 
.PHONY: clean show

clean:
	$(RM) $(OBJ) $(TARGET)

show:
	@echo "Object files: $(OBJ)"
	@echo "Target: $(TARGET)"
	@echo "LDFLAGS: $(LDFLAGS)"
	@echo "LIBS: $(LIBS)"
