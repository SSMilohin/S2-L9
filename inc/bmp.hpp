#pragma once
#include <iostream>
#include <fstream>

namespace sm
{
    #pragma pack(1) // Отключить выравнивание
    struct BMPHEADER
    {
        unsigned short    Type;
        unsigned int      Size;
        unsigned short    Reserved1;
        unsigned short    Reserved2;
        unsigned int      OffBits;
    };
    #pragma pack()

    #pragma pack(1)
    struct BMPINFO
    {
        unsigned int    Size;
        int             Width;
        int             Height;
        unsigned short  Planes;
        unsigned short  BitCount;
        unsigned int    Compression;
        unsigned int    SizeImage;
        int             XPelsPerMeter;
        int             YPelsPerMeter;
        unsigned int    ClrUsed;
        unsigned int    ClrImportant;
    };
    #pragma pack()

    #pragma pack(1)
    struct Pixel
    {
        unsigned char b;
        unsigned char g;
        unsigned char r;
    };
    #pragma pack()

    class BMP
    {
    public:
        BMP(int height, int width)
        {
            m_height = height;
            m_width = width;
            m_pixels = new Pixel * [m_height];
            for (int i = 0; i < m_height; i++)
                m_pixels[i] = new Pixel[m_width];

            // Формирование заголовка
            m_header.Type = 0x4D42; // Тип данных BMP
            m_header.Size = 14 + 40 + (3 * m_width * m_height);
            if (m_width % 4 != 0)
                m_header.Size += (4 - (3 * m_width) % 4) * m_height;
            m_header.OffBits = 54;
            m_header.Reserved1 = 0;
            m_header.Reserved2 = 0;

            // Формирование информации об изображении
            m_info.BitCount = 24;
            m_info.ClrImportant = 0;
            m_info.ClrUsed = 0;
            m_info.Compression = 0;
            m_info.Height = m_height;
            m_info.Planes = 1;
            m_info.Size = 40;
            m_info.SizeImage = m_header.Size - 54;
            m_info.Width = m_width;
            m_info.XPelsPerMeter = 0;
            m_info.YPelsPerMeter = 0;
        }

        BMP(std::istream& in)
        {
            in.read(reinterpret_cast<char*>(&m_header), sizeof(BMPHEADER));
            in.read(reinterpret_cast<char*>(&m_info), sizeof(BMPINFO));

            m_height = m_info.Height;
            m_width = m_info.Width;
            m_pixels = new Pixel * [m_height];
            for (int i = 0; i < m_height; i++)
                m_pixels[i] = new Pixel[m_width];

            for (int i = 0; i < m_height; i++)
            {
                for (int j = 0; j < m_width; j++)
                    in.read(reinterpret_cast<char*>(&m_pixels[i][j]), sizeof(Pixel));

                if ((3 * m_width) % 4 != 0)
                    for (int j = 0; j < 4 - (3 * m_width) % 4; j++)
                    {
                        char c;
                        in.read(&c, 1);
                    }
            }
        }

        BMP(const BMP& copy)
        {
            m_header = copy.m_header;
            m_info = copy.m_info;
            m_height = copy.m_height;
            m_width = copy.m_width;
            m_pixels = new Pixel * [m_height];
            for (int i = 0; i < m_height; i++)
                m_pixels[i] = new Pixel[m_width];

            for (int i = 0; i < m_height; i++)
                for (int j = 0; j < m_width; j++)
                    m_pixels[i][j] = copy.m_pixels[i][j];
        }

        void fill(Pixel** pixels, int height, int width)
        {
            for (int i = 0; i < height; i++)
                for (int j = 0; j < width; j++)
                    m_pixels[i][j] = pixels[i][j];
        }

        void print(std::ofstream& out)
        {
            out.write(reinterpret_cast<char*>(&m_header), sizeof(BMPHEADER));
            out.write(reinterpret_cast<char*>(&m_info), sizeof(BMPINFO));
            for (int i = 0; i < m_height; i++)
            {
                for (int j = 0; j < m_width; j++)
                    out.write(reinterpret_cast<char*>(&m_pixels[i][j]), sizeof(Pixel));

                if ((3 * m_width) % 4 != 0)
                    for (int j = 0; j < 4 - (3 * m_width) % 4; j++)
                    {
                        char c = 0;
                        out.write(&c, 1);
                    }
            }
        }

        void customFilter()
        {
            for (int i = 0; i < m_height; i++)
                for (int j = 0; j < m_width; j++)
                {
                    if (m_pixels[i][j].b - 50 > 0)
                        m_pixels[i][j].b -= 50;
                    if (m_pixels[i][j].g - 50 > 0)
                        m_pixels[i][j].g -= 50;
                    if (m_pixels[i][j].r - 50 > 0)
                        m_pixels[i][j].r -= 50;
                }
        }

        int getWidth() { return m_width; }
        int getHeight() { return m_height; }
        Pixel** getPixels() { return m_pixels; }

        ~BMP()
        {
            for (int i = 0; i < m_height; i++)
                delete[] m_pixels[i];
            delete[] m_pixels;
        }
    private:
        int m_width, m_height;
        Pixel** m_pixels;
        BMPHEADER m_header;
        BMPINFO m_info;
    };
}