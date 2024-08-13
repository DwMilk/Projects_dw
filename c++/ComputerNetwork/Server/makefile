# 编译器和编译标志 Complier and Compilation Flags
CXX := g++
# 编译标志：启用所有警告，优化级别设置为 O2，加入调试信息
#Compilation flags:enable all warnings, set optimization level to 02,include debug information
CXXFLAGS := -Wall -Wpedantic -g
#Linker Flags:can be selectively added during compilation,e.g., -pthread for multi-threading support
# 链接器标志：可在编译时选择性添加，例如 -pthread 用于多线程支持
LDFLAGS := -lmysqlclient -lsqlite3 

# 目录
# 存放目标文件的目录
# Directories
# Directory for storing object files
BUILD_DIR := .deps

# 文件
# 获取当前目录下的所有 .cpp 源文件
# Files
# Get all .cpp source files in the current directory
SRC := $(wildcard ./*.cpp)
# 将源文件路径转换为目标文件路径，存放在 BUILD_DIR 目录中
# Convert source file paths to object file paths, stored in BUILD_DIR
OBJ := $(patsubst ./%.cpp, $(BUILD_DIR)/%.o, $(SRC))
# 最终生成的可执行文件名称
# Name of the final executable
TARGET := server

# 目标
# Targets
.PHONY : all clean show

# 默认目标：生成可执行文件
# Default target: generate executable
all: $(TARGET)

# 生成可执行文件的规则，依赖于所有的目标文件
# Rule for generating the executable, depends on all object files
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# 生成目标文件的规则，依赖于对应的源文件
# Rule for generating object files, depends on corresponding source files
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# 创建目标文件存放目录
# Create directory for storing object files
$(BUILD_DIR):
	mkdir -p $@

# 清理构建文件
# Clean build files
clean:
	$(RM) -r $(BUILD_DIR) $(TARGET)

# 显示源文件和目标文件信息，用于调试
# Display source and object file information, used for debugging
show:
	@echo "Source files: $(SRC)"
	@echo "Object files: $(OBJ)"