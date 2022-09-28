// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include "string.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

static const int bitsInElem = 8;
static const int shiftSize = 3;

TBitField::TBitField(int len)
{
    if (len <= 0)
        throw invalid_argument("index below zero");
    BitLen = len;
    MemLen = (len + bitsInElem - 1) >> shiftSize;
    pMem = new TELEM[MemLen];
    memset(pMem, 0, MemLen * sizeof(TELEM));
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));
}

TBitField::~TBitField()
{
    delete [] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n >> shiftSize;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1 << (n & (bitsInElem - 1));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if(n >= BitLen || n < 0)
        throw out_of_range("bit n is out of range");
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if(n >= BitLen || n < 0)
        throw out_of_range("bit n is out of range");
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if(n >= BitLen || n < 0)
        throw out_of_range("bit n is out of range");
    return pMem[GetMemIndex(n)] & GetMemMask(n);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if(this == &bf)
        return *this;
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    delete []pMem;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
//    TBitField tmp(bf);
//    swap(*this, tmp);
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if(BitLen != bf.BitLen)
        return 0;

    for (int i = 0; i < MemLen; i++) {
        if(pMem[i] != bf.pMem[i])
            return 0;
    }
  return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if(*this == bf)
        return 0;
  return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
//    if(BitLen != bf.BitLen)
//        throw out_of_range("TBitField objects have different size");
//
//    TBitField tmp(*this);
//    for (int i = 0; i < bf.MemLen; i++) {
//        tmp.pMem[i] |= bf.pMem[i];
//    }
    int max = BitLen > bf.BitLen ? BitLen : bf.BitLen;
    TBitField tmp(max);

    int i = 0;
    while(i < BitLen && i < bf.BitLen){
        tmp.pMem[i] = pMem[i] | bf.pMem[i];
        i++;
    }
    while(i < BitLen){
        tmp.pMem[i] = pMem[i];
        i++;
    }
    while(i < bf.BitLen){
        tmp.pMem[i] = bf.pMem[i];
        i++;
    }
    return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
//    if(BitLen != bf.BitLen)
//        throw out_of_range("TBitField objects have different size");
//
//    TBitField tmp(*this);
//    for (int i = 0; i < bf.MemLen; i++) {
//        tmp.pMem[i] &= bf.pMem[i];
//    }
    int min, max;
    if(BitLen < bf.BitLen) { min = BitLen; max = bf.BitLen; }
    else{ min = bf.BitLen; max = BitLen; }
    TBitField tmp(max);

    int i = 0;
    while(i < min){
        tmp.pMem[i] = pMem[i] & bf.pMem[i];
        i++;
    }
    return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField tmp(BitLen);
    int i = 0;
    for (i = 0; i < MemLen; i++)
        tmp.pMem[i] = ~pMem[i];

    int ostat = BitLen & ((1 << shiftSize) - 1); // сколько битов нам надо в последнем байте
    tmp.pMem[i-1] = tmp.pMem[i-1] & ((1 << ostat) - 1); // делаем маску для нужных нам битов в байте, а остальные зануляем


    return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    int len = bf.GetLength();
    for (int i = 0; i < len; i++) {
        if(bf.GetBit(i))
            ostr << '1';
        else
            ostr << '0';
    }
    return ostr;
}
