
# Protokol

## Téma

Pro svou semestrální práci jsem si vybral **algoritmus generování optimálního rozvrhu** pro semestr na FITu.

Optimální rozvrh může být pro každého jiný a může mít několik různých parametrů, jako například soudružnost hodin během dne (co nejméně prázdných mezer), soudružnost dnů v týdnu (žádné prázdné dny uprostřed týdne) a krátký pracovní týden (například začátek v úterý a konec ve čtvrtek), nezačínat moc brzy (například #730ne) nebo moc pozdě, případně preference některých konkrétních hodin.

Pro tuto úlohu jsem si z témat probíraných v předmětu vybral genetický algoritmus.

## Technologie

Algoritmus jsem napsal v C++ s obsluhou jen v příkazové řádce. 

## Zpracování vstupu

Protože přístup ke KOSApi je neveřejný, musel jsem vstup (tedy paralelky předmětů, které má algoritmus rozvrhovat) vyřešit jinak. Nový KOS má na stránce předmětu rozumně formátovanou tabulku všech paralelek. Napsal jsem si tedy pro tabulku lehce komplikovanější parser (`src/Custom/FITCTUFileImporter`). Pro každý předmět tedy stačí celý obsah tabulky (bez hlavičky) vykopírovat a vložit do textového souboru. Jediná potřeba formátování je vytvoření prázdného řádku mezi záznamy jednotlivých předmětů.

Po spuštění aplikace pak stačí zadat cestu k tomuto textovému souboru.

Pomocí standardního vstupu se pak dají upravit právě různé parametry generace. Výstup algoritmu je pak také zobrazen na standardní vstup.

## Evoluční algoritmus

Počet genomů v generaci se vypočítává z délky genomu, uživatel si může nastavit počet generací.

### Reprezentace záznamů

Nejprve jsem musel vyřešit problém reprezentace záznamů jako genomu.

Každý semestr má několik předmětů (v kódu `Course`), každý předmět má několik typů paralelek ("cvičení", "přednášky", "laboratoře", v kódu `Schedule`), tyto typy mají několik paralelek (v kódu `Entry`) a každá paralelka se může vázat na více hodin (v kódu `TimeInterval`).

Student musí mít pro každý typ paralelky každého předmětu zapsanou jednu paralelku. Pro reprezentaci genomu jsem tedy využil hybrid bitového pole, kde každý index v poli genomu odkazuje na typ paralelky pro nějaký předmět. Hodnota v tomto poli je index paralelky pro tento typ paralelky tohoto předmětu. (viz. obrázek) To znamená, že pro každý prvek tohoto pole genomu mají hodnoty horní limit počet paralelek tohoto typu pro tento předmět.

### Crossover

Z hlediska reprezentace dat jsem nemohl využít permutující operátory, protože by se stalo, že hodnota přepermutuje do jiného indexu v poli genomu, kde by tak indexovala neexistující paralelku. 

Vytvořil jsem tedy uniform crossover (gen je vybrán z levého nebo pravého rodiče se stejnou pravděpodobností). 

Dále také k-point crossover (vyberou se body, mezi kterými genom patří buď z levého nebo pravého rodiče). Experimentováním jsem zjistil, že efektivita algoritmu je dobrá, pokud se 1...k u k-point crossoveru vybere podle velikosti genomu. Na základě velikosti genomu si tedy vytvořím několik k-point crossoverů od 1 až po nějaké k.

Protože jsem experimentací zjistil, že větší generace je pro tento problém výhodnější než více generací, vytvářím až `generationSize * generationSize` potomků. Protože jich vytvářím hodně, chci je hodně různorodé, každý crossover operátor tedy vrací jednoho potomka místo dvou (které jsou si opačné).

### Mutace

Experimentováním jsem nastavil vhodné pravděpodobnosti mutací. Mutace probíhá na náhodném místě genomu, s určitou pravděpodobností a s počtem možných mutací závisejícím na délce genomu.

Mutace nastaví na daném indexu náhodnou hodnotu, v rozsahu omezeným počtem paralelek pro daný typ paralelky daného předmětu.

### Selekce

Selekce kromě vybrání genomů s největší hodnotou fitness také využívá elitismus, tedy do nové generace je také přenesen několik nejlepších genomů z generace předchozí.

#### Fitness

Výpočet fitness funkce je nejkomplikovanější součástí algoritmu.

Uživatel si může nastavit několik volitelných preferencí pro vygenerování rozvrhu. Pokud jsou preference zapnuté, fitness funkce vytvoří pro každou tuto vlastnost skóre:
- _Udržení paralelek ve dne pohromadě_ - skóre je počet mezer ve dne
- _Udržení dnů v týdnu pohromadě a minimalizace délky pracovního týdne_ - skóre je počet dní od první paralelky v týdnu po poslední paralelku
- _Maximální počet hodin ve dne bez přestávky_ (jde o opak první preference, který má zase zamezit aby se rozvrh ve dne naplnil od rána do večera) - skóre je počet minut nad stanovený limit z hodin v každém dni
- _Nejdřívější hodina začátku rozvrhu a nejpozdější hodina začátku rozvrhu_ - skóre je počet minut od této meze do startu paralelky pro každou paralelku před/po této mezi
- _Bonus a malus za specifickou paralelku_ - u jednotlivých paralelek se dá nastavit bonus (nebo malus), který zvýhodní genomy které tuto paralelku obsahují

pak vlastnost která se nedá vypnout:
- _Kolize_ - skóre je počet kolidujících paralelek v rozvrhu

dále pak nastavení, které nemění způsob výpočtu fitness ale také ho ovlivňují:
- jak velká mezera v minutách se stále počítá jako paralelky pohromadě
- ignorované typy paralelek pro předměty (např. ignorování přednášek u předmětu), což způsobí že se při výpočtu fitness na tento typ paralelky nebude brán ohled

Skóre všech těchto preferencí (kromě bonusu pro specifiké paralelky) je namapováno inverzně na škálu 0-10 (tedy největší hodnota se namapuje na 0, nejnižší se namapuje na 10). Celkový fitness genomu je pak vážený součet těchto namapových skóre. Pečlivou experimentací jsem pak určil váhy pro jednotlivé vlastnosti tak, aby dávali dobrý kompromis pro kvalitu rozvrhů v dané vlastnosti.

## Závěr

Do ukázkových vstupů jsem vložil předměty z mého minulého semestru. Po nastavený vhodných preferencí dostávám identický rozvrh s tím, jaký jsem si sestavil minulý semestr ručně. S úspěšností algoritmu jsem tedy spokojen.
