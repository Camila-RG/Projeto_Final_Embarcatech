
// Fontes para A-Z, a-z e 0-9. Os caracteres tem 8x8 pixels


static uint8_t font[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Nothing
    0x00, 0x3e, 0x41, 0x41, 0x41, 0x3e, 0x00, 0x00, //0
    0x00, 0x00, 0x42, 0x7f, 0x40, 0x00, 0x00, 0x00, //1
    0x00, 0x00, 0x62, 0x51, 0x49, 0x66, 0x00, 0x00, //2
    0x00, 0x00, 0x22, 0x41, 0x49, 0x49, 0x36, 0x00, //3
    0x00, 0x00, 0x18, 0x14, 0x12, 0x7f, 0x10, 0x00, //4
    0x00, 0x00, 0x27, 0x45, 0x45, 0x45, 0x39, 0x00, //5
    0x00, 0x00, 0x3e, 0x49, 0x49, 0x49, 0x32, 0x00, //6
    0x00, 0x00, 0x03, 0x61, 0x19, 0x05, 0x03, 0x00, //7
    0x00, 0x00, 0x36, 0x49, 0x49, 0x49, 0x36, 0x00, //8
    0x00, 0x00, 0x06, 0x49, 0x49, 0x49, 0x3e, 0x00, //9
    0x00, 0x7c, 0x82, 0x82, 0x82, 0x7c, 0x80, 0x40, //A
    0x00, 0x84, 0xfe, 0x92, 0x92, 0x92, 0x6c, 0x00, //B
    0x00, 0x7c, 0xc6, 0x82, 0x82, 0x89, 0x4c, 0x00, //C
    0x00, 0x82, 0xfe, 0x82, 0x82, 0x82, 0x7c, 0x00, //D
    0x00, 0x82, 0xfe, 0x92, 0x92, 0x82, 0x6e, 0x00, //E
    0x00, 0x82, 0xfe, 0x92, 0x02, 0x0a, 0x04, 0x00, //F
    0x00, 0x7c, 0x82, 0x82, 0x82, 0x92, 0x74, 0x00, //G
    0x00, 0x82, 0xfe, 0x92, 0x10, 0x10, 0xfe, 0x82, //H
    0x00, 0x00, 0x04, 0x82, 0xfe, 0x82, 0x00, 0x00, //I
    0x00, 0x64, 0xa2, 0x82, 0x7e, 0x02, 0x00, 0x00, //J
    0x00, 0x82, 0xfe, 0x90, 0x28, 0xc6, 0x82, 0x00, //K
    0x00, 0x00, 0x82, 0xfe, 0x82, 0x80, 0xc0, 0x00, //L
    0x06, 0x03, 0xfe, 0x02, 0x8c, 0x02, 0xfe, 0x80, //M
    0x00, 0x82, 0xfe, 0x8c, 0x30, 0xc2, 0xfe, 0x02, //N
    0x00, 0x7c, 0x80, 0x9e, 0x81, 0x81, 0x7e, 0x00, //O
    0x00, 0x04, 0x82, 0xfe, 0x92, 0x12, 0x0c, 0x00, //P
    0x00, 0x7c, 0x82, 0x82, 0xe2, 0xc6, 0x7c, 0x80, //Q
    0x00, 0x82, 0xfe, 0x92, 0x12, 0xec, 0x80, 0x40, //R
    0x00, 0x00, 0xcc, 0x92, 0x92, 0x66, 0x00, 0x00, //S
    0x00, 0x48, 0x8c, 0x84, 0xfc, 0x04, 0x02, 0x00, //T
    0x02, 0x7e, 0x80, 0x80, 0x80, 0x7e, 0x80, 0x40, //U
    0x00, 0x00, 0x0e, 0x30, 0xc0, 0x30, 0x0e, 0x00, //V
    0x04, 0x7e, 0x80, 0x40, 0x3c, 0x40, 0x80, 0x7e, //W
    0x00, 0x04, 0x82, 0xee, 0x10, 0xee, 0x82, 0x40, //X
    0x04, 0x02, 0x5e, 0x90, 0x90, 0x5e, 0x02, 0x00, //Y
    0x00, 0xc6, 0xa2, 0x92, 0x8a, 0xc4, 0x00, 0x00, //Z
    
    0x00, 0x78, 0x84, 0x84, 0x84, 0x78, 0x80, 0x00, // a
    0x00, 0x7E, 0x90, 0x88, 0x88, 0x70, 0x00, 0x00, // b
    0x00, 0x78, 0x84, 0x84, 0x84, 0x48, 0x00, 0x00, // c
    0x00, 0x70, 0x88, 0x88, 0x88, 0xFE, 0x00, 0x00, // d
    0x00, 0x78, 0x94, 0x94, 0x94, 0x58, 0x00, 0x00, // e
    0x00, 0x00, 0x10, 0xFC, 0x12, 0x02, 0x04, 0x00, // f
    0x00, 0x1C, 0xA2, 0xA2, 0xA2, 0x7E, 0x00, 0x00, // g
    0x00, 0xFE, 0x08, 0x08, 0x08, 0xF0, 0x80, 0x00, // h
    0x00, 0x00, 0x00, 0x08, 0xFA, 0x80, 0x00, 0x00, // i
    0x00, 0x00, 0xC0, 0x84, 0x7D, 0x00, 0x00, 0x00, // j
    0x00, 0x00, 0xFC, 0x10, 0x28, 0xC4, 0x00, 0x00, // k
    0x00, 0x00, 0x00, 0xFC, 0x80, 0x00, 0x00, 0x00, // l
    0X18, 0x08, 0xF0, 0x08, 0xF0, 0x08, 0xF0, 0x80, // m
    0x00, 0x10, 0x08, 0xF0, 0x08, 0xF0, 0x80, 0x00, // n
    0x00, 0x00, 0x70, 0x88, 0x88, 0x70, 0x00, 0x00, // o
    0x00, 0x00, 0x08, 0xFC, 0x24, 0x24, 0x18, 0x00, // p
    0x00, 0x0E, 0x11, 0x11, 0x51, 0xFE, 0x40, 0x20, // q
    0x00, 0x00, 0xFC, 0x08, 0x04, 0x0C, 0x00, 0x00, // r
    0x00, 0x00, 0x98, 0x94, 0x94, 0x64, 0x00, 0x00, // s
    0x00, 0x04, 0x7E, 0x84, 0x84, 0x60, 0x00, 0x00, // t
    0x00, 0x08, 0x78, 0x80, 0x80, 0x78, 0x80, 0x00, // u
    0x00, 0x38, 0x40, 0x80, 0x40, 0x38, 0x00, 0x00, // v
    0x78, 0x80, 0x40, 0x20, 0x40, 0x80, 0x78, 0x00, // w
    0x00, 0x88, 0x50, 0x20, 0x50, 0x88, 0x00, 0x00, // x
    0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C, 0x00, 0x00, // y
    0x00, 0x00, 0xC4, 0xC4, 0xA4, 0x8C, 0x00, 0x00, // z
    };