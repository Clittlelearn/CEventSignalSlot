cd build && cmake .. -G "MinGW Makefiles" && mingw32-make
%cd build && g++ ../*.cpp ../windows/*.cpp -I ../windows/ -o CEvent -lws2_32 -gdwarf-2%