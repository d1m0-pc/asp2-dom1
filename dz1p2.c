#include<stdio.h>
#include<stdlib.h>
struct Cvor {
    int kljuc;
    struct Cvor *levo,*desno,*otac;

};

struct Stablo {
    struct Cvor *koren;
    int broj; //broj kljuceva u stablu
};

int Unesi_broj(char poruka[]) {
    int x,r,c;
    printf("%s",poruka);
    r=scanf("%d",&x);
    for (;r!=1;) {
        if (r==EOF) exit(0);
        printf("Unesi ceo broj \n");
        for (c=getchar();c!='\n' && c!=EOF;c=getchar()) {}
        printf("%s",poruka);
        r=scanf("%d",&x);
    }
    return x;
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

//Upisuje kljuc kao tekst u buf (npr. -15 -> "-15"), vraca broj znakova
int Broj_u_tekst(int kljuc, char buf[]) {
    char cifre[20];
    long x=kljuc;
    int br=0,len=0,i;
    if (x<0) {
        buf[len]='-';
        len++;
        x=-x;
    }
    if (x==0) {
        cifre[br]='0';
        br++;
    }
    while (x>0) {
        cifre[br]='0'+x%10;
        br++;
        x=x/10;
    }
    for (i=br-1;i>=0;i--) {
        buf[len]=cifre[i];
        len++;
    }
    buf[len]=0;
    return len;
}

void Ispis(struct Stablo *t) {
    struct Cvor **red, *c, *p;
    int *dub;
    char *mreza, buf[20];
    int n,d,len,maxlen=0,h=0,w,rows,cols;
    if (t->koren==NULL) {
        printf("Prazno stablo\n");
        return;
    }
    if (t->broj>50) {
        printf("Ispis je moguc samo za stablo do 50 kljuceva\n");
        return;
    }
    n=t->broj;
    red=malloc(n*sizeof(struct Cvor *));
    dub=malloc(n*sizeof(int));
    if (red==NULL || dub==NULL) {
        printf("Nema memorije za ispis\n");
        free(red);
        free(dub);
        return;
    }
    //cvorovi u rastucem poretku, preko pokazivaca na oca
    c=t->koren;
    while (c->levo!=NULL) c=c->levo;
    for (int i=0;i<n;i++) {
        red[i]=c;
        if (c->desno!=NULL) {
            c=c->desno;
            while (c->levo!=NULL) c=c->levo;
        }else {
            while (c->otac!=NULL && c==c->otac->desno) c=c->otac;
            c=c->otac;
        }
    }
    for (int i=0;i<n;i++) {
        len=Broj_u_tekst(red[i]->kljuc,buf);
        if (len>maxlen) maxlen=len;
        d=0;
        for (p=red[i];p->otac!=NULL;p=p->otac) d++;
        dub[i]=d;
        if (d>h) h=d;
    }
    w=maxlen+1;
    rows=h+1;
    cols=n*w;
    mreza=malloc(rows*(cols+1));
    if (mreza==NULL) {
        printf("Nema memorije za ispis\n");
        free(red);
        free(dub);
        return;
    }
    for (int i=0;i<rows*(cols+1);i++) mreza[i]=' ';
    for (int i=0;i<n;i++) {
        len=Broj_u_tekst(red[i]->kljuc,buf);
        for (int j=0;j<len;j++) mreza[dub[i]*(cols+1)+i*w+j]=buf[j];
    }
    for (int i=0;i<rows;i++) {
        int x;
        for (x=cols-1;x>=0 && mreza[i*(cols+1)+x]==' ';x--) {}
        for (int j=0;j<=x;j++) printf("%c",mreza[i*(cols+1)+j]);
        printf("\n");
    }
    free(mreza);
    free(red);
    free(dub);
}

int main() {
struct Stablo *t = NULL;
    int i,n,izbor,kljuc,pristupi,uspeh;
    do {
        printf("1. Formiranje praznog stabla\n");
        printf("2. Umetanje kljuceva\n");
        printf("3. Pretraga kljuca\n");
        printf("4. Brisanje kljuca\n");
        printf("5. Brisanje stabla iz memorije\n");
        printf("6. Ispis stabla\n");
        printf("0. Kraj\n");
        izbor=Unesi_broj("Izbor: ");

        if (izbor==1) {
            if (t!=NULL) {
                printf("Stablo vec postoji \n");
            }else {
                t=malloc(sizeof(struct Stablo));
                if (t==NULL) {
                    printf("Nema memorije \n");
                }else {
                    t->koren = NULL;
                    t->broj = 0;
                    printf("Kreirano prazno stablo \n");
                }
            }
        }else if (izbor>=2 && izbor<=6) {
            if (t==NULL) {
                printf("stablo ne postoji prvo ga formirajte\n");
            }else if (izbor==2) {
                n=Unesi_broj("Koliko kljuceva umecemo ");
                while (n<0) n=Unesi_broj("Ne mozemo imati negativan broj kljuceva");
                for (i=0;i<n;i++) {
                    kljuc=Unesi_broj("Kljuc ");
                    pristupi=Umetni(t,kljuc,&uspeh);
                    if (uspeh<0) printf("Nema memorije\n");
                    else if (uspeh==0) printf("Kljuc vec postoji, nije umetnut \n");
                    printf("Broj pristupa %d\n",pristupi);
                }
            }else if (izbor==3) {
                kljuc=Unesi_broj("Kljuc ");
                pristupi=Pretraga(t,kljuc,&uspeh);
                if (uspeh) printf("Kljuc je pronadjen\n");
                else printf("Kljuc nije pronadjen\n");
                printf("Broj pristupa %d\n",pristupi);
            }else if (izbor==4) {
                kljuc=Unesi_broj("Kljuc ");
                pristupi=Obrisi_kljuc(t,kljuc,&uspeh);
                if (uspeh==0) printf("Kljuc ne postoji\n");
                printf("Broj pristupa %d\n",pristupi);
            }else if (izbor==5) {
                Obrisi_stablo(t);
                t=NULL;
                printf("Stablo je obrisano iz memorije\n");
            }else {
                Ispis(t);
            }
        }
        else if (izbor!=0) printf("Nepostojeca opcija\n");
    }while (izbor!=0);
    Obrisi_stablo(t);
    return 0;
}