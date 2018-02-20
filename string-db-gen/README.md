<h3 align="center">
String DB Gen
</h3>

###### A quick and dirty data dataset generator for generating real-word distributed datasets to evaluate and stress-test (string) databases

-------
<p align="center">
    <a href="#motivation">Motivation</a> &bull; 
    <a href="#installation">Installation</a> &bull; 
    <a href="#quick-start">Quick Start</a> &bull; 
    <a href="#license">License</a> &bull; 
    <a href="#contributing">Contributing</a>
</p>
 
-------

Get in contact with the developer on Twitter: [@marcus_pinnecke](https://twitter.com/marcus_pinnecke)

## Motivation
System evaluation is often all about having the matching dataset at hand. Synthetic dataset are a handy tool when it comes to evaluating the effect of data scale. Unfortunately, current benchmark dataset generators either miss to explicitly focus on (variable-length) strings, pay not attention to real-world distribution of string lengths, or misses to consider word and letter frequency in the generated strings. Ignoring the string length distribution (e.g., by forcing strings to have a particular size) hides the complexity a real-world system will have to handle different string lengths - especially when these string lengths are skewed. Likewise, by irgnoring the word and letter frequency inside the strings (e.g., by generating strings over arbitrary characters drawn from a uniform distribution) distorts the effects of compressions the systems under evaluation may apply (especially if these system apply runtime compression that focusses on this frequency, such as Huffman coding). A consequence of such missing features limit the realibility of comparison evaluations. `String DB Gen` closes this gap by generating string datasets from different domains emphasising variable length strings with a natural letter frequency. 

## Installation
The easiest way is to built `String DB Gen` from sources and fetch external dependencies. 

### Bash Commands
For this, type into the bash

```
git clone https://github.com/geckodb/gecko-misc.git
cd gecko-misc/string-db-gen
wget https://www.dropbox.com/s/vhbq52vrti8isxx/dewiki-dataset.tar.gz?dl=1
tar xvzf dewiki-dataset.tar.gz?dl=1
rm dewiki-dataset.tar.gz?dl=1
mkdir -p dewiki-dataset/files
mv files/* dewiki-dataset/files
rm -rf files/
mvn dependency:resolve
ant
mkdir output
``` 

Afterwards, the exectuable jar `build/string-db-gen.jar` is ready to generate datasets.

### Behind the Scenes
The bash commands above fetch `dewiki-dataset.tar.gz` from our public DropBox folder. This archive contains analysis results of the German Wikipedia articles dump as of 2018/Feb/05 and is used for string generating. In case you want to setup other files, see usage information of  `build/string-db-gen.jar`. What it needs to change the entire string creation process from a `dewiki`-based analysis to a custom one, is to provide 4 files:
####  Start Words File
A plain text file where each line is one word that was used as the first word in one sentence from the original corpus. Avoid duplicate entries.

*Example*
```
Word
anschluss
diese
die
(...)
```

### Word Frequency File
A CSV file formatted as `<Frequency>;<Length>;<Word>` (with header) where `word` is one word in the corpus, `frequency` is the total number of occurrences of `word` in the corpus, and `length` is the length of `word` (exclusing null terminator).

*Example*
```
Frequency;Length;Word
67888;1;%
24632;1;+
15033;1;0
(...)
```

### Next Words File
A plain text file that maps each word to successor words in the form `<word> <succ1> <succ2> ... <succN>` where `word` is some word in the corpus, and `succ1` was one word directly written after `word` in the corpus, `succ2` is another word somewhere else written directly after `word` and so on.

*Example*
```
kosten fixkostenproportionalisierung forderung forderungsmanagement    
seefahrer landen heinrich ließen zugleich
(...)
```


### Sentence Length File
A CSV file formatted as `<StringLength>;...` (with header) where `<StringLength>` is a article/sentence lengths in the original corpus. This file is used to create a histogram on the sentence lengths in order to create a non-uniform distribution which is used in `String DB Gen`s random number generator for sentence lengths. 

*Example*
```
StringLength;Tag
7772;Default
18526;Default
20475;Default
3392;Default
```	

This file will be processed during cache building with `hd.jar` internally. Which bin width is used can be controlled via the `-w` parameter in `build/string-db-gen.jar`.

## Quick Start

`String DB Gen` is shipped with four pre-configured string generation configurations, called Scenarios. A scenario is a description of the minimum/maximum string length along with particular number generator to model a certain real-world domain. Before a data generation process can be started, the scenario must be specified over the program argument `-s <alias>`.

### Scenarios

The following scenarios are supported out-of-the-box:

| Scenario | Min Len | Max Len | Number Generator | Alias |
|----------|---------|---------|------------------|-------|
| Social Networking Service | 1 | 280 | normal distribution | `social` |
| Instant Messaging Service | 10 | 65536 | zipf distribution | `instant` |
| Synthetic Transaction Benchmark | 10 | 50 | uniform distribution | `synth` |
| Public Knowledge Base Articles | 16 | 2776418 | histogram-based distribution | `base` |

Each scenario generates a different set of strings according the min/max string length and the number generator used. The generated strings themself are constructed internally using a probabilistic generatative grammar that result from a analysis of approx. 2 billion articles of the German Wikipedia.

To select a particular scenario either use `-s <alias>` directly, or create a (temporary) variable in bash. Optionally, you can use `base-zipf` as replacement for the alias `base` to switch from a histogram-based number generator to a zipf-based number generator (with exponent `1`). If you want zipf-based number generation for base with exponent `2` or `3`, use `base-zipf-2` or `base-zipf-3` instead.

#### Select Social Networking Service
```
SCENARIO_NAME=social
```

#### Select Instant Messaging Service
```
SCENARIO_NAME=instant
```

#### Select Synthetic Transaction Benchmark
```
SCENARIO_NAME=synth
```

#### Select Public Knowledge Base Articles
```
SCENARIO_NAME=base
```

### Define the Data Scale

`String DB Gen` is built with the idea in mind to explictly scale to a particular pre-defined size in byte. To run the data generator, it is required to specify the target string dataset size (in byte) via the ` -t <size>` program argument. Alternativelly, create a (temporary) variable in bash.

#### 1 KiB
```
TARGET_DATASIZE_BYTE=1024
```

#### 1 MiB
```
TARGET_DATASIZE_BYTE=1048576
```

#### 1 GiB
```
TARGET_DATASIZE_BYTE=1073741824
```

#### 1 TiB
```
TARGET_DATASIZE_BYTE=1099511627776
```

### Start Data Generation

To generate natural text, `String DB Gen` requires to built its generative grammar. For this, it first pre-processes and caches the analysis files used to generate the generative grammar. Then, it uses this cache to generate text according the user-defined dataset scale and scenario.



#### Pre Processing and Caching

For the purpose of fast construction of this grammer and for reasonable character settings, `String DB Gen` performs a pre-processing on the required input files that are needed for the grammar (i.e., file stored in `dewiki-dataset.tar.gz`). However, this pre-processing results in a cleaned ready-to-use set of files, the *cache*. This cache is constructed the first time `String DB Gen` is started by default. For this purpose, `String DB Gen` must know where the required input files are (via `-a`, `-b`, `-c` and `-d` arguments, e.g., `-a dewiki-dataset/files/dewiki-articles-word-freq.csv -b dewiki-dataset/files/dewiki-articles-next-words.txt -c dewiki-dataset/files/dewiki-articles-starter-words.csv -d dewiki-dataset/files/dewiki-articles-lengths.txt`). Further, the file for `-d` will be processed internally with `build/hd.jar` (see below) in order to create a histogram on the sentences length for the non-uniform number generator. Until the cache is explicitly removed via the `--clean-cache` argument, the input file arguments (`-a`, `-b`, `-c`, `-d`, and `-w`) are ignored and the grammar is constructed from the cache (which is stored by in `cache/`) once the cache is created. 


For ease of use, type the following inside your bash if you want to use a pre-built cache based on the contents of `dewiki-dataset.tar.gz`:

```
wget https://www.dropbox.com/s/x3llefc3oabnchb/dewiki-dataset-cache.tar.gz?dl=1
tar xvzf dewiki-dataset-cache.tar.gz?dl=1
rm dewiki-dataset-cache.tar.gz?dl=1
```


#### Data Generation


When you defined the variables to setup the scenario and data scale (i.e., `SCENARIO_NAME` and `TARGET_DATASIZE_BYTE`) the data generation can start. Type the following into your bash:

``` 
java -jar build/string-db-gen.jar -a dewiki-dataset/files/dewiki-articles-word-freq.csv -b dewiki-dataset/files/dewiki-articles-next-words.txt -c dewiki-dataset/files/dewiki-articles-starter-words.csv -d dewiki-dataset/files/dewiki-articles-lengths.txt -s ${SCENARIO_NAME} -t ${TARGET_DATASIZE_BYTE} -o output/${SCENARIO_NAME}.csv
```

`String DB Gen` will provide you with status information similar to these:

```
This is String-DB-Gen. Copyright (c) Marcus Pinnecke 2018
https://github.com/geckodb/gecko-misc/string-db-gen
Loading word frequencies...
Loading sequent words...
Loading starter words...
Start data generator...
Elapsed: 0h 00min 55sec	 ETA: 0h 19min 52sec	48,1 KiB of 1,0 MiB		4,6956062%
Elapsed: 0h 00min 57sec	 ETA: 0h 18min 24sec	53,4 KiB of 1,0 MiB		5,2142143%
...
```

This will generate a dataset `output/${SCENARIO_NAME}.csv` which is ready to use after the process terminates.

##### Note 

At the first start of `String DB Gen` the cache is created (when not already present by pre-built caches). Once the cache is available, the arguments `-a`, `-b`, `-c`, and `-d` are ignored. Hence, they can be left out. Type the following into you bash to generate data when the cache is already present.

``` 
java -jar build/string-db-gen.jar -s ${SCENARIO_NAME} -t ${TARGET_DATASIZE_BYTE} -o output/${SCENARIO_NAME}.csv
```

##### Profiling

Whenever you want to store the time it takes to generate a specific dataset, use `-i` (`--profile`) `<FILE>` to append statistics to the file `<FILE>`. For the purpose of distinguishing between several profile settings, use `-u` (`--tag`) `<TAG>` to add a tag string to the statistics. 

**Example**

```
$ java -jar build/string-db-gen.jar -i output/profile.csv -u "Sample1"             \
                                    -s ${SCENARIO_NAME} -t ${TARGET_DATASIZE_BYTE} \
                                    -o output/${SCENARIO_NAME}.csv 
$ cat output/profile.csv
60828;Sample1
```

#### Example Output

The following first 20 lines might be generated for a `${SCENARIO_NAME}` scenario (`head -20 output/${SCENARIO_NAME}.csv`)

```
id;total_size_byte;length;string
0;161;160;tassilo inkognito nicht namen posener les kategorie schweiz kategorie schweiz lyrik sowie indigo glas werden iso 639 aktuelle r a kufi inschrift liste in gehen 
1;326;164;oderunt bericht ganz budapest illner an kategorie sportgeschichte der niederlande kategorie schweiz kategorie bestens gedeckten stiegenaufgang gelangt in gehen sinn
2;479;152;556 kategorie schweiz kategorie schweiz benediktiner kloster kategorie nach romanen selbstbezeichnung nicht namen posener les gestorben kanadischer hait
3;586;106;zdv der kategorie sportgeschichte bei alfonso katholischer ostkirchen das und second team training erstes 
4;739;152;spezialgebiete redaktion wikipedia wikimedia im museum p mit temple temple inland im in gehen sinnvoll zur deutschen regiolekt die kategorie bestens nac
5;851;111;differenzielle und second all hockey dezember daxen in evtl 9 jahr nach apokalyptischen reiter maler datei 16 m
6;999;147;year filmpreis inszenierten 1618 sophia hochschullehrer rudolf deutscher irgendwas hier ebenfalls weitere details navigationsleiste verwaltungsglie
7;1177;177;alwine bochum im in gehen sind kann aber verstarb bonfini ist motive ausgaben fehlen ist orientalischer gerichte coordinate tool in gehen sinnvoll zur deutschen kultur kategorie
8;1344;166;apsa 21 03 19 stadtgeschichte erinnerungen verwitwete waschfrau traute man wurde bewegt wichtige branche krause willy kategorie nach apokalyptischen antisemitismus ka
9;1483;138;podhorany ist werden iso kategorie bestens besorgt der niederlande kategorie nach romanen selbstbezeichnung georgios griechenland kategori
10;1657;173;belikow erinnert tages post dampf aus atmung die 1960 kategorie nach apokalyptischen reiter kategorie schweiz person arad kategorie bestens und die britischer guyanischer po
11;1763;105;kapustin jar flight nach romanen die us produktion england richard kategorie sportgeschichte die literatu
12;1860;96;rehner biochemie bei xii danach ist orientalischer musik von auf hilfsschiff en zirbes ist motiv
13;1998;137;cynthia geboren albert frankreich volumen halbierenden in gehen sinnvoll zur taekwondoin kategorie sportgeschichte eingegangen so nach ap
14;2157;158;moskowski er ist motive briefmarken philatelie 1950er rennwagen ist werden navigationsleiste vorlage gallipoli suvla war modell in evtl 29 de albert innenmini
15;2303;145;7650 kaname ide client software sprites in evtl 29 jpg in gehen sind insgesamt 1919 bayern reichstagsabgeordneter gutfleisch ist motive briefmark
16;2449;145;muris ab beginnt nun und second all ed autor sortierung kategorie schweiz lyrik sowie indigo glas werden iso kategorie sportgeschichte dies 1 she
17;2550;100;naphta feuer n nick kategorie nach apokalyptischen rituals bei datei 16 filmkamera nahezu wehrlose o
18;2657;106;crusaders winnipeg gewann spielte 1869 kategorie sportgeschichte dies 1 she nimmt ist orientalischer kleid
``` 

### Determining Letter Frequency

`String DB Gen` is shipped with a built-in letter frequency analysis tool. You run it by typing the following inside your bash:

```
java -jar build/cf.jar --file <file>
```

where `<file>`is the text file `cf` should analyse. The output is formatted as CSV of the form `<code>;<character>;<count>` where `<code>` is the character code, `<character>` is the character and `<count>` is the absolute number of occurrences of `<character>` in `<file>`. Please note that `cf` is *case insensitive*.

**Example Output**

```
Code;Character;Count
32; ;443417
97;a;225816
98;b;57722
99;c;100478
100;d;96665
101;e;479593
102;f;33353
103;g;116761
104;h;119120
```		

**Check Letter Frequency of Generated Scenarios**

To check the letter frequency of generated scenarios, use `cf`. For example:

```
TARGET_DATASIZE_BYTE=1048576
java -jar build/string-db-gen.jar -s social -t ${TARGET_DATASIZE_BYTE} -o output/social.csv
java -jar build/string-db-gen.jar -s instant -t ${TARGET_DATASIZE_BYTE} -o output/instant.csv
java -jar build/string-db-gen.jar -s synth -t ${TARGET_DATASIZE_BYTE} -o output/synth.csv
java -jar build/string-db-gen.jar -s base -t ${TARGET_DATASIZE_BYTE} -o output/base.csv
java -jar build/cf.jar --file output/social.csv > output/social-cf.csv
java -jar build/cf.jar --file output/instant.csv > output/instant-cf.csv
java -jar build/cf.jar --file output/synth.csv > output/synth-cf.csv
java -jar build/cf.jar --file output/base.csv > output/base-cf.csv
```

### Determining String Lengths

`String DB Gen` is shipped with a built-in string length analysis tool. You run it by typing the following into your bash:

```
java -jar sl.jar -f <FILE> [-h] [-n] [-t <NAME>] [-x]
```

where `FILE` is the file name of a textural files, and `NAME` is a optional tag name (`Default` by default). The tool maps each line in `FILE` to its number of characters and outputs the result formatted as CSV to standard out. Optinally, you can disable to print the CSV header by using `-n` (resp. `--no-header`) and disable to print results by using `-x` (resp. `--no-content`). Feel free to combine `-n`, `-x`, and `-tag` for weird piping stuff.

**Example Input** (stored in `misc/sample-text.txt`)

```
Lorem ipsum dolor sit amet, consetetur sadipscing elitr, 
sed diam nonumy eirmod tempor invidunt 
ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero 
eos et accusam et justo duo dolores et ea rebum. 
```

**Example Output**


By running
```
java -jar build/sl.jar -file misc/sample-text.txt -tag "SAMPLE"
```
the following is prompt to standard output

```
StringLength;Tag
57;SAMPLE
39;SAMPLE
68;SAMPLE
49;SAMPLE
```

**Example For Weird Piping Stuff**

```
java -jar build/sl.jar -file /dev/null --no-content >> output/sl-test.csv && 
java -jar build/sl.jar -file misc/sample-text.txt -tag "source 1" --no-header >> output/sl-test.csv && 
java -jar build/sl.jar -file misc/sample-text-2.txt -tag "source 2" --no-header >> output/sl-test.csv && 
cat output/sl-test.csv

StringLength;Tag
57;source 1
39;source 1
68;source 1
49;source 1
18;source 2
23;source 2
41;source 2
26;source 2
49;source 2
49;source 2
49;source 2
```


### Creating Histogram-Based Distributions
In order to create random values based on a non-uniform custom-defined distribution, we ship `String DB Gen` is a tool to create a number generator that creates numbers according a given histogram. To create the input files for this number generator, use the tool `hd.jar` in `build/`. The usage is as follows:

```
java -jar build/hd.jar -f <FILE> [-h] -w [-r] <SIZE>
```
where `FILE` is the file name of a CSV file that matches the form `<StringLength>;...` (with header), and `-w` is a parameter to set the bin size to the valuer `SIZE`. By using `-r` (`--relative`), the tool will produce relative numbers per bucket instead of absolute ones.

**Example Input** (using `sl.jar`)
```
mkdir sample-sl-hd-temp && 
java -jar build/sl.jar -f misc/sample-text-3.txt -tag "SAMPLE" > sample-sl-hd-temp/test-sl-text && 
java -jar build/hd.jar --f sample-sl-hd-temp/test-sl-text -w 2 > sample-sl-hd-temp/histogram.csv && 
cat sample-sl-hd-temp/histogram.csv && 
rm -rf sample-sl-hd-temp/
```

**Example Output**
```
MaxLength;Count;Threshold
68;12;0
8;2;12
72;1;14
12;1;15
20;6;16
22;4;22
88;6;26
24;1;32
26;2;33
30;5;35
38;5;40
52;11;45
54;6;56
56;6;62
60;5;68
```

### Extract Strings from JSON Documents
`String DB Gen` comes with a tool to extract values by a given key in a sequence of JSON documents. For this purpose, use the tool `build/je.jar`. This tool reads a file where each line is one complete JSON document. From each line, the value for a user-defined key is extracted and printed to standard out (if the key is present at the first level of the document). 

The usage is as follows:

```
java -jar je.jar -f <FILE> [-h] -k <STRING>
```
where `<FILE>` is the path to a text file (example see below), and `<STRING>` is the key name that is used (cases are ignored). 

**Example Input**

```
$ cat misc/jsonstream.txt 
{"created_at":"Fri Dec 01 10:20:00 +0000 2017", "text":"Hello"}
{"created_at":"Thu Oct 06 03:16:40 +0000 2016", "text":"World"}
```

**Example Output**

```
$ java -jar build/je.jar -f misc/jsonstream.txt -k "text"
Hello
World
```



# License
This project is licensed under the terms of the GNU LESSER GENERAL PUBLIC LICENSE. See the LICENSE file.

# Contributing
1. Discuss about your idea in a new issue
2. Fork Gecko Misc (https://github.com/geckodb/gecko-misc.git)
3. Create your one new feature branch (git checkout -b my-feature)
4. Commit your changes (git commit -am 'Add some feature')
5. Push to the branch (git push origin my-feature)
6. Create a new Pull Request
