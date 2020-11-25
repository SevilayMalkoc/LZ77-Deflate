#include <stdio.h>
#include <stddef.h> // NULL
#include <stdlib.h> // calloc
#include <stdint.h>
#include <stdint.h>
#include <string.h>
#define OFFSETBITS 5
#define LENGTHBITS (8-OFFSETBITS)
#define OFFSETMASK ((1 << (OFFSETBITS)) - 1)
#define LENGTHMASK ((1 << (LENGTHBITS)) - 1)
#define GETOFFSET(x) (x >> LENGTHBITS)
#define GETLENGTH(x) (x & LENGTHMASK)
#define OFFSETLENGTH(x,y) (x << LENGTHBITS | y)
struct dugum
{

    unsigned char karakter[1000];
    unsigned char encodedosya[1000];
    struct dugum *next;

};
struct dugum* start = NULL;


struct dugum* dugumOlustur(char *harf[],char *encode[])
{
    //printf("aaaaaaaa");
    struct dugum* node = (struct dugum*)malloc(sizeof(struct dugum));

    strcpy(node->karakter,harf);
    strcpy(node->encodedosya,encode);
    node->next = NULL;

    return node;
}
void sonaEkle(char *harf[],char *encode[])
{
    //printf("aaaaaaaa");
    struct dugum* sonaEklenecek = dugumOlustur(harf,encode);
    //Eðer listede hiç eleman yoksa yeni eleman ekliyoruz
    if (start == NULL)
    {
        start = sonaEklenecek;
        // printf("%d",start->plaka);

    }
    else
    {
        struct dugum* temp = start;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = sonaEklenecek;
    }

}

void metinyazdirdosya(char metin[],int boyut)
{
   // printf("metinyazdirdosya\n\n\n\n");
    // printf("%s",&metin[0]);
    FILE *dosya = fopen ("deflatecikti.txt", "wb" );

    struct dugum* gecici = start;
    printf("DEFLATE(huffman) ile encode edilmis metin:\n");
    for(int i=0; i<boyut; i++)
    {
        while (gecici != NULL)
        {
            // printf("aaaaaaaa");
            //printf("%c",gecici->karakter[0]);
            if(metin[i]==gecici->karakter[0])
            {
                printf("%s",gecici->encodedosya);
                fprintf(dosya, "%s ",gecici->encodedosya);
            }
            //  printf(" %s-> %s\n", gecici->karakter,gecici->encodedosya);
            gecici = gecici->next;
        }
        gecici = start;
    }

      fclose(dosya);

}

typedef struct huffmann
{
    char c;
    int frekans;
    struct huffmann *left;
    struct huffmann *right;
} HuffmanAgaci;

typedef struct huffmann_pointeray
{
    int cap;
    int boyut;
    HuffmanAgaci **items;
} HUFMANNARRAY;

HuffmanAgaci *HuffmanAgaci_new(char c, int frekans)
{
    HuffmanAgaci *t = malloc(sizeof(HuffmanAgaci));
    t->c = c;
    t->frekans = frekans;
    t->left = NULL;
    t->right = NULL;
    return t;
}


/*
* Selection sort, büyükten küçüğe
*/
void SelectionSort(HUFMANNARRAY *pointer)
{
    int i, k,temp;
    int max_index, max_value;


    for (i = 0; i < pointer->boyut - 1; i++)
    {
        max_index = i;
        max_value = pointer->items[i]->frekans;

        for (k = i + 1; k < pointer->boyut; k++)
        {
            if (pointer->items[k]->frekans > max_value)
            {
                max_value = pointer->items[k]->frekans;
                max_index = k;
            }
        }

        if (i != max_index)
        {
            //printf("%d\n",pointer->items[max_index]);
            temp= pointer->items[i];
            pointer->items[i] = pointer->items[max_index];
            pointer->items[max_index] =temp;
        }

    }
}

HuffmanAgaci *hufmann_dizi_pop(HUFMANNARRAY *pointer)
{
    return pointer->items[--pointer->boyut];
}

void *hufmann_dizi_ekle(HUFMANNARRAY *pointer, HuffmanAgaci *t)
{
    if (pointer->boyut + 1 == pointer->cap)
    {
        pointer->cap *= 2;
        pointer->items = realloc(pointer->items, pointer->cap * sizeof(HuffmanAgaci *));
    }

    pointer->items[pointer->boyut++] = t;
}

HUFMANNARRAY *hufmann_dizi_olustur()
{
    HUFMANNARRAY *pointer = malloc(sizeof(HUFMANNARRAY));
    pointer->cap = 8;
    pointer->boyut = 0;
    pointer->items = malloc(pointer->cap * sizeof(HuffmanAgaci *) );
    return pointer;
}


void yazdir(HuffmanAgaci *t, char *prefix, int boyut_prefix,char *harf[],char *encoded[])
{
    int i=0,k=0;
    //char *harf[boyut_prefix],*encoded[boyut_prefix];
    if (t->left == NULL && t->right == NULL)
    {
        prefix[boyut_prefix] = 0;
        printf("%c: %s\n", t->c, prefix);

        harf[i]=t->c;
        encoded[i]=prefix;
        sonaEkle(harf,prefix);
        // strcpy(encoded[i],prefix);
        //printf("%c %s\n",harf[i],encoded[i]);
        i++;

        //return encoded,harf;
    }

    if (t->left)
    {
        prefix[boyut_prefix++] = '0';
        yazdir(t->left, prefix, boyut_prefix,&harf,&encoded);
        boyut_prefix--;
    }

    if (t->right)
    {
        prefix[boyut_prefix++] = '1';
        yazdir(t->right, prefix, boyut_prefix,&harf,&encoded);
        boyut_prefix--;
    }

}

int frekansBasamakBul(char metin[],int boyut,int token_sayisi)
{
    //frekans bulma
    struct dugum* deneme = start;
    float count;
    int i=0;
    int *basamakSayisi,boyut2=0;
    printf("\n");
    while(deneme != NULL)
    {
        boyut2++;
        deneme = deneme->next;
    }

    int x[boyut2];
    int x2[boyut2];
    deneme=start;

    while(deneme!= NULL)
    {
        x[i]=0;
        for(int j=0; j<boyut; j++)
        {
            if(metin[j]==deneme->karakter[0])
            {
                x[i]++;
                x2[i]=deneme->karakter[0];

            }
            // printf(" %s-> %s\n", gecici->karakter,gecici->encodedosya);
        }
        i++;
        deneme = deneme->next;

    }
    deneme = start;
    i=0;
    printf("frekansBul\n");
    while(deneme != NULL)
    {
        printf("%c harfi %d kere girilmistir...\n",x2[i],x[i]); //Çıktımızı veriyor.
        i++;
        deneme = deneme->next;
    }
//   basamakSayisi=basamakBul();
 int basamaks=1;
    struct dugum* temp = start;
    int a=boyutBul();
    int xBasamak[a];
    printf("\n");

        i=0;
        while (temp != NULL)
        {

                xBasamak[i]=strlen(temp->encodedosya);
                // printf("%d",strlen(temp->encodedosya));
                // printf("%d",x[i]);
              temp = temp->next;   // printf("%s",temp->encodedosya);
              i++;
        }
            // printf(" %s-> %s\n", gecici->karakter,gecici->encodedosya);


    printf("BasamakBul\n");

    for(int i=0; i<boyut2; i++)
    {
        printf("%d",xBasamak[i]);
    }

    //boyut hesapmala
    int toplamBoyut=0;
   for(int i=0; i<boyut2; i++)
    {
        toplamBoyut+=x[i]*xBasamak[i];

    }

    //printf("\n\nSikistirilmis Deflate Dosya Boyutu: %d\n",toplamBoyut);
    //Normal Dosya BOyutu
    int normalBoyut=0;
    for(int i=0;i<boyut2;i++)
    {
        normalBoyut+=x[i];
    }
   // printf("frekans toplami %d",normalBoyut*6);
        normalBoyut=normalBoyut*8;
    printf("\n\nDosyanin normal boyutu: %d",normalBoyut);
    printf("\nDeflate(hufmann) Dosya Boyutu: %d\n",toplamBoyut);
    float sikisma=100-((toplamBoyut*100)/normalBoyut);
    printf("Deflate(huffman) sikistima orani :%%%.1f\n",sikisma);
    float sikisma2=100-((token_sayisi*100)/normalBoyut);
     printf("Encode sikistima orani :%%%.1f\n",sikisma2);

}


struct token {
    uint8_t offset_len;
    char c;
};


/*
* iki string'in ilk kaç karakteri özdeþ?
* maksimum limit sayýsý kadar kontrol yapar.
*/
 int prefix_match_length(char *s1, char *s2, int limit)
{
    int len = 0;

    while (*s1++ == *s2++ && len < limit)
        len++;

    return len;
}

/*
* memcpy fonksiyonu ile benzer. Byte düzeyinde
* kopyalama yapma garantisi olduðu için, bu
* versiyonu kullanýyoruz.
*/
void lz77memcpy(char *s1, char *s2, int size)
{
    while (size--)
        *s1++ = *s2++;
}

/*
* token array'ini, karakter array'ine dönüþtürür.
*/
/*
* LZ77 ile sýkýþtýrýlacak bir metin alýr.
* token array'i döndürür.
* Eðer numTokens NULL deðilse, token sayýsýný
* numTokens ile gösterilen yere kaydeder.
*/
struct token *encode(char *text, int limit, int *numTokens)
{
    // cap (kapasite) hafýzada kaç tokenlik yer ayýrdýðýmýz.
    int cap = 1 << 3;

    // kaç token oluþturduðumuz.
    int _numTokens = 0;

    // oluþturulacak token
    struct token t;

    // lookahead ve search buffer'larýnýn baþlangýç pozisyonlarý
    char *lookahead, *search;

    // tokenler için yer ayýr.
    struct token *encoded = malloc(cap * sizeof(struct token));

    // token oluþturma döngüsü
    for (lookahead = text; lookahead < text + limit; lookahead++)
    {
        // search buffer'ý lookahead buffer'ýn 31 (OFFSETMASK) karakter
        // gerisine koyuyoruz.
        search = lookahead - OFFSETMASK;

        // search buffer'ýn metnin dýþýna çýkmasýna engel ol.
        if (search < text)
            search = text;

        // search bufferda bulunan en uzun eþleþmenin
        // boyutu
        int max_len = 0;

        // search bufferda bulunan en uzun eþleþmenin
        // pozisyonu
        char *max_match = lookahead;

        // search buffer içinde arama yap.
        for (; search < lookahead; search++)
        {
            int len = prefix_match_length(search, lookahead, LENGTHMASK);
           // printf("%d\n",len);

            if (len > max_len)
            {
                max_len = len;
                //printf("%d\n",max_len);
                max_match = search;
            }
        }

        /*
        * ! ÖNEMLÝ !
        * Eðer eþleþmenin içine metnin son karakteri de dahil olmuþsa,
        * tokenin içine bir karakter koyabilmek için, eþleþmeyi kýsaltmamýz
        * gerekiyor.
        */
        if (lookahead + max_len >= text + limit)
        {
            max_len = text + limit - lookahead - 1;
            printf("%d\n",max_len);
        }


        // bulunan eþleþmeye göre token oluþtur.
        t.offset_len = OFFSETLENGTH(lookahead - max_match, max_len);
        lookahead += max_len;
        t.c = *lookahead;

        // gerekirse, hafýzada yer aç
        if (_numTokens + 1 > cap)
        {
            cap = cap << 1;
            encoded = realloc(encoded, cap * sizeof(struct token));
        }

        // oluþturulan tokeni, array'e kaydet.
        encoded[_numTokens++] = t;
    }

    if (numTokens)
        *numTokens = _numTokens;

    return encoded;
}

// bir dosyanýn tamamýný tek seferde
// okur. Büyük dosyalarý okumak için uygun
// deðildir.

int boyutBul()
{
    //Önce dosyayı okuma modunda ve binary olarak açarız...
    FILE *dosya = fopen("metin.txt", "rb");
    //dosyanın sonuna gideriz.
    fseek(dosya,0,SEEK_END);
    //pozisyonumuzu söyleriz.
    //printf("Bu dosya %d byte boyutundadir.\n",ftell(dosya));
    //başka bir halt yapmayacaksak dosyayı kapatırız.
    // fclose(dosya);
    return ftell(dosya);
}

int main(void)
{
    FILE *f;
    FILE *myfile = fopen ( "metin.txt", "rb" );
     unsigned long frekansDizi[0xFF];
    int token_sayisi,a;
    char *decoded_metin = "";
    struct token *encoded_metin;
    char *harf,*encoded;
    int metin_boyutu;

    metin_boyutu=boyutBul();
    char kaynak_metin[metin_boyutu],metin[metin_boyutu];
    // = "bu oylesine uzun, oylesine girift bir metin ki, muhakkak gereksiz bitlerini temizlemek gerekir.";
    char *pcTemp,*myInt;
    int i,n=0;


    while (!feof(myfile))
    {
        //myInt=fgetc(myfile);
        fscanf(myfile, "%c", &myInt);
        // printf("%c",myInt);
        kaynak_metin[n]=myInt;
        metin[n]=myInt;
        n++;
        //  metin = realloc(metin, n+1* boyutof(char));
    }
    kaynak_metin[metin_boyutu-1]='\0';
    metin[metin_boyutu-1]='\0';
    // printf("\n");


    encoded_metin = encode(kaynak_metin, metin_boyutu, &token_sayisi);

    if (f = fopen("lz77cikti.txt", "wb"))
    {
       fwrite(encoded_metin, sizeof(struct token), token_sayisi, f);

        fclose(f);
    }


     HUFMANNARRAY *pointer = hufmann_dizi_olustur();

    memset(&frekansDizi[0], 0, sizeof(frekansDizi));

    for (pcTemp = &metin[0]; *pcTemp != 0; pcTemp++)
    {
        frekansDizi[(int)*pcTemp]++;

    }

    for (i = 0; i < 255; i++)
    {
        if (frekansDizi[i] > 0)
        {
            hufmann_dizi_ekle(pointer, HuffmanAgaci_new(i, frekansDizi[i]));
        }
    }

    while (pointer->boyut > 1)
    {
        SelectionSort(pointer);
        HuffmanAgaci *t1 = hufmann_dizi_pop(pointer);
        HuffmanAgaci *t2 = hufmann_dizi_pop(pointer);
        HuffmanAgaci *t3 = calloc(1, sizeof(HuffmanAgaci));
        t3->left = t1;
        t3->right = t2;
        t3->frekans = t1->frekans + t2->frekans;
        hufmann_dizi_ekle(pointer, t3);

    }

    char *buffer = malloc(256);
     printf("Deflate(huffman) harf karsiliklari:\n");
    yazdir(pointer->items[0], buffer, 0,&harf,&encoded);
   // yazdirdosya();
    metinyazdirdosya(&metin,metin_boyutu);
    frekansBasamakBul(&metin,metin_boyutu,token_sayisi);
      printf("LZ77 Encode Boyutu: %d\n",token_sayisi );

    // metinYazdir(&metin,a,0,&harf,&encoded);


    return 0;
}
