#pragma once

namespace core
{

    class cTexture
    {

    private:
        struct TargaHeader
        {
            unsigned char data1[12];
            unsigned short width;
            unsigned short height;
            unsigned char bpp;
            unsigned char data2;
        };

    public:
        cTexture();
        cTexture(const cTexture&) = delete;
        ~cTexture() { /*empty destructor*/ }

        bool Load(const char*);
        void Destroy();
        void Bind();

        unsigned int GetTexture();
        int GetWidth();
        int GetHeight();

    private:
        bool LoadTarga32Bit(const char*);

    private:
        unsigned char* _targaData;
        unsigned int _texture_id;
        //ID3D11Texture2D* m_texture;
        //ID3D11ShaderResourceView* m_textureView;
        int _width, _height;

    };

}
