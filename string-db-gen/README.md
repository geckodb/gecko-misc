
### Program Options

```

usage: ${JAR-NAME}
 -o,--output <arg>     output file name
 -s,--scenario <arg>   scenario name ('social', 'instant', 'synth',
                       'base')
 -t,--target <arg>     target size in byte

```


### Example Run

```
$ java -jar ${JAR-NAME} -s base -o /temp/base.csv -t 3298534883328
```

Intermediate output in Std Err

```
Elapsed: 0h 00min 21sec		ETA: 23h 49min 09sec		74,1 KiB of 3,0 TiB		0,0000023%
```

Final output in file `/temp/base.csv`:

```
id;total_size_byte;length;string
0;2;1;ã
1;41;38;Ök¶9Á
2;259;217;_±°¼Ô-Á×©±îËAëÒ4Y3·ÞÛîRÏ¨»y¥-Óù;-	Ú:h
...
```
