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

Arguments can be passed to the macros from the command line by running in batch mode. If arguments are strings backslashes must proceed their quotation mark. For example, to load a ROOT file called ```MyAna.root``` and output a ROOT file called ```histograms.root``` and also print pdf images of all the plots run from the command line
```
root -l -b -q "readTree.cxx(\"MyAna.root\", \"histograms.root\", kTRUE)"
```
