For start testing you should donwload, install and build cxxtest library.
link: http://cxxtest.com/
After all of this, add cxxtestgen to PATH(or /usr/bin), add cxxtest folder to your compiler libs path and then follow next steps:
1) Create .cpp file via: cxxtestgen --error-printer -o runner.cpp <testHeader.h>
2) Compile it: g++ -I . runner.cpp -o runner
3) Execute: ./runner
