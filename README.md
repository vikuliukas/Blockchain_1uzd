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

# Idiegimas (Unix kompiuteryje)

- `git clone https://github.com/vikuliukas/Blockchain_1uzd.git`
- `cd Blockchain_1uzd`
- `make`
- `./main`

# Instrukcijos

- Pasirinkite ar norite tekstą įvesti ranka ar nuskaityti nuo failo. Jei ranka įveskite 0, jei nuo failo - 1.
- Jei norite nuksaityti nuo failo pasirinkite ar norite, kad visam tekstui būtu vienas hash ar sukurti hash kiekvienai teksto eilutei. Jei visam failui įveskite 0, jei kiekvienai teksto eilutei - 1.
- Jei pasirinktote sukaityti nuo failo dar nums reikės įvesti failo, nuo kurio norite nuskaityti teksta, pavadinimą (pvz. tekstas.txt).
- Jei pasirinkote įvestį ranka, jums reikės įvesti norimą tekstą.

- Jei pasirinkote, kad įvesite teksta ranka ar jums reikia vieno hash visam nuskaitytam tekstui, hash bus atspausdintas jums į terminalą.
- Jei pasirinkote, kad jums reikia skirtingo hash kiekvienai teksto eilutei, rezultatus rasite sugeneruotame rezultatas.txt file.



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

2. Hash visada išvedamas vienodo dydžio. Kiekvieną teskto simbolį paverčiu į Unicode numerį ir suskirstau į keturias dalis (pasirinkau keturias dalis nes int tipo kintamasis nesutalpintu tokio skaičiaus, kuris paskui pavertus is dešimtainės skaičiavimo sistemos į šešioliktainę būtų 64 simbolių skaičius, tačiau vienas maksimalus dešimtainės systemos `unsigned long long int` yra lygus 16 šešioliktainės sistemos skaičių (FFFFFFFFFFFFFFFF), todėl 64 symboliams reikėjo 4 int kintamųjų). Jeigu failas neturi pakankamai raidžių, kad susidarytu 64 symbolių hash, funkcijoje `makelongerifneeded` failas papildomas, kad tam užtektu. Po kelių testu pastebėjau, kad kartais naudojant lietuviškas raides kartais vistek būna 15 simbolių `hex` kintamajam, tokiu atveju jį papildau 0.

## Hash algoritmas
```c++
void hashalgorithm( hash&current ){
    std::string copy = current.getfixedinput();
    std::wstring_convert<std::codecvt_utf8_utf16<char32_t>,char32_t> converter;
    std::u32string input32 = converter.from_bytes(copy);
    unsigned long long int sum[5] = {1};
    int speperating = 1;
    int i = 1;
    for(char32_t &character : input32) {
        if(speperating == 1){
            sum[0] = ( i + i ) * sum[0] + character;
        }
        if(speperating == 2){
            sum[1] = ( i + i ) * sum[1] + character;
        }
        if(speperating == 3){
            sum[2] = ( i + i ) * sum[2] + character;
        }
        if(speperating == 4){
            sum[3] = ( i + i ) * sum[3] + character;
            speperating = 0;
        }
        speperating ++;
        i++;
    }

    std::string hex [5] = {""};
    for(int i = 0; i < 4; i++){
        hex[i] = inttohex(sum[i]);
        while(hex[i].length()<16){
            hex[i] += "0";
        }
    }
    std::string unshuffledhash = hex[0] + hex[1] + hex[2] + hex[3];
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

6. Tam, kad du skirtingi žodžiai negautu tokio pat hash kodo, keliose vietose dėmęsį atkreipiau į raidės vietą žodyje, ne vien raidės unicode skaitmenį. Tam kad gautum tokia pačią sumą ir seed reikia, kad ne vien tokios pat raidės būtu panaudojamos, bet ir to, kad jos būtų taip pat išsidėsčiusios.

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
4d412cb7e32f8aa3cd2cec8c15c30f8bec96abd40b694371239b4ea9ed196258
Užtruko: 0.0001118 sek.
```

test2.txt
```
z
```

Resultatas:
```
99428363e3a281c499bba206fcfdb4d24c69b887fbeca333a114d48ecce5671c
Užtruko: 0.000181 sek.
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
95b997eae7b08ce860aa2e5df1bd84bbb4fcaf6ce8afdb55d54b49ff9250d2a2
Užtruko: 0.0006541 sek.
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
cfe846ab98d8b36da13b0a3129f1d27c8641421f001d91a5edfd8bf9cc159ec3
Užtruko: 0.0005273 sek.
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
73ace611660dd852dd1546fd78cfcc5413162830bd18411806fb254a05cfb4b4
Užtruko: 0.0008509 sel.
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
c2656fbc587d2d33c44411fb1401a566f145685c8de801140dc61a82fb0dd73b
Užtruko: 0.0005416 sek.
```

## 4 Testas.
-Tuščias failas Ir išveskite output'us. Nepriklausomai nuo Input'o, Output'ai turi būti vienodo dydžio. Tokiu būdu pademonstruosite, kad Jūsų hash funkcija atitinka 1-3-ą reikalavimus.

test7.txt - tuščias.

Resultatas:
```
e96c27a4c2de7578ce48b8ae93c96c88813cc2f394bf3b161089aba2bd143c14
Užtruko: 0.0001084 sek.
```

## 5 Testas.
-Ištirkite Jūsų sukurtos hash funkcijos efektyvumą: tuo tikslu suhash'uokite kiekvieną eilutę iš konstitucija.txt failo.

Resultatas: (pirmos 10 eiluču viso resulatato failo);
```
eecae4611b370ba22c2a4ede028141fbf2f5d42626e85f0fce7bb7de2e8e63d9
eb381d27996171545b523f149980b0173dc6d5d5e3c256b46d3cf23bd3bbcd06
ec57c97ef15a0378d30c2cb9ae4f97b4508e867c69a1404a81061f6ab4fdaa38
c01dec4d269daf1c66180fe832c23a98ff22ca504fc6ba48754f1508d49a2461
b1c3afc36199ddb473d1a4f8e9e4414208c1829f5aa040ee43ddbf3e7b448cf1
0e09595df8ae254fec9c6681ca0c9092e6f6063547b9a947018f319256445b41
e1c63e6c126de70afc98fb882771280fa51305ac3f9f91db40b38f047fcb9e10
df3963c97d93ea7b2c9c4c4cdf8f5142bd59df9e5088a5f02ca2f817efa4147b
202872e449fb6563dfb424e0627877e6960d975f366d00f35ce03ab2a51f554d
055b7fde085168e43cfe85498290aacfb88f2b31ba8a5780bf6a31371f4acce6
05ff50a2c4652c3c09a998a319c0889bca0847887ae8f69a95881ef7d80b2b47
Užtruko: 0.0253131 sek.
```

## 6 Testas.
- failas su 1000 000 atsitiktinių simbolių eilučių, kurios kiekviena turi 5 simbolius. (pavadintas gen1.txt).

gen1.txt pirmos 10 elitučiu.
```
bfsSl
dNCTy
LyRID
OlTyJ
bquOc
ijgbW
ORaTo
aXJuw
UEDeH
IrgDb
```

resultatas.txt pirmos 10 eilučiu.
```
458d9e4c8937e323c28c82f7a83150207b34ab3940136cd2f11ebe12b1b080df
046a8d552c0a3814c01858004658c128e51fa4219d53c7af4bb3b95fe81d6834
716bc59783c36f825cc8fb8816b4b1e0b373412fb9cda280aba62519836604f3
b0502fdc89dc903f8ccb023f641e9db34ff663f4a4af2de8af4b104c983d4c6e
033bc556de5a7472227a3c825b3749c73b219441eeba2a08a06f633b148da8cf
b047a56c40049fe0397853c17e51818b2230f693a52f795c432bab9bdae12956
d62b30131fd4281a52f8595f5e31b651d5da06f8fc69ee638fad004eccf6722b
cfbf823ece93e12872bed2636f4acf411ca3b55cdde9c2e04697958f0809a45a
9533364f9be26718f1c434985e6c249bd636c6a995278adbf1903a71bad44cf5
f4b0bf5e1c633bd836b05b9592fa61fd7138398cade68b11ec5b12224127aa19
Užtruko: 23.081 sek.
```

## 7 Testas 
- Failas su 1000 000 atsitiktinių simbolių eilučių, kurių poros skiriasi tik vienu simboliu ir yra 5 simbolių ilgio. (pavadintas gen2.txt).

gen2.txt pirmos 10 eilučiu.
```
Valfs
Va!fs
fbJFy
fbJ!y
WjIqy
!jIqy
yzwXn
!zwXn
hBnIr
!BnIr
```

resultatas.txt pirmos 10 eilučiu.
```
bb45680b36eea6b0ba3c9106458038d018af1d1b32c215414a21588d344e3fa7
543a4851839fb2c1a88319686e8b61464eacdb301ee4473e917b2024fd6c307a
2f84b43c644af3a381ad994cd8dea502a3921d7efcd1b52b63a828418a42e990
2fbdd144839e329c2cf120da9cf460d2d81843a928647e5b838ebc1aaba49543
544d18072671393bbfb434a33c082985bf903aabec476e8486a0b3c170c5bf4d
933bc00923b340032a6924bbbc7401a79edd80cec1f83064c5f4893af5cd59ab
98a30b956244a1c3a320a02455b2b3dd6eec8688d3f4f025e331bccfe3920f54
f433a85f5ad812421a6ce32c9b83590bd16463d15ad38b105433036ea8b280e3
e2edf9c1b4edc58794bf66ec9d1e80cfa8cbef61cdaca33322131d37c22b5655
08c29091b9c67a3c9321cce65a81fe42c69db480c529f3cebae257ae9d33751f
Užtruko: 22.8981 sek.
```

## Versiju istorija

# [v1.0](https://github.com/vikuliukas/Blockchain_1uzd/releases/tag/v1.0)

*Koreguota*

- funkcijoje hashalgoritm pridėta salygą, kad jei mažai simbolių vienoje hash dayje, ją papildytu.

*Pridėta*

- Pridėtos failų generavimo funkcijos, kurios buvo reikalingos 5, 6 ir 7 testui.
- Pridėta kita nuskaitymo nuo failo funkcija, kuri nuskaito failo teksta po vieną eilutę ir jos saugo vektorije.
- Pridėtas 5, 6 ir 7 testai į ReadMe failą.

# [v0.1](https://github.com/vikuliukas/Blockchain_1uzd/releases/tag/v0.1)

- Pirminė programos versija