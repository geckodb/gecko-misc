var totalTime;

var authorNameCnt,randomValCnt,authorIdCnt,coAuthorIDCnt,journalIDCnt,venueIDCnt,referenceIDCnt,paperIDCnt,aggregationValCnt;

var authorName=["Michael A. Henning"
    ,"Robert A. Van Gorder"
    ,"Munish Gandhi"
    ,"Edward L. Robertson"
    ,"Etiel Petrinja"
    ,"Giancarlo Succi"
    ,"Carlos E. Salgado"
    ,"Juliana Teixeira"
    ,"Ricardo J. Machado"
    ,"Rita Suzana Pitangu"
    ,"Ying Wu"
    ,"Thomas K. Doyle"
    ,"Colin Fyfe"
    ,"Robert D. Austin"
    ,"Richard L. Nolan"
    ,"Shannon O'Donnell"
    ,"Kook Jin Ahn"
    ,"Sudipto Guha"
    ,"Andrew McGregor"
    ,"B. K. Bog"
    ,"K. Sparck Jones"
    ,"Sine Viesmose Birch"
    ,"Thomas Stig Jacobse"
    ,"Jacob Jon Jensen"
    ,"Christoffer Moesgaa"
    ,"Niels Nørgaard Samu"
    ,"Jiří Srba"
    ,"Luc Hoegaerts"
    ,"Johan A. K. Suykens"
    ,"Joos Vandewalle"
    ,"Bart De Moor"
    ,"M. Prim"
    ,"Joan Oliver"
    ,"Vicenç Soler"
    ,"Qingxiao Guan"
    ,"Jing Dong"
    ,"Tieniu Tan"
    ,"Mike D. Rinderknech"
    ,"Werner L. Popp"
    ,"Olivier Lambercy"
    ,"Roger Gassert"
    ,"Diana Inkpen"
    ,"Graeme Hirst"
    ,"Bart Demoen"
    ,"André Mariën"
    ,"Allain Callebaut"
    ,"Reinhard Klein"
    ,"Ilya V. Osipov"
    ,"Anna Y. Prasikova"
    ,"Alex A. Volinsky"
    ,"Probir Kumar Banerj"
    ,"Hamid Bagha"
    ,"Volker Frehe"
    ,"Florian Stiel"
    ,"Frank Teuteberg"
    ,"Eleanor Leist"
    ,"Dan J. Smith"
    ,"Jesper Boserup Thes"
    ,"Tamas Gergely"
    ,"Peter Beck"
    ,"Pierre E. Bonzon"
    ,"Kenro Yatake"
    ,"Toshiaki Aoki"
    ,"Takuya Katayama"
    ,"Peter Herrmann"
    ,"Manfred Noël"
    ,"Takahisa Toda"
    ,"Vladimir V. Lukin"
    ,"Victor I. Chemerovs"
    ,"Vladimir P. Melnik"
    ,"Sari Peltonen"
    ,"Pauli Kuosmanen"
    ,"Thomas Portele"
    ,"Silke Goronzy"
    ,"Martin Emele"
    ,"Andreas Kellner"
    ,"Sunna Torge"
    ,"Jürgen te Vrugt"
    ,"David J. Anderson"
    ,"Giulio Concas"
    ,"Maria Ilaria Lunesu"
    ,"Michele Marchesi"
    ,"Hongyu Zhang"
    ,"Jeff Conklin"
    ,"Susan Leigh Star"
    ,"Philipp Marcus"
    ,"Moritz Kessel"
    ,"Claudia Linnhoff-Po"
    ,"Jason Cope"
    ,"Henry M. Tufo"
    ,"Klaus-Dieter Althof"
    ,"Frank Maurer"
    ,"Ralph Traphöner"
    ,"Stefan Weiß"
    ,"Manaal Faruqui"
    ,"Sebastian Padó"
    ,"Guo-Hong Ding"
    ,"Yifei Zhu"
    ,"Chengrong Li"
    ,"Bo Xu"
    ,"John Newman"
    ,"Ali Abu-El Humos"
    ,"Tesfa Haile"
    ,"Hyunju Kim"
    ,"Simone Gaffling"
    ,"Florian Jäger"
    ,"Volker Daum"
    ,"Miyuki Tauchi"
    ,"Elke Lütjen-Drecol"
    ,"Jaewook Chung"
    ,"M. Anwar Hasan"
    ,"Donald M. Allen"];//112

var authorName1=["Yao-Sian Huang"
    ,"Su Hyun Lee"
    ,"Ann Yi"
    ,"Zihao Yan"
    ,"V. Valtchinov"
    ,"Edwin D. Boudreaux"
    ,"Justin Mower"
    ,"Sumate Permwonguswa"
    ,"Guillaume Cretin"
    ,"Philippe Glevarec"
    ,"Cidric Roll"
    ,"Kumara Mendis"
    ,"Alexandru Nelus"
    ,"Marjorie Kelley"
    ,"Amber L. Rosado"
    ,"E Migliore"
    ,"Jacalyn Buck"
    ,"Morten Grønnesby"
    ,"Juan Carlos Aviles S"
    ,"Hasse Melbye"
    ,"Jessica Garvey Smith"
    ,"Michelle P. Zhou"
    ,"Megan Chamberlain"
    ,"Alton P. Jensen"
    ,"Julie H. Ducom"
    ,"Russ Beebe"
    ,"Carrie Reale"
    ,"Erik J. Groessl"
    ,"Rashmee U. Shah"
    ,"Chaminda Bulumulla"
    ,"Carissa Abrego-Colli"
    ,"Betsy Pillion"
    ,"Eivind Rinde"
    ,"Cassios Marques"
    ,"Auli Lepistö"
    ,"Jacqueline Lama"
    ,"Dmitry Khodyakov"
    ,"Kate Marie"
    ,"Marika Booth"
    ,"Gabrielle N. Jones"
    ,"Marley R Hodson"
    ,"Tanin Izadi"
    ,"Iván González Jiméne"
    ,"Nastassja Lewinski"
    ,"Jeremy Kong"
    ,"Stephanie McLauchlan"
    ,"Gorazd Plaper"
    ,"Milivoj Piletic"
    ,"Bozo Urh"
    ,"Jamie R. Robinson"
    ,"Amina Wagner"
    ,"Ariel Deardorff"
    ,"Amy Knopp"
    ,"Dawn S. Milliner"
    ,"Ron Moody"
    ,"Yizhen Zhong"
    ,"Sam L. Thomas"
    ,"Cristofaro Mune"
    ,"Eloi Sanfelix Gonzal"
    ,"Muhammad Hatib"
    ,"Omer Porzecanski"
    ,"Isaias Scalabrin Bia"
    ,"Amna Riaz"
    ,"Tai Chieh Wu"
    ,"Makiko Kobayashi"
    ,"Che Hua Yang"
    ,"François-Marie Caron"
    ,"Naima Abubakar"
    ,"Elisa Puigdomènech"
    ,"Santiago Felipe Gome"
    ,"Espallargues Carrera"
    ,"Castell Abat Conxa"
    ,"Anne Martin"
    ,"Rajeeb Rashid"
    ,"Vinicius Renan de Ca"
    ,"Yuying Cao"
    ,"Kathryn S. Egan"
    ,"Karma L. Kreizenbeck"
    ,"Catherine R. Fedoren"
    ,"April Alfiler"
    ,"Heather Noble"
    ,"Tracy Kusnir-Wong"
    ,"Ada Mohedano"
    ,"Fm Stewart"
    ,"Benjamin E. Greer"
    ,"Scott D. Ramsey"
    ,"Westleigh Quattrone"
    ,"L. M. Baker"
    ,"Ashit Gosalia"
    ,"Riyi Qiu"
    ,"Effi Tripler"
    ,"Xiangqi Peng"
    ,"Klara Grcic"
    ,"C William Nylin"
    ,"Riitta Jokela"
    ,"Alastair Black"
    ,"Marko Moberg"
    ,"Khadija AbouElgaseem"
    ,"Kea Turner"
    ,"Reza Aminzadeh"
    ,"Matthias Van den Bos"
    ,"Matilde Irigoyen"
    ,"Ibrahim Abada"
    ,"Joe Stiles"
    ,"F. L. R. Pimentel"
    ,"Guy Pe'er"
    ,"Dorota Celińska"
    ,"Miguel E. Litvin"
    ,"Janice Terry"
    ,"Joris Ravaglia"
    ,"Richard A. Fourner"
    ,"Joseph C. Gardiner"];//112

var authorName2=[,"Ibrahim Abada"
    ,"Joe Stiles"
    ,"F. L. R. Piment"
    ,"Guy Pe'er"
    ,"Dorota Celińska"
    ,"Miguel E. Litvi"
    ,"Janice Terry"
    ,"Joris Ravaglia"
    ,"Richard A. Four"
    ,"Joseph C. Gardi"
    ,"Mahmoud Saoud"
    ,"J. Vince Pulido"
    ,"Tenma Nakamura"
    ,"Ryoich Matsuse"
    ,"Kiyoharu Iwata"
    ,"William Hobbs"
    ,"Shahrukh Haider"
    ,"Angely Oyola"
    ,"Efrat Muller"
    ,"Denis Agniel"
    ,"Rubén Martín Ga"
    ,"Christian P. En"
    ,"Anna Glazyrina"
    ,"Gabriela Citlal"
    ,"Marcelo Perez R"
    ,"Jiri Bradle"
    ,"Perumal Rajaram"
    ,"Ayushi Singh"
    ,"Jay Gubbi"
    ,"Vijayaregunatha"
    ,"Varadarajan Vij"
    ,"A Kalet"
    ,"Daniele Cono D’"
    ,"Samuel L. Volch"
    ,"Kyle Hernandez"
    ,"Xinyan Ou"
    ,"Max Reppen"
    ,"Bui Minh Tuan"
    ,"Georgios Feretz"
    ,"Jyothi Thota"
    ,"Jonathon Bolin"
    ,"William Macke"
    ,"Jon Hoffman"
    ,"Sam Beckmann"
    ,"Christoph Klöff"
    ,"Ann-Kathrin Kra"
    ,"Manuel Guimarãe"
    ,"Joel L. Gallagh"
    ,"Alisa B. Busch"
    ,"Marissa Gordon"
    ,"S. Malek"
    ,"Yury Vilk"
    ,"Elizabeth Cavag"
    ,"Darren Toh"
    ,"Abrham Workineh"
    ,"W T. Scruggs"
    ,"Joseph A. Scall"
    ,"Rob Dorrington"
    ,"Ria Laubscher"
    ,"Ian M. Timæus"
    ,"Evangelos Karam"
    ,"Konstantinos Bo"
    ,"Asia Aljahdali"
    ,"E. Menti"
    ,"Corrado Lanera"
    ,"Giulia Lorenzon"
    ,"Daniela Giachin"
    ,"Nacira Guerouah"
    ,"Anne Marie Dela"
    ,"Leomar Scheunem"
    ,"Ashok Subash"
    ,"Orathai Sangpet"
    ,"Nut Juangmarisa"
    ,"Supakorn Warodo"
    ,"Ilijia Bogunovi"
    ,"Casey Garvey"
    ,"Amanda M. Midbo"
    ,"Shibei Zhou"
    ,"Amanda Bonnell"
    ,"Janine Farrell"
    ,"Devon Haag"
    ,"M Bondyra"
    ,"David Unger"
    ,"Elizabeth Ellio"
    ,"Augusto Garcia"
    ,"Saroj Sharma"
    ,"Dennis Shpitz"
    ,"Darei Chi"
    ,"Yeon-Tae Yu"
    ,"Sanjit Manohar"
    ,"Gautam K. Naik"
    ,"Ho-Geum Song"
    ,"Chien-Yi Yen"
    ,"Li-Han Chang"
    ,"Karim El Mernis"
    ,"Lori M. Idemoto"
    ,"Sarah Alameddin"
    ,"Catherine Ivory"
    ,"Sarah Osmundson"
    ,"Yar Rouf"
    ,"Dorota Bielińsk"
    ,"Piotr Wa¸ż"
    ,"Fadwa Oukhay"
    ,"Yik-Ki J. Wan"
    ,"Anders Kolstad"
    ,"Simon Litlehama"
    ,"W R. Adrion"
    ,"Fabien Petitjea"
    ,"Jaen Alberto Ar"
    ,"Cecilia Gomez-C"
    ,"Francisco Gavi"
    ,"Tadashi Ishii"
    ,"Anupriya Gupta"];//112

var randomVal=["data",
    "mining",
    "relation",
    "bigdata",
    "database",
    "data structure",
    "codd",
    "marcus",
    "gabriel",
    "gunter",
    "methodology",
    "computing systems",
    "systems",
    "monitor",
    "interface",
    "natural",
    "arguments",
    "simulation",
    "workload charecterization",
    "Robotic system"];//100

var randomVal1=["Monitoring","finite-state","fuzzy logic","sebastian","citation network","scholarly network",
    "Heterogeneous networks","graph database","database management system","big data",
    "Programming problems","Transactions","User study","database optimization technique","social networks",
    "Biometrics","Engineering application","data replication","Myra","Recommender systems"];//20

var randomVal2=["embeded systems","Business intelligence","Datastructures","data duplication","automotive systems",
    "Virtual reality","defect detection systems","SCRUM","artificial reality","Robert",
    "marketing statergies","simulation optimization","discrete mathematics","semantic segmentation","Reinforcement learning",
    "string theory","quantum theory","Saad Bin Qaisar","fifth generation networks","protien network"];//20

var authorID=["au_1",
    "au_2",
    "au_3",
    "au_4",
    "au_5",
    "au_6",
    "au_7",
    "au_8",
    "au_9",
    "au_10",
    "au_11",
    "au_12",
    "au_13",
    "au_14",
    "au_15",
    "au_16",
    "au_17",
    "au_18",
    "au_19",
    "au_20",
    "au_21",
    "au_22",
    "au_23",
    "au_24",
    "au_25",
    "au_26",
    "au_27",
    "au_28",
    "au_29",
    "au_30",
    "au_31",
    "au_32",
    "au_33",
    "au_34",
    "au_35",
    "au_36",
    "au_37",
    "au_38",
    "au_39",
    "au_40",
    "au_41",
    "au_42",
    "au_43",
    "au_44",
    "au_45",
    "au_46",
    "au_47",
    "au_48",
    "au_49",
    "au_50",
    "au_51",
    "au_52",
    "au_53",
    "au_54",
    "au_55",
    "au_56",
    "au_57",
    "au_58",
    "au_59",
    "au_60",
    "au_61",
    "au_62",
    "au_63",
    "au_64",
    "au_65",
    "au_66",
    "au_67",
    "au_68",
    "au_69",
    "au_70",
    "au_71",
    "au_72",
    "au_73",
    "au_74",
    "au_75",
    "au_76",
    "au_77",
    "au_78",
    "au_79",
    "au_80",
    "au_81",
    "au_82",
    "au_83",
    "au_84",
    "au_85",
    "au_86",
    "au_87",
    "au_88",
    "au_89",
    "au_90",
    "au_91",
    "au_92",
    "au_93",
    "au_94",
    "au_95",
    "au_96",
    "au_97",
    "au_98",
    "au_99",
    "au_100",
    "au_101",
    "au_102",
    "au_103",
    "au_104",
    "au_105",
    "au_106",
    "au_107",
    "au_108",
    "au_109",
    "au_110",
    "au_111",
    "au_112"];//112

var authorID1=["au_1766212",
    "au_1766213",
    "au_1766214",
    "au_1766215",
    "au_1766216",
    "au_1766217",
    "au_1766218",
    "au_1766219",
    "au_1766220",
    "au_1766221",
    "au_1766222",
    "au_1766223",
    "au_1766224",
    "au_1766225",
    "au_1766226",
    "au_1766227",
    "au_1766228",
    "au_1766229",
    "au_1766230",
    "au_1766231",
    "au_1766232",
    "au_1766233",
    "au_1766234",
    "au_1766235",
    "au_1766236",
    "au_1766237",
    "au_1766238",
    "au_1766239",
    "au_1766240",
    "au_1766241",
    "au_1766242",
    "au_1766243",
    "au_1766244",
    "au_1766245",
    "au_1766246",
    "au_1766247",
    "au_1766248",
    "au_1766249",
    "au_1766250",
    "au_1766251",
    "au_1766252",
    "au_1766253",
    "au_1766254",
    "au_1766255",
    "au_1766256",
    "au_1766257",
    "au_1766258",
    "au_1766259",
    "au_1766260",
    "au_1766261",
    "au_1766262",
    "au_1766263",
    "au_1766264",
    "au_1766265",
    "au_1766266",
    "au_1766267",
    "au_1766268",
    "au_1766269",
    "au_1766270",
    "au_1766271",
    "au_1766272",
    "au_1766273",
    "au_1766274",
    "au_1766275",
    "au_1766276",
    "au_1766277",
    "au_1766278",
    "au_1766279",
    "au_1766280",
    "au_1766281",
    "au_1766282",
    "au_1766283",
    "au_1766284",
    "au_1766285",
    "au_1766286",
    "au_1766287",
    "au_1766288",
    "au_1766289",
    "au_1766290",
    "au_1766291",
    "au_1766292",
    "au_1766293",
    "au_1766294",
    "au_1766295",
    "au_1766296",
    "au_1766297",
    "au_1766298",
    "au_1766299",
    "au_1766300",
    "au_1766301",
    "au_1766302",
    "au_1766303",
    "au_1766304",
    "au_1766305",
    "au_1766306",
    "au_1766307",
    "au_1766308",
    "au_1766309",
    "au_1766310",
    "au_1766311",
    "au_1766312",
    "au_1766313",
    "au_1766314",
    "au_1766082",
    "au_1766083",
    "au_1766084",
    "au_1766085",
    "au_1766086",
    "au_1766087",
    "au_1766088",
    "au_1766089",
    "au_1766090"];//112

var authorID2=["au_1766100",
    "au_1766101",
    "au_1766102",
    "au_1766103",
    "au_1766104",
    "au_1766105",
    "au_1766106",
    "au_1766107",
    "au_1766108",
    "au_1766109",
    "au_1766110",
    "au_1766111",
    "au_1766112",
    "au_1766113",
    "au_1766114",
    "au_1766115",
    "au_1766116",
    "au_1766117",
    "au_1766118",
    "au_1766119",
    "au_1766120",
    "au_1766121",
    "au_1766122",
    "au_1766123",
    "au_1766124",
    "au_1766125",
    "au_1766126",
    "au_1766127",
    "au_1766128",
    "au_1766129",
    "au_1766130",
    "au_1766131",
    "au_1766132",
    "au_1766133",
    "au_1766134",
    "au_1766135",
    "au_1766136",
    "au_1766137",
    "au_1766138",
    "au_1766139",
    "au_1766140",
    "au_1766141",
    "au_1766142",
    "au_1766143",
    "au_1766144",
    "au_1766145",
    "au_1766146",
    "au_1766147",
    "au_1766148",
    "au_1766149",
    "au_1766150",
    "au_1766151",
    "au_1766152",
    "au_1766153",
    "au_1766154",
    "au_1766155",
    "au_1766156",
    "au_1766157",
    "au_1766158",
    "au_1766159",
    "au_1766160",
    "au_1766161",
    "au_1766162",
    "au_1766163",
    "au_1766164",
    "au_1766165",
    "au_1766166",
    "au_1766167",
    "au_1766168",
    "au_1766169",
    "au_1766170",
    "au_1766171",
    "au_1766172",
    "au_1766173",
    "au_1766174",
    "au_1766175",
    "au_1766176",
    "au_1766177",
    "au_1766178",
    "au_1766179",
    "au_1766180",
    "au_1766181",
    "au_1766182",
    "au_1766183",
    "au_1766184",
    "au_1766185",
    "au_1766186",
    "au_1766187",
    "au_1766188",
    "au_1766189",
    "au_1766190",
    "au_1766191",
    "au_1766192",
    "au_1766193",
    "au_1766194",
    "au_1766195",
    "au_1766196",
    "au_1766197",
    "au_1766198",
    "au_1766199",
    "au_1766200",
    "au_1766201",
    "au_1766202",
    "au_1766203",
    "au_1766204",
    "au_1766205",
    "au_1766206",
    "au_1766207",
    "au_1766208",
    "au_1766209",
    "au_1766210",
    "au_1766211"];//112

var coAuthorID=["au_113",
    "au_114",
    "au_115",
    "au_116",
    "au_117",
    "au_118",
    "au_119",
    "au_120",
    "au_121",
    "au_122",
    "au_123",
    "au_124",
    "au_125",
    "au_126",
    "au_127",
    "au_128",
    "au_129",
    "au_130",
    "au_131",
    "au_132",
    "au_133",
    "au_134",
    "au_135",
    "au_136",
    "au_137",
    "au_138",
    "au_139",
    "au_140",
    "au_141",
    "au_142",
    "au_143",
    "au_144",
    "au_145",
    "au_146",
    "au_147",
    "au_148",
    "au_149",
    "au_150",
    "au_151",
    "au_152",
    "au_153",
    "au_154",
    "au_155",
    "au_156",
    "au_157",
    "au_158",
    "au_159",
    "au_160",
    "au_161",
    "au_162",
    "au_163",
    "au_164",
    "au_165",
    "au_166",
    "au_167",
    "au_168",
    "au_169",
    "au_170",
    "au_171",
    "au_172",
    "au_173",
    "au_174",
    "au_175",
    "au_176",
    "au_177",
    "au_178",
    "au_179",
    "au_180",
    "au_181",
    "au_182",
    "au_183",
    "au_184",
    "au_185",
    "au_186",
    "au_187",
    "au_188",
    "au_189",
    "au_190",
    "au_191",
    "au_192",
    "au_193",
    "au_194",
    "au_195",
    "au_196",
    "au_197",
    "au_198",
    "au_199",
    "au_200",
    "au_201",
    "au_202",
    "au_203",
    "au_204",
    "au_205",
    "au_206",
    "au_207",
    "au_208",
    "au_209",
    "au_210",
    "au_211",
    "au_212",
    "au_213",
    "au_214",
    "au_215",
    "au_216",
    "au_217",
    "au_218",
    "au_219",
    "au_220",
    "au_221",
    "au_222",
    "au_223",
    "au_224",
    "au_225",
    "au_226",
    "au_227"];//115

var coAuthorID1=["au_1766431",
    "au_1766432",
    "au_1766433",
    "au_1766434",
    "au_1766435",
    "au_1766436",
    "au_1766437",
    "au_1766438",
    "au_1766439",
    "au_1766440",
    "au_1766441",
    "au_1766442",
    "au_1766443",
    "au_1766444",
    "au_1766445",
    "au_1766446",
    "au_1766447",
    "au_1766448",
    "au_1766449",
    "au_1766450",
    "au_1766451",
    "au_1766452",
    "au_1766453",
    "au_1766454",
    "au_1766455",
    "au_1766456",
    "au_1766457",
    "au_1766458",
    "au_1766459",
    "au_1766460",
    "au_1766461",
    "au_1766462",
    "au_1766463",
    "au_1766464",
    "au_1766465",
    "au_1766466",
    "au_1766467",
    "au_1766468",
    "au_1766469",
    "au_1766470",
    "au_1766471",
    "au_1766472",
    "au_1766473",
    "au_1766474",
    "au_1766475",
    "au_1766476",
    "au_1766477",
    "au_1766478",
    "au_1766479",
    "au_1766480",
    "au_1766481",
    "au_1766482",
    "au_1766483",
    "au_1766484",
    "au_1766485",
    "au_1766486",
    "au_1766487",
    "au_1766488",
    "au_1766489",
    "au_1766490",
    "au_1766491",
    "au_1766492",
    "au_1766493",
    "au_1766494",
    "au_1766495",
    "au_1766496",
    "au_1766497",
    "au_1766498",
    "au_1766499",
    "au_1766500",
    "au_1766501",
    "au_1766502",
    "au_1766503",
    "au_1766504",
    "au_1766505",
    "au_1766506",
    "au_1766507",
    "au_1766508",
    "au_1766509",
    "au_1766510",
    "au_1766511",
    "au_1766512",
    "au_1766513",
    "au_1766514",
    "au_1766515",
    "au_1766516",
    "au_1766517",
    "au_1766518",
    "au_1766519",
    "au_1766520",
    "au_1766521",
    "au_1766522",
    "au_1766523",
    "au_1766524",
    "au_1766525",
    "au_1766526",
    "au_1766527",
    "au_1766528",
    "au_1766529",
    "au_1766530",
    "au_1766531",
    "au_1766532",
    "au_1766533",
    "au_1766534",
    "au_1766535",
    "au_1766536",
    "au_1766537",
    "au_1766538",
    "au_1766539",
    "au_1766540",
    "au_1766541",
    "au_1766542",
    "au_1766543",
    "au_1766544",
    "au_1766545"];//115

var coAuthorID2=["au_1766315",
    "au_1766316",
    "au_1766317",
    "au_1766318",
    "au_1766319",
    "au_1766320",
    "au_1766321",
    "au_1766322",
    "au_1766323",
    "au_1766324",
    "au_1766325",
    "au_1766326",
    "au_1766327",
    "au_1766328",
    "au_1766329",
    "au_1766330",
    "au_1766331",
    "au_1766332",
    "au_1766333",
    "au_1766334",
    "au_1766335",
    "au_1766336",
    "au_1766337",
    "au_1766338",
    "au_1766339",
    "au_1766340",
    "au_1766341",
    "au_1766342",
    "au_1766343",
    "au_1766344",
    "au_1766345",
    "au_1766346",
    "au_1766347",
    "au_1766348",
    "au_1766349",
    "au_1766350",
    "au_1766351",
    "au_1766352",
    "au_1766353",
    "au_1766354",
    "au_1766355",
    "au_1766356",
    "au_1766357",
    "au_1766358",
    "au_1766359",
    "au_1766360",
    "au_1766361",
    "au_1766362",
    "au_1766363",
    "au_1766364",
    "au_1766365",
    "au_1766366",
    "au_1766367",
    "au_1766368",
    "au_1766369",
    "au_1766370",
    "au_1766371",
    "au_1766372",
    "au_1766373",
    "au_1766374",
    "au_1766375",
    "au_1766376",
    "au_1766377",
    "au_1766378",
    "au_1766379",
    "au_1766380",
    "au_1766381",
    "au_1766382",
    "au_1766383",
    "au_1766384",
    "au_1766385",
    "au_1766386",
    "au_1766387",
    "au_1766388",
    "au_1766389",
    "au_1766390",
    "au_1766391",
    "au_1766392",
    "au_1766393",
    "au_1766394",
    "au_1766395",
    "au_1766396",
    "au_1766397",
    "au_1766398",
    "au_1766399",
    "au_1766400",
    "au_1766401",
    "au_1766402",
    "au_1766403",
    "au_1766404",
    "au_1766405",
    "au_1766406",
    "au_1766407",
    "au_1766408",
    "au_1766409",
    "au_1766410",
    "au_1766411",
    "au_1766412",
    "au_1766413",
    "au_1766414",
    "au_1766415",
    "au_1766416",
    "au_1766417",
    "au_1766418",
    "au_1766419",
    "au_1766420",
    "au_1766421",
    "au_1766422",
    "au_1766423",
    "au_1766424",
    "au_1766425",
    "au_1766426",
    "au_1766427",
    "au_1766428",
    "au_1766429"];//115

var journalID=["jour_10",
    "jour_11",
    "jour_12",
    "jour_13",
    "jour_14",
    "jour_15",
    "jour_16",
    "jour_17",
    "jour_18",
    "jour_19",
    "jour_20",
    "jour_21",
    "jour_22",
    "jour_23",
    "jour_24",
    "jour_25",
    "jour_26",
    "jour_27",
    "jour_28",
    "jour_29",
    "jour_30",
    "jour_31",
    "jour_32",
    "jour_33",
    "jour_34",
    "jour_35",
    "jour_36",
    "jour_37",
    "jour_38",
    "jour_39",
    "jour_40",

    /* "jour_41",
     "jour_42",
     "jour_43",
     "jour_44",
     "jour_45",
     "jour_46",
     "jour_47",
     "jour_48",
     "jour_49",
     "jour_50",
     "jour_51",
     "jour_52",
     "jour_53",
     "jour_54",
     "jour_55",
     "jour_56",
     "jour_57",
     "jour_58",
     "jour_59",
     "jour_60",
     "jour_61",
     "jour_62",
     "jour_63",
     "jour_64",
     "jour_65",
     "jour_66",
     "jour_67",
     "jour_68",
     "jour_69",
     "jour_70",
     "jour_71",
     "jour_72",
     "jour_73",
     "jour_74",
     "jour_75",
     "jour_76",
     "jour_77",
     "jour_78",
     "jour_79",
     "jour_80",
     "jour_81",
     "jour_82",
     "jour_83",
     "jour_84",
     "jour_85",
     "jour_86",
     "jour_87",
     "jour_88",
     "jour_89",
     "jour_90",
     "jour_91",
     "jour_92",
     "jour_93",
     "jour_94",
     "jour_95",
     "jour_96",
     "jour_97",
     "jour_98",
     "jour_99",
     "jour_100",
     "jour_101",
     "jour_102",
     "jour_103",
     "jour_104",
     "jour_105",
     "jour_106",
     "jour_107",
     "jour_108",
     "jour_109",
     "jour_110",
     "jour_111",
     "jour_112",
     "jour_113",
     "jour_114",
     "jour_115",
     "jour_116",
     "jour_117",
     "jour_118",
     "jour_119",
     "jour_120"*/];//111

var journalID1=["jour_1571",
    "jour_1572",
    "jour_1573",
    "jour_1574",
    "jour_1575",
    "jour_1576",
    "jour_1577",
    "jour_1578",
    "jour_1579",
    "jour_1580",
    "jour_1581",
    "jour_1582",
    "jour_1583",
    "jour_1584",
    "jour_1585",
    "jour_1586",
    "jour_1587",
    "jour_1588",
    "jour_1589",
    "jour_1590",
    "jour_1591",
    "jour_1592",
    "jour_1593",
    "jour_1594",
    "jour_1595",
    "jour_1596",
    "jour_1597",
    "jour_1598",
    "jour_1599",
    "jour_1600",
    "jour_1601",
    /*
    "jour_1602",
    "jour_1603",
    "jour_1604",
    "jour_1605",
    "jour_1606",
    "jour_1607",
    "jour_1608",
    "jour_1609",
    "jour_1610",
    "jour_1611",
    "jour_1612",
    "jour_1613",
    "jour_1614",
    "jour_1615",
    "jour_1616",
    "jour_1617",
    "jour_1618",
    "jour_1619",
    "jour_1620",
    "jour_1621",
    "jour_1622",
    "jour_1623",
    "jour_1624",
    "jour_1625",
    "jour_1626",
    "jour_1627",
    "jour_1628",
    "jour_1629",
    "jour_1630",
    "jour_1631",
    "jour_1632",
    "jour_1633",
    "jour_1634",
    "jour_1635",
    "jour_1636",
    "jour_1637",
    "jour_1638",
    "jour_1639",
    "jour_1640",
    "jour_1641",
    "jour_1642",
    "jour_1643",
    "jour_1644",
    "jour_1645",
    "jour_1646",
    "jour_1647",
    "jour_1648",
    "jour_1649",
    "jour_1650",
    "jour_1651",
    "jour_1652",
    "jour_1653",
    "jour_1654",
    "jour_1655",
    "jour_1656",
    "jour_1657",
    "jour_1658",
    "jour_1659",
    "jour_1660",
    "jour_1661",
    "jour_1662",
    "jour_1663",
    "jour_1664",
    "jour_1665",
    "jour_1666",
    "jour_1667",
    "jour_1668",
    "jour_1669",
    "jour_1670",
    "jour_1671",
    "jour_1672",
    "jour_1673",
    "jour_1674",
    "jour_1675",
    "jour_1565",
    "jour_1566",
    "jour_1567",
    "jour_1568",
    "jour_1569",
    "jour_1570"*/];//111

var journalID2=["jour_1452",
    "jour_1453",
    "jour_1454",
    "jour_1455",
    "jour_1456",
    "jour_1457",
    "jour_1458",
    "jour_1459",
    "jour_1460",
    "jour_1461",
    "jour_1462",
    "jour_1463",
    "jour_1464",
    "jour_1465",
    "jour_1466",
    "jour_1467",
    "jour_1468",
    "jour_1469",
    "jour_1470",
    "jour_1471",
    "jour_1472",
    "jour_1473",
    "jour_1474",
    "jour_1475",
    "jour_1476",
    "jour_1477",
    "jour_1478",
    "jour_1479",
    "jour_1480",
    "jour_1481",
    "jour_1482",

    /* "jour_1483",
     "jour_1484",
     "jour_1485",
     "jour_1486",
     "jour_1487",
     "jour_1488",
     "jour_1489",
     "jour_1490",
     "jour_1491",
     "jour_1492",
     "jour_1493",
     "jour_1494",
     "jour_1495",
     "jour_1496",
     "jour_1497",
     "jour_1498",
     "jour_1499",
     "jour_1500",
     "jour_1501",
     "jour_1502",
     "jour_1503",
     "jour_1504",
     "jour_1505",
     "jour_1506",
     "jour_1507",
     "jour_1508",
     "jour_1509",
     "jour_1510",
     "jour_1511",
     "jour_1512",
     "jour_1513",
     "jour_1514",
     "jour_1515",
     "jour_1516",
     "jour_1517",
     "jour_1518",
     "jour_1519",
     "jour_1520",
     "jour_1521",
     "jour_1522",
     "jour_1523",
     "jour_1524",
     "jour_1525",
     "jour_1526",
     "jour_1527",
     "jour_1528",
     "jour_1529",
     "jour_1530",
     "jour_1531",
     "jour_1532",
     "jour_1533",
     "jour_1534",
     "jour_1535",
     "jour_1536",
     "jour_1537",
     "jour_1538",
     "jour_1539",
     "jour_1540",
     "jour_1541",
     "jour_1542",
     "jour_1543",
     "jour_1544",
     "jour_1545",
     "jour_1546",
     "jour_1547",
     "jour_1548",
     "jour_1549",
     "jour_1550",
     "jour_1551",
     "jour_1552",
     "jour_1553",
     "jour_1554",
     "jour_1555",
     "jour_1556",
     "jour_1557",
     "jour_1558",
     "jour_1559",
     "jour_1560",
     "jour_1561",
     "jour_1562"*/];//111

var venueID=["ven_10",
    "ven_11",
    "ven_12",
    "ven_13",
    "ven_14",
    "ven_15",
    "ven_16",
    "ven_17",
    "ven_18",
    "ven_19",
    "ven_20",
    "ven_21",
    "ven_22",
    "ven_23",
    "ven_24",
    "ven_25",
    "ven_26",
    "ven_27",
    "ven_28",
    "ven_29",
    "ven_30",
    "ven_31",
    "ven_32",
    "ven_33",
    "ven_34",
    "ven_35",
    "ven_36",
    "ven_37",
    "ven_38",
    "ven_39",
    "ven_40",
    "ven_41",
    "ven_42",
    "ven_43",
    "ven_44",
    "ven_45",
    "ven_46",
    "ven_47",
    "ven_48",
    "ven_49",
    "ven_50",
    "ven_51",
    "ven_52",
    "ven_53",
    "ven_54",
    "ven_55",
    "ven_56",
    "ven_57",
    "ven_58",
    "ven_59",
    "ven_60",
    "ven_61",
    "ven_62",
    "ven_63",
    "ven_64",
    "ven_65",
    "ven_66",
    "ven_67",
    "ven_68",
    "ven_69",
    "ven_70",
    "ven_71",
    "ven_72",
    "ven_73",
    "ven_74",
    "ven_75",
    "ven_76",
    "ven_77",
    "ven_78",
    "ven_79",
    "ven_80",
    "ven_81",
    "ven_82",
    "ven_83",
    "ven_84",
    "ven_85",
    "ven_86",
    "ven_87",
    "ven_88",
    "ven_89",
    "ven_90",
    "ven_91",
    "ven_92"];//83

var venueID1=["ven_100",
    "ven_101",
    "ven_102",
    "ven_103",
    "ven_104",
    "ven_105",
    "ven_106",
    "ven_107",
    "ven_108",
    "ven_109",
    "ven_110",
    "ven_111",
    "ven_112",
    "ven_113",
    "ven_114",
    "ven_115",
    "ven_116",
    "ven_117",
    "ven_118",
    "ven_119",
    "ven_120",
    "ven_121",
    "ven_122",
    "ven_123",
    "ven_124",
    "ven_125",
    "ven_126",
    "ven_127",
    "ven_128",
    "ven_129",
    "ven_130",
    "ven_131",
    "ven_132",
    "ven_133",
    "ven_134",
    "ven_135",
    "ven_136",
    "ven_137",
    "ven_138",
    "ven_139",
    "ven_140",
    "ven_141",
    "ven_142",
    "ven_143",
    "ven_144",
    "ven_145",
    "ven_146",
    "ven_147",
    "ven_148",
    "ven_149",
    "ven_150",
    "ven_151",
    "ven_152",
    "ven_153",
    "ven_154",
    "ven_155",
    "ven_156",
    "ven_157",
    "ven_158",
    "ven_159",
    "ven_160",
    "ven_161",
    "ven_162",
    "ven_163",
    "ven_164",
    "ven_165",
    "ven_166",
    "ven_167",
    "ven_168",
    "ven_169",
    "ven_170",
    "ven_171",
    "ven_172",
    "ven_173",
    "ven_174",
    "ven_175",
    "ven_176",
    "ven_177",
    "ven_178",
    "ven_179",
    "ven_180",
    "ven_181",
    "ven_182"];//83

var venueID2=["ven_200",
    "ven_201",
    "ven_202",
    "ven_203",
    "ven_204",
    "ven_205",
    "ven_206",
    "ven_207",
    "ven_208",
    "ven_209",
    "ven_210",
    "ven_211",
    "ven_212",
    "ven_213",
    "ven_214",
    "ven_215",
    "ven_216",
    "ven_217",
    "ven_218",
    "ven_219",
    "ven_220",
    "ven_221",
    "ven_222",
    "ven_223",
    "ven_224",
    "ven_225",
    "ven_226",
    "ven_227",
    "ven_228",
    "ven_229",
    "ven_230",
    "ven_231",
    "ven_232",
    "ven_233",
    "ven_234",
    "ven_235",
    "ven_236",
    "ven_237",
    "ven_238",
    "ven_239",
    "ven_240",
    "ven_241",
    "ven_242",
    "ven_243",
    "ven_244",
    "ven_245",
    "ven_246",
    "ven_247",
    "ven_248",
    "ven_249",
    "ven_250",
    "ven_251",
    "ven_252",
    "ven_253",
    "ven_254",
    "ven_255",
    "ven_256",
    "ven_257",
    "ven_258",
    "ven_259",
    "ven_260",
    "ven_261",
    "ven_262",
    "ven_263",
    "ven_264",
    "ven_265",
    "ven_266",
    "ven_267",
    "ven_268",
    "ven_269",
    "ven_270",
    "ven_271",
    "ven_272",
    "ven_273",
    "ven_274",
    "ven_275",
    "ven_276",
    "ven_277",
    "ven_278",
    "ven_279",
    "ven_280",
    "ven_281",
    "ven_282"];//83

var referenceID=["02b94941-769d-479c-8746-b1d8be9e6b52",
    "07d7e942-f9bf-4b51-93a3-d608cfa793b5",
    "0cea81ca-d21a-478e-b562-443a983de65f",
    "0f325629-acbd-4a46-8d83-6b52128d4d9a",
    "114bbc35-82eb-408c-aea3-642f9a75626e",
    "27122991-0a5d-4998-88ba-9e7b25b275a5",
    "301a152d-f292-42b7-8be9-a267eff8b6c1",
    "40108066-a2e8-4450-bbaf-f297b0f19254",
    "5357e88e-ee32-4b7a-ab79-156a4487eda3",
    "699be3f6-7f03-49a1-8951-5301302e9c54",
    "73f07920-554d-4f47-962f-aa4c0234c799",
    "7d9feaf3-61c1-4d82-becc-4ca183b0286e",
    "7ed39d94-a2b2-483b-942d-6ac2bf952584",
    "872c541f-6f1c-4f1a-a89b-feea9c499d77",
    "a34ade6f-8035-435c-b9a9-292990a00d2d",
    "aaa3a44e-fda2-46a2-b6f6-19b360dfe5b6",
    "ae4bc4e8-6ffe-4eb1-bc2d-323a03d2082a",
    "bc55829e-f045-4e27-869a-8864ad164510",
    "de4bebaf-624f-4c51-874a-8b8fe76d27f5",
    "f4b9c676-ca08-4685-92e9-5c6489d1470d",
    "277e0d2b-42e4-4dee-bb35-a41b64418fe1",
    "4f39c450-57e1-4f07-a9d9-5368c80d9bb4",
    "545f9d97-37bf-49f0-9c73-3e80d3720b3e",
    "6a450332-303b-4bac-8d50-b5b09b49f367",
    "6f28424e-4782-46d2-83d9-d0e4adc59d64",
    "922e9d61-cfab-4731-8d88-311d1c8d9792",
    "a4efe776-b313-45b3-b91f-e71ca2e97558",
    "a819bd3f-aea5-4deb-ae33-c65899ea432d",
    "c94ed232-9fdc-4072-bb14-c4d6a7d5c002",
    "16e14617-5d53-45ee-852a-2e85c21ddcc3",
    "64d5ab6d-725c-4677-b14c-5207e2ce40b0",
    "47324f05-0a5c-4ccd-bd6a-cd174374f624",
    "50357804-2e74-427c-b57c-4049e75ba0cd",
    "9db7767b-74be-4857-8596-856ece9f9d8c",
    "0a890c7b-02e6-41ca-96d9-5ad5ac881aaf",
    "f3b29252-d925-49e9-95e9-17a5badccb98",
    "ec2ef2af-b5dc-4f3c-a002-a3e22f13acdb",
    "50c35238-44a0-401f-a2a5-7f254ce38ea5",
    "0550c938-50af-4f24-884c-a5f1b837f22e",
    "233cef62-e48a-4e0f-af51-1f55495e460b",
    "2688aaf4-3202-4bb4-9533-0e6000051686",
    "3d4fb474-f0c5-435f-8eac-bc424e5d951c",
    "424d8359-2046-4f62-a156-9cdedc2ce337",
    "4c4b174a-3849-468b-be09-b74c19791dad",
    "b66907ee-fec6-432a-b2f9-52f34c13cd52",
    "c46b4081-09ea-468a-a703-2c54d4b29ef1",
    "d571d475-a604-41c5-8cca-7ddb8df8b646",
    "db0a7178-cabb-4f39-a0a0-58520a02d04a",
    "e1e132f9-6b33-4b94-8679-6bbe9ad68144",
    "f0373648-7a58-47f2-a020-4ed073fada84",
    "278b98ab-eac6-4cdb-b67d-15ebd96cb1e3",
    "2cc04a5c-e967-4a44-9f3f-665b50252276",
    "397598ab-7df5-490d-8d8b-e36bbcab94d1",
    "72ac112d-f605-4362-86e9-78e67b08da0c",
    "a77ccefc-479b-4a90-9503-87f19013d7de",
    "b7963686-1e3b-49c5-8f8b-f2a1311dbc60",
    "c870002c-618a-480b-bc6c-0673c1dc8d21",
    "cb3a6688-0e1a-4f03-81b9-546b29701c85",
    "cd705452-48b1-46dd-b6a2-9977493bf4a1",
    "d79c89e1-eef8-4007-82dc-fb3d42b4fe67",
    "d95775f9-d211-4ec6-abbd-c0a62bfb48ed",
    "5c7cc304-2108-4c02-b23e-ac93533ce029",
    "5fe469c7-2d52-4ade-8b07-de1a4b8b3ab0",
    "9154cf06-b467-43f6-9408-6f9c90ea1867",
    "bacf1a91-af60-414d-ba2b-a7123a48cadd",
    "37692427-4118-4fdd-851b-59fbc8309474",
    "de1f5175-6908-46d8-bfbd-1eedaa65b903",
    "eeb16aa6-0d8d-4d67-989c-adc1aec18a0a",
    "092b6d68-b0da-4a7b-b522-5b7e5cf67b6c",
    "12f60f65-32a6-44ce-b76e-437832ef42d5",
    "27be2f1b-652d-4493-be96-681bbb6477c1",
    "2fc20cdd-2ade-4a7b-9f93-832d53504b51",
    "3b787b0e-036b-41af-a4c1-87bdce09be8d",
    "4bef1dc4-d87b-424d-a0a9-26d4510f3494",
    "4cb31217-abb3-4456-aeb7-3aedff91dc1b",
    "5626b702-f849-44c0-9348-1d7b4c86e016",
    "6620b89f-1cd7-4e14-b5e3-c57dd46a1d00",
    "691649e5-da00-409c-90af-912f957724e9",
    "6f5dab07-f5ef-4a9a-a3e1-4853496f8579",
    "70330179-7438-4b0f-b159-e6b402edd40a",
    "7a5653a0-83a2-4bea-bc08-e4a4c9c11c36",
    "8ed6942c-50f4-4dc7-99a3-f072f6a985a3",
    "9ef66ebf-124f-4c63-a0c2-bf0062e623d5",
    "dc2abcb4-433a-4f05-9be1-663806292f50",
    "e4b4c01a-bacb-4559-b57e-e45a3bbeb768",
    "e92558db-94eb-4772-a5d8-5c78668337ac",
    "fa3002d0-5e71-404d-b9e0-cbe04c26bf13",
    "10de429c-310a-402a-8b70-e2ac25756e86",
    "2fa4730b-306c-4216-8421-ee9c379b8019",
    "30cefa91-268e-4aaa-bf93-2655048097a2",
    "9ce859fd-d714-44c1-9a6a-6da5c4b68b46"];//91

var referenceID1=["0f295723-2dc2-42d8-852f-bc9420aff242",
    "0f298fad-9629-4d71-8b97-7199de714fc4",
    "0f29950e-e22e-4a96-a48e-e2c9661057f4",
    "0f29a055-5d12-45d8-b83b-e7ce4adce223",
    "0f29ab01-be94-4d93-8506-c6a9fafcbe1a",
    "0f29b448-e261-4652-9f53-ec7b0f14d76f",
    "0f29b516-15ea-417d-aaa8-becf62b38408",
    "0f29cba0-3cb1-4269-8fd3-46dc16b75bd4",
    "0f29ccc9-13bf-4a7a-8baf-ab4cb2c6a8fb",
    "0f29e511-6a5b-4a10-b7d6-736af1f2e563",
    "0f2a0fda-8682-49c5-a450-d880ccb3956c",
    "0f2a12cd-3efe-4ca0-82f7-dc7e0c5fe62e",
    "0f2a1be3-fdb3-4ce4-8636-6cfe0111477b",
    "0f2a6f2b-49f3-4eae-95d3-23c095fa1cb9",
    "0f2a9a80-7947-4259-8ef0-09014d4eab83",
    "0f2aaeda-6cad-4837-8be4-f2a169ec380b",
    "0f2ab14c-e047-49f7-9509-45ccdf394a5c",
    "0f2abdaf-72ba-4452-843b-9d0993a5c3ad",
    "0f2ad3e0-f5d4-4adb-91f8-638792da7f45",
    "0f2ad42c-dc5e-4930-9b67-ad3f0560cb1b",
    "0f2ada3e-de0c-492c-bdd9-db3845b9515c",
    "0f2ae28b-8a2f-44b7-aa3c-8b269848b7f9",
    "0f2af0c6-f630-485d-acba-dfde85dd4728",
    "0f2af101-1549-4148-b57c-b96525fab07a",
    "0f2b1be0-851b-4501-b056-28d562e4d85a",
    "0f2b1f59-7de8-48e7-b643-f24cafc2d2cd",
    "0f2b286f-5bee-4617-bc6f-9657e5db041a",
    "0f2b293f-2c83-4357-ba95-69d5872cff81",
    "0f2b3887-3526-4a20-9ece-c86f292cad5e",
    "0f2b5690-515d-445a-9095-c520057894fd",
    "0f2b745f-1f38-4b21-bcd0-951e0ffa9b6d",
    "0f2b7a01-f8a5-4264-87bb-24f2c779f711",
    "0f2b9a37-8626-49b5-90f9-966638a49869",
    "0f2b9c8a-119b-4248-b351-9618536345b5",
    "0f2bd161-f99c-4c6a-9fda-a56553491153",
    "0f2be359-a4ab-42de-b266-f6e535024afe",
    "0f2c21fc-12fa-46cb-b030-40538a9c92d6",
    "0f2c23a2-9bb0-42c8-923b-d039c5a56f46",
    "0f2c2faa-7fb9-447e-bb18-0cc247d450fe",
    "0f2c4668-e658-405d-873b-cdf2c81d7f2f",
    "0f2c5357-9894-4823-9226-10c7065e41ea",
    "0f2c788a-ccae-4124-9a39-553c4d9c7d2c",
    "0f2c7bd1-3218-4f59-987f-e0d6715a4681",
    "0f2c7c87-9cff-4c0b-af35-fdfc92f53979",
    "0f2ce56a-1a50-4c6a-9218-1ec778be72fd",
    "0f2cf3ba-7045-4fd2-a710-17e0f4ea62b3",
    "0f2d10ed-900a-4901-9899-51c9205639c2",
    "0f2d32ec-38e4-4100-a349-1ac925fea50a",
    "0f2d3bca-8427-47b8-92c9-bc6f854f3ecc",
    "0f2d58ae-1b3e-4a6e-9fd6-abd5b64207dd",
    "0f2d5b04-f224-4ba3-9421-4beb7ca533b0",
    "0f2d9715-1e39-439f-b5ff-7ed78c8b8d1c",
    "0f2d9eed-9c6a-4ec4-8751-c1da230738b0",
    "0f2dc15b-9378-4ada-b18a-3cf733bc6a26",
    "0f2dd964-71de-43d7-adf3-472bfa962450",
    "0f2de084-6acb-4b92-8d4e-79cbe330176a",
    "0f2e0f06-67fe-4d1d-984e-7dbb3d5af500",
    "0f2e173e-3f75-41fe-a467-c48d3e2f0ef7",
    "0f2e2f4b-6997-49ed-926b-4c14e901bbc4",
    "0f2e3c38-03d9-44b9-8b40-17c4d5158560",
    "0f2e67d8-a9df-4ab3-adfb-6d34b2cb50f5",
    "0f2e709d-def2-446b-a777-2307478ad2b1",
    "0f2e7881-cc98-4304-b68c-bb596739c62f",
    "0f2e7b49-6b09-4db3-a073-182b35f2ca4d",
    "0f2e942f-82ba-41cb-be44-4e7cd5c7ac2a",
    "0f2e9c1a-923a-47c7-ba7b-46b46e5f6388",
    "0f2eaa9d-e68d-4570-a901-6265091f98d9",
    "0f2eb124-1306-434f-b7bf-7aa009e8a70b",
    "0f2eb157-32f6-415b-b86d-ca50f941f341",
    "0f2ec1df-3b98-4b5c-bdde-5d032c6b11ef",
    "0f2ecc8e-f828-41e1-8a2d-366f81c72ea5",
    "0f2ede43-4cc0-4870-b1f1-1a274450632f",
    "0f2ee692-a232-48f7-926a-dbb4ead170c3",
    "0f2f1a3b-112d-4ee6-a012-619203899559",
    "0f2f1bc4-3b56-4574-8b18-81a4f460a5bb",
    "0f2f295f-9d3f-47d3-9857-de8c713af9be",
    "0f2f72e0-caef-4c1a-9ba6-73d154a964b3",
    "0f2f794c-12e3-4377-9be1-f8e7b7c779c0",
    "0f2f7a87-98e4-462d-9575-4bc10069d801",
    "0f2f95d7-fe0b-48f6-8613-b906ce827bca",
    "0f2f9bae-4d0d-4158-934d-9fb57a1b2bc8",
    "0f2f9de5-822b-496c-a124-5fe5afb6d079",
    "0f2fb8fe-9f0a-464d-9768-a241ba3951c5",
    "0f2fbae9-19f0-4502-a6ba-9061c70fdc9b",
    "0f2fdd9c-39c7-48c9-8387-db9e8e3ecef6",
    "0f2fe26a-2e29-4af4-a3cc-02efab2a7e2a",
    "0f2ffa3a-0549-4788-a861-179ac1a3ff7e",
    "0f301430-8b7c-4fe4-8953-9ead69794f21",
    "0f301f39-8290-4e45-aa7f-c9d8318693db",
    "0f302140-20a3-4065-8dd6-59176f2d4e27",
    "0f30257e-b57b-47aa-8376-7a26e7669981"];//91

var referenceID2=["bc16727e-3c67-4b28-a142-98b8ec0e864a",
    "bc1fab4d-0e43-455d-904a-558ae0ee17ea",
    "bc291cf4-8f87-4b6b-8977-cb8f05fa0042",
    "bc41137d-0c65-41ad-bc0d-dd820b046cb8",
    "bc50d92e-c712-49be-9944-50dab0e9ea08",
    "bc51b720-e392-4e60-8139-6bd0ff13b83f",
    "bc9c6979-16c5-4b7e-97a7-8ef525672082",
    "bcb2b2fb-f0cb-4311-9263-9151f0ec33fc",
    "bcceec9c-5101-4984-bc51-e73d21429920",
    "bcf90bc2-85e1-4755-b744-8020d123f825",
    "bd2e9390-8873-4635-9811-4a8ba0c97ed4",
    "bd436899-8968-40af-9191-f36d89a40864",
    "bd6387ae-d696-4637-8cf8-3afb2c938bc4",
    "bd7b6e19-ff96-43db-bd0e-02d19bf30d2e",
    "bdef5052-126c-404f-b132-093b1ea0fb21",
    "be0bcd47-98c8-43a3-8834-c6b865f27c28",
    "be40ef6b-0ee8-48c0-a0e5-025b44a90076",
    "be7daf3a-ed47-48ff-ba56-8b6944ce88a9",
    "be9f293c-8826-4bc0-befa-cd1f5f4bdc02",
    "beb3770d-61ae-4954-832e-472de12172a9",
    "bf10c344-50e5-4761-8db1-fd151c341402",
    "bf30215c-d6e4-4771-9e9e-d56a94567fef",
    "bf32d38f-13e5-4470-87d6-08d6785320ca",
    "bf45bb60-ad5f-45a1-8931-1cf0e92689b0",
    "bf478a5a-4d9c-4c11-9167-cf92f7a416ae",
    "bf484625-b255-4d76-9285-1f3184aa85e6",
    "bfc3fe3a-093e-4fc8-ae10-baf8924413ce",
    "c023ac83-c86d-41d5-a0ab-a40590b1ecfd",
    "c0367989-4a2a-47bf-9da9-e90de6c1b2b2",
    "c067960f-39c1-486f-8400-ce65fdb29dc5",
    "c071b170-cdfc-49a9-8d2a-89938a4a1404",
    "c08010b2-1394-4874-ad53-92d0c01ecdd9",
    "c095a23b-dbed-497e-b01c-26f3c645848a",
    "c0de9f93-ebfb-459b-9dcd-cc28888a7311",
    "c0f77035-3d04-470c-9c4c-93cd36f5a072",
    "c16b152c-54b8-4727-bb1a-6370e8bce739",
    "c1bcc5b3-0f4b-41fb-837a-af823c2a29c1",
    "c23922f1-760e-4704-b592-e49630206e12",
    "c26e04ca-1f59-4ffa-9f60-0bc5eb3a00cc",
    "c2823960-3321-4d1b-a80d-0b9898242659",
    "c2876bb2-400d-435d-8a97-6e9260b83955",
    "c28aa970-760d-4ed6-831c-8ffe46cbbaef",
    "c312b4f2-a98d-41d6-9509-0c2206589826",
    "c3280415-0443-4812-9430-39434bcba23a",
    "c37cb186-e175-4995-9497-eaa5efc8fde9",
    "c37d3c47-322c-4c24-a6ea-2733b33ce5a0",
    "c3d11e78-8b69-4c32-8f47-db5a583e9032",
    "c505c089-89f3-4b9a-b6df-4be7e22f5c95",
    "c55b9037-2fbc-450f-9b17-456c2c64e7c2",
    "c5a1f876-2f80-4bda-8654-aeb8b4160ae2",
    "c5aa294b-c42b-4c98-ad4d-7d774e259e55",
    "c5accb0e-b107-468d-8444-913df4d1290b",
    "c617ac6d-5cbe-43a9-a8b9-0ea980a7f95f",
    "c61f0f25-7390-47a0-94e1-e284ab96b7fc",
    "c6563b2e-41a7-4f75-8979-5e5a08dbc839",
    "c65aaf47-d4a9-4ba5-993a-abcdb9a481f5",
    "c679d72d-c148-45cd-b853-58e884686d17",
    "c6935f56-c3c1-43db-be79-33086a26a9bb",
    "c6a68a86-b1fa-4c7e-bcd1-19c74c0f6aaf",
    "c7037670-9daa-49b0-bcb2-2937a121d943",
    "c72de2ea-706f-4171-946a-c3d42d8d70ef",
    "c74c875c-280b-4e41-858d-0ded1db5aac9",
    "c7686caf-9c4c-4767-b3ea-d0df1a580a23",
    "c7c64c4c-2d20-4a07-a58d-af1263a6515c",
    "c80b6fc6-77e7-46aa-af6d-c039009f7885",
    "c8969f6c-6e54-4bbb-984a-d9abb2898b5c",
    "c8a53ccb-a35a-42c0-b212-389d6f64c0c5",
    "c90f7da5-300c-408d-a4d4-fea9736d5f1b",
    "c9878a7a-2758-4cb3-855d-0eb99bb2b940",
    "c9a1e698-3078-4790-b01b-39f016ae2c43",
    "c9c18b0c-17b1-4750-8936-0828855cdf82",
    "c9dc78a8-0289-4932-8b1a-0e65b888e819",
    "c9ee7e89-e377-4a68-9b6e-ef4d32c70554",
    "c9f78456-ba84-46a2-9388-815c1053d21c",
    "ca42adc3-6db8-4389-8c73-68a246351e98",
    "ca5f7251-894f-47cc-a64a-d4bdaa085837",
    "cad7a629-38c5-4445-8391-acf394430745",
    "cb1195f1-6273-4ccc-9aa2-5413e3c7427e",
    "cb589d50-70df-42db-98e4-6de3c4c904d2",
    "cb7176ee-27eb-4574-b260-38efac9179d7",
    "cbd04ada-c271-47f4-a138-fcfc8968ef04",
    "cbed5bfc-b22a-47cd-86a1-4ac8b3033980",
    "cc12acc9-89ff-4b6d-bb1d-34380390d357",
    "cc34611a-abaa-4076-9194-25060031b693",
    "cc3a26a2-9e7b-4980-a33a-1a427c73a0da",
    "cc5f7519-62fe-41af-b077-081fcb55edc0",
    "ccae04f8-a75b-4c6b-87f6-47c042c97db2",
    "ccc16a29-ec79-42c0-9b1a-e477f23ce0b1",
    "ccd5718e-4fdb-4ffc-a340-e4a2d027ffeb",
    "ccf50a08-876d-4160-af05-a8626525b1ac",
    "cd237d5e-4a8f-4f49-805b-c0c5deb8162a"];//91

var paperID=["b6dbe75f-195f-4078-8ddd-e2aceb6d6a51",
    "bacfb1f9-c844-4427-b8dc-4a0afbd0d046",
    "ca618cbf-0057-4a4c-84de-58d0f6db7f01",
    "de1ea7eb-7436-4d26-8f0a-ba1d074aad9d",
    "e2c691eb-5dbb-45f3-a92e-775e23546eda",
    "ed112a02-c3a0-47c2-8658-98f2a601edf5",
    "fe54fb33-2e3b-4903-87dc-6d693bb5d066",
    "2664f27c-72bc-4585-a033-fb232f41857a",
    "b50351df-9d85-4e81-a9dd-ae4eee6131a7",
    "98487c34-b190-4be6-8621-4de0ee752904",
    "0175a181-0556-46fa-8026-b5f69f5a389a",
    "b2d1002e-0c19-45c4-857c-7671be2d4184",
    "e05f24a2-3393-4c55-846a-fe1fc8bb4224",
    "120a014a-5c66-4f10-ad65-b52520588614",
    "a549de7d-bc85-447b-a73d-c5231c81f2ae",
    "1cd0e2ff-67b6-4bf5-b43a-9edd99fed655",
    "8449f34c-6251-40de-aed7-707fddf496f9",
    "a63e60b7-3c8e-4a57-af51-e65d257656ad",
    "b2971cb7-e13e-4b64-98f3-3d1d37b6a797",
    "1d08f9bb-6f1c-4459-b5db-49bcdf4c7a69",
    "338ddf38-a668-4954-a04a-35dcc39a7f50",
    "3396b20f-4dcc-4241-b2c2-4c8c0c9337e0",
    "395b8669-daf9-469f-8f2a-329aa4f6c61c",
    "3c4eba99-a781-4e3b-9514-18aedd0717dd",
    "3f54524e-f112-4bc4-a0ee-ed16e402baea",
    "45fc5656-893e-43cd-953a-8fa6f40bd3d4",
    "4d17dafd-6cb4-45b6-b71a-d82fde7b5259",
    "74ed3052-785b-4b20-a640-8b942d8cae90",
    "8b5b0860-d0ca-4991-af9e-838206fa9409",
    "938da5cf-6214-4c67-840f-8aa22a087d06",
    "9bf82297-2ee7-44c2-bb8e-d47afb64321c",
    "a0265e5f-35fd-4e6b-b2f1-97d5c3f2581f",
    "aa5eeae4-9da7-4b8e-b2a9-36c6d00b7708",
    "b51546d3-1ecc-4553-97eb-712f24c8c205",
    "bbe98b17-5ec0-4d72-9af1-93c925006417",
    "bd28cfd9-d53a-4e68-9b64-8d672e49faea",
    "bffe8d3c-2eac-4674-8c1c-96e9fc58bab1",
    "ceebe249-a59c-4335-aff4-ec155d4cc906",
    "da010e7c-4ac2-4cbb-9582-d2635cbdde6b",
    "f60da828-730b-4e22-83ad-43f29d25e057",
    "1af27d71-3701-4fc0-88d8-ae5c80b6231b",
    "2d92175a-e09f-4214-853f-2ba1d6149caa",
    "4b6729bb-f070-476c-a414-c82f197a4df3",
    "9f503a71-9c97-46a2-8afd-5c1030bc19cc",
    "e8e2db44-1b7e-4133-802a-093c49546c5f",
    "f2d07d37-eb79-4db4-9049-c307fd320924",
    "7c224cc0-166b-414e-9e0a-bb66e10388f6",
    "f44123fa-c53b-4a34-8a63-a7568ce40ab7",
    "06af7552-e787-45bf-8f34-701e7e4ddfd1",
    "9c7f9a29-2aa7-4b88-847e-f95984c41662",
    "29bf3339-c393-4b5c-873e-c9175d08d2d3",
    "cccd082e-ab43-4e62-b56f-cf2ee2c42c95",
    "0c5c35a1-3061-48e7-8724-2d0dacf5c117",
    "42f83a87-cb84-4785-a566-48124f9c559a",
    "44b2781d-5063-422b-953a-a71d1e24ecd1",
    "6f3491e8-9bb3-4eec-8080-a6ddcbbb764a",
    "807d6cd6-45d5-41ff-b051-462deb541342",
    "8643029d-a99d-4bc8-8586-dbd5d4e380f2",
    "9f6ab7ef-3b11-49ea-ab5a-904c75e25ae4",
    "ab14a9d4-c510-4509-a522-4b0ad2078696",
    "b4440450-bbbd-4e29-bdac-962ec8b0d289",
    "bc301ba3-0335-4bfa-a220-efe158b18d7a",
    "c0b8d2fa-770f-4dd9-b351-0a5db6ea75ff",
    "f4499e8c-49f3-428f-96ad-87a4aa03e1d3",
    "0bc5f182-cbb1-42e7-945f-f03c36d63583",
    "2f130ea1-eb9e-433a-a5fa-c6bf8c4663e0",
    "380c33db-0b3e-4148-84a7-2de0382b81ee",
    "3a9a16ab-d6c0-4328-b5fd-41334da9768c",
    "8478fa3f-114b-4f2a-b7ee-0df0ee8e5b79",
    "9b4dd8d0-9eab-4ea1-8ece-b0900f05dd43",
    "9e0a6697-56f4-4295-9356-5fefc18f6ccb",
    "a70978a7-7fa1-4130-93e9-2b1236f82dc3",
    "d3ae0c78-9094-401b-a7e2-42318c197aba",
    "ed9a3b23-7666-4b3f-903b-b58e5009c659",
    "466ba554-00ac-4bbc-98c4-267ea6078d7a",
    "4726ebce-0e40-4333-aa67-3d35fe4cfae5",
    "f27157c3-25d2-4f6d-ba63-9d7999b83597",
    "812c45f5-5628-4444-869a-1d8762319d1f",
    "ee5d0213-1043-4a4a-b9b1-1fbeb1644754",
    "efd3e963-7cea-4f2c-b3b8-05cc9b450e61",
    "2321f6b3-1498-4011-9d8e-2c62ff15e5e7",
    "1103375f-b61b-450f-b62a-9d8c74853828",
    "5bf5244e-bfa1-408d-93c8-ffc12c919dae",
    "14a69378-df24-41f3-a41e-9da8c980885c",
    "1c3283e6-ec41-4fc8-9525-06fde69050a7",
    "3b5bbfee-5e2e-4075-94e9-32eedeca1269",
    "efe9a241-4617-4cef-849e-f03f60586ada",
    "11ec3792-919a-4a53-b842-b44e446b6b83",
    "018eb5e7-79ca-49ac-81e8-40481648ddb3",
    "07a2d1c2-4471-4e67-ba30-f05406965c21",
    "0aceb5b7-cfc2-48cc-a192-b2434b0e7b28",
    "0d71a311-a395-4a34-8d73-ebc3ab1cd6ab",
    "1c1f38aa-57c4-41a8-b642-8ebdf44b7dd4",
    "37b7b3cf-3f16-4112-88cc-1e887672098f",
    "37fba5ce-d48a-40c5-863b-463e4f8586f7",
    "3b74df52-9ceb-4f08-b97c-43efe7d062fb",
    "3d3c186c-e4a8-4a97-888f-53a7e27c9998"];//97

var paperID1=["0f207941-7f65-4613-a84d-54e003a51a1c",
    "0f20bd40-568c-4204-8881-39ad606ace62",
    "0f20c5b6-ebc7-454a-993c-71296730bb92",
    "0f20def0-8f73-4d7e-ac94-16ccc75e9141",
    "0f20e516-b483-443a-bb59-9b4c005caed2",
    "0f20f99c-b5d5-439c-b9fb-a4660c38d290",
    "0f21155e-1f27-4e6c-b975-df9f5494640a",
    "0f2146f7-4952-4754-8ced-c91e04d5eb81",
    "0f2158a6-5129-4795-9547-96ff75a688f8",
    "0f215c57-baec-47f4-9389-5e2f9f5d2d60",
    "0f218035-1ffe-48a4-806b-7a6d1ef7b9ec",
    "0f21be7c-d006-43ad-88c5-aba55b2e28d5",
    "0f21c65e-617b-441e-a679-8db370422580",
    "0f21cc00-885d-48b7-8911-4256ec3aa0af",
    "0f21cd4d-56e6-4696-84fb-6c7bef426c76",
    "0f21dcc8-0b27-4c9e-988d-3c11d061f837",
    "0f21ed0b-1eae-4fea-b3c5-cca967874597",
    "0f21f1e8-be44-4926-8c75-e563c8e03bcc",
    "0f220c49-ff6f-4e52-b02d-db5e66236ef5",
    "0f223044-fd8b-4996-8ccd-f9d61196da3c",
    "0f223b38-1cf5-4700-8481-7084b1d7dcaf",
    "0f2240f3-e8e7-4056-aab1-7266abf32e84",
    "0f2247c7-0435-4c71-9908-f20ef44d7172",
    "0f224a49-88fd-4050-9d27-d365d17d72e6",
    "0f226978-1684-46ee-82cd-6c55f6516265",
    "0f2279ad-5b0f-45d9-96b6-62975ec75b56",
    "0f227cb4-d0e3-47cd-aa18-755f6b8d3d7d",
    "0f22806d-16ef-4d4f-a67d-d6f03262a9da",
    "0f228779-02cd-4393-bec9-0c8a2fa68d91",
    "0f22bb9c-5250-450f-bb09-496a39f2fc23",
    "0f22be53-3e4c-45e0-b645-37cd8b0cfc0e",
    "0f22c540-07eb-4922-9400-e8eec71730c6",
    "0f22d026-788a-4b1a-aea3-404c9fdd4d1e",
    "0f22eaee-1055-47b2-ae25-4dcf0bcca2be",
    "0f22f843-c315-4c71-8167-e0d5686765cb",
    "0f230187-970c-49c9-835b-08d313b7d575",
    "0f230b9f-1a73-4f72-b6a4-6bfa499892c3",
    "0f2317cf-49ec-4043-9aff-5617488167b8",
    "0f233b3e-bcb2-45f5-b3d1-602a88c3c461",
    "0f2377ab-0454-42bc-9228-6f5478bb7671",
    "0f23785f-40b0-459c-8ef7-485865b560d0",
    "0f237fa9-a831-4481-9a1c-2a9386d15be2",
    "0f23a610-06a2-4fc4-8c6a-b691e3540a52",
    "0f23abed-2373-4ee7-bd66-92ed6b815406",
    "0f23c2ef-cd60-4b0a-b2f1-0ff3add0f007",
    "0f23dfec-c383-4e1c-b0d6-64f79262ec12",
    "0f23e08d-492e-4fa3-b596-cb1822883e24",
    "0f23fd60-24a7-47f5-8403-91507bc97bd0",
    "0f240b6c-40a3-4fad-9352-2a554b31e5ca",
    "0f243f3d-f69d-419a-a553-92a9774cbf9a",
    "0f245bdb-6b75-4275-9074-8a0989e1637b",
    "0f24bb67-5aba-4cbd-99c5-2e7ae4057d60",
    "0f24e0d5-5ef2-4e3f-870e-dbf0c1d86f1e",
    "0f253e5c-601e-4a43-a169-52b784343f22",
    "0f254386-33bb-4df2-b238-612d4b998d55",
    "0f258867-0cb0-430e-8bd8-f908dbf61b3b",
    "0f258f90-6f69-4f2b-b413-ff1276a6b5bb",
    "0f259a03-4b0c-4432-a8ea-a9c73b003c71",
    "0f25a14b-574d-4913-a4bd-0a04e24efe5b",
    "0f25c4ef-c0a8-428a-b6c8-23e386b9a9b2",
    "0f25e0c2-0fcf-4cbd-878f-971ff47c09db",
    "0f25fbce-d171-447a-b1e3-512eba01590c",
    "0f260d50-8915-4c77-9ff2-3bdd4f0201ff",
    "0f261bf1-912f-4bfd-b556-6869f6f97676",
    "0f262476-83b7-4644-9e25-a8d379974e82",
    "0f262593-4c92-4111-8997-580ccc6171fb",
    "0f264a86-100a-4db5-8563-d8915a228480",
    "0f265abc-14e0-47ac-bb3d-7bc82f1e500f",
    "0f266c55-073c-461b-a444-10d9e0b4d5e5",
    "0f267667-00e8-46fd-a81f-d83cc9956190",
    "0f26776e-edcf-49e9-aa73-68521384ce6f",
    "0f267b92-aac1-476b-be8f-cdd929149783",
    "0f26b634-268c-4174-8856-e8ba79b6f0e8",
    "0f26bd20-e593-4f9a-83ec-6aa664abf38a",
    "0f26c577-701d-470a-bd79-1f373a10d7ef",
    "0f26d4eb-f912-46d3-98b4-f4c15a6e515a",
    "0f26fe1e-4af7-415e-87ab-dde8c751d4fd",
    "0f26fea3-bf90-49ab-9c9c-c8fffdbeed5c",
    "0f27465f-e123-4fa7-b400-e5155802c8b6",
    "0f2750e1-673e-43d3-9dbf-1a1ba8000a07",
    "0f275100-5241-49ba-9f12-e4fe5a8a5465",
    "0f27c8ad-245b-4ef8-a4a6-42afd3096674",
    "0f27cdcd-36d7-44fb-b2a4-1fce5c0a2e82",
    "0f27d2f8-080b-4815-b63c-e483d898c81c",
    "0f2822a3-97c4-4909-b36e-bccdaa1bb9e1",
    "0f283cd8-c997-4bdb-88a7-fd6c19b6c294",
    "0f284078-43d8-4bf2-987f-620b5ef0306f",
    "0f2870fa-55ff-44e7-b3ef-e16508b4cb4a",
    "0f288808-e6be-4a1c-bcf4-6d6b21bda7e2",
    "0f288966-84e3-44e0-b658-871972ed40c1",
    "0f28918b-1200-4912-b105-c1cc7e92fb2f",
    "0f28a259-4bff-4252-960d-f2d39e09266b",
    "0f28bc5c-2c3b-4841-85d8-a85a316ea4a0",
    "0f28cbae-324a-4950-9aeb-7016c8c260bc",
    "0f28cfc5-e204-46a2-aa9a-e60209f0013f",
    "0f28e612-abd7-4bb1-939d-af6dc2e910b7",
    "0f290b24-96ae-48f7-9304-9209bba8db17"];//97

var paperID2=["f13fb155-0d57-4f8e-8142-d7a9a6bd5e94",
    "f173466a-ee01-47b1-b0d5-bc846cf82761",
    "f1a4bc3d-7410-4293-b6bd-b7b78208158c",
    "f1d1cfc0-f4bb-4d2b-a3d2-8522c5800143",
    "f1f9e96d-313c-4e48-975a-a232c0b688a0",
    "f2530666-97f9-4a35-b3e4-3c02595d34fd",
    "f274bd07-b7b4-48d9-a49a-6fc67b2d8149",
    "f2a5245c-8010-475a-94e8-69a99c97fc13",
    "f2e94248-f3b9-4394-bb3c-01fa64814a9b",
    "f374e915-0566-4d39-9fb4-ad4522c6e9cb",
    "f37e71f2-94d4-4679-ae87-29e3fc015d59",
    "f3a1e3f9-f66f-4991-8cba-df3a3eaa5977",
    "f3bf753a-f58a-4e7a-9bb0-0ea94dc622eb",
    "f45d813d-d3c4-4786-a1f2-9f0e058ce506",
    "f4dba2bb-fbe2-4351-8422-f91be1fa18f8",
    "f54f649c-54c2-4d25-b71e-ea50c1a80e0e",
    "f602f6d8-0d4f-4e27-bf50-b0090a777688",
    "f6759aee-87c1-4e3b-8497-5920872cf406",
    "f683cb95-3ca4-47f8-840c-47f75c949a14",
    "f68aa15d-2d75-4579-ae0f-8d04b64bd856",
    "f6b5e16c-94d1-4973-b926-6324aa136d9a",
    "f7063ad2-5132-48ef-a663-e0566df8f41a",
    "f73622bf-3c3e-442e-859c-2d3d53637769",
    "f747edd6-1230-4b16-a327-325109ac1af3",
    "f74b3295-f6a4-4b20-a5bd-eb91f008e913",
    "f75be644-1f5b-46e8-ab11-5d44586ab48e",
    "f7889f2b-10aa-40e2-9d39-712851787a3e",
    "f7c38128-b1f4-4aa4-9fb4-35a7aad59ff3",
    "f85ec183-3d3a-4b2c-aa58-8a47683142ec",
    "f891adce-95a2-42d0-bd71-7cf1878b846b",
    "f92e4cc3-e6d3-4114-b96f-73a88da16aef",
    "f95b3bb4-c2ab-48e1-8c0f-c2bfd5ecb76e",
    "f96c7260-ef0e-446a-a114-933ee2559d38",
    "f983ed47-536a-4143-b4f5-88a55c3c19f4",
    "fa11b6da-b401-4262-b2d2-6f9e3f900aa6",
    "fa1dc661-bf92-4c8f-9745-a31b06270703",
    "fa25e657-cf14-4e85-a768-10e776b33260",
    "fa3094df-79c9-49d0-8718-3c69a7c0e481",
    "fa5b9398-a68b-419f-97c6-d52f91b78b05",
    "fa6c54a3-1171-48dc-82b3-81186f2652e1",
    "fae0fd0a-5a1b-44cd-b38c-d0d0716f7eac",
    "fae49c74-7fee-427d-9931-ee341e572bd4",
    "faeb104d-bfa7-4daf-a264-cd9fe63b7d37",
    "fb1a1320-b363-47cd-bc9e-5d69595ceb5b",
    "fb4192e3-7979-4e96-95d3-0a744c38f819",
    "fb460cb3-3224-464a-a0fd-3df3acc7d39a",
    "fb6dc45a-7091-43cc-a173-ec56068187dc",
    "fbbc1fd7-697b-4172-be30-b032c84acd24",
    "fc0809cd-e8b5-4f50-a929-e09312c59fe0",
    "fc27af72-9f8c-4bda-8f19-11100096ae59",
    "fc28a7ef-ace9-4c78-a469-c42cc7dd1074",
    "fc2baede-04d9-411d-8113-483595202be9",
    "fc80db52-d4ae-4d48-9cea-2db8d2c1a91e",
    "fca93a4a-32ab-4277-91f8-6cf94787c07e",
    "fcacc1c4-af61-46be-9487-6894945b1b0e",
    "fcba21ad-9aaa-4d62-9ae2-470310429a49",
    "fcdeea27-105a-45bb-b34d-0eecf6f415bf",
    "fcf39371-a4cd-45d6-a99e-21d58b3a4ea4",
    "fd23743e-8601-4601-80e0-5c17f0da2fe2",
    "fdb93b15-70d8-451a-9d20-cd326e0c2bd3",
    "fe801f8f-6283-4cca-b2dd-15f55e7159aa",
    "fe9de131-7dd5-4345-90f8-f35bcd5e795f",
    "febf1d16-9501-470b-9a12-271aa60f8bad",
    "fece56be-a319-4cfd-a59c-b5b612c436a2",
    "fee4a0fb-3dd4-4d0c-9a5e-f7def73f5a6e",
    "feed36b8-0e46-4984-a463-ae159525c521",
    "ff5ce050-ea8d-40e8-a25f-c629bed2ff9c",
    "ff941105-0890-447a-a15f-1623dec0870e",
    "e5fe0ef1-349a-4739-b162-cd43824d4b0d",
    "e6699f5c-2305-4b2d-9431-b389bf102bb4",
    "e6d6eaad-1b13-4791-beee-bc8ac52ef8fb",
    "e6e996bb-bd8f-4504-a1a5-dad86fb1fc49",
    "e799d79b-1578-412c-b699-8205cfd43adb",
    "e7d135e6-7206-4ae1-95e5-447517708676",
    "e820c16c-d196-4985-b851-ed64a234b2c1",
    "e8530971-af56-430b-b1fa-b7e93dc8f832",
    "e86af6a1-9358-4786-b66f-4c6fc2f7de6c",
    "e88d7d8d-71ee-403f-97ed-823e42983bc8",
    "e8d84e89-b4f7-4956-8ca9-0dc2233c9776",
    "e8ee1a91-0545-476f-ab84-119501d2e90f",
    "e9126a58-7ea3-4390-9c7a-1a9165077c30",
    "e930af35-efb3-49c7-9375-80bacafb1a24",
    "e9327f0a-ea72-4a0e-89d1-7c0db4b5547b",
    "e936e57c-a06f-4c69-9c99-ff22644838b9",
    "e954e564-fafc-4b6f-9744-055ffa8334c3",
    "e95c0ded-6949-44fe-8057-dd199681c475",
    "e96cb972-c263-4dca-8b70-6c4acd152bdb",
    "e9ca2b89-e164-4805-b989-748c9b503943",
    "ea1a4274-5f34-46a4-8e77-5b8cd6e892f0",
    "ea22194d-a588-48ac-83f2-b7b27c80972b",
    "ea2462bc-b265-4b5d-8132-b6aa5cddba06",
    "ea3eb6de-e5ad-425d-899f-60e7fc9c6628",
    "ea473177-748f-42b6-b369-a824f544019f",
    "ea864814-cf86-44bf-a7f2-9c330be3dd3d",
    "ea8b9179-8349-4abb-9b55-d29256fba97a",
    "eaa9dbc6-79b4-428e-8a97-783c3102ee1f",
    "eae5cb1a-ef7c-4e18-9d28-532ecdc589df"];//97

var aggregationVal=["5978024e-76c2-4990-a330-6c51125c88d0",
    "5978037d-fa62-4273-ae96-e68b258324bc",
    "59780572-6ff1-414d-ad06-3eeaf6e6db6d",
    "59782727-caa5-495c-bf41-13623b76ed62",
    "59782cb8-6403-4b91-903d-f642517acc6f",
    "597838ff-9655-411a-9c49-127f417e053d",
    "5978415d-7686-43d7-97c2-d0eafe80082e",
    "59784d19-e509-4400-a555-9e0a577fd1de",
    "59786abb-c6bc-4d7b-b207-c028cc367d38",
    "59787094-f8a6-461f-92f6-426f911db886",
    "5978728a-46a8-41fe-bddc-68b254200188",
    "59788988-14d9-4ec1-a195-c4bed2d18986",
    "5978d1ab-1ff9-47be-a213-26c67481dc18",
    "5978d3e9-2a82-4f19-a6db-9ce2e4efbcdc",
    "5978ef8b-b37c-41d7-a09c-36b1ca3f8446",
    "59790888-9375-4955-b1d8-3e989daa6fee",
    "59792b02-9ed9-43e0-8296-1f855ecc0098",
    "59793be7-9610-403e-8e50-45adcdab8136",
    "5979404b-9fe6-4751-8c37-554944797853",
    "59795a82-a459-4dee-99d2-a0e7d583aff3",
    "5979771c-e5da-4b50-be3d-9ec7b4882b52",
    "59797ca4-24ff-4039-9ab7-dc07e6349868",
    "59798136-bcdf-429c-811c-c3b8e857e92a",
    "59798ae6-8344-4084-9696-994fabf48eae",
    "5979ada6-5bf4-4f44-b959-6c8d530b034b",
    "5979cc39-db7c-45e4-8a08-6e05af3b6129",
    "5979e06b-998e-47f6-b5bb-00c69e855946",
    "5979e743-79dd-4673-9dbe-09f206164d57",
    "5979f264-49ad-4386-b2a4-13b0e4b06c46",
    "5979f32f-4d6c-4321-b1e9-3f71aa33a1cf",
    "597a1380-b83a-4170-99de-c6bf064fe6b3",
    "597a165e-fbaa-4609-a122-cdf292b2b55c",
    "597a1e5b-f465-4cc3-9e73-bfe76358c8c7",
    "597a21d1-6469-45e4-8e4b-6e4bf7862de5",
    "597a2429-83bc-41df-8233-f1e750e6f574",
    "597a3bd4-817a-4ec0-9817-8bd618305daa",
    "597a4a8f-ebe4-4665-96fc-5f02b22d5143",
    "597a547e-8da7-4d53-a0f1-b0530e55410c",
    "597a582c-e47b-40bb-8bf9-38bcdb559e83",
    "597a82f8-f984-459b-a8ad-9e5f8c00d0b2",
    "597a8410-5c30-4f26-b96f-bbcc1d7ad4cb",
    "597a9fe6-77b1-4321-936b-c1ddf057297a",
    "597aa03c-4ad8-4821-aa19-248040cfcd59",
    "597ac59c-554a-40f7-a26e-f13a018d759a",
    "597ad008-d56f-4410-8821-16f0f7723e69",
    "597ae797-f257-4db8-bc96-e27f80fee285",
    "597b1151-ad10-4c30-9bbb-a861bb738938",
    "597b26a2-15eb-431b-b1a4-0ee6f6849b5a",
    "597b5beb-2b8e-42e0-9a73-7dfb97cf4304",
    "597b69b6-9fe2-4f59-b5fd-152a572bd007",
    "597b6c72-7391-41e0-acaa-39a386e0151d",
    "597b6d5e-a3fa-4456-9e55-37e82ed2af93",
    "597b9744-e896-4b87-8a86-627fd7301e99",
    "597b9e52-52f3-4a7e-8bee-54e8e2f2024e",
    "597bcb3c-b16c-460f-a5b3-760da381d6fd",
    "597be1e9-c785-4961-909f-ae8327686331",
    "597bed4d-e855-43c8-90fe-40ba399dc1fa",
    "597c109e-9791-4e6b-86a1-fff18a7436ee",
    "597c2259-eec9-45df-a93b-9c133e8527c7",
    "597c2769-89db-423a-b265-050bf20467d2",
    "597c2d03-6408-4019-bb56-21831c7afdb9",
    "597c44ba-e06c-427b-8369-5cbf29643885",
    "597c6837-f838-45b5-ab92-2b4b63239fbf",
    "597c6a1b-bc6c-4f56-944d-7b93248c2b3c",
    "597c7112-4dc9-48b6-af36-9ac1acaaa67d",
    "597c8ec3-c419-41d9-a772-f3957a908903",
    "597c913d-0216-4e8d-86f2-3b679b3367ae",
    "597c9b80-6374-46c8-9c05-a2e35b7baeda",
    "597ca0ec-f3e8-4153-84f8-8a47671e1119",
    "597ca5d6-367f-4785-a2ff-ac92c9644d37",
    "597caba2-b269-4eea-9103-a184fb93ffc5",
    "597d21a8-5e4d-4fe4-b4ba-b8b69667288c",
    "597d4323-9f3a-4138-8d63-d12a11ff95a3",
    "597d615e-450a-445c-b520-410d00595995",
    "597d9602-91a3-4f57-8528-52a35980a538",
    "597dbb50-62e3-4cb9-8b90-ed8e25903b78",
    "597dc3e9-2a30-4b93-90bb-8d1e77a84a74",
    "597dc74c-26ae-482e-9b2c-96e7023ec78a",
    "597dd13b-4f50-4bd8-acf2-de90085379b2"];//79

function executeQueriresWithoutCaching() {
    var arr=new Array();
    var arr2=new Array();
    var Finaltime=0;
    var clearUrl="http://localhost:9200/dblpvertexes,dblprelation_authorship,dblprelation_citation,dblprelation_coauthorship,dblprelation_journalofpaper,dblprelation_venueofpaper/_cache/clear?request=true";
    $.ajax({
        dataType: "json",
        url: clearUrl,
        async: false,
        success: function (data) {
            console.log(data._shards.successful);
        }
    });
    for(var loopcnt=0; loopcnt<100;loopcnt++){
        totalTime=0,authorNameCnt=0,authorIdCnt=0,randomValCnt=0,coAuthorIDCnt=0,journalIDCnt=0,venueIDCnt=0,referenceIDCnt=0,paperIDCnt=0,aggregationValCnt=0;
        randomValArr=[5, 6, 10, 5, 9, 1, 5, 5, 8, 1, 15, 12, 18, 10, 8, 12, 7, 16, 0, 13, 17, 17, 5, 2, 3, 6, 13, 10, 7, 4, 9, 14, 0, 14, 3, 1, 16, 8, 14, 4, 16, 3, 10, 9, 5, 11, 4, 18, 17, 6, 10, 17, 12, 17, 1, 10, 16, 11, 9, 18, 16, 5, 12, 4, 11, 9, 16, 7, 1, 2, 13, 18, 0, 0, 15, 7, 5, 13, 13, 7, 8, 0, 10, 6, 11, 13, 10, 10, 8, 7, 14, 11, 11, 7, 1, 11, 6, 11, 0, 18];
        arr=[3, 8, 4, 9, 4, 1, 5, 9, 4, 8, 1, 3, 1, 10, 1, 4, 2, 7, 6, 7, 7, 2, 9, 6, 9, 10, 1, 9, 1, 8, 1, 2, 2, 5, 9, 2, 5, 4, 1, 10, 1, 10, 3, 7, 1, 3, 5, 2, 4, 3, 10, 10, 10, 5, 2, 8, 1, 5, 3, 9, 6, 10, 5, 5, 10, 10, 10, 4, 5, 6, 3, 8, 9, 3, 3, 2, 6, 3, 4, 2, 3, 5, 4, 4, 7, 5, 1, 1, 3, 1, 1, 3, 2, 2, 6, 5, 1, 5, 2, 7, 1, 1, 7, 9, 3, 7, 10, 7, 9, 4, 10, 2, 7, 1, 4, 5, 4, 8, 9, 6, 1, 1, 10, 6, 9, 1, 3, 1, 1, 5, 10, 6, 9, 5, 1, 8, 6, 7, 8, 10, 5, 9, 7, 1, 3, 4, 3, 2, 5, 7, 4, 7, 1, 4, 5, 3, 7, 7, 4, 5, 8, 10, 4, 6, 2, 9, 6, 4, 10, 4, 7, 6, 2, 8, 9, 7, 5, 3, 2, 9, 3, 10, 6, 10, 9, 4, 10, 5, 5, 2, 8, 7, 2, 1, 3, 9, 2, 5, 3, 4, 9, 3, 10, 8, 1, 5, 6, 6, 2, 10, 3, 8, 9, 3, 6, 4, 3, 6, 3, 2, 8, 6, 7, 7, 7, 10, 8, 4, 8, 5, 8, 6, 3, 4, 6, 1, 4, 10, 10, 5, 6, 8, 8, 5, 1, 2, 4, 3, 9, 3, 3, 9, 2, 7, 3, 8, 4, 1, 3, 10, 2, 5, 1, 7, 6, 8, 5, 2, 10, 4, 5, 2, 5, 4, 6, 9, 1, 3, 4, 5, 1, 8, 1, 9, 3, 4, 7, 10, 9, 7, 2, 2, 1, 7, 9, 4, 5, 5, 8, 3, 7, 3, 3, 7, 5, 5, 4, 1, 8, 7, 9, 10, 9, 5, 1, 4, 8, 7, 10, 9, 7, 2, 6, 5, 8, 2, 4, 5, 9, 3, 2, 4, 6, 1, 3, 9, 5, 2, 1, 10, 10, 2, 4, 3, 1, 1, 4, 9, 9, 4, 8, 4, 4, 9, 4, 7, 6, 8, 7, 10, 8, 4, 5, 8, 8, 6, 9, 2, 5, 10, 3, 8, 9, 2, 1, 9, 9, 4, 6, 5, 8, 5, 1, 3, 3, 2, 4, 4, 8, 7, 9, 4, 3, 7, 10, 4, 8, 4, 5, 10, 4, 4, 2, 2, 8, 7, 2, 3, 10, 6, 10, 9, 2, 4, 4, 8, 4, 6, 5, 5, 7, 1, 9, 10, 3, 10, 3, 5, 9, 10, 8, 1, 5, 4, 1, 5, 1, 9, 10, 7, 4, 7, 3, 1, 7, 6, 7, 6, 4, 4, 8, 3, 4, 4, 8, 6, 2, 1, 10, 8, 8, 10, 7, 3, 2, 1, 1, 3, 1, 10, 3, 4, 4, 8, 5, 3, 6, 1, 10, 7, 10, 6, 7, 5, 6, 3, 3, 6, 5, 3, 7, 5, 8, 6, 2, 6, 6, 2, 4, 9, 2, 9, 2, 5, 6, 5, 4, 6, 3, 9, 6, 3, 4, 5, 6, 4, 2, 8, 7, 10, 6, 1, 8, 3, 6, 10, 5, 6, 2, 6, 2, 2, 3, 2, 5, 4, 5, 9, 9, 8, 2, 10, 8, 3, 6, 4, 4, 6, 3, 7, 1, 8, 8, 1, 9, 7, 5, 6, 8, 2, 4, 10, 5, 9, 10, 3, 4, 1, 3, 2, 3, 1, 4, 10, 4, 7, 4, 2, 8, 10, 10, 4, 2, 4, 3, 3, 1, 7, 5, 4, 2, 7, 10, 10, 1, 8, 8, 4, 6, 1, 2, 8, 10, 7, 1, 8, 4, 7, 8, 10, 8, 6, 1, 1, 2, 10, 6, 8, 5, 7, 1, 2, 2, 4, 6, 3, 2, 7, 2, 3, 10, 1, 4, 2, 3, 3, 1, 10, 3, 5, 2, 1, 2, 9, 7, 1, 10, 1, 5, 8, 5, 8, 7, 9, 4, 5, 6, 6, 7, 8, 1, 5, 4, 6, 8, 8, 3, 4, 1, 10, 9, 8, 2, 8, 1, 10, 4, 4, 4, 3, 10, 5, 1, 8, 2, 7, 8, 4, 3, 8, 7, 2, 10, 10, 8, 5, 1, 7, 10, 5, 6, 2, 8, 4, 1, 3, 4, 1, 7, 4, 2, 1, 5, 4, 7, 5, 6, 8, 3, 9, 10, 8, 10, 9, 2, 6, 3, 1, 3, 9, 10, 2, 5, 4, 5, 1, 8, 5, 6, 9, 2, 9, 5, 5, 2, 9, 3, 1, 6, 7, 8, 6, 3, 10, 2, 9, 8, 3, 5, 3, 10, 7, 2, 4, 5, 8, 9, 10, 1, 5, 5, 1, 2, 9, 10, 7, 7, 5, 5, 2, 4, 10, 3, 2, 3, 5, 1, 3, 9, 3, 5, 7, 7, 8, 1, 4, 8, 5, 4, 9, 5, 7, 5, 4, 7, 2, 7, 3, 6, 6, 10, 10, 10, 1, 6, 7, 7, 2, 3, 6, 4, 7, 3, 1, 9, 8, 7, 6, 9, 5, 6, 1, 7, 2, 9, 4, 9, 4, 2, 3, 3, 7, 8, 1, 7, 6, 5, 1, 3, 8, 10, 5, 9, 3, 5, 4, 10, 8, 3, 3, 2, 8, 6, 10, 3, 4, 6, 10, 1, 8, 2, 10, 7, 7, 10, 1, 5, 7, 7, 3, 2, 4, 1, 2, 3, 8, 6, 6, 10, 3, 3, 8, 6, 8, 7, 8, 8, 1, 1, 1, 2, 10, 1, 1, 10, 8, 5, 10, 6, 4, 5, 3, 10, 2, 9, 2, 10, 1, 8, 8, 9, 10, 10, 1, 8, 1, 3, 5, 10, 3, 4, 3, 5, 8, 1, 10, 1, 1, 5, 6, 5, 7, 1, 6, 9, 1, 4, 4, 9, 1, 8, 2, 4, 4, 5, 3, 9, 4, 4, 5, 1, 10, 7, 5, 3, 5, 3, 5, 2, 5, 9, 7, 1, 2, 5, 2, 10, 4, 2, 6, 2, 8, 3, 10, 7, 2, 6, 9, 5, 5, 1, 7, 3, 9, 7, 4, 3, 9, 6, 6];
        arr2=[3, 23, 27, 29, 15, 27, 24, 6, 5, 19, 13, 21, 23, 28, 8, 16, 16, 20, 28, 24, 23, 10, 25, 4, 10, 7, 25, 5, 25, 1, 15, 14, 25, 15, 13, 14, 20, 7, 1, 3, 12, 19, 4, 28, 19, 9, 16, 17, 22, 8, 21, 25, 18, 16, 18, 7, 1, 15, 0, 1, 0, 17, 17, 15, 18, 25, 14, 23, 9, 28, 16, 25, 11, 8, 4, 27, 22, 7, 9, 4, 12, 27, 16, 0, 14, 13, 17, 2, 5, 19, 18, 18, 16, 15, 6, 26, 15, 7, 0, 10, 28, 13, 25, 26, 21, 6, 29, 10, 24, 15, 21];
        for (var i=0;i<1000;i++){
            switch (arr[i]){
                case 1:{
                    var url="http://localhost:9200/dblpvertexes/_search?q=vType:author AND AUTHOR_NAME:" + authorName[authorNameCnt++]+"&request_cache=false";
                    $.ajax({
                        dataType: "json",
                        url: url,
                        async: false,
                        success: function (data) {
                            totalTime =totalTime+ parseInt(data.took);
                        }
                    });
                    break;
                }
                case 9:{

                }
                case 3:{
                    var url="http://localhost:9200/dblprelation_authorship/_search?q=AUTHOR_ID:"+authorID[authorIdCnt++]+"&request_cache=false";
                    $.ajax({
                        dataType: "json",
                        url: url,
                        async: false,
                        success: function (data) {
                            totalTime =totalTime+ parseInt(data.took);
                        }
                    });
                    break;
                }
                case 4:{
                    var url="http://localhost:9200/dblprelation_coauthorship/_search?q=COAUTHOR_ID:"+coAuthorID[coAuthorIDCnt++]+"&request_cache=false";
                    $.ajax({
                        dataType: "json",
                        url: url,
                        async: false,
                        success: function (data) {
                            totalTime =totalTime+ parseInt(data.took);
                        }
                    });
                    break;
                }
                case 5:{
                    var url="http://localhost:9200/dblprelation_journalofpaper/_search?q=JOURNAL_ID:"+journalID[arr2[journalIDCnt++]]+"&request_cache=false";
                    $.ajax({
                        dataType: "json",
                        url: url,
                        async: false,
                        success: function (data) {
                            totalTime =totalTime+ parseInt(data.took);
                        }
                    });
                    break;
                }
                case 6:{
                    var url="http://localhost:9200/dblprelation_venueofpaper/_search?q=VENUE_ID:"+venueID[venueIDCnt++]+"&request_cache=false";
                    $.ajax({
                        dataType: "json",
                        url: url,
                        async: false,
                        success: function (data) {
                            totalTime =totalTime+ parseInt(data.took);
                        }
                    });
                    break;
                }
                case 7:{
                    var url="http://localhost:9200/dblprelation_citation/_search?q=REFERENCE_ID:"+referenceID[referenceIDCnt++]+"&request_cache=false";
                    $.ajax({
                        dataType: "json",
                        url: url,
                        async: false,
                        success: function (data) {
                            totalTime =totalTime+ parseInt(data.took);
                        }
                    });
                    break;
                }
                case 8:{
                    var url="http://localhost:9200/dblprelation_citation/_search?q=PAPER_ID:"+paperID[paperIDCnt++]+"&request_cache=false";
                    $.ajax({
                        dataType: "json",
                        url: url,
                        async: false,
                        success: function (data) {
                            totalTime =totalTime+ parseInt(data.took);
                        }
                    });
                    break;
                }
                case 2:{
                    var urls;
                    var  query = '{\n' +
                        '            "size":0,\n' +
                        '            "query":{\n' +
                        '                "query_string":{\n' +
                        '                    "query":' +"\""+ randomVal[randomValArr[aggregationValCnt++]] +"\""+ '\n' +
                        '                }\n' +
                        '            },\n' +
                        '            "aggs":{\n' +
                        '                "paperByYear":{\n' +
                        '                    "terms":{\n' +
                        '                        "field":"Year.keyword",\n' +
                        '                        "size":2000\n' +
                        '                    }\n' +
                        '                }\n' +
                        '            }\n' +
                        '        }';
                    $.ajax({
                        type:"POST",
                        url: 'http://localhost:9200/dblpvertexes/_search?request_cache=false',
                        contentType: 'application/json',
                        dataType: "json",
                        async: false,
                        data:query,
                        success: function (data) {
                            totalTime =totalTime+ parseInt(data.took);
                        }
                    });

                    break;
                    //var url='{"size":0,"query": {"bool":{"must": [{"match":{"REFERENCE_ID.keyword:'+aggregationVal[aggregationValCnt++]+'}}]}},"aggs":{"paperByYear":{"terms":{"field":"REFERENCE_ID.keyword","size":10}}}';

                    /*
                                        var query="{\"size\":0,\n" +
                                        "  \"query\": {\n" +
                                        "    \"bool\": {\n" +
                                        "      \"must\": [\n" +
                                        "        {\n" +
                                        "          \"match\": {\n" +
                                                  "REFERENCE_ID.keyword:\"" +aggregationVal[aggregationValCnt++]+"\""+
                                        "          }\n" +
                                        "        }\n" +
                                        "      ]\n" +
                                        "    }\n" +
                                        "  },\n" +
                                        "   \"aggs\":{\n" +
                                        "                \"paperByYear\":{\n" +
                                        "                    \"terms\":{\n" +
                                        "                        \"field\":\"REFERENCE_ID.keyword\",\n" +
                                        "                        \"size\":10\n" +
                                        "                    }\n" +
                                        "                }\n" +
                                        "            }\n" +
                                        "}";
                                    $.ajax({
                                        type:"POST",
                                        url: 'http://localhost:9200/dblprelation_citation/_search/',
                                        dataType: "application/json",
                                        async: false,
                                        data:query,
                                        success: function (data) {
                                            totalTime =totalTime+ parseInt(data.took);
                                        }
                                    });*/
                }
                case 10:{

                    var url= "http://localhost:9200/dblpvertexes/_search?q="+randomVal[randomValArr[randomValCnt++]]+"&request_cache=false";
                    $.ajax({
                        dataType: "json",
                        url: url,
                        async: false,
                        success: function (data) {
                            totalTime =totalTime+ parseInt(data.took);
                        }
                    });
                    break;
                }
            }
        }
        Finaltime=Finaltime+totalTime;

    }


    document.getElementById("totalTimewocaching").innerText=Finaltime;
    var clearUrl="http://localhost:9200/dblpvertexes,dblprelation_authorship,dblprelation_citation,dblprelation_coauthorship,dblprelation_journalofpaper,dblprelation_venueofpaper/_cache/clear?request=true";
    $.ajax({
        dataType: "json",
        url: clearUrl,
        async: false,
        success: function (data) {
            console.log(data._shards.successful);
        }
    });
}

function executeQueriresWithCaching() {
    var arr2=new Array();
    var arr=new Array();
    var Finaltime=0;

    for(var loopcnt=0; loopcnt<10;loopcnt++) {
    totalTime=0,authorNameCnt=0,authorIdCnt=0,randomValCnt=0,coAuthorIDCnt=0,journalIDCnt=0,venueIDCnt=0,referenceIDCnt=0,paperIDCnt=0,aggregationValCnt=0;
    randomValArr=[5, 6, 10, 5, 9, 1, 5, 5, 8, 1, 15, 12, 18, 10, 8, 12, 7, 16, 0, 13, 17, 17, 5, 2, 3, 6, 13, 10, 7, 4, 9, 14, 0, 14, 3, 1, 16, 8, 14, 4, 16, 3, 10, 9, 5, 11, 4, 18, 17, 6, 10, 17, 12, 17, 1, 10, 16, 11, 9, 18, 16, 5, 12, 4, 11, 9, 16, 7, 1, 2, 13, 18, 0, 0, 15, 7, 5, 13, 13, 7, 8, 0, 10, 6, 11, 13, 10, 10, 8, 7, 14, 11, 11, 7, 1, 11, 6, 11, 0, 18];
    arr=[3, 8, 4, 9, 4, 1, 5, 9, 4, 8, 1, 3, 1, 10, 1, 4, 2, 7, 6, 7, 7, 2, 9, 6, 9, 10, 1, 9, 1, 8, 1, 2, 2, 5, 9, 2, 5, 4, 1, 10, 1, 10, 3, 7, 1, 3, 5, 2, 4, 3, 10, 10, 10, 5, 2, 8, 1, 5, 3, 9, 6, 10, 5, 5, 10, 10, 10, 4, 5, 6, 3, 8, 9, 3, 3, 2, 6, 3, 4, 2, 3, 5, 4, 4, 7, 5, 1, 1, 3, 1, 1, 3, 2, 2, 6, 5, 1, 5, 2, 7, 1, 1, 7, 9, 3, 7, 10, 7, 9, 4, 10, 2, 7, 1, 4, 5, 4, 8, 9, 6, 1, 1, 10, 6, 9, 1, 3, 1, 1, 5, 10, 6, 9, 5, 1, 8, 6, 7, 8, 10, 5, 9, 7, 1, 3, 4, 3, 2, 5, 7, 4, 7, 1, 4, 5, 3, 7, 7, 4, 5, 8, 10, 4, 6, 2, 9, 6, 4, 10, 4, 7, 6, 2, 8, 9, 7, 5, 3, 2, 9, 3, 10, 6, 10, 9, 4, 10, 5, 5, 2, 8, 7, 2, 1, 3, 9, 2, 5, 3, 4, 9, 3, 10, 8, 1, 5, 6, 6, 2, 10, 3, 8, 9, 3, 6, 4, 3, 6, 3, 2, 8, 6, 7, 7, 7, 10, 8, 4, 8, 5, 8, 6, 3, 4, 6, 1, 4, 10, 10, 5, 6, 8, 8, 5, 1, 2, 4, 3, 9, 3, 3, 9, 2, 7, 3, 8, 4, 1, 3, 10, 2, 5, 1, 7, 6, 8, 5, 2, 10, 4, 5, 2, 5, 4, 6, 9, 1, 3, 4, 5, 1, 8, 1, 9, 3, 4, 7, 10, 9, 7, 2, 2, 1, 7, 9, 4, 5, 5, 8, 3, 7, 3, 3, 7, 5, 5, 4, 1, 8, 7, 9, 10, 9, 5, 1, 4, 8, 7, 10, 9, 7, 2, 6, 5, 8, 2, 4, 5, 9, 3, 2, 4, 6, 1, 3, 9, 5, 2, 1, 10, 10, 2, 4, 3, 1, 1, 4, 9, 9, 4, 8, 4, 4, 9, 4, 7, 6, 8, 7, 10, 8, 4, 5, 8, 8, 6, 9, 2, 5, 10, 3, 8, 9, 2, 1, 9, 9, 4, 6, 5, 8, 5, 1, 3, 3, 2, 4, 4, 8, 7, 9, 4, 3, 7, 10, 4, 8, 4, 5, 10, 4, 4, 2, 2, 8, 7, 2, 3, 10, 6, 10, 9, 2, 4, 4, 8, 4, 6, 5, 5, 7, 1, 9, 10, 3, 10, 3, 5, 9, 10, 8, 1, 5, 4, 1, 5, 1, 9, 10, 7, 4, 7, 3, 1, 7, 6, 7, 6, 4, 4, 8, 3, 4, 4, 8, 6, 2, 1, 10, 8, 8, 10, 7, 3, 2, 1, 1, 3, 1, 10, 3, 4, 4, 8, 5, 3, 6, 1, 10, 7, 10, 6, 7, 5, 6, 3, 3, 6, 5, 3, 7, 5, 8, 6, 2, 6, 6, 2, 4, 9, 2, 9, 2, 5, 6, 5, 4, 6, 3, 9, 6, 3, 4, 5, 6, 4, 2, 8, 7, 10, 6, 1, 8, 3, 6, 10, 5, 6, 2, 6, 2, 2, 3, 2, 5, 4, 5, 9, 9, 8, 2, 10, 8, 3, 6, 4, 4, 6, 3, 7, 1, 8, 8, 1, 9, 7, 5, 6, 8, 2, 4, 10, 5, 9, 10, 3, 4, 1, 3, 2, 3, 1, 4, 10, 4, 7, 4, 2, 8, 10, 10, 4, 2, 4, 3, 3, 1, 7, 5, 4, 2, 7, 10, 10, 1, 8, 8, 4, 6, 1, 2, 8, 10, 7, 1, 8, 4, 7, 8, 10, 8, 6, 1, 1, 2, 10, 6, 8, 5, 7, 1, 2, 2, 4, 6, 3, 2, 7, 2, 3, 10, 1, 4, 2, 3, 3, 1, 10, 3, 5, 2, 1, 2, 9, 7, 1, 10, 1, 5, 8, 5, 8, 7, 9, 4, 5, 6, 6, 7, 8, 1, 5, 4, 6, 8, 8, 3, 4, 1, 10, 9, 8, 2, 8, 1, 10, 4, 4, 4, 3, 10, 5, 1, 8, 2, 7, 8, 4, 3, 8, 7, 2, 10, 10, 8, 5, 1, 7, 10, 5, 6, 2, 8, 4, 1, 3, 4, 1, 7, 4, 2, 1, 5, 4, 7, 5, 6, 8, 3, 9, 10, 8, 10, 9, 2, 6, 3, 1, 3, 9, 10, 2, 5, 4, 5, 1, 8, 5, 6, 9, 2, 9, 5, 5, 2, 9, 3, 1, 6, 7, 8, 6, 3, 10, 2, 9, 8, 3, 5, 3, 10, 7, 2, 4, 5, 8, 9, 10, 1, 5, 5, 1, 2, 9, 10, 7, 7, 5, 5, 2, 4, 10, 3, 2, 3, 5, 1, 3, 9, 3, 5, 7, 7, 8, 1, 4, 8, 5, 4, 9, 5, 7, 5, 4, 7, 2, 7, 3, 6, 6, 10, 10, 10, 1, 6, 7, 7, 2, 3, 6, 4, 7, 3, 1, 9, 8, 7, 6, 9, 5, 6, 1, 7, 2, 9, 4, 9, 4, 2, 3, 3, 7, 8, 1, 7, 6, 5, 1, 3, 8, 10, 5, 9, 3, 5, 4, 10, 8, 3, 3, 2, 8, 6, 10, 3, 4, 6, 10, 1, 8, 2, 10, 7, 7, 10, 1, 5, 7, 7, 3, 2, 4, 1, 2, 3, 8, 6, 6, 10, 3, 3, 8, 6, 8, 7, 8, 8, 1, 1, 1, 2, 10, 1, 1, 10, 8, 5, 10, 6, 4, 5, 3, 10, 2, 9, 2, 10, 1, 8, 8, 9, 10, 10, 1, 8, 1, 3, 5, 10, 3, 4, 3, 5, 8, 1, 10, 1, 1, 5, 6, 5, 7, 1, 6, 9, 1, 4, 4, 9, 1, 8, 2, 4, 4, 5, 3, 9, 4, 4, 5, 1, 10, 7, 5, 3, 5, 3, 5, 2, 5, 9, 7, 1, 2, 5, 2, 10, 4, 2, 6, 2, 8, 3, 10, 7, 2, 6, 9, 5, 5, 1, 7, 3, 9, 7, 4, 3, 9, 6, 6];
    arr2=[3, 23, 27, 29, 15, 27, 24, 6, 5, 19, 13, 21, 23, 28, 8, 16, 16, 20, 28, 24, 23, 10, 25, 4, 10, 7, 25, 5, 25, 1, 15, 14, 25, 15, 13, 14, 20, 7, 1, 3, 12, 19, 4, 28, 19, 9, 16, 17, 22, 8, 21, 25, 18, 16, 18, 7, 1, 15, 0, 1, 0, 17, 17, 15, 18, 25, 14, 23, 9, 28, 16, 25, 11, 8, 4, 27, 22, 7, 9, 4, 12, 27, 16, 0, 14, 13, 17, 2, 5, 19, 18, 18, 16, 15, 6, 26, 15, 7, 0, 10, 28, 13, 25, 26, 21, 6, 29, 10, 24, 15, 21];
    //randomValArr=[4, 8, 3, 9, 7, 7, 6, 16, 6, 2, 4, 0, 18, 17, 4, 15, 18, 12, 3, 4, 1, 14, 9, 14, 6, 9, 14, 1, 4, 0, 18, 11, 5, 11, 6, 16, 13, 0, 1, 17, 13, 17, 13, 10, 5, 15, 8, 5, 12, 4, 14, 11, 7, 10, 13, 14, 17, 11, 5, 3, 16, 11, 6, 7, 18, 15, 13, 13, 13, 15, 11, 10, 9, 6, 0, 5, 0, 2, 16, 17, 7, 0, 2, 6, 2, 12, 11, 10, 17, 14, 12, 18, 9, 9, 15, 10, 2, 7, 7, 13];
    //arr=[1, 2, 1, 9, 9, 9, 2, 7, 1, 7, 3, 1, 3, 4, 5, 6, 8, 10, 9, 10, 6, 2, 1, 3, 6, 1, 8, 7, 4, 5, 2, 4, 7, 3, 1, 10, 7, 8, 7, 7, 4, 10, 10, 2, 4, 3, 4, 7, 6, 10, 1, 8, 4, 3, 7, 4, 9, 7, 3, 10, 3, 3, 2, 10, 8, 2, 7, 6, 1, 3, 3, 6, 7, 8, 8, 3, 4, 8, 8, 4, 9, 5, 7, 1, 2, 6, 2, 9, 8, 5, 2, 4, 1, 5, 8, 8, 8, 7, 10, 1, 6, 2, 5, 10, 2, 6, 8, 1, 9, 3, 1, 2, 1, 6, 6, 3, 9, 7, 3, 4, 10, 4, 2, 4, 4, 2, 6, 6, 4, 2, 6, 5, 4, 10, 9, 5, 7, 2, 2, 10, 7, 9, 2, 8, 6, 4, 7, 6, 1, 3, 7, 7, 1, 5, 2, 8, 8, 2, 7, 1, 1, 1, 10, 4, 5, 9, 5, 10, 3, 3, 9, 5, 8, 8, 9, 8, 9, 2, 9, 7, 7, 5, 6, 9, 9, 4, 1, 8, 9, 2, 4, 10, 9, 8, 6, 5, 1, 6, 2, 5, 1, 5, 5, 9, 8, 6, 4, 3, 3, 7, 9, 2, 3, 1, 9, 7, 3, 4, 10, 10, 5, 3, 9, 10, 7, 3, 5, 6, 1, 3, 4, 4, 3, 9, 7, 8, 9, 2, 7, 10, 8, 9, 1, 7, 5, 10, 6, 3, 5, 8, 1, 8, 6, 1, 4, 10, 3, 10, 5, 1, 6, 10, 6, 2, 7, 8, 8, 2, 2, 6, 4, 10, 2, 1, 8, 6, 2, 1, 3, 1, 6, 6, 4, 4, 4, 2, 9, 8, 10, 7, 8, 1, 2, 1, 1, 8, 7, 2, 5, 6, 10, 10, 7, 2, 4, 10, 4, 9, 8, 9, 6, 5, 5, 1, 10, 5, 10, 1, 8, 5, 1, 4, 9, 9, 3, 6, 1, 6, 1, 5, 8, 2, 8, 6, 10, 7, 1, 8, 10, 1, 9, 10, 3, 2, 8, 10, 3, 1, 4, 1, 3, 10, 2, 3, 5, 9, 6, 10, 4, 4, 5, 1, 5, 4, 1, 7, 1, 9, 8, 5, 4, 10, 8, 6, 10, 3, 2, 1, 8, 4, 6, 4, 8, 4, 3, 5, 5, 4, 4, 6, 6, 3, 4, 10, 8, 6, 8, 9, 8, 7, 5, 10, 1, 2, 10, 9, 7, 8, 3, 2, 1, 8, 5, 4, 8, 8, 1, 8, 10, 4, 9, 8, 5, 9, 7, 7, 9, 2, 1, 8, 10, 8, 6, 9, 7, 5, 9, 4, 6, 1, 3, 2, 7, 1, 5, 9, 2, 6, 1, 6, 5, 5, 3, 4, 5, 6, 6, 3, 3, 8, 10, 9, 4, 3, 8, 6, 1, 5, 1, 1, 1, 3, 5, 5, 6, 2, 10, 7, 2, 1, 7, 6, 1, 5, 6, 10, 10, 6, 3, 5, 1, 3, 4, 4, 8, 3, 8, 7, 5, 4, 7, 6, 8, 2, 2, 5, 3, 5, 9, 10, 9, 2, 2, 5, 8, 9, 3, 2, 7, 4, 5, 1, 8, 3, 8, 1, 5, 10, 2, 9, 10, 9, 8, 6, 3, 1, 1, 4, 1, 10, 5, 2, 10, 8, 1, 4, 4, 3, 9, 9, 7, 2, 9, 8, 7, 6, 6, 1, 3, 3, 10, 1, 7, 4, 8, 1, 6, 7, 2, 6, 9, 7, 5, 2, 7, 8, 6, 1, 2, 1, 4, 5, 10, 9, 4, 1, 9, 8, 6, 4, 5, 3, 3, 8, 3, 5, 3, 8, 5, 10, 3, 8, 9, 4, 6, 8, 7, 2, 7, 3, 8, 9, 2, 3, 5, 10, 10, 7, 4, 9, 3, 4, 1, 5, 8, 2, 1, 6, 5, 3, 6, 1, 4, 4, 7, 9, 6, 7, 8, 3, 3, 7, 7, 1, 2, 3, 3, 4, 1, 2, 5, 4, 7, 4, 6, 4, 5, 6, 4, 9, 9, 9, 2, 3, 5, 2, 5, 3, 3, 3, 4, 8, 3, 5, 5, 8, 10, 7, 6, 5, 6, 10, 10, 9, 4, 4, 10, 7, 7, 1, 5, 9, 8, 5, 3, 1, 8, 3, 9, 10, 1, 8, 7, 2, 9, 4, 7, 6, 4, 9, 7, 8, 10, 9, 6, 4, 9, 6, 9, 10, 1, 4, 5, 8, 6, 2, 4, 10, 8, 6, 4, 4, 10, 10, 5, 8, 9, 1, 1, 2, 1, 1, 4, 7, 5, 4, 5, 1, 5, 5, 5, 10, 9, 1, 1, 10, 3, 4, 10, 3, 3, 5, 9, 1, 5, 3, 4, 3, 4, 5, 2, 9, 6, 9, 3, 10, 10, 3, 8, 10, 8, 3, 2, 6, 8, 4, 2, 9, 7, 1, 3, 2, 6, 3, 3, 1, 8, 4, 4, 1, 5, 8, 5, 6, 7, 9, 1, 9, 9, 8, 2, 1, 6, 2, 4, 5, 2, 7, 1, 9, 9, 10, 6, 6, 2, 3, 7, 3, 9, 10, 3, 1, 3, 8, 3, 4, 8, 4, 8, 2, 6, 8, 6, 1, 4, 1, 7, 4, 5, 8, 6, 8, 8, 9, 2, 1, 7, 6, 6, 9, 3, 9, 1, 10, 4, 6, 7, 1, 10, 1, 9, 2, 7, 4, 1, 2, 2, 6, 9, 1, 9, 2, 8, 9, 10, 2, 7, 7, 8, 8, 6, 3, 1, 3, 10, 3, 8, 6, 6, 6, 9, 8, 4, 3, 10, 6, 10, 10, 1, 7, 8, 3, 4, 8, 9, 4, 8, 6, 7, 4, 9, 10, 10, 1, 5, 6, 6, 5, 6, 2, 9, 7, 8, 9, 7, 8, 7, 9, 8, 6, 1, 10, 6, 1, 4, 6, 6, 7, 2, 4, 2, 3, 2, 5, 9, 9, 9, 2, 5, 6, 1, 6, 1, 10, 10, 7, 10, 8, 7, 6, 10, 7, 10, 8, 7, 2, 6, 5, 7, 10, 2, 9, 6, 1, 9, 5, 8, 10, 8, 2, 9, 1, 4, 7, 5, 4, 3, 7, 6, 2];


        for (var i = 0; i < 1000; i++) {
            switch (arr[i]) {
                case 1: {
                    var url = "http://localhost:9200/dblpvertexes/_search?q=vType:author AND AUTHOR_NAME:" + authorName1[authorNameCnt++] + "&request_cache=true";
                    $.ajax({
                        dataType: "json",
                        url: url,
                        async: false,
                        success: function (data) {
                            totalTime = totalTime + parseInt(data.took);
                        }
                    });
                    break;
                }
                case 2: {
                    var query = '{\n' +
                        '            "size":0,\n' +
                        '            "query":{\n' +
                        '                "query_string":{\n' +
                        '                    "query":' + "\"" + randomVal1[randomValArr[aggregationValCnt++]] + "\"" + '\n' +
                        '                }\n' +
                        '            },\n' +
                        '            "aggs":{\n' +
                        '                "paperByYear":{\n' +
                        '                    "terms":{\n' +
                        '                        "field":"Year.keyword",\n' +
                        '                        "size":2000\n' +
                        '                    }\n' +
                        '                }\n' +
                        '            }\n' +
                        '        }';
                    $.ajax({
                        type: "POST",
                        url: 'http://localhost:9200/dblpvertexes/_search?request_cache=true',
                        contentType: 'application/json',
                        dataType: "json",
                        async: false,
                        data: query,
                        success: function (data) {
                            totalTime = totalTime + parseInt(data.took);
                        }
                    });
                    break;
                }
                case 3: {
                    var url = "http://localhost:9200/dblprelation_authorship/_search?q=AUTHOR_ID:" + authorID1[authorIdCnt++] + "&request_cache=true";
                    $.ajax({
                        dataType: "json",
                        url: url,
                        async: false,
                        success: function (data) {
                            totalTime = totalTime + parseInt(data.took);
                        }
                    });
                    break;
                }
                case 4: {
                    var url = "http://localhost:9200/dblprelation_coauthorship/_search?q=COAUTHOR_ID:" + coAuthorID1[coAuthorIDCnt++] + "&request_cache=true";
                    $.ajax({
                        dataType: "json",
                        url: url,
                        async: false,
                        success: function (data) {
                            totalTime = totalTime + parseInt(data.took);
                        }
                    });
                    break;
                }
                case 5: {
                    var url = "http://localhost:9200/dblprelation_journalofpaper/_search?q=JOURNAL_ID:" + journalID1[arr2[journalIDCnt++]] + "&request_cache=true";
                    $.ajax({
                        dataType: "json",
                        url: url,
                        async: false,
                        success: function (data) {
                            totalTime = totalTime + parseInt(data.took);
                        }
                    });
                    break;
                }
                case 6: {
                    var url = "http://localhost:9200/dblprelation_venueofpaper/_search?q=VENUE_ID:" + venueID1[venueIDCnt++] + "&request_cache=true";
                    $.ajax({
                        dataType: "json",
                        url: url,
                        async: false,
                        success: function (data) {
                            totalTime = totalTime + parseInt(data.took);
                        }
                    });
                    break;
                }
                case 7: {
                    var url = "http://localhost:9200/dblprelation_citation/_search?q=REFERENCE_ID:" + referenceID1[referenceIDCnt++] + "&request_cache=true";
                    $.ajax({
                        dataType: "json",
                        url: url,
                        async: false,
                        success: function (data) {
                            totalTime = totalTime + parseInt(data.took);
                        }
                    });
                    break;
                }
                case 8: {
                    var url = "http://localhost:9200/dblprelation_citation/_search?q=PAPER_ID:" + paperID1[paperIDCnt++] + "&request_cache=true";
                    $.ajax({
                        dataType: "json",
                        url: url,
                        async: false,
                        success: function (data) {
                            totalTime = totalTime + parseInt(data.took);
                        }
                    });
                    break;
                }
                case 9: {
                    //var url='{"size":0,"query": {"bool":{"must": [{"match":{"REFERENCE_ID.keyword:'+aggregationVal[aggregationValCnt++]+'}}]}},"aggs":{"paperByYear":{"terms":{"field":"REFERENCE_ID.keyword","size":10}}}';

                    /*"http://localhost:9200/dblpvertexes/_search?request_cache=true\n" +
                    "{\"size\":0,\n" +
                    "  \"query\": {\n" +
                    "    \"bool\": {\n" +
                    "      \"must\": [\n" +
                    "        {\n" +
                    "          \"match\": {\n" +
                    "REFERENCE_ID.keyword:" +aggregationVal[aggregationValCnt++]+
                    "          }\n" +
                    "        }\n" +
                    "      ]\n" +
                    "    }\n" +
                    "  },\n" +
                    "   \"aggs\":{\n" +
                    "                \"paperByYear\":{\n" +
                    "                    \"terms\":{\n" +
                    "                        \"field\":\"REFERENCE_ID.keyword\",\n" +
                    "                        \"size\":10\n" +
                    "                    }\n" +
                    "                }\n" +
                    "            }\n" +
                    "}";*/
                    /* $.ajax({
                         dataType: "json",
                         url: url,
                         async: false,
                         success: function (data) {
                             totalTime =totalTime+ parseInt(data.took);
                         }
                     });*/
                }
                case 10: {

                    var url = "http://localhost:9200/dblpvertexes/_search?q=" + randomVal1[randomValArr[randomValCnt++]] + "&request_cache=true";
                    $.ajax({
                        dataType: "json",
                        url: url,
                        async: false,
                        success: function (data) {
                            totalTime = totalTime + parseInt(data.took);
                        }
                    });
                    break;
                }
            }
        }
        Finaltime=Finaltime+totalTime;
    }
    document.getElementById("totalTimewcaching").innerText=Finaltime;
    var clearUrl="http://localhost:9200/dblpvertexes,dblprelation_authorship,dblprelation_citation,dblprelation_coauthorship,dblprelation_journalofpaper,dblprelation_venueofpaper/_cache/clear?request=true";
    $.ajax({
        dataType: "json",
        url: clearUrl,
        async: false,
        success: function (data) {
            console.log(data._shards.successful);
        }
    });
}

function executeQueriresWithSelectedCaching() {
    var arr=new Array();
    var arr2=new Array();
    var Finaltime=0;
    /* var clearUrl="http://localhost:9200/dblpvertexes,dblprelation_authorship,dblprelation_citation,dblprelation_coauthorship,dblprelation_journalofpaper,dblprelation_venueofpaper/_cache/clear?request=true";
     $.ajax({
         dataType: "json",
         url: clearUrl,
         async: false,
         success: function (data) {
             console.log(data._shards.successful);
         }
     });*/
    for(var loopcnt=0; loopcnt<10;loopcnt++) {
    totalTime=0,authorNameCnt=0,authorIdCnt=0,randomValCnt=0,coAuthorIDCnt=0,journalIDCnt=0,venueIDCnt=0,referenceIDCnt=0,paperIDCnt=0,aggregationValCnt=0;
    randomValArr=[5, 6, 10, 5, 9, 1, 5, 5, 8, 1, 15, 12, 18, 10, 8, 12, 7, 16, 0, 13, 17, 17, 5, 2, 3, 6, 13, 10, 7, 4, 9, 14, 0, 14, 3, 1, 16, 8, 14, 4, 16, 3, 10, 9, 5, 11, 4, 18, 17, 6, 10, 17, 12, 17, 1, 10, 16, 11, 9, 18, 16, 5, 12, 4, 11, 9, 16, 7, 1, 2, 13, 18, 0, 0, 15, 7, 5, 13, 13, 7, 8, 0, 10, 6, 11, 13, 10, 10, 8, 7, 14, 11, 11, 7, 1, 11, 6, 11, 0, 18];
    arr=[3, 8, 4, 9, 4, 1, 5, 9, 4, 8, 1, 3, 1, 10, 1, 4, 2, 7, 6, 7, 7, 2, 9, 6, 9, 10, 1, 9, 1, 8, 1, 2, 2, 5, 9, 2, 5, 4, 1, 10, 1, 10, 3, 7, 1, 3, 5, 2, 4, 3, 10, 10, 10, 5, 2, 8, 1, 5, 3, 9, 6, 10, 5, 5, 10, 10, 10, 4, 5, 6, 3, 8, 9, 3, 3, 2, 6, 3, 4, 2, 3, 5, 4, 4, 7, 5, 1, 1, 3, 1, 1, 3, 2, 2, 6, 5, 1, 5, 2, 7, 1, 1, 7, 9, 3, 7, 10, 7, 9, 4, 10, 2, 7, 1, 4, 5, 4, 8, 9, 6, 1, 1, 10, 6, 9, 1, 3, 1, 1, 5, 10, 6, 9, 5, 1, 8, 6, 7, 8, 10, 5, 9, 7, 1, 3, 4, 3, 2, 5, 7, 4, 7, 1, 4, 5, 3, 7, 7, 4, 5, 8, 10, 4, 6, 2, 9, 6, 4, 10, 4, 7, 6, 2, 8, 9, 7, 5, 3, 2, 9, 3, 10, 6, 10, 9, 4, 10, 5, 5, 2, 8, 7, 2, 1, 3, 9, 2, 5, 3, 4, 9, 3, 10, 8, 1, 5, 6, 6, 2, 10, 3, 8, 9, 3, 6, 4, 3, 6, 3, 2, 8, 6, 7, 7, 7, 10, 8, 4, 8, 5, 8, 6, 3, 4, 6, 1, 4, 10, 10, 5, 6, 8, 8, 5, 1, 2, 4, 3, 9, 3, 3, 9, 2, 7, 3, 8, 4, 1, 3, 10, 2, 5, 1, 7, 6, 8, 5, 2, 10, 4, 5, 2, 5, 4, 6, 9, 1, 3, 4, 5, 1, 8, 1, 9, 3, 4, 7, 10, 9, 7, 2, 2, 1, 7, 9, 4, 5, 5, 8, 3, 7, 3, 3, 7, 5, 5, 4, 1, 8, 7, 9, 10, 9, 5, 1, 4, 8, 7, 10, 9, 7, 2, 6, 5, 8, 2, 4, 5, 9, 3, 2, 4, 6, 1, 3, 9, 5, 2, 1, 10, 10, 2, 4, 3, 1, 1, 4, 9, 9, 4, 8, 4, 4, 9, 4, 7, 6, 8, 7, 10, 8, 4, 5, 8, 8, 6, 9, 2, 5, 10, 3, 8, 9, 2, 1, 9, 9, 4, 6, 5, 8, 5, 1, 3, 3, 2, 4, 4, 8, 7, 9, 4, 3, 7, 10, 4, 8, 4, 5, 10, 4, 4, 2, 2, 8, 7, 2, 3, 10, 6, 10, 9, 2, 4, 4, 8, 4, 6, 5, 5, 7, 1, 9, 10, 3, 10, 3, 5, 9, 10, 8, 1, 5, 4, 1, 5, 1, 9, 10, 7, 4, 7, 3, 1, 7, 6, 7, 6, 4, 4, 8, 3, 4, 4, 8, 6, 2, 1, 10, 8, 8, 10, 7, 3, 2, 1, 1, 3, 1, 10, 3, 4, 4, 8, 5, 3, 6, 1, 10, 7, 10, 6, 7, 5, 6, 3, 3, 6, 5, 3, 7, 5, 8, 6, 2, 6, 6, 2, 4, 9, 2, 9, 2, 5, 6, 5, 4, 6, 3, 9, 6, 3, 4, 5, 6, 4, 2, 8, 7, 10, 6, 1, 8, 3, 6, 10, 5, 6, 2, 6, 2, 2, 3, 2, 5, 4, 5, 9, 9, 8, 2, 10, 8, 3, 6, 4, 4, 6, 3, 7, 1, 8, 8, 1, 9, 7, 5, 6, 8, 2, 4, 10, 5, 9, 10, 3, 4, 1, 3, 2, 3, 1, 4, 10, 4, 7, 4, 2, 8, 10, 10, 4, 2, 4, 3, 3, 1, 7, 5, 4, 2, 7, 10, 10, 1, 8, 8, 4, 6, 1, 2, 8, 10, 7, 1, 8, 4, 7, 8, 10, 8, 6, 1, 1, 2, 10, 6, 8, 5, 7, 1, 2, 2, 4, 6, 3, 2, 7, 2, 3, 10, 1, 4, 2, 3, 3, 1, 10, 3, 5, 2, 1, 2, 9, 7, 1, 10, 1, 5, 8, 5, 8, 7, 9, 4, 5, 6, 6, 7, 8, 1, 5, 4, 6, 8, 8, 3, 4, 1, 10, 9, 8, 2, 8, 1, 10, 4, 4, 4, 3, 10, 5, 1, 8, 2, 7, 8, 4, 3, 8, 7, 2, 10, 10, 8, 5, 1, 7, 10, 5, 6, 2, 8, 4, 1, 3, 4, 1, 7, 4, 2, 1, 5, 4, 7, 5, 6, 8, 3, 9, 10, 8, 10, 9, 2, 6, 3, 1, 3, 9, 10, 2, 5, 4, 5, 1, 8, 5, 6, 9, 2, 9, 5, 5, 2, 9, 3, 1, 6, 7, 8, 6, 3, 10, 2, 9, 8, 3, 5, 3, 10, 7, 2, 4, 5, 8, 9, 10, 1, 5, 5, 1, 2, 9, 10, 7, 7, 5, 5, 2, 4, 10, 3, 2, 3, 5, 1, 3, 9, 3, 5, 7, 7, 8, 1, 4, 8, 5, 4, 9, 5, 7, 5, 4, 7, 2, 7, 3, 6, 6, 10, 10, 10, 1, 6, 7, 7, 2, 3, 6, 4, 7, 3, 1, 9, 8, 7, 6, 9, 5, 6, 1, 7, 2, 9, 4, 9, 4, 2, 3, 3, 7, 8, 1, 7, 6, 5, 1, 3, 8, 10, 5, 9, 3, 5, 4, 10, 8, 3, 3, 2, 8, 6, 10, 3, 4, 6, 10, 1, 8, 2, 10, 7, 7, 10, 1, 5, 7, 7, 3, 2, 4, 1, 2, 3, 8, 6, 6, 10, 3, 3, 8, 6, 8, 7, 8, 8, 1, 1, 1, 2, 10, 1, 1, 10, 8, 5, 10, 6, 4, 5, 3, 10, 2, 9, 2, 10, 1, 8, 8, 9, 10, 10, 1, 8, 1, 3, 5, 10, 3, 4, 3, 5, 8, 1, 10, 1, 1, 5, 6, 5, 7, 1, 6, 9, 1, 4, 4, 9, 1, 8, 2, 4, 4, 5, 3, 9, 4, 4, 5, 1, 10, 7, 5, 3, 5, 3, 5, 2, 5, 9, 7, 1, 2, 5, 2, 10, 4, 2, 6, 2, 8, 3, 10, 7, 2, 6, 9, 5, 5, 1, 7, 3, 9, 7, 4, 3, 9, 6, 6];
    arr2=[3, 23, 27, 29, 15, 27, 24, 6, 5, 19, 13, 21, 23, 28, 8, 16, 16, 20, 28, 24, 23, 10, 25, 4, 10, 7, 25, 5, 25, 1, 15, 14, 25, 15, 13, 14, 20, 7, 1, 3, 12, 19, 4, 28, 19, 9, 16, 17, 22, 8, 21, 25, 18, 16, 18, 7, 1, 15, 0, 1, 0, 17, 17, 15, 18, 25, 14, 23, 9, 28, 16, 25, 11, 8, 4, 27, 22, 7, 9, 4, 12, 27, 16, 0, 14, 13, 17, 2, 5, 19, 18, 18, 16, 15, 6, 26, 15, 7, 0, 10, 28, 13, 25, 26, 21, 6, 29, 10, 24, 15, 21];
    //randomValArr=[11, 10, 4, 13, 15, 1, 4, 5, 10, 9, 15, 0, 6, 8, 6, 9, 6, 17, 16, 14, 5, 4, 13, 0, 14, 2, 6, 2, 4, 7, 3, 15, 15, 8, 17, 1, 10, 0, 1, 3, 13, 1, 3, 15, 13, 18, 12, 3, 16, 7, 14, 4, 4, 4, 16, 4, 16, 0, 11, 13, 6, 10, 1, 17, 10, 12, 5, 4, 6, 6, 15, 15, 14, 2, 17, 0, 2, 3, 8, 8, 8, 5, 14, 10, 8, 7, 11, 10, 15, 11, 0, 11, 17, 4, 2, 10, 2, 11, 17, 9];
    //arr=[10, 10, 8, 1, 2, 4, 1, 1, 8, 3, 10, 5, 10, 5, 9, 3, 5, 2, 9, 1, 8, 4, 4, 4, 4, 6, 7, 2, 6, 6, 6, 5, 8, 7, 1, 1, 3, 5, 9, 9, 8, 4, 10, 9, 6, 9, 7, 7, 3, 3, 10, 8, 4, 9, 2, 4, 4, 1, 8, 4, 3, 10, 7, 10, 4, 3, 3, 7, 3, 5, 3, 3, 10, 5, 1, 7, 4, 4, 5, 7, 5, 3, 9, 10, 2, 9, 3, 7, 3, 8, 6, 9, 2, 5, 2, 1, 7, 1, 10, 3, 2, 2, 8, 3, 6, 3, 3, 7, 7, 1, 2, 7, 5, 9, 8, 8, 4, 4, 9, 9, 5, 7, 7, 9, 3, 10, 1, 3, 10, 10, 4, 6, 1, 4, 2, 2, 2, 6, 2, 9, 9, 10, 8, 8, 10, 6, 9, 5, 1, 4, 2, 3, 9, 1, 3, 2, 9, 10, 8, 7, 8, 6, 7, 3, 6, 3, 3, 3, 8, 2, 10, 9, 8, 1, 4, 7, 3, 3, 8, 6, 7, 3, 3, 8, 10, 8, 8, 1, 2, 9, 6, 1, 1, 8, 9, 6, 5, 3, 6, 4, 9, 6, 9, 7, 2, 10, 1, 1, 10, 10, 8, 2, 2, 5, 1, 1, 6, 3, 2, 6, 5, 10, 5, 9, 1, 2, 3, 4, 2, 10, 5, 5, 10, 2, 3, 5, 7, 8, 5, 6, 8, 5, 9, 8, 3, 8, 8, 8, 8, 1, 1, 9, 6, 6, 2, 6, 8, 5, 1, 9, 9, 4, 9, 5, 8, 8, 8, 7, 10, 1, 9, 6, 8, 7, 2, 8, 2, 6, 2, 9, 1, 9, 6, 5, 9, 7, 6, 7, 7, 10, 1, 7, 7, 1, 7, 7, 7, 1, 9, 5, 4, 4, 5, 8, 3, 5, 4, 9, 7, 5, 5, 4, 7, 3, 9, 5, 5, 9, 4, 9, 4, 5, 1, 1, 7, 5, 9, 3, 6, 4, 4, 9, 5, 3, 5, 1, 4, 9, 2, 10, 2, 9, 4, 8, 9, 10, 8, 1, 4, 8, 1, 8, 1, 5, 3, 1, 6, 8, 10, 1, 10, 7, 9, 7, 5, 8, 10, 9, 7, 5, 3, 6, 8, 7, 10, 8, 2, 9, 3, 9, 4, 8, 5, 2, 3, 6, 4, 10, 8, 3, 8, 8, 5, 4, 1, 6, 2, 4, 1, 6, 6, 10, 3, 9, 7, 9, 3, 5, 3, 5, 6, 4, 1, 2, 1, 8, 9, 8, 2, 7, 1, 8, 1, 5, 3, 6, 5, 4, 10, 1, 8, 5, 10, 9, 9, 9, 3, 10, 9, 5, 5, 6, 8, 6, 1, 7, 4, 1, 10, 8, 7, 8, 3, 6, 8, 10, 1, 3, 3, 1, 8, 2, 4, 3, 4, 1, 1, 6, 9, 10, 3, 8, 6, 6, 7, 8, 3, 5, 4, 9, 7, 1, 1, 9, 8, 5, 4, 10, 7, 5, 2, 2, 10, 1, 6, 4, 7, 10, 7, 6, 5, 4, 5, 4, 2, 2, 3, 7, 10, 5, 4, 2, 2, 10, 2, 6, 6, 10, 5, 4, 1, 1, 10, 8, 4, 10, 3, 1, 6, 7, 8, 6, 3, 10, 6, 5, 2, 6, 2, 2, 8, 2, 4, 5, 4, 2, 9, 2, 9, 1, 8, 2, 9, 9, 4, 5, 1, 5, 2, 1, 3, 4, 10, 4, 2, 3, 1, 1, 1, 6, 4, 10, 5, 8, 7, 7, 7, 10, 5, 5, 2, 5, 6, 3, 7, 10, 10, 5, 3, 5, 2, 8, 7, 2, 4, 3, 2, 10, 10, 1, 2, 4, 10, 2, 9, 4, 7, 4, 8, 9, 4, 7, 5, 1, 4, 3, 10, 2, 7, 8, 6, 4, 7, 6, 10, 1, 9, 7, 5, 5, 8, 6, 6, 6, 4, 3, 2, 6, 2, 5, 10, 4, 4, 6, 8, 3, 9, 4, 4, 6, 4, 3, 9, 9, 8, 6, 3, 2, 8, 2, 4, 8, 8, 4, 1, 3, 4, 6, 1, 10, 7, 7, 9, 10, 3, 8, 2, 1, 10, 2, 3, 6, 3, 1, 3, 5, 6, 1, 1, 7, 8, 3, 7, 1, 7, 9, 9, 1, 2, 10, 3, 3, 10, 6, 2, 4, 10, 3, 7, 9, 9, 5, 8, 1, 10, 4, 3, 8, 8, 3, 4, 10, 7, 8, 6, 4, 10, 6, 5, 8, 8, 7, 9, 7, 4, 10, 10, 1, 9, 4, 5, 5, 3, 5, 4, 6, 8, 4, 3, 6, 7, 7, 6, 2, 2, 6, 1, 5, 9, 8, 9, 3, 1, 10, 7, 7, 5, 6, 10, 5, 9, 7, 5, 9, 9, 1, 7, 10, 7, 2, 10, 3, 5, 2, 3, 2, 7, 2, 3, 4, 3, 6, 3, 10, 5, 9, 6, 9, 7, 7, 6, 5, 6, 10, 9, 1, 2, 6, 8, 4, 10, 10, 6, 9, 3, 8, 1, 9, 2, 6, 6, 7, 4, 2, 7, 4, 8, 2, 1, 7, 3, 7, 1, 3, 8, 6, 3, 6, 3, 2, 10, 1, 6, 4, 6, 10, 5, 7, 5, 2, 1, 8, 2, 10, 6, 10, 6, 3, 4, 6, 8, 1, 7, 5, 2, 5, 10, 3, 6, 5, 2, 3, 1, 10, 6, 4, 10, 4, 7, 6, 2, 7, 2, 2, 4, 4, 8, 5, 5, 8, 3, 4, 3, 7, 8, 10, 3, 2, 5, 1, 10, 6, 8, 7, 8, 10, 5, 10, 10, 9, 1, 6, 9, 6, 2, 7, 8, 6, 1, 8, 9, 1, 7, 8, 1, 3, 10, 10, 3, 6, 3, 4, 7, 8, 4, 8, 4, 2, 7, 10, 5, 3, 1, 7, 6, 2, 1, 7, 5, 2, 9, 7, 10, 9, 10, 4, 4, 8, 2, 1, 1, 9, 1, 8, 4, 6, 9, 10, 4, 8, 7, 1, 3, 6, 6, 2, 6, 6, 3, 3, 9, 9, 3, 2, 5, 5, 9, 1, 8, 6, 9, 4, 3, 8, 1, 5, 9, 8, 8, 2];


        for (var i = 0; i < 1000; i++) {
            switch (arr[i]) {
                case 1: {
                    var url = "http://localhost:9200/dblpvertexes/_search?q=vType:author AND AUTHOR_NAME:" + authorName2[authorNameCnt++] + "&request_cache=false";
                    $.ajax({
                        dataType: "json",
                        url: url,
                        async: false,
                        success: function (data) {
                            totalTime = totalTime + parseInt(data.took);
                        }
                    });
                }
                case 2: {
                    var query = '{\n' +
                        '            "size":0,\n' +
                        '            "query":{\n' +
                        '                "query_string":{\n' +
                        '                    "query":' + "\"" + randomVal2[randomValArr[aggregationValCnt++]] + "\"" + '\n' +
                        '                }\n' +
                        '            },\n' +
                        '            "aggs":{\n' +
                        '                "paperByYear":{\n' +
                        '                    "terms":{\n' +
                        '                        "field":"Year.keyword",\n' +
                        '                        "size":2000\n' +
                        '                    }\n' +
                        '                }\n' +
                        '            }\n' +
                        '        }';
                    $.ajax({
                        type: "POST",
                        url: 'http://localhost:9200/dblpvertexes/_search?request_cache=true',
                        contentType: 'application/json',
                        dataType: "json",
                        async: false,
                        data: query,
                        success: function (data) {
                            totalTime = totalTime + parseInt(data.took);
                        }
                    });
                    break;
                }
                case 3: {
                    var url = "http://localhost:9200/dblprelation_authorship/_search?q=AUTHOR_ID:" + authorID2[authorIdCnt++] + "&request_cache=false";
                    $.ajax({
                        dataType: "json",
                        url: url,
                        async: false,
                        success: function (data) {
                            totalTime = totalTime + parseInt(data.took);
                        }
                    });
                    break;
                }
                case 4: {
                    var url = "http://localhost:9200/dblprelation_coauthorship/_search?q=COAUTHOR_ID:" + coAuthorID2[coAuthorIDCnt++] + "&request_cache=false";
                    $.ajax({
                        dataType: "json",
                        url: url,
                        async: false,
                        success: function (data) {
                            totalTime = totalTime + parseInt(data.took);
                        }
                    });
                    break;
                }
                case 5: {
                    var url = "http://localhost:9200/dblprelation_journalofpaper/_search?q=JOURNAL_ID:" + journalID2[arr2[journalIDCnt++]] + "&request_cache=true";
                    $.ajax({
                        dataType: "json",
                        url: url,
                        async: false,
                        success: function (data) {
                            totalTime = totalTime + parseInt(data.took);
                        }
                    });
                    break;
                }
                case 6: {
                    var url = "http://localhost:9200/dblprelation_venueofpaper/_search?q=VENUE_ID:" + venueID2[venueIDCnt++] + "&request_cache=false";
                    $.ajax({
                        dataType: "json",
                        url: url,
                        async: false,
                        success: function (data) {
                            totalTime = totalTime + parseInt(data.took);
                        }
                    });
                    break;
                }
                case 7: {
                    var url = "http://localhost:9200/dblprelation_citation/_search?q=REFERENCE_ID:" + referenceID2[referenceIDCnt++] + "&request_cache=false";
                    $.ajax({
                        dataType: "json",
                        url: url,
                        async: false,
                        success: function (data) {
                            totalTime = totalTime + parseInt(data.took);
                        }
                    });
                    break;
                }
                case 8: {
                    var url = "http://localhost:9200/dblprelation_citation/_search?q=PAPER_ID:" + paperID2[paperIDCnt++] + "&request_cache=false";
                    $.ajax({
                        dataType: "json",
                        url: url,
                        async: false,
                        success: function (data) {
                            totalTime = totalTime + parseInt(data.took);
                        }
                    });
                    break;
                }
                case 9: {
                    /* var url="http://localhost:9200/dblpvertexes/_search?request_cache=false\n" +
                         "{\"size\":0,\n" +
                         "  \"query\": {\n" +
                         "    \"bool\": {\n" +
                         "      \"must\": [\n" +
                         "        {\n" +
                         "          \"match\": {\n" +
                         "REFERENCE_ID.keyword:" +aggregationVal[aggregationValCnt++]+
                         "          }\n" +
                         "        }\n" +
                         "      ]\n" +
                         "    }\n" +
                         "  },\n" +
                         "   \"aggs\":{\n" +
                         "                \"paperByYear\":{\n" +
                         "                    \"terms\":{\n" +
                         "                        \"field\":\"REFERENCE_ID.keyword\",\n" +
                         "                        \"size\":10\n" +
                         "                    }\n" +
                         "                }\n" +
                         "            }\n" +
                         "}";*/
                    /* var url='{"size":0,"query": {"bool":{"must": [{"match":{"REFERENCE_ID.keyword:'+aggregationVal[aggregationValCnt++]+'}}]}},"aggs":{"paperByYear":{"terms":{"field":"REFERENCE_ID.keyword","size":10}}}';
                      $.ajax({
                          dataType: "json",
                          url: url,
                          async: false,
                          success: function (data) {
                              totalTime =totalTime+ parseInt(data.took);
                          }
                      });*/
                }
                case 10: {

                    var url = "http://localhost:9200/dblpvertexes/_search?q=" + randomVal2[randomValArr[randomValCnt++]] + "&request_cache=false";
                    $.ajax({
                        dataType: "json",
                        url: url,
                        async: false,
                        success: function (data) {
                            totalTime = totalTime + parseInt(data.took);
                        }
                    });
                    break;
                }
            }
        }
        Finaltime=Finaltime+totalTime;
    }
    document.getElementById("totalTimewSelectedcaching").innerText=Finaltime;
    var clearUrl="http://localhost:9200/dblpvertexes,dblprelation_authorship,dblprelation_citation,dblprelation_coauthorship,dblprelation_journalofpaper,dblprelation_venueofpaper/_cache/clear?request=true";
    $.ajax({
        dataType: "json",
        url: clearUrl,
        async: false,
        success: function (data) {
            console.log(data._shards.successful);
        }
    });
}
