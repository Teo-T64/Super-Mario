# Super Mario
Super Mario projekt OOP

**Pokretanje projekta**
- Downloadati kao .zip ili koristiti naredbu `git clone https://github.com/Teo-T64/Super-Mario.git`
- Preduvjeti za pokretanje: SFML 3.0(dostupan u projektu), box2d za VS koristeći vcpkg.
**Instaliranje box2d**
- Otvoriti cmd/Powershell i unijeti naredbe:
`git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat`
- zatim nakon instaliranja vxproj unijeti naredbu za instalaciju box2d `.\vcpkg install box2d:x64-windows`, te integrirati sa VS `.\vcpkg integrate install`
- Kako bi se projekt mogao funkcionirati potrebno je u VS imati Solution Configuration na Debug i Solution platforms na x64
**Kontrole u igri**
- Strelica desno - kretanje desno
- Strelica lijevo - kretanje lijevo
- Strelica gore/Space - skakanje
- Shift + Strelica gore(lijevo,desno) - povećanje brzine kretnje
- Esc - pauziranje igrice
- Space - resetiranje igrice nakon smrti ili pobjede

**Ovisnosti u projektu**
  
**SFML 3.0**
- Graphics: Za iscrtavanje mapa, likova i UI-ja
- Audio: Za pozadinsku glazbu.
- Window: Za obradu događaja tipkovnice i upravljanje prozorom

**Box2d 3.0**
- Zaslužan za upravljanje gravitacijom, detekciju sudara između igrača i objekata, kretanje te za kreiranje statičkih i dinamičkih tijela

**C++ STL**
-<filesystem>- za traženje mape resources/ i automatsko učitavanje datoteka
-<vector>: Za spremanje popisa objekata 
-<iostream>: Za ispisivanje debug informacija u konzolu 
-<string>: Za upravljanje tekstom i putanjama

**Mapa `resources/`**
- Smješteni su .png datoteke za animacije likova, neprijatelja i novčića, mape, backgrounda , .ttf za font i .wav, .ogg datoteke za zvukove

**Rješavanje mogućih problema**
-Ako igra javi "FATAL ERROR:Could not find resources folder.",
provjerite nalazi li se mapa resources u istom direktoriju kao i .exe datoteka ili jednu razinu iznad
(projekt automatski pretražuje do 3 razine dubine)
