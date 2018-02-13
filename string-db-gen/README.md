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
rm files/
ant
mkdir output
``` 

Afterwards, the exectuable jar `build/string-db-gen.jar` is ready to generate datasets.

### Behind the Scenes
The bash commands above fetch `dewiki-dataset.tar.gz` from our public DropBox folder. This archive contains analysis results of the German Wikipedia articles dump as of 2018/Feb/05 and is used for string generating. In case you want to setup other files, see usage information of  `build/string-db-gen.jar`.


## Quick Start

`String DB Gen` is shipped with four pre-configured string generation configurations, called Scenarios. A scenario is a description of the minimum/maximum string length along with particular number generator to model a certain real-world domain. Before a data generation process can be started, the scenario must be specified over the program argument `-s <alias>`.

### Scenarios

The following scenarios are supported out-of-the-box:

| Scenario | Min Len | Max Len | Number Generator | Alias |
|----------|---------|---------|------------------|-------|
| Social Networking Service | 1 | 280 | normal distribution | `social` |
| Instant Messaging Service | 10 | 65536 | zipf distribution | `instant` |
| Synthetic Transaction Benchmark | 10 | 50 | uniform distribution | `synth` |
| Public Knowledge Base Articles | 16 | 2776418 | zipf distribution | `base` |

Each scenario generates a different set of strings according the min/max string length and the number generator used. The generated strings themself are constructed internally using a probabilistic generatative grammar that result from a analysis of approx. 2 billion articles of the German Wikipedia.

To select a particular scenario either use `-s <alias>` directly, or create a (temporary) variable in bash.

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

For the purpose of fast construction of this grammer and for reasonable character settings, `String DB Gen` performs a pre-processing on the required input files that are needed for the grammar (i.e., file stored in `dewiki-dataset.tar.gz`). However, this pre-processing results in a cleaned ready-to-use set of files, the *cache*. This cache is constructed the first time `String DB Gen` is started by default. For this purpose, `String DB Gen` must know where the required input files are (via `-a`, `-b`, and `-c` arguments, e.g., `-a dewiki-dataset/files/dewiki-articles-word-freq.csv -b dewiki-dataset/files/dewiki-articles-next-words.txt -c dewiki-dataset/files/dewiki-articles-starter-words.csv`). Until the cache is explicitly removed via the `--clean-cache` argument, the input file arguments (`-a`, `-b`, and `-c`) are ignored and the grammar is constructed from the cache (which is stored by in `cache/`) once the cache is created. 


For ease of use, type the following inside your bash if you want to use a pre-built cache based on the contents of `dewiki-dataset.tar.gz`:

```
wget https://www.dropbox.com/s/x3llefc3oabnchb/dewiki-dataset-cache.tar.gz?dl=1
tar xvzf dewiki-dataset-cache.tar.gz?dl=1
rm dewiki-dataset-cache.tar.gz?dl=1
```


#### Data Generation


When you defined the variables to setup the scenario and data scale (i.e., `SCENARIO_NAME` and `TARGET_DATASIZE_BYTE`) the data generation can start. Type the following into your bash:

``` 
java -jar build/string-db-gen.jar -a dewiki-dataset/files/dewiki-articles-word-freq.csv -b dewiki-dataset/files/dewiki-articles-next-words.txt -c dewiki-dataset/files/dewiki-articles-starter-words.csv -s ${SCENARIO_NAME} -t ${TARGET_DATASIZE_BYTE} -o output/${SCENARIO_NAME}.csv
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

At the first start of `String DB Gen` the cache is created (when not already present by pre-built caches). Once the cache is available, the arguments `-a`, `-b`, and `-c` are ignored. Hence, they can be left out. Type the following into you bash to generate data when the cache is already present.

``` 
java -jar build/string-db-gen.jar -s ${SCENARIO_NAME} -t ${TARGET_DATASIZE_BYTE} -o output/${SCENARIO_NAME}.csv
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

#### Determining Letter Frequency

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


# License
This project is licensed under the terms of the GNU LESSER GENERAL PUBLIC LICENSE. See the LICENSE file.

# Contributing
1. Discuss about your idea in a new issue
2. Fork Gecko Misc (https://github.com/geckodb/gecko-misc.git)
3. Create your one new feature branch (git checkout -b my-feature)
4. Commit your changes (git commit -am 'Add some feature')
5. Push to the branch (git push origin my-feature)
6. Create a new Pull Request