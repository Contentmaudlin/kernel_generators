TESTS = fib str_tok

CC = g++
CXX = $(CC)

INCLUDE = src/include impl
CXXFLAGS = -g -Wall -std=c++17 $(foreach i, $(INCLUDE), -I$(i))
LDFLAGS = -g

TEST_DIR = tests
OUT = out
TEST_EXES = $(foreach t, $(TESTS), $(OUT)/$(t))
TEST_OBJS = $(foreach t, $(TESTS), $(TEST_DIR)/$(t).o)

PHONY += default
default: $(TEST_EXES)

PHONY += $(TESTS)
$(TESTS): %: $(OUT)/%

$(TEST_EXES): $(OUT)/%: $(TEST_DIR)/%.o | $(OUT)
	$(CC) $(LDFLAGS) -o $@ $<
$(TEST_OBJS): %.o: %.cpp

$(OUT):
	mkdir -p $@

PHONY += clean
clean:
	rm -rf $(TEST_OBJS) $(OUT)

.PHONY: $(PHONY)
