#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

#define HASH_BOYUTU 100 // Hash tablosu boyutu

// Motor yapisi
typedef struct Motor
{
    char motorAdi[50];
    char model[20];
    int stokAdedi;
    struct Motor *sonraki;
} Motor;

// Bayi yapisi
typedef struct Bayi
{
    char bayiAdi[50];
    Motor *motorListesi;
    struct Bayi *sonraki;
} Bayi;

// sehir yapisi
typedef struct Sehir
{
    char sehirAdi[30];
    Bayi *bayiListesi;
    struct Sehir *sonraki;
} Sehir;

// Hash tablosu yapisi
typedef struct HashTablo
{
    Bayi *tablo[HASH_BOYUTU];
} HashTablo;

// Þehirlerin baðlý listesinin baþlangýcý
Sehir *sehirListesi = NULL;

// Hash tablosunun baþlangýcý
HashTablo *bayiHashTablosu = NULL;

// Hash fonksiyonu
int hashFonksiyonu(char *anahtar)
{
    int toplam = 0;
    for (int i = 0; anahtar[i] != '\0'; i++)
    {
        toplam += tolower(anahtar[i]);
    }
    return toplam % HASH_BOYUTU;
}

// Hash tablosu olusturma
HashTablo *hashTabloOlustur()
{
    HashTablo *hashTablo = (HashTablo *)malloc(sizeof(HashTablo));
    for (int i = 0; i < HASH_BOYUTU; i++)
    {
        hashTablo->tablo[i] = NULL;
    }
    return hashTablo;
}

// Hash tablosuna bayi ekleme
void hashTabloyaBayiEkle(HashTablo *hashTablo, Bayi *bayi)
{
    int index = hashFonksiyonu(bayi->bayiAdi);
    bayi->sonraki = hashTablo->tablo[index];
    hashTablo->tablo[index] = bayi;
}

// Hash tablosundan bayi arama
Bayi *hashTablodanBayiBul(HashTablo *hashTablo, char *bayiAdi)
{
    int index = hashFonksiyonu(bayiAdi);
    Bayi *bayi = hashTablo->tablo[index];
    while (bayi != NULL)
    {
        if (strcasecmp(bayi->bayiAdi, bayiAdi) == 0)
        {
            return bayi;
        }
        bayi = bayi->sonraki;
    }
    return NULL;
}

// Yeni bir motor dugumu olusturma
Motor *yeniMotorOlustur(char *motorAdi, char *model, int stokAdedi)
{
    Motor *yeniMotor = (Motor *)malloc(sizeof(Motor));
    strcpy(yeniMotor->motorAdi, motorAdi);
    strcpy(yeniMotor->model, model);
    yeniMotor->stokAdedi = stokAdedi;
    yeniMotor->sonraki = NULL;
    return yeniMotor;
}

// Yeni bir bayi dugumu olusturma
Bayi *yeniBayiOlustur(char *bayiAdi)
{
    Bayi *yeniBayi = (Bayi *)malloc(sizeof(Bayi));
    strcpy(yeniBayi->bayiAdi, bayiAdi);
    yeniBayi->motorListesi = NULL;
    yeniBayi->sonraki = NULL;
    return yeniBayi;
}

// Yeni bir sehir dugumu olusutrma
Sehir *yeniSehirOlustur(char *sehirAdi)
{
    Sehir *yeniSehir = (Sehir *)malloc(sizeof(Sehir));
    strcpy(yeniSehir->sehirAdi, sehirAdi);
    yeniSehir->bayiListesi = NULL;
    yeniSehir->sonraki = NULL;
    return yeniSehir;
}

// sehre bayi ekleme
void sehreBayiEkle(char *sehirAdi, char *bayiAdi)
{
    Sehir *sehir = sehirListesi;

    // sehir var mi kontrol et
    while (sehir != NULL && strcasecmp(sehir->sehirAdi, sehirAdi) != 0)
    {
        sehir = sehir->sonraki;
    }

    // sehir yoksa olustur ve listeye ekle
    if (sehir == NULL)
    {
        sehir = yeniSehirOlustur(sehirAdi);
        sehir->sonraki = sehirListesi;
        sehirListesi = sehir;
    }

    // Bayi ekle
    Bayi *yeniBayi = yeniBayiOlustur(bayiAdi);
    yeniBayi->sonraki = sehir->bayiListesi;
    sehir->bayiListesi = yeniBayi;

    // Bayiyi hash tablosuna ekle
    hashTabloyaBayiEkle(bayiHashTablosu, yeniBayi);
}

// Bayiye motor ekleme
void bayiyeMotorEkle(char *bayiAdi, char *motorAdi, char *model, int stokAdedi)
{
    Bayi *bayi = hashTablodanBayiBul(bayiHashTablosu, bayiAdi);
    if (bayi == NULL)
    {
        printf("Bayi bulunamadi!\n");
        return;
    }

    Motor *yeniMotor = yeniMotorOlustur(motorAdi, model, stokAdedi);
    yeniMotor->sonraki = bayi->motorListesi;
    bayi->motorListesi = yeniMotor;
}

// sehirdeki bayileri listeleme
void sehirdekiBayileriListele(char *sehirAdi)
{
    Sehir *sehir = sehirListesi;

    while (sehir != NULL && strcasecmp(sehir->sehirAdi, sehirAdi) != 0)
    {
        sehir = sehir->sonraki;
    }

    if (sehir == NULL)
    {
        printf("Bu sehirde bayi bulunamadi!\n");
        return;
    }

    printf("\n%s sehrindeki bayiler:\n", sehir->sehirAdi);
    Bayi *bayi = sehir->bayiListesi;
    int i = 1;

    while (bayi != NULL)
    {
        printf("%d. %s\n", i++, bayi->bayiAdi);
        bayi = bayi->sonraki;
    }
}

// Bayideki motorlari listeleme
void bayidekiMotorlariListele(char *bayiAdi)
{
    Bayi *bayi = hashTablodanBayiBul(bayiHashTablosu, bayiAdi);
    if (bayi == NULL)
    {
        printf("Bayi bulunamadi!\n");
        return;
    }

    printf("\n%s bayisindeki motorlar:\n", bayi->bayiAdi);
    Motor *motor = bayi->motorListesi;
    int j = 1;

    while (motor != NULL)
    {
        printf("%d. %s | Model: %s | Stok: %d\n", j++, motor->motorAdi, motor->model, motor->stokAdedi);
        motor = motor->sonraki;
    }
}

int main()
{
    setlocale(LC_ALL, "Turkish");

    // Hash tablosunu baslat
    bayiHashTablosu = hashTabloOlustur();

    // ornek bayi ve motor ekleme
    sehreBayiEkle("Ankara", "Ankara Bayi");
    sehreBayiEkle("Istanbul", "Istanbul Bayi");
    sehreBayiEkle("Istanbul", "Kadikoy Bayi");

bayiyeMotorEkle("Ankara Bayi", "Yamaha RayZR", "RayZR", 4);
bayiyeMotorEkle("Ankara Bayi", "Yamaha D'elight", "D'elight", 2);
bayiyeMotorEkle("Ankara Bayi", "Yamaha NMAX 125", "NMAX 125", 6);
bayiyeMotorEkle("Ankara Bayi", "Yamaha NMAX 155", "NMAX 155", 5);
bayiyeMotorEkle("Istanbul Bayi", "Yamaha Tricity 155", "Tricity 155", 3);
bayiyeMotorEkle("Kadikoy Bayi", "Yamaha Tricity 300", "Tricity 300", 2);

// Sport Scooter
bayiyeMotorEkle("Ankara Bayi", "Yamaha XMAX 250", "XMAX 250", 5);
bayiyeMotorEkle("Ankara Bayi", "Yamaha XMAX 250 Tech MAX", "XMAX 250 Tech MAX", 4);
bayiyeMotorEkle("Istanbul Bayi", "Yamaha XMAX 300 Tech MAX", "XMAX 300 Tech MAX", 2);
bayiyeMotorEkle("Kadikoy Bayi", "Yamaha TMAX Tech MAX", "TMAX Tech MAX", 1);

// Super Sport
bayiyeMotorEkle("Ankara Bayi", "Yamaha R125", "R125", 3);
bayiyeMotorEkle("Ankara Bayi", "Yamaha R25", "R25", 4);
bayiyeMotorEkle("Istanbul Bayi", "Yamaha R3", "R3", 2);
bayiyeMotorEkle("Kadikoy Bayi", "Yamaha R7", "R7", 2);
bayiyeMotorEkle("Kadikoy Bayi", "Yamaha R1", "R1", 1);

// Hyper Naked
bayiyeMotorEkle("Ankara Bayi", "Yamaha MT-125", "MT-125", 4);
bayiyeMotorEkle("Ankara Bayi", "Yamaha MT-25", "MT-25", 3);
bayiyeMotorEkle("Istanbul Bayi", "Yamaha MT-07 PURE", "MT-07 PURE", 5);
bayiyeMotorEkle("Kadikoy Bayi", "Yamaha MT-07", "MT-07", 6);
bayiyeMotorEkle("Kadikoy Bayi", "Yamaha MT-09 IV. NESÝL", "MT-09 IV. NESÝL", 2);
bayiyeMotorEkle("Kadikoy Bayi", "Yamaha MT-10", "MT-10", 1);

// Sport Touring
bayiyeMotorEkle("Ankara Bayi", "Yamaha Tracer 7", "Tracer 7", 3);
bayiyeMotorEkle("Istanbul Bayi", "Yamaha Tracer 9", "Tracer 9", 2);
bayiyeMotorEkle("Kadikoy Bayi", "Yamaha Tracer 9 GT", "Tracer 9 GT", 1);
bayiyeMotorEkle("Kadikoy Bayi", "Yamaha NIKEN GT", "NIKEN GT", 1);

// Sport Heritage
bayiyeMotorEkle("Ankara Bayi", "Yamaha XSR125", "XSR125", 5);
bayiyeMotorEkle("Istanbul Bayi", "Yamaha XSR700", "XSR700", 3);
bayiyeMotorEkle("Kadikoy Bayi", "Yamaha XSR700 XTribute", "XSR700 XTribute", 2);
bayiyeMotorEkle("Kadikoy Bayi", "Yamaha XSR900 GP", "XSR900 GP", 1);

// Adventure
bayiyeMotorEkle("Ankara Bayi", "Yamaha Ténéré 700", "Ténéré 700", 4);
bayiyeMotorEkle("Istanbul Bayi", "Yamaha Ténéré 700 World Raid", "Ténéré 700 World Raid", 3);

// Off Road Competition
bayiyeMotorEkle("Kadikoy Bayi", "Yamaha PW50", "PW50", 5);

    char sehirAdi[30];
    char bayiAdi[50];

    // Åžehir ve bayi seÃ§imini yap
    printf("Stok kontrol etmek istediginiz sehrin adini girin: ");
    fgets(sehirAdi, 30, stdin);
    sehirAdi[strcspn(sehirAdi, "\n")] = '\0';

    sehirdekiBayileriListele(sehirAdi);

    printf("\nStoklarini gormek istediginiz bayi adini yazin:");
    fgets(bayiAdi, 50, stdin);
    bayiAdi[strcspn(bayiAdi, "\n")] = '\0';

    bayidekiMotorlariListele(bayiAdi);

    return 0;
}
