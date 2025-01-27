cd ../..
mkdir -p build
rm build/*
cd build
cmake ../src/tui #-DCMAKE_EXPORT_COMPILE_COMMANDS=1
cmake --build .
./status_tui
