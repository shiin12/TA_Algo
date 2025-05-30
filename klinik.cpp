#include <iostream>
using namespace std;

struct Pasien
{
    char nama[50];
    int usia;
    char keluhan[100];
    Pasien *next;
    Pasien *prev;
};

Pasien *head = nullptr;
Pasien *tail = nullptr;

void simpanKeFile(Pasien *p)
{
    FILE *file = fopen("riwayat_pasien.dat", "ab");
    if (!file)
    {
        cout << "Gagal membuka file.\n";
        return;
    }
    fwrite(p, sizeof(Pasien) - 2 * sizeof(Pasien *), 1, file);
    fclose(file);
}

void tambahPasien()
{
    Pasien *baru = new Pasien;
    cout << "Nama: ";
    cin.ignore();
    cin.getline(baru->nama, 50);
    cout << "Usia: ";
    cin >> baru->usia;
    cin.ignore();
    cout << "Keluhan: ";
    cin.getline(baru->keluhan, 100);

    baru->next = nullptr;
    baru->prev = tail;

    if (!head)
    {
        head = tail = baru;
    }
    else
    {
        tail->next = baru;
        tail = baru;
    }

    cout << "Pasien berhasil ditambahkan.\n";
}

void periksaPasien()
{
    if (!head)
    {
        cout << "Tidak ada pasien dalam antrian.\n";
        return;
    }

    Pasien *p = head;
    cout << "\nMemeriksa pasien:\n";
    cout << "Nama: " << p->nama << ", Usia: " << p->usia << ", Keluhan: " << p->keluhan << endl;

    simpanKeFile(p);

    head = head->next;
    if (head)
        head->prev = nullptr;
    else
        tail = nullptr;

    delete p;
    cout << "Pasien selesai diperiksa dan disimpan ke riwayat.\n";
}

void tampilkanPasien()
{
    if (!head)
    {
        cout << "Antrian kosong.\n";
        return;
    }

    Pasien *temp = head;
    cout << "\n=== Daftar Pasien dalam Antrian ===\n";
    while (temp)
    {
        cout << "Nama: " << temp->nama << ", Usia: " << temp->usia << ", Keluhan: " << temp->keluhan << endl;
        temp = temp->next;
    }
}

void cariPasien()
{
    if (!head)
    {
        cout << "Antrian kosong.\n";
        return;
    }

    char namaCari[50];
    cout << "Masukkan nama pasien: ";
    cin.ignore();
    cin.getline(namaCari, 50);

    Pasien *temp = head;
    while (temp)
    {
        bool cocok = true;
        for (int i = 0; i < 50; i++)
        {
            if (namaCari[i] != temp->nama[i])
            {
                cocok = false;
                break;
            }
            if (namaCari[i] == '\0' && temp->nama[i] == '\0')
                break;
        }
        if (cocok)
        {
            cout << "Pasien ditemukan:\n";
            cout << "Nama: " << temp->nama << ", Usia: " << temp->usia << ", Keluhan: " << temp->keluhan << endl;
            return;
        }
        temp = temp->next;
    }

    cout << "Pasien tidak ditemukan.\n";
}

void tukarNode(Pasien *a, Pasien *b)
{
    if (a == b)
        return;

    // Tukar posisi pointer a dan b
    Pasien *prevA = a->prev;
    Pasien *nextB = b->next;

    if (a->next != b)
    {
        Pasien *nextA = a->next;
        Pasien *prevB = b->prev;

        if (prevA)
            prevA->next = b;
        if (nextA)
            nextA->prev = b;
        if (prevB)
            prevB->next = a;
        if (nextB)
            nextB->prev = a;

        a->next = nextB;
        a->prev = prevB;
        b->next = nextA;
        b->prev = prevA;
    }
    else
    {
        if (prevA)
            prevA->next = b;
        if (nextB)
            nextB->prev = a;

        a->next = nextB;
        a->prev = b;
        b->next = a;
        b->prev = prevA;
    }

    // Perbarui head dan tail jika perlu
    if (head == a)
        head = b;
    else if (head == b)
        head = a;

    if (tail == a)
        tail = b;
    else if (tail == b)
        tail = a;
}

void urutkanUsia()
{
    if (!head || !head->next)
        return;

    bool swapped;
    do
    {
        swapped = false;
        Pasien *curr = head;

        while (curr && curr->next)
        {
            if (curr->usia > curr->next->usia)
            {
                tukarNode(curr, curr->next);
                swapped = true;
                break; // restart dari head setelah tukar
            }
            else
            {
                curr = curr->next;
            }
        }
    } while (swapped);

    cout << "Antrian berhasil diurutkan berdasarkan usia.\n";
}

int main()
{
    int pilihan;
    do
    {
        cout << "\n=== Sistem Antrian Klinik ===\n";
        cout << "1. Tambah Pasien\n";
        cout << "2. Periksa Pasien\n";
        cout << "3. Tampilkan Antrian\n";
        cout << "4. Cari Pasien\n";
        cout << "5. Urutkan Berdasarkan Usia\n";
        cout << "0. Keluar\n";
        cout << "Pilihan: ";
        cin >> pilihan;

        switch (pilihan)
        {
        case 1:
            tambahPasien();
            break;
        case 2:
            periksaPasien();
            break;
        case 3:
            tampilkanPasien();
            break;
        case 4:
            cariPasien();
            break;
        case 5:
            urutkanUsia();
            break;
        case 0:
            cout << "Terima kasih!\n";
            break;
        default:
            cout << "Pilihan tidak valid.\n";
        }
    } while (pilihan != 0);

    return 0;
}