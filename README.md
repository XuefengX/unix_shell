# unix_shell
A simple unix shell practice  
Written in c++.

Usage:  
1. Enter ```./build```
2. Compile: ```make```
3. Run the shell: ```make run``` 

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
$ cd ~
$ ./startMyWebServer& #start the webserver, but give me my shell back!
$ $MYVAR=something
$ echo $MYVAR
$ exit
```