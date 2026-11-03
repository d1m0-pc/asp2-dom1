#include<stdio.h>
#include<stdlib.h>
#include<time.h>
//Prvi zadatak:Promenjljiv niz i prebacivanje na pocetak
//Povratna vrednost je broj pristupa nizu
//
//ako nema memorije prekida program
void *mem(void *p) {
    if (!p) {printf ("Nema memorije \n"); exit(1);}
    return p;
}
//pocetni unos Izbor:
int P_unos(const char *poruka) {
    int x, c, rezultat;

    printf("%s", poruka);
    fflush(stdout);
    rezultat = scanf("%d", &x);
    while (rezultat != 1) {
        if (rezultat == EOF) exit(0);
        printf("Unesi ceo broj\n");
        c = getchar();
        while (c != '\n' && c != EOF) c = getchar();
        printf("%s", poruka);
        fflush(stdout);
        rezultat = scanf("%d", &x);
    }
    return x;
}
typedef struct {
    int *a;
    int n,cap;
}Niz;

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
void Ispisi_niz(Niz *s) {
    printf("Niz (n=%d, kapacitet=%d) ", s->n, s->cap);
    for (int i=0;i<s->n;i++) {
        printf("%d ", s->a[i]);
    }
    printf("\n");
}
void Brisi_niz(Niz *s) {
    if (s) {
        free(s->a);
        free(s);
    }
}
//unos niza
Niz *Unesi_niz(FILE *f) {
    Niz *s;
    int n,k,ok,cap,r;
    if (f) {
        if (fscanf(f, "%d",&cap)!=1 || cap<1) {
            printf("Nije ispravan kapacitet\n");
            return NULL;
        }
    }else {
            cap=P_unos("Pocetni kapacitet: ");
            while (cap < 1) cap=P_unos("Mora biti >= 1 ");
        }
        s=Nov_niz(cap);
        if (f) {
            while ((r=fscanf(f,"%d",&k))==1) {
                Dodaj_niz(s,k,&ok);
                if (!ok) printf("kljuc %d se ponavlja \n",k);
            }
            if (r!=EOF) {
                printf("Greska u datoteci, ucitano sve do greske \n");
            }
        }else {
            n=P_unos("Broj kljuceva (0 je prazan niz )");
            while (n<0) n=P_unos("Mora biti >= 1 ");
            for (int i=0;i<n;i++) {
                k=P_unos("kljuc: ");
                Dodaj_niz(s,k,&ok);
                if (!ok) printf("kljuc %d se ponavlja \n",k);
            }
        }
    return s;
}
//generisanje n slucajnih kljuceva pomocu lo hi
Niz *Generisi_niz() {
    Niz *s;
    int n,k,cap,lo=0, hi=0, ok;
    cap=P_unos("Pocetni kapacitet: ");
    while (cap < 1) cap=P_unos("Mora biti >= 1 ");
    n=P_unos("Broj kljuceva (0 je prazan niz ): ");
    while (n<0) n=P_unos("Mora biti >= 1 ");
    if (n>0) {
        lo=P_unos("Donja granica : ");
        hi=P_unos("Gornja granica : ");
        for (;hi-lo+1 < n;) {
            printf("Nema %d razlicitih brojeva na tom opsegu \n",n);
            lo=P_unos("Donja granica : ");
            hi=P_unos("Gornja granica : ");
        }
    }
    s=Nov_niz(cap);
    while (s->n < n) {
        k=lo + rand() % (hi-lo+1);
        Dodaj_niz(s,k,&ok); //ponovljen kljuc se obija pa bira novi
    }
    return s;
}
int main() {
    Niz *s = NULL, *novi;
    int izbor,br,k,ok,poz;
    char ime[100];
    FILE *f;

    srand(time(NULL));
    for (;;) {
        printf("\n1. Unos niza sa tastature\n2. Unos niza iz datoteke\n3. Generisanje niza\n"
                   "4. Dodavanje kljuca (na kraj)\n5. Brisanje kljuca\n6. Pretraga kljuca\n"
                   "7. Ispis niza\n8. Brisanje niza iz memorije\n0. Kraj\n");
        izbor= P_unos("Izbor: ");
        if (izbor==0) break;
        if (izbor>=1 && izbor<=3) {
            f=NULL;
            if (izbor==2) {
                printf("Ime datoteke: \n");
                scanf("%99s",ime);
                f=fopen(ime,"r");
                if (!f) {
                    printf("Datoteka se ne moze otvoriti \n");
                    continue;
                }
            }
                if (izbor==3) novi=Generisi_niz();
                else novi=Unesi_niz(f);
                if (f) fclose(f);
                if (novi) {
                    Brisi_niz(s);
                    s=novi;
                    Ispisi_niz(s);
                }
        }
        else if (izbor>=4 && izbor<=8) {
            if (!s) {
                printf("Niz ne postoji, dodaj ga \n");
                continue;
            }
            if (izbor==4) {
                k=P_unos("kljuc: ");
                br=Dodaj_niz(s,k,&ok);
                if (!ok) printf("kljuc vec postoji \n");
                printf("Broj pristupa %d\n",br);
            }else if (izbor==5) {
                k=P_unos("kljuc: ");
                br=Obrisi_niz(s,k,&ok);
                if (!ok) printf("kljuc ne postoji \n");
                printf("Broj pristupa %d\n",br);
            }else if (izbor==6) {
                k=P_unos("kljuc: ");
                br=Pretrazi_niz(s,k,&poz);
                if (poz < 0) printf("kljuc nije pronadjen \n");
                else printf("Kljuc je pronadjen na poziciji %d i prebacen na pocetak. \n",poz);
                printf("Broj pristupa %d\n",br);
            }else if (izbor==8) {
                Brisi_niz(s);
                s=NULL;
                printf("Niz je obrisan iz memorije\n");
                continue;
            }
            Ispisi_niz(s);
        }else if (izbor!=0) printf("Nepostojeca opcija \n");
    }
    Brisi_niz(s);
    return 0;
}