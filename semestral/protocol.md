
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

### Reprezentace záznamů

Nejprve jsem musel vyřešit problém reprezentace záznamů jako genomu.

Každý semestr má několik předmětů (v kódu `Course`), každý předmět má několik typů paralelek ("cvičení", "přednášky", "laboratoře", v kódu `Schedule`), tyto typy mají několik paralelek (v kódu `Entry`) a každá paralelka se může vázat na více hodin (v kódu `TimeInterval`).

Student musí mít pro každý typ paralelky každého předmětu zapsanou jednu paralelku. Pro reprezentaci genomu jsem tedy využil hybrid bitového pole, kde každý index v poli genomu odkazuje na typ paralelky pro nějaký předmět. Hodnota v tomto poli je index paralelky pro tento typ paralelky tohoto předmětu. (viz. obrázek) To znamená, že pro každý prvek tohoto pole genomu mají hodnoty horní limit počet paralelek tohoto typu pro tento předmět.

### Crossover funkce

Z hlediska reprezentace dat jsem nemohl využít permutující operátory, protože by se stalo, že hodnota přepermutuje do jiného indexu v poli genomu, kde by tak indexovala neexistující paralelku. 

Vytvořil jsem tedy uniform crossover (gen je vybrán z levého nebo pravého rodiče se stejnou pravděpodobností). 

Dále také k-point crossover (vyberou se body, mezi kterými genom patří buď z levého nebo pravého rodiče). Experimentováním jsem zjistil, že efektivita algoritmu je dobrá, pokud se 1...k u k-point crossoveru vybere podle velikosti genomu. Na základě velikosti genomu si tedy vytvořím několik k-point crossoverů od 1 až po nějaké k.

Protože jsem experimentací zjistil, že větší generace je pro tento problém výhodnější než více generací, vytvářím až `generationSize * generationSize` potomků. Protože jich vytvářím hodně, chci je hodně různorodé, každý crossover operátor tedy vrací jednoho potomka místo dvou (které jsou si opačné).
