CPPFLAGS ?= -Wall -Wextra -Werror=format -Werror=return-type -Werror=vla -Werror=uninitialized -Werror=maybe-uninitialized
CFLAGS ?= -Werror-implicit-function-declaration -Werror=declaration-after-statement
CXXFLAGS ?= -std=gnu++11
VG_OPT ?= -g -O1
VG_FLAGS ?= -q --track-origins=yes --malloc-fill=66 --free-fill=66 --error-exitcode=1 --leak-check=full --track-fds=yes
ifneq ($(findstring --track-fds=yes, $(VG_FLAGS)), )
  VG_FILTER ?= tools/filter_unclosed_files
endif
ASAN_OPT ?= -fsanitize=address -fno-omit-frame-pointer -g -O2
#LDFLAGS ?= -lm
STYLE_CHECKER ?= tools/StyleChecker/formatter

# disable builtin rules
%: %.c
%: %.cpp
#%: %.o
#%.o: %.c

# Coding Style Checker
$(STYLE_CHECKER):
	cd `dirname $@` && $(MAKE) 

# 1-step compilation
# Valgrind
%.exe: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(VG_OPT) $(LDFLAGS) $^ -o $@

%.exe: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(VG_OPT) $(LDFLAGS) $^ -o $@

# AddressSanitizer
%.x: %.c $(STYLE_CHECKER)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(ASAN_OPT) $(LDFLAGS) $< -o $@
	$(STYLE_CHECKER) $< || $(STYLE_CHECKER) $< $< > /dev/null || exit 0

%.x: %.cpp $(STYLE_CHECKER)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(ASAN_OPT) $(LDFLAGS) $< -o $@
	$(STYLE_CHECKER) $< || $(STYLE_CHECKER) $< $< > /dev/null || exit 0

# My Prec-c-c-cious
.PRECIOUS: %.exe %.x # %.txt

# Run
# Clean run + input.txt + output.txt
%.txt: %.exe
	tee $@ | ./$< | tee output.txt

# Run under Valgrind and AddressSanitizer
%: %.exe %.x %.txt
	$(VG_FILTER) valgrind $(VG_FLAGS) ./$@.exe < $@.txt > output_vg.txt
	@diff output.txt output_vg.txt
	@$(RM) output_vg.txt

	ASAN_OPTIONS=symbolize=1 ASAN_SYMBOLIZER_PATH=/usr/bin/llvm-symbolizer ./$@.x < $@.txt > output_asan.txt
	@diff output.txt output_asan.txt
	@$(RM) output_asan.txt

	@echo "=== SUCCESS! ==="

clean:
	$(RM) *.exe *.x input.txt output.txt output_vg.txt output_asan.txt

# Trash:
# 2-steps compilation
#%.exe: %.o
#	$(CC) $(LDFLAGS) $^ -o $@

#%.o: %.c
#	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<

#%.o: %.cpp
#	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

#.SECONDARY: %.exe
