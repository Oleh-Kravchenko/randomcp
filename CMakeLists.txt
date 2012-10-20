PROJECT(randomcp)

ADD_DEFINITIONS(-pedantic -std=gnu99)

ADD_EXECUTABLE(randomcp
	main.c
	fcopy.c
	fcopy.h
	conf.c
	conf.h
	re.c
	re.h
)
