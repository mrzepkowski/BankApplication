################################################################

POLISH EXPRESS
Autor programu: Michał Rzepkowski

################################################################

OPIS I DZIAŁANIE


Symulacja systemu bankowego z zaimplementowanymi funkcjami
zakladania i usuwania konta bankowego, wpłacania i wypłacania 
środków, przelewów między kontami oraz obsługi bazy zawartej w 
pliku.
Program najpierw wyswietla tylko 3 opcje: załozenia konta
zalogowania się na istniejące oraz zamknięcia aplikacji.
Inne opcje, czyli kolejno – wpłata i wypłata środków, 
przelew, wylogowanie się i usunięcie konta – są dostępne 
jedynie z poziomu zalogowanego uzytkownika. Obsługa bazy jest
zarządzana przez program, uzytkownik nie ma dostępu do tej
opcji.
Aby wybrać odpowiednią opcję, nalezy wybrać numer jej
odpowiadający, np. 1 by załozyć konto – gdy uzytkownik nie jest 
zalogowany – lub 3 by przelać pieniądze na inne konto – gdy
uzytkownik jest zalogowany.
W trakcie wpisywania danych, np. hasła przy tworzeniu konta
lub kwoty do wypłacenia, mozna wpisać 0, jeśli uzytkownik
zmienił zdanie i chce jednak wybrać inną opcję.

################################################################

OBSLUGA


Uruchomienie programu:
make run

Czyszczenie katalogu:
make clean

Spakowanie katalogu:
make tar

################################################################

BAZA DANYCH


Kolejne kolumny w bazie danych są oddzielone przecinkami, a
wiersze kończą się znakiem nowej linii.
Pierwsza kolumna – ID konta bankowego
Druga – nazwa właściciela
Trzecia – stan konta
Czwarta – hasło

################################################################

DOKUMENTACJA KLAS I METOD


class BankAccount {}:
Klasa ta zapewnia obsługę pojedynczego konta bankowego i danych
związanych z nimi, tzn. ID konta, nazwy właściciela, stanu konta
i hasła zabezpieczającego dostęp.

Konstruktor BankAccount(); - inicjuje "pusty" obiekt klasy
BankAccount. Obiekt ten charakteryzuje się tym, ze jego ID
wynosi 0.

Konstruktor BankAccount(int id, std::string name, float bal,
std::string pass); – inicjuje obiekt klasy BankAccount danymi
podanymi jako argumenty.

int get_id(); - metoda słuząca do pobrania ID konta bankowego.

std::string get_owner_name(); - metoda słuząca do pobrania nazwy
właściciela konta.

float get_balance(); - metoda słuząca do pobrania stanu konta.

std::string get_password(); - metoda słuząca do pobrania hasła.

void deposit(float amount); - metoda słuząca do wpłacenia
srodkow na konto

bool withdraw(float amount); - sluzy do wypłacenia srodków z
konta. Zwraca TRUE – jeśli stan konta starczy na wypłacenie
srodków – lub FALSE – w przeciwnym wypadku.

void clear(); - "sprzątaczka", usuwa dane konta bankowego =>
ustawia ID na 0 i czysci napisy owner_name i password.

bool pass_correct(std::string pass) - sprawdza poprawnosc
hasla wprowadzonego przez uzytkownika. Zwraca TRUE dla
poprawnego hasla, FALSE – w przeciwnym wypadku.

bool is_empty() – zwraca TRUE dla "pustego" konta bankowego.


----------------------------------------------------------------


class Bank {}:
Klasa ta słuzy do komunikacji między uzytkownikiem a kontem
bankowym, między dwoma kontami bankowymi i do komunikacji z
bazą danych. Zawiera wszystkie metody właściwe do obsługi
odpowiednich funkcji aplikacji. Obiekt tej klasy przechowuje
mapę z kolejnymi kontami bankowymi (klucz to ID konta, a
wartosć to obiekt konta bankowego), stan czy z aplikacji
korzysta uzytkownik zalogowany lub niezalogowany (bool is_logged)
oraz obiekt klasy BankAccount, który przechowuje dane uzytkownika 
ktory jest  zalogowany w danej chwili 
(BankAccount currently_logged).

Konstruktor Bank(char *filename); – inicjuje zmienną
przechowującą nazwę pliku z bazą danych, uruchamia metodę
read_from_base() (następny punkt).

void read_from_base(); – metoda prywatna, słuzy do odczytu
danych z bazy. Są one zapisywane w prywatnej mapie "accounts",
gdzie kluczem jest ID konta (1 kolumna w bazie).

Destruktur ~Bank(); – jeśli jest zalogowany uzytkownik, to
uruchamiana jest metoda logout() (dalsza część), a następnie
write_to_base() (następny punkt).

void write_to_base(); – metoda prywatna, słuzy do zapisywania
danych w bazie.

void create_account(int id, std::string name, float balance,
std::string password); – uzywana przez read_from_base() do
zapisywania kont bankowych z bazy do mapy na potrzeby działania
programu.

void create_account(std::string name, std::string password); –
słuzy do tworzenia konta bankowego na podstawie nazwy
własciciela i hasła podanych przez uzytkownika. Nowemu kontu
bankowemu przydzielane jest losowe, unikalne ID z przedziału
[1, 10000] oraz stan konta równy 0 na starcie. Takie konto
jest zapisywane w mapie.

void remove_account(); – słuzy do usuwania konta, na którym w
danej chwili jest zalogowany uzytkownik.

bool login(int id, std::string password); – słuzy do zmiany
stanu pola is_logged na true, czyli takiego, w którym obiekt
klasy Bank jest w stanie zalogowania. Wyświetla wiadomość
powitalną dla uzytkownika i jego obecny stan konta. Zapisuje
konto z mapy na podstawie podanego ID w obiekcie, który
przechowuje stan obecnie zalogowanego konta. Zwraca TRUE, 
gdy podane haslo i ID są poprawne i FALSE w przeciwnym wypadku.

void logout(); – sluzy do zmianu stanu pola is_logged na false.
Przypisuje stan obiektu currently_logged do obiektu klasy
BankAccount w mapie, który odpowiada ID obiektu
currently_logged.

void deposit(float amount); - posrednik pomiedzy uzytkownikiem a
kontem bankowym. Jesli amount > 0, to wywoluje metodę deposit
obiektu currently_logged.

bool withdraw(float amount); - posrednik pomiedzy uzytkownikiem
a kontem bankowym. Jesli amount > 0, to wywoluje metodę withdraw
obiektu currently_logged. Zwraca TRUE, gdy uda się wpłacić
pieniądze lub kwota wynosi 0. Zwraca FALSE gdy się nie uda lub
gdy kwota jest mniejsza niz 0.

bool transfer(int id, float amount); – posrednik pomiedzy
uzytkownikiem a jego kontem bankowym i kontem, na ktore
chce przelac pieniadze. Dla konta uzytkownika uruchamiana jest
metoda withdraw z klasy BankAccount, a dla konta odpowiedniego
dla podanego ID uruchamiana jest metoda deposit z tej klasy.
Zwraca TRUE gdy uda się wypłacić pieniadze uzytkownika i gdy
amount >= 0. Zwraca FALSE w przeciwnym wypadku + gdy podane ID
jest takie samo jak ID uzytkownika lub gdy podane ID nie
istnieje w bazie.

################################################################