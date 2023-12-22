Juraj Povinec 2AIN1

Mala verzia projektu obsahuje:
    Implementaciu triedy Cities
    Implementaciu triedy Flight
    Abstraktnu triedu Infrastructure
    Implementaciu triedy Gate
    Implementaciu triedy Runway

    7 testov k triede Cities
    7 testov k triede Flight
    1 test k triede Infrastructure
    2 testy k triede Gate
    2 testy k triede Runway

Trieda Cities:
    Tato trieda obsauje mnozinu miest, ktore mozu byt origin/destination letu.
    Dalej obsahuje zakladne metody na pracu s touto mnozinou.

Trieda Flight:
    Tato trieda reprezentuje let a udrziava udaje o nom. Dalej obsahuje metody, ktore simuluju rozne stavy letu.

Trieda Infrastructure:
    Tato abstraktna trieda reprezentuje dolezitu infrastrukturu na obsluhu letov na letisku (Gate a Runway).

Triedy Gate a Runway:
    Tieto triedy implementuju abstraktnu triedu Infrastructure. Udrziava informacie o dostupnosti, identifikacne cislo a
    metody na pridavanie/odoberanie letov.

Triedy FileManager a Airport este nie su implementovane. Trieda File Manager bude sluzit na ukladanie a nacitavanie
dat zo suboru. Trieda Airport bude sluzit na obsluhu letiska, vyhladavanie letov, vypisovanie roznych info o letoch atd...

Okrem implementacie zvysnych tried planujem do finalnej verzie projektu vylepsit aktualne implementovane triedy
(napr. kompatibilita pre mesta s medzerami, pridat kopirovacie a presuvacie konstruktory do tried Cities a Flight,
na ktore pouzivam inteligentne smerniky).