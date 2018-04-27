# for ever <test>.cpp file you put in the tests/ directory
# add it to the TESTS variable
TESTS = fib fib_basic fib_iter fib_until str_tok

SRC_DIR = ../src

CC = g++
CXX = $(CC)

INCLUDE = src/include impl
CXXFLAGS = -g -Wall -std=c++17 $(foreach i, $(INCLUDE), -I$(i))
LDFLAGS = -g

SRC_LN = src
OUT_DIR = out
TEST_DIR = tests

TEST_EXES = $(foreach t, $(TESTS), $(OUT_DIR)/$(t))
TEST_OBJS = $(foreach t, $(TESTS), $(TEST_DIR)/$(t).o)

KGEN_HEADERS = $(SRC_LN)/include/kgen.hpp
IMPL_HEADERS = impl/*.hpp

PHONY += default
default: $(TEST_EXES)

$(KGEN_HEADERS): | $(SRC_LN)
$(SRC_LN):
	ln -s $(SRC_DIR) $@
$(OUT_DIR):
	mkdir -p $@

PHONY += $(TESTS)
$(TESTS): %: $(OUT_DIR)/%

$(TEST_EXES): $(OUT_DIR)/%: $(TEST_DIR)/%.o | $(OUT_DIR)
	$(CC) $(LDFLAGS) -o $@ $<
$(TEST_OBJS): %.o: %.cpp $(KGEN_HEADERS) $(IMPL_HEADERS)


PHONY += clean
clean:
	rm -rf $(TEST_OBJS) $(OUT_DIR)
mrproper: clean
	rm -rf $(SRC_LN)

.PHONY: $(PHONY)
