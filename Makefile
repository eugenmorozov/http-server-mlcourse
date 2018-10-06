all:
	g++ -std=c++11  -o  $(CLION_EXE_DIR)/http-server  main.cpp ./httpserver/Client.cpp ./httpserver/HttpServer.cpp ./httpserver/WorkerQueue.cpp ./httpserver/MimeType.cpp ./httpserver/HttpResponse.cpp ./httpserver/HttpRequest.cpp ./httpserver/HttpRequestParser.cpp ./httpserver/HttpRequestHandler.cpp  -levent -levent_pthreads -lpthread
