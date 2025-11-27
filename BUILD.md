# Build

- up: [README](./README.md)

## Windows

```bash
make PLATFORM=windows TARGET=windows
```

### Run

```bash
./build/windows/main.exe
```


## Linux

```bash
make PLATFORM=linux TARGET=linux
```

or just

```bash
make
```

### Run

```bash
./build/linux/main
```


## Cross compile for Windows on Linux

```bash
make PLATFORM=linux TARGET=windows
```

or

```bash
make TARGET=windows
```

### Run

```bash
./build/windows/main.exe
```