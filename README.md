[README.md](https://github.com/user-attachments/files/34333521/README.md)
# Domaći zadatak 1: Nizovi, BST i Splay stablo

Tri programa u jeziku C koja se bave pretragom i organizacijom ključeva (celih brojeva), uz brojanje pristupa elementima strukture.

## Sadržaj

| Fajl | Opis |
|------|------|
| `dz1p1.c` | Niz promenljivog kapaciteta sa prebacivanjem pronađenog ključa na početak |
| `dz1p2.c` | Splay stablo (sa ispisom stabla) |
| `dz1p3.c` | Poređenje tri strukture: niz, BST i Splay stablo |

## Prevođenje i pokretanje

```
gcc -o dz1p1 dz1p1.c
gcc -o dz1p2 dz1p2.c
gcc -o dz1p3 dz1p3.c
./dz1p1
```

Svi programi su meni-orijentisani. Unos mora biti ceo broj; pri pogrešnom unosu program traži ponovni unos. Kraj ulaza (EOF) prekida program.

## Brojanje pristupa

Svaka operacija vraća broj pristupa elementima strukture.

**Niz (prebacivanje na početak)**
- Linearna pretraga: svaki pregledan element je jedan pristup.
- Dodavanje: pretraga (provera duplikata) + 1 pristup za upis na kraj.
- Brisanje: pretraga + pomeranje elemenata iza obrisanog.
- Pretraga: ako je ključ pronađen na poziciji `p > 0`, dodaje se `p + 1` pristupa za prebacivanje na početak. Neuspešna pretraga košta `n + 1`.

**Splay stablo**
- Svaki posećen čvor pri spuštanju od korena je 1 pristup.
- Svaka rotacija se računa kao 3 pristupa.
- Posle pretrage, umetanja i brisanja, čvor (ili poslednji posećeni čvor) se podiže u koren.

**BST**
- Svaki posećen čvor je 1 pristup; brisanje dodaje pristupe za nalaženje sledbenika i povezivanje.

## dz1p1.c: Niz sa prebacivanjem na početak

Dinamički niz čiji se kapacitet udvostručuje kada se popuni. Ključevi su jedinstveni.

Meni:
1. Unos niza sa tastature
2. Unos niza iz datoteke (prvi broj je početni kapacitet, zatim ključevi)
3. Generisanje niza (slučajni različiti ključevi iz opsega `[lo, hi]`)
4. Dodavanje ključa (na kraj)
5. Brisanje ključa
6. Pretraga ključa (pronađen ključ se prebacuje na početak)
7. Ispis niza
8. Brisanje niza iz memorije
0. Kraj

Ponovljeni ključevi pri unosu se prijavljuju i preskaču.

## dz1p2.c: Splay stablo

Binarno stablo pretrage sa pokazivačem na oca, koje se samopodešava splay operacijom (jednostruka rotacija, cik-cik i cik-cak).

Meni:
1. Formiranje praznog stabla
2. Umetanje ključeva
3. Pretraga ključa
4. Brisanje ključa
5. Brisanje stabla iz memorije
6. Ispis stabla
0. Kraj

Napomene:
- Brisanje: traženi čvor se podiže u koren, zatim se podstabla spajaju preko najvećeg čvora levog podstabla (prethodnika).
- Ispis stabla je moguć samo za stabla do 50 ključeva. Ključevi su raspoređeni po inorder poretku (kolone), a po dubini (redovi).
- Oslobađanje memorije je nerekurzivno (od lista ka ocu).

## dz1p3.c: Poređenje struktura

Iste operacije se izvršavaju nad sve tri strukture (niz, BST, Splay stablo), a ispisuje se prosečan broj pristupa po operaciji.

Meni:
1. Merenje iz jedne ili više datoteka
2. Merenje sa operacijama unetim sa tastature
0. Kraj

### Format ulaza

Svaka operacija je jedan zapis:

```
I kljuc    (umetanje)
D kljuc    (brisanje)
S kljuc    (pretraga)
```

Primer datoteke:

```
I 5
I 3
I 8
S 3
D 5
S 10
```

Pri unosu sa tastature merenje se završava slovom `K`. Neispravan zapis se prijavljuje i preskače. Pri merenju iz više datoteka, svaka datoteka se meri nezavisno, počev od praznih struktura, i daje jedan red tabele.

### Izlaz

```
Broj operacija | Niz, sa prebacivanjem | BST      | Splay stablo
            6 |                  2.50 |     2.33 |         9.17
```

Kolone prikazuju broj izvršenih operacija i prosečan broj pristupa po operaciji za svaku strukturu.

## Napomene o implementaciji

- BST kod je preuzet sa fajla na sajtu predmeta (`02_BinTree_src`), uz prevod imena na srpski, brojanje pristupa i izbacivanje rekurzije.
- Ako ponestane memorije u toku merenja, merenje se prekida uz poruku.
