# TTree-MakePlots
Example tutorial of how to make a TTree in ROOT and then read that TTree to make histograms quickly using the Project() method.

Executing ```makeTree.cxx``` from the command line
```
$ root -l -q makeTree.cxx
```
will ceate a ROOT file that has a TTree in it with two branches. To quickly browse the structure of a ROOT file from the command line, open the file with ROOT directly
```
$ root -l MyAna.root
```
and then use the ```ls()``` method on the pointer that has been created to the file
```
root [1] _file0->ls()
```

Executing ```readTree.cxx``` from the command line
```
$ root -l -b -q readTree.cxx
```
will then load the ROOT file, read the TTree, make plots from the branches of the TTree, and then write those plots to a ROOT file.

Arguments can be passed to the macros from the command line by running in batch mode in two different ways. The following examples will all load a ROOT file called ```MyAna.root``` and output a ROOT file called ```histograms.root``` and also print pdf images of all the plots.
###### Fixed Arguments
To pass constant arguments of any type from the command line enclose the macro name and arguments in single quotes.
```
root -l -b -q 'readTree.cxx("MyAna.root", "histograms.root", kTRUE)'
```
###### Shell Variable Arguments
To pass shell variable arguments enclose the macro name and arguments in double quotes. If arguments are strings backslashes must proceed their quotation mark. The following would be the contents of the simpilest shell script where all three arguments are shell variables.<sup id="ref1">[1](#footnote1)</sup>
```bash
#!/bin/bash
myInput=\"MyAna.root\"
myOutput=\"histograms.root\"
myBool=kTRUE
root -l -b -q "readTree.cxx(${myInput}, ${myOutput}, ${myBool})"
```
## References
<a name="footnote1">1</a>: jfcaron's answer to [**passing arguments to a macro**](https://root.cern.ch/phpBB3/viewtopic.php?f=3&t=2882&p=69672&#p69672 "RootTalk: RE: passing arguments to a macro") on RootTalk. [↩](#ref1)
