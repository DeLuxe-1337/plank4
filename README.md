- Plank4

Compile/run in debug mode:
```
clear && make clean sanitize run
```

Debugger:
```
gdb ./bin/app
```

Project line count bash command:
```
find . -type f \( -name "*.c" -or -name "*.h" \) -exec wc -l {} +
```