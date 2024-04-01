# Conway's - Game of life

For local build:
```
mkdir build
cd build
cmake -D32BLIT_DIR=/path/to/32blit-sdk/ ..
```

For 32Blit build:
```
mkdir build.stm32
cd build.stm32
cmake .. -DCMAKE_TOOLCHAIN_FILE=/path/to/32blit-sdk/32blit.toolchain
```
