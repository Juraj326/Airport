# Letisko

Ako tému projektu som si zvolil letisko. Snažil som sa vytvoriť riadiaci systém letiska, ktorý umožňuje spravovať a
simulovať prichádzajúce a odchádzajúce lety. Projekt sa skladá zo šiestich, logicky oddelených tried:

1. Cities
2. Flight
3. Infrastructure
    1. Gate
    2. Runway
6. Airport

---

## Triedy

### Cities

Trieda Cities reprezentuje množinu letových spojení s letiskom. Cieľom tejto triedy je udržiavanie a modifikovanie
letových spojení.  
Jej jediným atribútom je `std::set<std::string> cities`, ktorý obsahuje zmienené mestá, ktoré sú
spojené s letiskom.  
Ďalej obsahuje základné metódy na prácu s množinou a metódy na poskytovanie informácii
o tejto množine.  
Trieda Cities poskytuje defaultny konštruktor, konštruktor `explicit Cities(const std::string &city)` a konštruktor
`explicit Cities(const std::vector<std::string> &listOfCities)`. Bližšia dokumentácia k metódam a konštruktorom sa nachádza
priamo pri ich implementáciách.

### Flight

Trieda Flight reprezentuje let, ktorý z letiska odlieta alebo naň prilieta. Trieda slúži na poskytovanie informácií o
lete a simuláciu letu. Trieda Flight má preto štyri atribúty:

- `int number` Identifikátor letu
- `FlightStatus status` Stav, v ktorom sa let nachádza
- `std::string origin` Miesto odletu
- `std::string destination` Destinácia

Okrem týchto atribútov obsahuje trieda Flight metódy na simulovanie letu (metódy `bool board()` až `bool disembark()`)
a klasické metódy pre OOP ako sú gettery a settery.
Trieda Flight poskytuje jediný konštruktor `explicit Flight(int number, FlightStatus status)`.
V súbore tejto triedy sú taktiež aj dve metódy pre `enum class FlightStatus`.
Všetká dokumentácia k týmto metódam sa potom nachádza pri ich definíciách.

### Infrastructure

Abstraktná trieda Infrastructure predstavuje kľúčovú infraštruktúru letiska, ktorú využívajú lietadlá, ako napríklad
Gate a Runway. Táto trieda má tri atribúty:

- `int number` Identifikátor infraštruktúry
- `std::shared_ptr<Flight> flight` Let, ktorý aktuálne využíva túto infraštruktúru
- `Status status` Stav infraštruktúry

Ďalej sú v tejto triede dve čisto virtuálne metódy `virtual bool assignFlight(std::shared_ptr<Flight> flight)` a
`virtual void releaseFlight()`, ktoré sú následne prekryté v triedach Gate a Runway. Okrem týchto dvoch metód sú v
triede ďalšie dve metódy, ktorých dokumentácia sa nachádza pri ich definíciách.  
Trieda Infrastructure poskytuje jediný konštruktor `Infrastructure(int number) : flight(nullptr), status(Status::VACANT)`.

### Gate

Trieda Gate je prvá z dvoch tried, ktoré implementujú abstraktnú triedu Infrastructure. Táto trieda má, podľa názvu, 
reprezentovať gate na letisku. 
V tejto triede dochádza k prekrytiu metód `bool assignFlight(std::shared_ptr<Flight> flight)` a
`void releaseFlight() override`. Zvyšok sa zdedí od triedy Infrastructure.  
Trieda Gate poskytuje jediný konštruktor `Gate(int number) : Infrastructure(number)`. 

### Runway

Trieda Runway taktiež implementuje abstraktnú triedu Infrastructure. Táto trieda reprezentuje runway na
letisku, a je  principiálne rovnaká ako trieda Gate.

### Airport

Trieda Airport reprezentuje celé letisko. Slúži na spravovanie infraštruktúry a riadenie letov. Táto trieda ma viacero
atribútov:

- `const std::string city` Mesto, v ktorom sa nachádza letisko
- `std::vector<std::shared_ptr<Flight>> flights` Zoznam všetkých letov
- `const size_t maximumNumberOfGates` Najvyšší možný počet gate-ov
- `std::map<std::shared_ptr<Gate>, std::shared_ptr<Flight>> gates` Zoznam všetkých gate-ov v tvare `<Gate, Let>`
- `const size_t maximumNumberOfRunways` Najvyšší možný počet runway-ov
- `std::map<std::shared_ptr<Runway>, std::shared_ptr<Flight>> runways` Zoznam všetkých runway-ov v tvare `Gate, Let`
- `std::shared_ptr<Cities> connections` Množina všetkých letovísk spojených s letiskom

Trieda airport poskytuje jediný konštruktor `Airport(const std::string &city, size_t maximumNumberOfGates, size_t maximumNumberOfRunways)`,
ktorý vytvorí prázdne letisko bez infraštruktúry. Ďalej do množiny destinácii pridá  mesto `city`, v ktorom sa nachádza toto
letisko a nastaví `maximumNumberOfGates` a `maximumNumberOfRunways`. Tieto dva atribúty sa po vytvorení letiska už nedajú modifikovať.
Dokumentácia ku všetkým metódam tejto triedy sa nachádza priamo pri ich implementáciách.

---

## Zmeny oproti malej verzii projektu

#### Trieda Cities

Trieda cities teraz dokáže uchovávať aj mestá s medzerami.

#### Trieda Flight

Z triedy Flight som presunul atribút `std::shared_ptr<Cities> connections` do triedy Airport, kde dáva väčší zmysel ho
uchovávať. Ďalej som odstránil atribúty `int gateNumber` a `int runwayNumber`, lebo výrazne zvyšovali komplexitu triedy
Airport. Odstránil som aj enumerátor `FlightStatus::CREATING` a metódy s ním spojené lebo sa stal nepotrebným a
taktiež výrazne zvyšoval komplexitu triedy Airport.

### Zmeny na základe spätnej väzby

Na základe spätnej väzby z malej verzie projektu som
- Definoval všetky číselné konštanty spolu na jednom mieste pre danú triedu.
- Upravil definície metód, aby brali potenciálne veľké parametre referenciou a nie hodnotou.
- Zmenil definície metód, aby namiesto potenciálne veľkých objektov vracali referenciu na tieto objekty.
- Premenoval viaceré premenné a názvy metód, aby mali deskriptívnejší názov.

### Konštanty

#### Trieda Flight

Definície všetkých konštánt som uviedol na jedno miesto spolu s ich názvami.

```c++
// PRED
// Flight.cpp

Flight::Flight(int number, std::shared_ptr<Cities> connections) : connections(std::move(connections)), status(FlightStatus::CREATING), gateNumber(-1), runwayNumber(-1) {
    if (number < 0 || number > 9999)
        throw std::invalid_argument("Flight number must be an integer ranging from 0 to 9999.");
    ...
}
```

```c++
// PO
// Flight.h

const int MIN_FLIGHT_NUMBER = 0;
const int MAX_FLIGHT_NUMBER = 9999;


// Flight.cpp

Flight::Flight(int number, FlightStatus status) :  status(status) {
    if (number < MIN_FLIGHT_NUMBER || number > MAX_FLIGHT_NUMBER)
        throw std::invalid_argument("Flight number must be an integer ranging from 0 to 9999.");
    ...
}
```

#### Trieda Infrastructure

Rovnaké zmeny som implementoval aj do abstraktnej triedy Infrastructure.

```c++
// PRED
// Infrastructure.h

Infrastructure(int number) : flight(nullptr), status(Status::VACANT) {
    if (number < 0 || number > 36)
        throw std::invalid_argument("Gate/Runway number must be an integer ranging from 0 to 36.");
    ...
}
```

```c++
// PO
// Infrastructure.h

const int MIN_INFRASTRUCTURE_NUMBER = 0;
const int MAX_INFRASTRUCTURE_NUMBER = 36;

Infrastructure(int number) : flight(nullptr), status(Status::VACANT) {
    if (number < MIN_INFRASTRUCTURE_NUMBER || number > MAX_INFRASTRUCTURE_NUMBER)
        throw std::invalid_argument("Gate/Runway number must be an integer ranging from 0 to 36.");
    ...
}
```

### Parametre

#### Trieda Flight

V triede Flight som zmenil metódy `bool Flight::setOrigin(std::string origin)` a
`bool Flight::setDestination(std::string destination)`, aby brali parametre referenciou.

```c++
// PRED
// Flight.h

bool setOrigin(std::string origin);
bool setDestination(std::string destination);
```

```c++
// PO
// Flight.h

bool setOrigin(const std::string &origin);
bool setDestination(const std::string &destination);
```

### Návratové hodnoty

#### Trieda Flight

V triede Flight som zmenil gettery `std::string getOrigin()` a `std::string getDestination()`, aby vracali objekt typu
`std::string` referenciou.

```c++
// PRED
// Flight.h

std::string getOrigin() const { return origin; };
std::string getDestination() const { return destination; };
```

```c++
// PO
// Flight.h

const std::string &getOrigin() const { return origin; };
const std::string &getDestination() const { return destination; };
```

### Názvy premenných a metód

#### Trieda Cities

V konštruktore `Cities::Cities(const std::vector<std::string>& listOfCities)` a v metóde
`Cities::add(const std::string& city)` som zvolil vhodnejší názov premennej.

```c++
// PRED
// Cities.cpp

Cities::Cities(const std::vector<std::string>& listOfCities) {
    ...
    for (char c : city)
        if (!isalpha(c))
    ...        
}

Cities::add(const std::string& city) {
    ...
    for (char c : city)
        if (!isalpha(c))
    ...
}
```

```c++
// PO
// Cities.cpp

Cities::Cities(const std::vector<std::string>& listOfCities) {
    ...
    for (char letter : city)
        if (!isalpha(letter))
    ...        
}

Cities::add(const std::string& city) {
    ...
    for (char letter : city)
        if (!isalpha(letter))
    ...
}
```

Metódu `const std::string Cities::print()` som premenoval na `const std::string Cities::getListOfCities()`,
aby mala výstižnejší názov a taktiež som zmenil názvy viacerým premenným v tejto metóde.

```c++
// PRED
// Cities.cpp

const std::string Cities::print() {
    ...
    std::ostringstream os;
    std::string del = ",";
    bool first = true;
    ...
}
```

```c++
// PO
// Cities.cpp

const std::string Cities::getListOfCities() {
    ...
    std::ostringstream cityStream;
    bool firstCity = true;
    ...
}
```

---

**Juraj Povinec 2AIN1**