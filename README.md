# unix_shell
A simple unix shell practice  
Written in c++.

Compile and Run:

1. cd ./build
2. compile: make
3. run: make run


Main features:
1. run program
2. redirection
3. pipes
4. builtins
5. bells and whistles

```bash
$ echo hello world > someFile
$ head < someFile
$ cat myFile | head -n 100 | tail -n 10 #gives me lines 90-100 of myFile
$ cd # cd to home
$ cd /home/Documents/
$ ./startMyWebServer& #start the webserver, but give me my shell back!
$ $MYVAR=something # set env
$ echo $MYVAR # get env
$ exit
```