OBJ   = obj/main.o      \
		obj/stack.o   \
		obj/errors.o    \
		obj/logs.o 		\
		obj/stack_debug.o 		\
		obj/general.o   \



FLAGS = -lubsan -D NDEBUG -g -std=c++14 -fmax-errors=1 			  	\
		-Wc++0x-compat -Wc++11-compat -Wc++14-compat  				\
		-Wcast-qual -Wchar-subscripts -Wconditionally-supported 	\
		-Wconversion  -Wctor-dtor-privacy -Wempty-body 				\
		-Wfloat-equal -Wformat-nonliteral -Wformat-security 		\
		-Wformat-signedness -Wformat=2 -Winline  -Wlogical-op 		\
		-Wmissing-declarations  -Wnon-virtual-dtor -Wopenmp-simd	\
		-Woverloaded-virtual -Wpacked -Wpointer-arith 				\
		-Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo	\
		-Wstack-usage=8192  -Wstrict-null-sentinel 					\
		-Wstrict-overflow=2 -Wsuggest-attribute=noreturn 			\
		-Wsuggest-final-methods -Wsuggest-final-types  				\
		-Wsuggest-override -Wswitch-default -Wswitch-enum 			\
		-Wsync-nand -Wundef  -Wunreachable-code -Wunused 			\
		-Wuseless-cast -Wvariadic-macros -Wno-literal-suffix 		\
		-Wno-missing-field-initializers -Wno-narrowing				\
		-Wno-old-style-cast -Wno-varargs -fcheck-new 				\
		-fsized-deallocation  -fstack-protector 					\
		-fstrict-overflow   -flto-odr-type-merging 					\
		-fno-omit-frame-pointer -fPIE -fsanitize=address  			\
		-fsanitize=bool -fsanitize=bounds -fsanitize=enum  			\
		-fsanitize=float-cast-overflow 								\
		-fsanitize=float-divide-by-zero 							\
		-fsanitize=integer-divide-by-zero -fsanitize=leak 			\
		-fsanitize=nonnull-attribute -fsanitize=null 				\
		-fsanitize=object-size -fsanitize=return 					\
		-fsanitize=returns-nonnull-attribute -fsanitize=shift 		\
		-fsanitize=signed-integer-overflow -fsanitize=undefined 	\
		-fsanitize=unreachable -fsanitize=vla-bound 				\

all: global

global: $(OBJ)
	g++ $(OBJ) -o stack -lm $(FLAGS)

obj/main.o: main.cpp src/stack/stack.h config.h src/errors_and_logs/errors.h src/errors_and_logs/logs.h src/stack/stack_debug.h
	g++ main.cpp -c -o obj/main.o $(FLAGS)

obj/stack.o: src/stack/stack.cpp src/stack/stack.h config.h src/errors_and_logs/errors.h src/stack/stack_debug.h src/general/general.h
	g++ src/stack/stack.cpp -c -o obj/stack.o $(FLAGS)

obj/errors.o: src/errors_and_logs/errors.cpp src/errors_and_logs/errors.h src/errors_and_logs/logs.h
	g++ src/errors_and_logs/errors.cpp -c -o obj/errors.o $(FLAGS)

obj/logs.o: src/errors_and_logs/logs.cpp src/errors_and_logs/logs.h config.h src/errors_and_logs/errors.h 
	g++ src/errors_and_logs/logs.cpp -c -o obj/logs.o $(FLAGS)

obj/stack_debug.o: src/stack/stack_debug.cpp src/stack/stack_debug.h src/stack/stack.h config.h src/errors_and_logs/errors.h src/general/general.h
	g++ src/stack/stack_debug.cpp -c -o obj/stack_debug.o $(FLAGS)

obj/general.o: src/general/general.cpp src/general/general.h 
	g++ src/general/general.cpp -c -o obj/general.o $(FLAGS)

.PHONY: cleanup

cleanup:
	rm obj/*.o stack