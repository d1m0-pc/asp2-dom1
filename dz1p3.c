#include<stdio.h>
#include<stdlib.h>

//Niz sa promenjljivim kapacitetom i prebacivanje na pocetak
typedef struct {
    int *a;
    int n, cap;
}Niz;

//Splay stablo za cvor 1 pristup za rotaciju 3
struct Cvor {
    int kljuc;
    struct Cvor *levo,*desno,*otac;

};

struct Stablo {
    struct Cvor *koren;
    int broj; //broj kljuceva u stablu
};

//BST - kod preuzet sa fajla na sajtu predmeta (02_BinTree_src), imena prevedena na srpski, uz dodato brojanje pristupa cvorovima, *ok (1,0,-1), bez rekurzije
typedef struct cvorBST {
    int kljuc;
    struct cvorBST *otac, *levo, *desno;
} CvorBST;

typedef struct { CvorBST *koren; } BST;


//prebacivanje na pocetak deo//
//ako nema memorije prekida program
void *mem(void *p) {
    if (!p) {printf ("Nema memorije \n"); exit(1);}
    return p;
}
//pocetni unos Izbor:
int P_unos(const char *poruka) {
    int x,c,rezultat;
    printf("%s",poruka);
    rezultat=scanf("%d",&x);
    while (rezultat !=1) {
        if (rezultat == EOF) exit(0);
        printf("Unesi ceo broj");
        c=getchar();
        while (c!= '\n' && c!= EOF) c=getchar();
        printf("%s",poruka);
        rezultat=scanf("%d",&x);
    }
    return x;
}

Niz *Nov_niz(int cap) {
    Niz *s = (Niz*)malloc(sizeof(Niz));
    s->a = (int*)malloc(sizeof(int)*cap);
    s->n = 0;
    s->cap = cap;
    return s;
}
//trazi bez promene niza, vraca indeks ili -1
int Trazi_niz(Niz *s, int k,int *br) {
    *br =0;
    for (int i=0;i<s->n;i++) {
        (*br)++;
        if (s->a[i] == k) return i;
    }
    return -1;
}
//*ok je rez operacije, 1-dodato , 0 vec postoji
int Dodaj_niz(Niz *s, int k,int *ok) {
    int br;
    if (Trazi_niz(s,k,&br)>=0) {
        *ok =0;
        return br;
    }
    if (s->n == s->cap) {
        s->cap *=2;
        s->a = (int*)mem(realloc(s->a,s->cap*sizeof(int)));
    }
    s->a[s->n++] = k;
    *ok = 1;
    return br+1;
}
//*ok: 1 obrisano, 0 ne postoji
int Obrisi_niz(Niz *s, int k,int *ok) {
    int br, p = Trazi_niz(s, k, &br);
    if (p<0) {
        *ok =0;
        return br;
    }
    for (int i=p; i< s->n-1;i++) {
        s->a[i] = s->a[i+1];
    }
    s->n--;
    *ok = 1;
    return br+s->n-p;
}
//*poz : pozicija na kojoj je kljuc pronadjen ili -1 za ne
int Pretrazi_niz(Niz *s, int k,int *poz) {
    int br, p=Trazi_niz(s, k, &br);
    *poz = p;
    if (p<0) return s->n+1;
    if (p>0) {
        for (int i=p;i>0;i--) s->a[i] = s->a[i-1];
        s->a[0]=k;
        br+=p+1;
    }
    return br;
}
void Brisi_niz(Niz *s) {
    if (s) {
        free(s->a);
        free(s);
    }
}


//splay deo//
//rotacija cvor sin ide iznad svog oca
void rotiraj(struct Stablo *t, struct Cvor *x) {
    struct Cvor *p = x->otac; //p je otac cvora x
    struct Cvor *d = p->otac; //d je deda cvora x
    if (x== p->levo) {
        p->levo = x->desno;
        if (x->desno!=NULL) x->desno->otac = p;
        x->desno = p;
    }else {
        p->desno = x->levo;
        if (x->levo!=NULL) x->levo->otac = p;
        x->levo = p;
    }
    p->otac = x;
    x->otac = d;
    if (d==NULL) t->koren = x;
    else if (d->levo==p) d->levo = x;
    else d->desno = x;
}

//podizemo cvor sa rotacijama u koren(Splay)
int podigni(struct Stablo *t, struct Cvor *x) {
    int rotacija =0;
    struct Cvor *p,*d;
    while (x->otac !=NULL) {
        p=x->otac;
        d=p->otac;
        if (d==NULL) {                  //jednostruka rotacija (otac koren)
            rotiraj(t,x);
            rotacija++;
        }else if ((d->levo==p && p->levo ==x) || (d->desno==p && p->desno==x)) {
            rotiraj(t,p);               //dvostruka cik-cik
            rotiraj(t,x);
            rotacija+=2;
        }else {
            rotiraj(t,x);               //dvostruka cik-cak
            rotiraj(t,x);
            rotacija+=2;
        }
    }
    return rotacija;
}

// Trazi kljuc od korena nadole. Svaki posecen cvor je jedan pristup.
//Vraca cvor sa kljucem (ili NULL), a u *poslednji upisuje poslednji posecen cvor.
struct Cvor *trazi(struct Stablo *t,int kljuc, struct Cvor **poslednji, int *pristupi) {
    struct Cvor *c = t->koren;
    *poslednji=NULL;
    for (;c!=NULL;) {
        (*pristupi)++;
        *poslednji=c;
        if (kljuc==c->kljuc) return c;
        if (kljuc<c->kljuc) c=c->levo;
        else c=c->desno;
    }
    return NULL;
}

//uspeh = 1 umetnut, =0 vec postoji, -1 null
int Umetni(struct Stablo *t, int kljuc, int *uspeh) {
    int pristupi=0;
    struct Cvor *poslednji;
    struct Cvor *c = trazi(t,kljuc,&poslednji,&pristupi);
    if (c!=NULL) {
        *uspeh = 0;
        return pristupi+3 * podigni(t,c);
    }
    c = malloc(sizeof(struct Cvor));
    if (c==NULL) {
        *uspeh = -1;
        return pristupi;
    }
    c->kljuc = kljuc;
    c->levo = NULL;
    c->desno = NULL;
    c->otac = poslednji;
    if (poslednji == NULL) t->koren = c;
    else if (kljuc < poslednji->kljuc) poslednji->levo = c;
    else poslednji->desno = c;
    t->broj++;
    *uspeh = 1;
    return pristupi + 3 * podigni(t,c);
}

int Pretraga(struct Stablo *t, int kljuc, int *nadjen) {
    int pristupi=0;
    struct Cvor *poslednji;
    struct Cvor *c =trazi(t,kljuc,&poslednji,&pristupi);

    if (c!=NULL) *nadjen = 1;
    else *nadjen = 0;
    if (poslednji!=NULL) {
        pristupi +=3 * podigni(t,poslednji);
    }
    return pristupi;
}

//Brisemo cvor ako postoji, trazeni (ili njegov poslednji) se podize u koren.
//Podstabla se spajaju preko najveceg cvora levog podstabla(prethodnika).
int Obrisi_kljuc(struct Stablo *t, int kljuc, int *uspeh) {
    int pristupi=0;
    struct Cvor *poslednji, *levo, *desno,*c;
    c=trazi(t,kljuc,&poslednji,&pristupi);
    *uspeh = 0;
    if (poslednji==NULL) return pristupi;  //prazno stablo
    pristupi +=3 * podigni(t,poslednji);
    if (c==NULL) return pristupi;  //nema kljuca

    levo=c->levo;
    desno=c->desno;
    free(c);
    t->broj--;
    *uspeh = 1;
    if (levo==NULL) {
        t->koren = desno;
        if (desno!=NULL) desno->otac = NULL;
    }else {
        levo->otac = NULL;
        t->koren=levo;
        c=levo;
        pristupi++;
        while (c->desno!=NULL) {
            c= c->desno;
            pristupi++;
        }
        pristupi +=3 * podigni(t,c);
        c->desno = desno;
        if (desno !=NULL) desno->otac = c;
    }
    return pristupi;
}

//Oslobadjanje svih cvorova krecuci se od lista ka ocu
void Oslobodi(struct Cvor *c) {
    struct Cvor *otac;
    for (;c!=NULL;) {
        if (c->levo != NULL) c=c->levo;
        else if (c->desno != NULL) c=c->desno;
        else {
            otac=c->otac;
            if (otac!=NULL) {
                if (otac->levo==c) otac->levo=NULL;
                else otac->desno=NULL;
            }
            free(c);
            c=otac;
        }
    }
}

void Obrisi_stablo(struct Stablo *t) {
    if (t!=NULL) {
        Oslobodi(t->koren);
        free(t);
    }
}


//BST deo//
BST *Novo_BST(void)
{
    BST *bt = malloc(sizeof(BST));
    if (bt) bt->koren = NULL;
    return bt;
}

int Trazi_BST(BST *stablo, int kljuc, CvorBST **cvor, CvorBST **otac)
{
    int br = 0;
    *cvor = stablo->koren;
    *otac = NULL;
    while (*cvor) {
        br++;
        if ((*cvor)->kljuc == kljuc) break;
        *otac = *cvor;
        if ((*cvor)->kljuc > kljuc) *cvor = (*cvor)->levo;
        else *cvor = (*cvor)->desno;
    }
    return br;
}

int Umetni_BST(BST *stablo, int kljuc, int *ok)
{
    CvorBST *cvor, *otac, *novi;
    int br = Trazi_BST(stablo, kljuc, &cvor, &otac);
    if (cvor) { *ok = 0; return br; }
    novi = malloc(sizeof(CvorBST));
    if (!novi) { *ok = -1; return br; }
    novi->kljuc = kljuc;
    novi->levo = NULL;
    novi->desno = NULL;
    novi->otac = otac;
    if (otac) {
        if (otac->kljuc > kljuc) otac->levo = novi;
        else otac->desno = novi;
    } else stablo->koren = novi;
    *ok = 1;
    return br + 1;
}

CvorBST *Sledbenik(CvorBST *cvor, int *br)
{
    cvor = cvor->desno;
    (*br)++;
    while (cvor->levo) { cvor = cvor->levo; (*br)++; }
    return cvor;
}

void Povezi_oca(CvorBST *otac, CvorBST *cvor, CvorBST *sledeci)
{
    if (otac->levo == cvor) otac->levo = sledeci;
    else otac->desno = sledeci;
}

int Obrisi_BST_kljuc(BST *stablo, int kljuc, int *ok)
{
    CvorBST *cvor, *otac, *sledeci;
    int br = Trazi_BST(stablo, kljuc, &cvor, &otac);
    if (!cvor) { *ok = 0; return br; }
    if (cvor->levo && cvor->desno) {          /* dva potomka: zameni sledbenikom */
        CvorBST *sled = Sledbenik(cvor, &br);
        cvor->kljuc = sled->kljuc;
        cvor = sled;
        otac = sled->otac;
    }
    if (cvor->levo) sledeci = cvor->levo;
    else sledeci = cvor->desno;
    if (sledeci) sledeci->otac = otac;
    if (otac) Povezi_oca(otac, cvor, sledeci);
    else stablo->koren = sledeci;
    free(cvor);
    *ok = 1;
    return br + 2;
}

void Obrisi_BST(BST *bt)
{
    CvorBST *c, *otac;
    if (bt == NULL) return;
    c = bt->koren;
    while (c != NULL) {                     // brisanje od lista ka ocu
        if (c->levo != NULL) c = c->levo;
        else if (c->desno != NULL) c = c->desno;
        else {
            otac = c->otac;
            if (otac != NULL) {
                if (otac->levo == c) otac->levo = NULL;
                else otac->desno = NULL;
            }
            free(c);
            c = otac;
        }
    }
    free(bt);
}

void zaglavlje(void)
{
    printf("\nBroj operacija | Niz, sa prebacivanjem | BST      | Splay stablo\n");
}

// Izvrsava operacije "O kljuc" iz f nad sve tri (prazne) strukture i ispisuje jedan red.
//tast = 1: ulaz sa tastature, unos se zavrsava slovom K.
void merenje(FILE *f, int tast)
{
    Niz *a = Nov_niz(16);
    BST *b = Novo_BST();
    struct Stablo *s = malloc(sizeof(struct Stablo));
    CvorBST *c, *p;
    long n = 0, za = 0, zb = 0, zs = 0;
    char op;
    int k, rezultat, ch, dobar, greska = 0, ok1 = 1, ok2 = 1, ok3 = 1, poz;
    int x1 = 0, x2 = 0, x3 = 0;

    if (s != NULL) {
        s->koren = NULL;
        s->broj = 0;
    }
    if (!a || !b || !s) {
        printf("Greska: nema memorije.\n");
    } else {
        if (tast) printf("Operacija (I/D/S kljuc, K = kraj): ");
        rezultat = fscanf(f, " %c", &op);
        while (rezultat == 1 && !(tast && op == 'K') && !greska) {
            dobar = (op == 'I' || op == 'D' || op == 'S') && fscanf(f, "%d", &k) == 1;
            if (!dobar) {
                printf("Greska: neispravan zapis operacije, preskocen.\n");
                while ((ch = getc(f)) != '\n' && ch != EOF) ;
            } else {
                if (op == 'I') {
                    x1 = Dodaj_niz(a, k, &ok1);
                    x2 = Umetni_BST(b, k, &ok2);
                    x3 = Umetni(s, k, &ok3);
                } else if (op == 'D') {
                    x1 = Obrisi_niz(a, k, &ok1);
                    x2 = Obrisi_BST_kljuc(b, k, &ok2);
                    x3 = Obrisi_kljuc(s, k, &ok3);
                } else {
                    x1 = Pretrazi_niz(a, k, &poz);
                    x2 = Trazi_BST(b, k, &c, &p);
                    x3 = Pretraga(s, k, &ok3);
                }
                if (ok1 < 0 || ok2 < 0 || ok3 < 0) {
                    printf("Greska: nema memorije, merenje je prekinuto.\n");
                    greska = 1;
                } else {
                    za += x1;
                    zb += x2;
                    zs += x3;
                    n++;
                }
            }
            if (tast) printf("Operacija (I/D/S kljuc, K = kraj): ");
            rezultat = fscanf(f, " %c", &op);
        }
        if (n > 0) printf("%14ld | %21.2f | %8.2f | %12.2f\n", n, (double)za / n, (double)zb / n, (double)zs / n);
        else printf("Nema operacija za merenje.\n");
    }
    Brisi_niz(a);
    Obrisi_BST(b);
    Obrisi_stablo(s);
}

int main(void)
{
    int izbor, br, i;
    char (*imena)[200];
    FILE *f;

    do {
        printf("\n1. Merenje iz jedne ili vise datoteka\n2. Merenje - operacije sa tastature\n0. Kraj\n");
        izbor = P_unos("Izbor: ");
        if (izbor == 1) {
            br = P_unos("Broj datoteka: ");
            imena = NULL;
            if (br > 0) imena = malloc(br * sizeof(*imena));
            if (br > 0 && !imena) printf("Greska: nema memorije.\n");
            else {
                for (i = 0; i < br; i++) {
                    printf("Ime datoteke %d: ", i + 1);
                    scanf("%199s", imena[i]);
                }
                zaglavlje();
                for (i = 0; i < br; i++) {
                    f = fopen(imena[i], "r");
                    if (!f) { printf("Greska: datoteka %s se ne moze otvoriti.\n", imena[i]); continue; }
                    merenje(f, 0);
                    fclose(f);
                }
                free(imena);
            }
        } else if (izbor == 2) {
            zaglavlje();
            merenje(stdin, 1);
        } else if (izbor != 0) printf("Greska: nepostojeca opcija.\n");
    } while (izbor != 0);
    return 0;
}