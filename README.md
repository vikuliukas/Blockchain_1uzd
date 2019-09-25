# Blockchain_1uzd
VU Blockchain pratybų pirma užduotis

# Užduoties tikslas
Sukurti veikiantį hash algoritmą, kuris atitiktu šiuos reikalavimus:
1. Maišos funkcijos įėjimas (angl. input) gali būti bet kokio dydžio simbolių eilutė (angl. string).
2. Maišos funkcijos išėjimas (angl. output) visuomet yra to paties fiksuoto dydžio rezultatas.
3. Maišos funkcija yra deterministinė, t. y., tam pačiam įvedimui (angl. input'ui) išvedimas (angl. output'as) visuomet yra tas pats.
4. Maišos funkcijos reikšmė/kodas (hash‘as) bet kokiai input reikšmėi yra apskaičiuojamas nesunkiai - efektyviai.
5. Iš funkcijos rezultato (output'o) praktiškai neįmanoma atgaminti įvedimo (input'o).
6. Praktiškai neįmanoma surasti tokių dviejų skirtingų argumentų (input'ų), kad jiems gautume tą patį hash'ą, t. y.,: m1 != m2, bet h(m1) = h(m2).
7. Bent minimaliai pakeitus įvedimą, pvz.vietoj "Lietuva" pateikus "lietuva", maišos funkcijos rezultatas-kodas turi skirtis.

# Mano Hash'o analizė

1. Tekstą, kurį naudosimę hash kurimui galima arba įvesti ranka arba nuskaityti nuo failo, kuriame nėra limito.

## Teksto nuskaitymas nuo failo
```c++
void inputfromfile( hash&current, std::string &filename ){
    std::ifstream f(filename);
    if(f.fail()){
        throw std::exception();
    }
    std::string content( (std::istreambuf_iterator<char>(f) ),(std::istreambuf_iterator<char>()    ) );
    current.setinput(content);
}
```

2. Hash visada išvedamas vienodo dydžio. Kiekvieną teskto simbolį paverčiu į Unicode numerį ir suskirstau į keturias dalis (pasirinkau keturias dalis nes int tipo kintamasis nesutalpintu tokio skaičiaus, kuris paskui pavertus is dešimtainės skaičiavimo sistemos į šešioliktainę būtų 64 simbolių skaičius, tačiau vienas maksimalus dešimtainės systemos `unsigned long long int` yra lygus 16 šešioliktainės sistemos skaičių (FFFFFFFFFFFFFFFF), todėl 64 symboliams reikėjo 4 int kintamųjų). Jeigu failas neturi pakankamai raidžių, kad susidarytu 64 symbolių hash, funkcijoje `makelongerifneeded` failas papildomas, kad tam užtektu.

## Hash algoritmas
```c++
void hashalgorithm( hash&current ){
    std::string copy = current.getfixedinput();
    std::wstring_convert<std::codecvt_utf8_utf16<char32_t>,char32_t> converter;
    std::u32string input32 = converter.from_bytes(copy);
    unsigned long long int sum1 = 1;
    unsigned long long int sum2 = 1;
    unsigned long long int sum3 = 1;
    unsigned long long int sum4 = 1;
    int speperating = 1;
    int i = 1;
    for(char32_t &character : input32) {
        if(speperating == 1){
            sum1 = i * sum1 + character;
        }
        if(speperating == 2){
            sum2 = i * sum2 + character;
        }
        if(speperating == 3){
            sum3 = i * sum3 + character;
        }
        if(speperating == 4){
            sum4 = i * sum4 + character;
            speperating = 0;
        }
        speperating ++;
        i++;
    }

    std::string hex1 = inttohex (sum1);
    std::string hex2 = inttohex (sum2);
    std::string hex3 = inttohex (sum3);
    std::string hex4 = inttohex (sum4);
    std::string unshuffledhash = hex1 + hex2 + hex3 + hex4;
    std::string shuffledhash = hashshuffle( unshuffledhash, current );
    current.setoutput(shuffledhash);
}
```

## Teksto paplidymas (jei tokio reikia)
```c++
void makelongerifneeded( hash &current ){
    if(current.getinput().length() < 60){
        std::string filler (60, '!');
        std::string copy = current.getinput();
        filler.replace(0,copy.length(),copy);
        std::cout<<filler<<"\n";
        current.setfixedinput(filler);
    }
    else{
        std::string copy = current.getinput();
        current.setfixedinput(copy);
    }
}
```

3. Toks pats hash tokiam pačiam žodžiui. Mano hash algoritme nėra su textu nesusijusiu hash pakeitimu. Didelė dalis algoritmo priklauso nuo koks yra raidės unicode skaičius. 4 skirtingi int tipo kintamieji saugo as 4 raidės unicode skaičiaus sumas, tos sumos dar yra padauginamos priklausomai nuo kelintos raidės skaitmenį prideda sau. (pvz. 3 suma prieš prisidėdama 6 raidės skaitmenį pasidaugina iš 6, taip užtikrinama, kad raidės vieta tekste turi įtakos hash kodui). Tam kad išvengčiau to, kad panašūs žodžiai skirtusi tik viena hash dalimi, panaudojau `shuffle` funckiją.
Šioje funkcijoje labai didelė svarba atitenka `seed` kintamajam, kadangi jei toks pats seed, taip pačiai išmaišys simbolius.
`seed` kintamajį apskaičiavau vėl pagal raidžių unicode skaitmenis, tačiau seed apskaičiuojamas nepildyto/originalaus teksto. `seed` nėra vienodas panašių žodžių nes raidės vieta tekste turi įtakos šiai sumai.

## Shuffle funkcija
```c+++
std::string hashshuffle( std::string &unshuffledhash, hash &current ){

    std::string copy = current.getinput();
    std::wstring_convert<std::codecvt_utf8_utf16<char32_t>,char32_t> converter;
    std::u32string input32 = converter.from_bytes(copy);
    int i = 1;
    int sum = 0;
    for(char32_t &character : input32) {
        sum = sum + character * i;
        i++;
    }
    unsigned seed = sum;
    std::shuffle(unshuffledhash.begin(), unshuffledhash.end(),std::default_random_engine(seed));
    return unshuffledhash;
}
```
4. Algoritmas nėra sudėtingas, kadangi pagrinde naudojamos paprastos sudeties, daugybos funkcijos.

5. Atkurti tekstą iš hash yra sudėtinga, kadangi nepaisant teksto dyžio, hash kodes išlieka toks pats. Taipogi, naudojama `shuffle` funkcija, kuri sumaišo hash simbolius. Dar sunku dėl to, kad nors vienos raidės pakeitimas tekste duoda kitokį hash kodą.

6. Tam, kad du skirtingi žodžiai nehautu tokio pat hash kodo, keliose vietose dėmęsį atkreipiau į raidės vietą žodyje, ne vien raidės unicode skaitmenį. Tam kad gautum tokia pačią sumą ir seed reikia, kad ne vien tokios pat raidės būtu panaudojamos, bet ir to, kad jos būtų taip pat išsidėsčiusios.

7. Tam, kad hash kodas pasikeistu pakeitus tik vieną simbolį, buvo jau anksčiau aptarta Shuffle funkcija, kuri priklauso ne tik nuo raidžių unicode skaičių sumos, tačiau ir nuo jų padeties tekste.

# TESTŲ REZULTATAI

## 1 Testas.
- Bent du failai būtų sudaryti tik iš vieno, tačiau skirtingo, simbolio.

test1.txt
```
a
```

Resultatas:
```
d566fc78ec3612d7b3426ab3ef6290dcef1cc03cdb9be9ea60de7086087487a9
Užtruko: 0.0001457 sek.
```

test2.txt
```
z
```

Resultatas:
```
b184d08ee28613ced84b51dc62dd07dfd694cb9e2e63ca7cd6e680e03f0f787a
Užtruko: 0.0001095 sek.
```

## 2 Testas.
- Bent du failai būtų sudaryti iš daug visiškai skirtingų simbolių (> 100000 simbolių).

test3.txt
```
Let's now look beyond Bitcoin. Bitcoin blockchain is open-source and the entire code is available on the GitHub. During the initial 
years beginning roughly in 2009, this open-source code was extended to release different cryptocurrencies. About 300 plus cryptocurrencies 
were introduced. Bitcoin supports an optional and special feature called scripts for conditional transfer of values. Ethereum Blockchain extended
the scripting feature into a full-blown code execution framework called smart contract. A smart contract provide the very powerful capability 
of code execution for embedding business logic on the blockchain. Based on such capabilities, three major types of blockchains emerge from 
Bitcoin foundation. Type one deals with the coins in cryptocurrency currency chain. Example, Bitcoin. Type two supports cryptocurrency and 
a business logic layer supported by code execution. Example, ethereum. Type three involves no currency but supports software execution for
business logic. Example, The Linux Foundation's Hyperledger. With the addition of code execution, comes the serious consideration about public 
access to the blockchain hence, the classification of public, private, and permissioned blockchains based on access limits. We have been 
watching Bitcoin blockchain continuously operational since its inception. All supported by its public participants. Thus Bitcoin is a fantastic 
example of a public blockchain class. Anybody can join and leave as they wish. Transaction blocks and the blockchain are publicly observable 
even though participants are anonymous. It is open-source. You can also create new coin digital currency by modifying the Bitcoin code. Wallet 
applications provide the basic interface to transfer value through the Bitcoin blockchain. In a private blockchain, access to the blockchain
is limited to selected participants for example, those participants within an organization. This restriction helps in simplifying the normal
operations such as block creation and contingence model. The third classification of blockchain is permissioned blockchain, also called 
consortium blockchain. It is meant for a consortium of collaborating parties to transact on a blockchain for ease of governance, provenance,
and accountability for example, a consortium of all automobile companies or healthcare organizations. Permissioned blockchain has the benefits 
of a public blockchain with allowing only users with permission to collaborate and transact. In summary, significant innovations such as smart 
contracts have opened up broader applications for blockchain technology. 
Private and permissioned blockchain allow for controlled access to the blockchain enabling many diverse business models.
```

Resultatas:
```
0568c9878a45c8a857f3070c798bb3245c8c1def3a7f26bad67e2032d93dd656
Užtruko:  0.000641 sek.
```

test4.txt
```
Here is the basic structure of a blockchain. Transaction is the basic element of the Bitcoin Blockchain. Transactions are validated and 
broadcast. Many transactions form a block. Many box form a chain through a digital data link. Blocks go through a consensus process, to 
select the next block that will be added to the chain. Chosen block is verified, and added to the current chain. Validation and consensus 
process are carried out by special peer nodes called miners. These are powerful computers executing software defined by the blockchain protocol. 
Let's now discuss the details of a single transaction in bitcoin. A fundamental concept of a bitcoin network is an Unspent Transaction Output, 
also known as UTXO. The set of all UTXOs in a bitcoin network collectively defined the state of the Bitcoin Blockchain. UTXO's are referenced as 
inputs in a transaction. UTXO's those are also outputs generated by a transaction. All of that UTXO's is in a system, are stored by the participant 
nodes in a database. Now let's review the role of the UTXO's in a Bitcoin Blockchain. The transaction uses the amount specified by one or more 
UTXOs and transmits it to one or more newly created output UTXOs, according to the request initiated by the sender. The structure of a given 
UTXO is very simple. It includes a unique identifier of the transaction that created this UTXO, an index or the position of the UTXO in the 
transaction output list, a value or the amount it is good for. And an optional script, the condition under which the output can be spent. 
The transaction itself includes a reference number of the current transaction, references to one no more input UTXOs, references to one or 
more output UTXOs newly generated by the current transaction, and the total input amount and output amount. Participants can validate the 
transaction contents. Does the UTXO's reference input exist in the network state? This is the only one of the many validation criteria.
```

Resultatas:
```
3b3f319eddc55c78e6aea342bc9558f86b2abac7a579961663bb1a96315f66e
Užtruko: 0.0005188 sek.
```

## 3 Testas.
- Bent du failai būtų sudaryti iš daug simbolių ir skirtųsi vienas nuo kito tik vienu simboliu.( test5.txt skiriasi nuo test6.txt virtoje
`A Spiritual...` yra `O spiritual` ).

test5.txt
```
One of the oldest forms of tattooing originates in Polynesian cultures. A spiritual art form practiced on islands like New Zealand, 
Hawaii and Samoa, tattoos were used to tell the story of a person’s life with geometric symbols and patterns that had specific meanings. 
Black ink was applied in thick, dark patterns that often covered large portions of the body, and the process itself of getting tattooed was 
a sacred and spiritual undertaking – as well as painful. Many Polynesians today honour their culture by getting tattoos in the traditional 
designs of their ancestors, and sometimes even with the traditional tools. The Polynesian style has also become popular in Western cultures 
– often called “tribal” – though these modern interpretations are usually applied with a tattoo gun.
Sometimes called “old school”, this style of tattoo art is the oldest and most well-known Western form. It follows a strict set of design 
rules that make it easy to spot, such as a limited but bold colour palette, thick black outlines, immaculate precision and two-dimensional, 
crisp images. The development of traditional tattooing is often credited to the legendary Sailor Jerry (real name Norman Collins), who learned 
from Japanese artists and introduced his own American viewpoint to the artform. Common themes are nautical symbols, hearts, eagles, daggers 
and roses. This is the style proudly worn by the original counterculture rebels. 
As the name suggests, Neo Traditional tattoos are a modern take on the original old-school style. Considering that the latter became popular 
around the 1950s onward, it seems only natural that modern artists, armed with new tattooing techniques and fresh perspectives, would put their 
own spin on the genre. Neo traditional artwork stays true in many ways to its predecessor, featuring bold black lines and the classic colour 
palette. However, designs also often feature three-dimensional images, white ink and sometimes a departure from the traditional subject matter.
```

Resultatas:
```
44547a491782c1b6ffe65babc4e80a272e6e973d7dc6aec248db473b8db700
Užtruko: 0.0005873 sel.
```
test6.txt
```
One of the oldest forms of tattooing originates in Polynesian cultures. O spiritual art form practiced on islands like New Zealand, 
Hawaii and Samoa, tattoos were used to tell the story of a person’s life with geometric symbols and patterns that had specific meanings. 
Black ink was applied in thick, dark patterns that often covered large portions of the body, and the process itself of getting tattooed was 
a sacred and spiritual undertaking – as well as painful. Many Polynesians today honour their culture by getting tattoos in the traditional 
designs of their ancestors, and sometimes even with the traditional tools. The Polynesian style has also become popular in Western cultures 
– often called “tribal” – though these modern interpretations are usually applied with a tattoo gun.
Sometimes called “old school”, this style of tattoo art is the oldest and most well-known Western form. It follows a strict set of design 
rules that make it easy to spot, such as a limited but bold colour palette, thick black outlines, immaculate precision and two-dimensional, 
crisp images. The development of traditional tattooing is often credited to the legendary Sailor Jerry (real name Norman Collins), who learned 
from Japanese artists and introduced his own American viewpoint to the artform. Common themes are nautical symbols, hearts, eagles, daggers 
and roses. This is the style proudly worn by the original counterculture rebels. 
As the name suggests, Neo Traditional tattoos are a modern take on the original old-school style. Considering that the latter became popular 
around the 1950s onward, it seems only natural that modern artists, armed with new tattooing techniques and fresh perspectives, would put their 
own spin on the genre. Neo traditional artwork stays true in many ways to its predecessor, featuring bold black lines and the classic colour 
palette. However, designs also often feature three-dimensional images, white ink and sometimes a departure from the traditional subject matter.
```

Resultatas:
```
9f07d87dd2cf4534b30a6c78e749b678ae1badc275718cc2db414519d28dce
Užtruko: 0.0005235 sek.
```

## 4 Testas.
-Tuščias failas Ir išveskite output'us. Nepriklausomai nuo Input'o, Output'ai turi būti vienodo dydžio. Tokiu būdu pademonstruosite, kad Jūsų hash funkcija atitinka 1-3-ą reikalavimus.

test7.txt - tuščias.

Resultatas:
```
0d866ea6affedf81f062e88612c3c6d1b4c3cd0bbd0670e97d94dcc4b867022e
Užtruko: 0.0001096 sek.
```

