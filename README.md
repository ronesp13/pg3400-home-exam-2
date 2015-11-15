# Home Exam 2
## Programmering i C for Linux
###### Espen Rønning & Pia Dokken Stranger Johannessen

### Compilation
To compile the program use the `Makefile` functionality by running the following command in the terminal.

```sh
$ make
```

### Usage
The program is designed to be a CLI program.

It is assumed that all the keyfiles are located in the folder `songLibrary`.

By default the program writes output to the terminal.
```sh
$ ./westergate -enc myMessageFile.txt songLibrary/keyFile.txt
```
If you want to store the output to a file, use the > operator.
```sh
$ ./westergate -enc myMessageFile.txt songLibrary/keyFile.txt > myoutputfile.txt
```

#### Encoding
Encode a messagefile by running the command:
```sh
$ ./westergate -enc messageFile.txt songLibrary/keyFile.txt
```
The command above will result in an encoding that is vulnerable to frequency analysis, therefore, if you want
a more secure encoding, specify a `d` value. In the example below 25 is given as an example, you can input your own value. In some cases a file can not be encrypted with a `d` value.

```sh
$ ./westergate -enc messageFile.txt songLibrary/keyFile.txt -d 25
```

#### Decoding
Decode a encoded file by running the command:
```sh
$ ./westergate -dec encodedMessageFile.txt songLibrary/keyFile.txt
```

#### Hacking
Hack an encoded file by running the command:
```sh
$ ./westergate -hax encodedMessageFile.txt
```

### About the solution
When "hacking" encoded files we're making the assumption that all the words in the hidden message must be present in the `/usr/share/dict/words` file.
With this assumption even the smallest typo might result in a valid key being rejected. A possible solution might be to, for each key, 
keep track of a percentage of how many words matches with this key. If a key generates 80% or more correct words then there's
a reasonable chance that this key is valid.

### References
* [http://www.programmingsimplified.com/c/source-code/c-program-binary-search](http://www.programmingsimplified.com/c/source-code/c-program-binary-search) - Used as inspiration for the binary search function.
* [https://gist.github.com/mycodeschool/7510222](https://gist.github.com/mycodeschool/7510222) - Used as inspiration for the queue implementation.
