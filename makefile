rune :	Interpreter.o Parser.o Preprocessor.o RuneInterpreter.o Scanner.o
		g++ Interpreter.o Parser.o Preprocessor.o RuneInterpreter.o Scanner.o -o rune -std=c++11

Interpreter.o :	Interpreter.cpp Interpreter.h syntaxTreeEnum.h tokenEnum.h
	g++ Interpreter.cpp -c -std=c++11

Parser.o :	Parser.cpp Parser.h syntaxTreeEnum.h tokenEnum.h
	g++ Parser.cpp -c -std=c++11

Preprocessor.o :	Preprocessor.cpp Preprocessor.h syntaxTreeEnum.h tokenEnum.h
	g++ Preprocessor.cpp -c -std=c++11

RuneInterpreter.o :	RuneInterpreter.cpp RuneInterpreter.h syntaxTreeEnum.h tokenEnum.h
	g++ RuneInterpreter.cpp -c -std=c++11

Scanner.o :	Scanner.cpp Scanner.h tokenEnum.h
	g++ Scanner.cpp -c -std=c++11

clean : rm Interpreter.o Parser.o Preprocessor.o RuneInterpreter.o Scanner.o