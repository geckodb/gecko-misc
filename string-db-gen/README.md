<h3 align="center">
String DB Gen
</h3>

###### A quick and dirty data dataset generator for generating real-word distributed datasets to evaluate and stress-test (string) databases

-------
<p align="center">
    <a href="#motivation">Motivation</a> &bull; 
    <a href="#installation">Installation</a> &bull; 
    <a href="#quick start">Quick Start</a> &bull; 
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

When you defined the variables to setup the scenario and data scale (i.e., `SCENARIO_NAME` and `TARGET_DATASIZE_BYTE`), type the following into your bash:

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

#### Example Output

The following first 20 lines might be generated for a `social` scenario (`head -20 output/social.csv`)

```
id;total_size_byte;length;string
0;2;1;s
1;4;1;s
2;46;41;poème pour begriffsklärung datei 16 filmk
3;103;56;mennicken – desktop programms von messung durch minimier
4;153;49;crocodile zwischen auf sortierung kategorie nach 
5;215;61;ségal ist orientalischer sitte offer ist orientalischer sitte
6;282;66;pejas alle für fließgewässer in evtl 29 de buss 1650 das und secon
7;319;36;mpt torpedo frankfurt berkersheim fl
8;355;35;herkenrath im in lösung häuser 36 r
9;391;35;pratje 1762 kategorie bestens wahre
10;471;79;property im in gehen sinnvoll zur taekwondoin kategorie schweiz kategorie schwe
11;536;64;krätschmer patrik mediziner von messung durch minimiere das kate
12;640;103;s0 oder groschke bandar e und second team in lösung einige unbenannte gehaltsgröße des deutscher untert
13;717;76;morali sowie indigo glas behälter kategorie sportgeschichte die kategorie na
14;783;65;rousson den begriffsklärung gerade geodäsie siehe auf kategorie l
15;1065;281;coda scène gewinner folgt genügt theorie neue eines anderen baute gründete banteay kdei srah der kategorie läßt kategorie sportgeschichte · kerbstock gletscherschmelze nur sprachliche gutachten gutachten gutachten das tim iserlohn genna stadium 1910 francesco des freier raphael he
16;1129;63;statistik siehe auch deutschland kategorie sportgeschichte gilt
17;1178;48;luzarches le toledot reihen schaltkontakten dies
18;1475;296;brisson jahre ludwig noll otto kategorie läßt netzkultur ru русская ивановка russkaja aus und die israelischer ram inhalte als begriffsklärung datei 16 mm begriffsklärung gerade geodäsie siehe auch kategorietext kategorie nach apokalyptischen ausmaßes daraufhin seit dieser der niederlande hohlma
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
