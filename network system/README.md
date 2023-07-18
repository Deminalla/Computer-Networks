Mokymo ir testavimo koledžo tinklo projektas
LEGENDA. Norint padidinti IT specialistų kiekį Lietuvoje kuriamas naujas Mokymo ir testavimo koledžas (MTK).

Jūsų darbas:
pateikti MTK kompiuterių tinklo projektą (dokumentą);
realizuoti su Cisco Packet Tracer MTK kompiuterių tinklą.

## Projektinio tinklo reikalavimai: ##
* turi būti fizinė ir loginė kompiuterių tinklo schema;
* suplanuoti optimalią reikiamą tinklinę įrangą;
* suplanuoti reikalingus serverinius resursus, bei jų prijungimą prie tinklo;
* serverinėse turi būti serveriai su iš visur pasiekiama WEB svetaine;
* IPT tinklą atveda iki POP/MDF kambario, tolesnis tinklas planuojamas ir diegiamas jūsų;
* IPT organizacijai duoda IPv4 ir IPv6 adresus (žr. savo variantą).
* turite nuspręsti ir pagrįsti kaip protingai padalinti MTK tinklą į optimalias dalis naudojant potinklius (rekomenduojama naudoti VLSM);
* turi būti ne mažiau kaip 6 potinkliai su išoriniais IP adresais, kitur galite naudoti vidinius;
* kur įmanoma kompiuterių tinklas turi veikti su  IPv4 ir su IPv6 adresais;
* pateikite IP adresus ir potinklio kaukes visiems planuojamiems įrenginiams, dalinant IP adresus kompiuterinėms darbo vietoms (KDV) pakanka * nurodyti pirmos ir paskutinės KDV IP adresus;
* suplanuoti belaidį tinklą visame MTK;
* suplanuoti vaizdo stebėjimo sistemą naudojančią IP kameras  (bent po vieną kamerą auditorijose/dirbtuvėse) ;
* ACL priemonėmis apribokite pasirinktų centrų ar auditorijų prieigą prie kitų resursų;
* sukurtas tinklas turi būti lengvai plečiamas, jeigu MTK po metų įsigis papildomų patalpų;

## Suplanuotų MTK patalpų ir kompiuterinės įrangos aprašymas: ##

#### Pagrindinis pastatas (MTK) ####
* Pirmas aukštas (dydis 30*50 metrų)
  * Serverinė su Interneto įvadu (vietiniai web, pašto ir ftp serveriai)
  * Auditorija (12 KDV)
  * Auditorija (14 KDV)
  * Administracija (10 KDV)
* Antras aukštas (dydis 30*50 metrų)
  * Multimedijos mokymo centras (16 KDV)
  * Dizaino mokymo centras (24 KDV)

#### Egzaminavimo centras (~100 m atstumas nuo MTK) #####
* Testavimo centras (18 KDV)

#### Išmaniųjų technologijų mokymo centras (~1,5 km atstumas nuo MTK) ####
* Dėstytojų kambarys (8 KDV)
* Elektronikos dirbtuvės (35 KDV).  
* Išmaniųjų technologijų mokymo centro serverinė
* Budėtojų ir apsaugos kambarys (3 KDV + IoT registracijos serveris)

#### Konferencijų salė (50x40 metrų, šalia išmaniųjų technologijų mokymo centro). ####
* 300 vietų
* Čia visur turi veikti bevielis tinklas ir internetas, galima naudoti keletą stotelių). 

**Pastaba. Išmaniųjų technologijų mokymo centre papildomai turi būti įrengti dūmų, CO2, CO, judesio aptikimo davikliai, o elektronikos dirbtuvėse - temperatūros monitorius bei termostatas. Visi šie įrenginiai turi būti prijungti prie registracijos serverio, kuris įrengtas budėtojų ir apsaugos kambaryje.**

### Duotas IP ###
* 187.18.5.0/24
* 2028:17c:ced1::/48

![image](https://github.com/Deminalla/Computer-Network/assets/65849358/0f9ebcc9-9ff4-4026-9856-9daa8e1e021a)

## Extra task ##
* Add a siren, which starts going off after the temperature rises to a specific point. Use MCU and write some code in it.
