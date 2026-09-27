#pragma once

/*
SHA-1 in C
By Steve Reid <steve@edmweb.com>
100% Public Domain

Test Vectors (from FIPS PUB 180-1)
"abc"
  A9993E36 4706816A BA3E2571 7850C26C 9CD0D89D
"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"
  84983E44 1C3BD26E BAAE4AA1 F95129E5 E54670F1
A million repetitions of "a"
  34AA973C D4C4DAA4 F61EEB2B DBAD2731 6534016F

Adapted to a C++ header-only implementation.
*/

#include <array>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>

namespace sha1
{
    class Hash
    {
    public:
        std::array<uint8_t, 20> bytes{};

        [[nodiscard]] std::string hex() const
        {
            static constexpr char DIGITS[] = "0123456789abcdef";
            std::string out(40, '0');
            for(size_t i = 0; i < bytes.size(); ++i)
            {
                out[i * 2] = DIGITS[bytes[i] >> 4];
                out[i * 2 + 1] = DIGITS[bytes[i] & 0x0F];
            }
            return out;
        }
    };

    class Hasher
    {
    private:
        uint32_t _state[5] = {};
        uint64_t _bit_count = 0;
        uint8_t _buffer[64] = {};
        size_t _buffer_len = 0;

        static uint32_t rol(uint32_t value, unsigned int bits)
        {
            return (value << bits) | (value >> (32 - bits));
        }

        void process_block(const uint8_t block[64])
        {
            uint32_t w[80];
            for(int i = 0; i < 16; ++i)
            {
                w[i] = (static_cast<uint32_t>(block[i * 4]) << 24)
                     | (static_cast<uint32_t>(block[i * 4 + 1]) << 16)
                     | (static_cast<uint32_t>(block[i * 4 + 2]) << 8)
                     | (static_cast<uint32_t>(block[i * 4 + 3]));
            }
            for(int i = 16; i < 80; ++i)
                w[i] = rol(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);

            uint32_t a = _state[0];
            uint32_t b = _state[1];
            uint32_t c = _state[2];
            uint32_t d = _state[3];
            uint32_t e = _state[4];

            for(int i = 0; i < 80; ++i)
            {
                uint32_t f;
                uint32_t k;
                if(i < 20)
                {
                    f = (b & c) | ((~b) & d);
                    k = 0x5A827999;
                }
                else if(i < 40)
                {
                    f = b ^ c ^ d;
                    k = 0x6ED9EBA1;
                }
                else if(i < 60)
                {
                    f = (b & c) | (b & d) | (c & d);
                    k = 0x8F1BBCDC;
                }
                else
                {
                    f = b ^ c ^ d;
                    k = 0xCA62C1D6;
                }

                const uint32_t temp = rol(a, 5) + f + e + k + w[i];
                e = d;
                d = c;
                c = rol(b, 30);
                b = a;
                a = temp;
            }

            _state[0] += a;
            _state[1] += b;
            _state[2] += c;
            _state[3] += d;
            _state[4] += e;
        }

    public:
        Hasher()
        {
            reset();
        }

        void reset()
        {
            _state[0] = 0x67452301;
            _state[1] = 0xEFCDAB89;
            _state[2] = 0x98BADCFE;
            _state[3] = 0x10325476;
            _state[4] = 0xC3D2E1F0;
            _bit_count = 0;
            _buffer_len = 0;
            std::memset(_buffer, 0, sizeof(_buffer));
        }

        void update(const void* data, size_t len)
        {
            const auto* bytes = static_cast<const uint8_t*>(data);
            _bit_count += static_cast<uint64_t>(len) * 8;

            if(_buffer_len != 0)
            {
                const size_t need = 64 - _buffer_len;
                if(len < need)
                {
                    std::memcpy(_buffer + _buffer_len, bytes, len);
                    _buffer_len += len;
                    return;
                }

                std::memcpy(_buffer + _buffer_len, bytes, need);
                process_block(_buffer);
                bytes += need;
                len -= need;
                _buffer_len = 0;
            }

            while(len >= 64)
            {
                process_block(bytes);
                bytes += 64;
                len -= 64;
            }

            if(len != 0)
            {
                std::memcpy(_buffer, bytes, len);
                _buffer_len = len;
            }
        }

        Hash finalize()
        {
            const uint64_t saved_bit_count = _bit_count;

            uint8_t padding = 0x80;
            update(&padding, 1);
            padding = 0x00;
            while((_bit_count % 512) != 448)
                update(&padding, 1);

            uint8_t length_bytes[8];
            for(int i = 0; i < 8; ++i)
                length_bytes[i] = static_cast<uint8_t>(saved_bit_count >> ((7 - i) * 8));
            update(length_bytes, sizeof(length_bytes));

            Hash hash;
            for(int i = 0; i < 5; ++i)
            {
                hash.bytes[i * 4]     = static_cast<uint8_t>(_state[i] >> 24);
                hash.bytes[i * 4 + 1] = static_cast<uint8_t>(_state[i] >> 16);
                hash.bytes[i * 4 + 2] = static_cast<uint8_t>(_state[i] >> 8);
                hash.bytes[i * 4 + 3] = static_cast<uint8_t>(_state[i]);
            }

            reset();
            return hash;
        }
    };

    inline Hash hash_bytes(const void* data, size_t len)
    {
        Hasher hasher;
        hasher.update(data, len);
        return hasher.finalize();
    }

    inline bool hash_file(const std::filesystem::path& path, Hash& out)
    {
        std::ifstream file(path, std::ios::binary);
        if(!file)
            return false;

        Hasher hasher;
        std::array<char, 65536> buffer{};
        while(file)
        {
            file.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
            const std::streamsize n = file.gcount();
            if(n > 0)
                hasher.update(buffer.data(), static_cast<size_t>(n));
        }

        if(file.bad())
            return false;

        out = hasher.finalize();
        return true;
    }
}
