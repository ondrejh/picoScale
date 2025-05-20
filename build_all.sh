# build for pico

mkdir -p build
cd build
rm -rf *
cmake ../src
make
cd ..

# build for PC

mkdir -p simul
cd simul
rm -rf *
cmake ../src -DBUILD_FOR_PICO=OFF
make
cd ..
