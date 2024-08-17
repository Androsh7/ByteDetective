This is a simple tool that reads executable binaries and can perform automated string analysis


### Syntax

```
malware_analysis [--bintostr] [--analysis] {filename}
```

-b, --bintostr {binaryfile} [minstrlen] : this reads an executable binary and trys to grab strings of a length equal to or greater than minstrlen The strings are then written to a new file which is made by appending '.string' to the filename i.e: malware.exe becomes malware.exe.string. Note the default for minstrlen is 4 characters

-a, --analysis {stringfile} : this reads a string file (create with option -b) and creates a report and writes it to a newfile named by appending '.report' to the filename i.e: malware.exe.string becomes malware.exe.string.report

-d, --debug : this prints optional debug optional (similar to -v verbose option on linux commands)

/?, --help : brings up this help menu

