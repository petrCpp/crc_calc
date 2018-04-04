#ifndef CRCHASH_H
#define CRCHASH_H

// ******************************************************************************
// Template class 'CRCHash' implements mirror-algorithm of CRC calculation.

//template <typename T, const T POLYNOM, const T INITIAL, const T FINAL>
//template <typename T, T POLYNOM, T INITIAL, T FINAL>
template <typename T>
class CRCHash
{
private:
    T crc;
    T initial;
    T final;
    T polynome;
    //T table[256];
    void calcTable(void);
    T table[256];

public:
    CRCHash(T _initial, T _final, T _polynome) :
            crc(_initial),
            initial(_initial),
            final(_final),
            polynome(_polynome)
    {
        calcTable();
    }
    CRCHash(const CRCHash & iCRCHash) :
            crc(iCRCHash.crc),
            initial(iCRCHash.initial),
            final(iCRCHash.final),
            polynome(iCRCHash.polynome)
    {
        for(uint16_t i=0; i<256; i++) table[i] = iCRCHash.table[i];
    }
    void reset(void) { crc = initial; }
    void update(const void *buffer, size_t length);
    inline void update(unsigned char value);
    T evaluate(void) const { return crc ^ final; }
    static T evaluate(const void *buffer, size_t length);
};

// ******************************************************************************
// Warning: Microsoft Visual C++ Compiler cannot compile methods of inner
// nested classes inside a template defined outside of template body! For inner
// nested classes inside a template, you must define functions inside the class.
// Such functions automatically become inline functions. This error is
// generated for code allowed by the standard of C++ language, however, not
// yet supported by Microsoft Visual C++. So, if you get an error, just move
// the method into a nested class of a template.

template <typename T>
void CRCHash<T>::calcTable(void)
{
    for (int i = 0, t = 0; i < 256; t = 8, i++)
    {
        table[i] = i;
        while (t--) table[i] = table[i] >> 1 ^ (table[i] & 1 ? polynome :0);
    }
}

//******************************************************************************

template <typename T>
void CRCHash<T>::update(unsigned char value)
{
    crc = crc >> 8 ^ table[value ^ crc & 0xFFU];
}

//******************************************************************************

template <typename T>
void CRCHash<T>::update(const void *buffer, size_t length)
{
    register const unsigned char * block =
            static_cast<const unsigned char *>(buffer);
    while (length--) update(*block++);
}

//******************************************************************************

template <typename T>
T CRCHash<T>::evaluate(const void *buffer, size_t length)
{
    CRCHash instance;
    instance.update(buffer, length);
    return instance.evaluate();
}

//******************************************************************************

typedef unsigned char  CRC08;
typedef unsigned short CRC16;
typedef unsigned long  CRC32;

//class CRC08Hash : public CRCHash<CRC08, 0x8CU, 0xFFU, 0xFFU> {};
//class CRC16Hash : public CRCHash<CRC16, 0xA001U, 0x0000U, 0x0000U> {};
//class CRC32Hash : public CRCHash<CRC32, 0xEDB88320UL, 0xFFFFFFFFUL, 0xFFFFFFFFUL> {};

//******************************************************************************

// typedef unsigned __int64 CRC64;
// class CRC64Hash : public CRCHash<CRC64, 0x000000000000001BUI64, 0xFFFFFFFFFFFFFFFFUI64, 0xFFFFFFFFFFFFFFFFUI64> {};

//******************************************************************************


// ******************************************************************************
// Template class 'CRCHash' implements mirror-algorithm of CRC calculation.

/*template <typename T, const T POLYNOM, const T INITIAL, const T FINAL>
class CRCHash
{
private:
    T CRC;
    struct CRCTable { T Data[256]; CRCTable(void); };
    static const CRCTable Table;
public:
    CRCHash(void) : CRC(INITIAL) {}
    CRCHash(const CRCHash & iCRCHash) : CRC(iCRCHash.CRC) {}
    void Reset(void) { CRC = INITIAL; }
    void Update(const void *Buffer, size_t Length);
    inline void Update(unsigned char Value);
    T Evaluate(void) const { return CRC ^ FINAL; }
    static T Evaluate(const void *Buffer, size_t Length);
};

template <typename T, T POLYNOM, T INITIAL, T FINAL>
const CRCHash<T, POLYNOM, INITIAL, FINAL>::CRCTable
CRCHash<T, POLYNOM, INITIAL, FINAL>::Table;

// ******************************************************************************
// Warning: Microsoft Visual C++ Compiler cannot compile methods of inner
// nested classes inside a template defined outside of template body! For inner
// nested classes inside a template, you must define functions inside the class.
// Such functions automatically become inline functions. This error is
// generated for code allowed by the standard of C++ language, however, not
// yet supported by Microsoft Visual C++. So, if you get an error, just move
// the method into a nested class of a template.

template <typename T, T POLYNOM, T INITIAL, T FINAL>
CRCHash<T, POLYNOM, INITIAL, FINAL>::
CRCTable::CRCTable(void)
{
    for (int i = 0, t = 0; i < 256; t = 8, i++)
    {
        Data[i] = i;
        while (t--) Data[i] = Data[i] >> 1 ^ (Data[i] & 1 ? POLYNOM :0);
    }
}

//******************************************************************************

template <typename T, T POLYNOM, T INITIAL, T FINAL>
void CRCHash<T, POLYNOM, INITIAL, FINAL>::
Update(unsigned char Value)
{
    CRC = CRC >> 8 ^ Table.Data[Value ^ CRC & 0xFFU];
}

//******************************************************************************

template <typename T, T POLYNOM, T INITIAL, T FINAL>
void CRCHash<T, POLYNOM, INITIAL, FINAL>::
Update(const void *Buffer, size_t Length)
{
    register const unsigned char * Block =
            static_cast<const unsigned char *>(Buffer);
    while (Length--) Update(*Block++);
}

//******************************************************************************

template <typename T, T POLYNOM, T INITIAL, T FINAL>
T CRCHash<T, POLYNOM, INITIAL, FINAL>::
Evaluate(const void *Buffer, size_t Length)
{
    CRCHash Instance;
    Instance.Update(Buffer, Length);
    return Instance.Evaluate();
}

// ******************************************************************************

typedef unsigned char  CRC08;
typedef unsigned short CRC16;
typedef unsigned long  CRC32;

class CRC08Hash : public CRCHash<CRC08, 0x8CU, 0xFFU, 0xFFU> {};
class CRC16Hash : public CRCHash<CRC16, 0xA001U, 0x0000U, 0x0000U> {};
class CRC32Hash : public CRCHash<CRC32, 0xEDB88320UL, 0xFFFFFFFFUL, 0xFFFFFFFFUL> {};
*/
// ******************************************************************************

// typedef unsigned __int64 CRC64;
// class CRC64Hash : public CRCHash<CRC64, 0x000000000000001BUI64, 0xFFFFFFFFFFFFFFFFUI64, 0xFFFFFFFFFFFFFFFFUI64> {};

//******************************************************************************

#endif // CRCHASH_H
