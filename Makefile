main:
	g++ -o parent.out ParentProcess.cpp -lpthread

reader: 
	g++ -o reader.out ReadProcess.cpp -lpthread

writer:
	g++ -o writer.out WriteProcess.cpp -lpthread

all:
	g++ -o parent.out ParentProcess.cpp -lpthread
	g++ -o reader.out ReadProcess.cpp -lpthread
	g++ -o writer.out WriteProcess.cpp -lpthread

clean:
	rm parent.out
	rm writer.out
	rm reader.out
	rm newfile.txt