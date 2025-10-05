#pragma once

#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

// Simple LZ77 compressor / decompressor (readable, not optimized)

// ---- configuration ----
static constexpr int WINDOW_SIZE = 1024; // look-back window
static constexpr int MIN_MATCH = 3;      // minimum match length to emit a match
// ------------------------

static void writeU32Le( std::vector< uint8_t >& out, uint32_t v ) {
    out.push_back( uint8_t( v & 0xFF ) );
    out.push_back( uint8_t( ( v >> 8 ) & 0xFF ) );
    out.push_back( uint8_t( ( v >> 16 ) & 0xFF ) );
    out.push_back( uint8_t( ( v >> 24 ) & 0xFF ) );
}
static auto read_u32_le( const std::vector< uint8_t >& in, size_t pos )
    -> uint32_t {
    uint32_t v = 0;
    v |= uint32_t( in[ pos + 0 ] );
    v |= uint32_t( in[ pos + 1 ] ) << 8;
    v |= uint32_t( in[ pos + 2 ] ) << 16;
    v |= uint32_t( in[ pos + 3 ] ) << 24;
    return v;
}
static void writeU16Le( std::vector< uint8_t >& out, uint16_t v ) {
    out.push_back( uint8_t( v & 0xFF ) );
    out.push_back( uint8_t( ( v >> 8 ) & 0xFF ) );
}
static auto read_u16_le( const std::vector< uint8_t >& in, size_t pos )
    -> uint16_t {
    return uint16_t( in[ pos ] ) | ( uint16_t( in[ pos + 1 ] ) << 8 );
}

// Compress input (string_view) -> binary blob
auto lz77_compress( std::string_view src ) -> std::vector< uint8_t > {
    std::vector< uint8_t > out;
    // placeholder for original size (fill after tokens or write now)
    writeU32Le( out, uint32_t( src.size() ) );

    size_t i = 0;
    const size_t N = src.size();

    while ( i < N ) {
        // search best match in window
        size_t best_len = 0;
        size_t best_dist = 0;
        size_t window_start = ( i > WINDOW_SIZE ) ? ( i - WINDOW_SIZE ) : 0;

        for ( size_t j = window_start; j < i; ++j ) {
            size_t k = 0;
            // match while equal and within bounds and length < 255
            while ( i + k < N && src[ j + k ] == src[ i + k ] && k < 255 &&
                    ( j + k ) < i ) {
                ++k;
            }
            if ( k > best_len ) {
                best_len = k;
                best_dist = i - j;
            }
            // small micro-optimisation: if we reached the theoretical max,
            // break
            if ( best_len == 255 )
                break;
        }

        if ( best_len >= MIN_MATCH ) {
            // Emit match token: 0x01 + offset(u16) + length(u8)
            out.push_back( 0x01 );
            writeU16Le( out,
                        uint16_t( best_dist ) );  // guaranteed <= WINDOW_SIZE
            out.push_back( uint8_t( best_len ) ); // 1..255
            i += best_len;
        } else {
            // Emit literal token: 0x00 + byte
            out.push_back( 0x00 );
            out.push_back( static_cast< uint8_t >( src[ i ] ) );
            ++i;
        }
    }

    return out;
}

// Decompress blob produced by lz77_compress
// Returns the decompressed bytes as a std::string
auto lz77_decompress( std::span< const uint8_t > blob ) -> std::string {
    if ( blob.size() < 8 ) {
        return {};
    }

    uint32_t orig_size =
        uint32_t( blob[ 0 ] ) | ( uint32_t( blob[ 1 ] ) << 8 ) |
        ( uint32_t( blob[ 2 ] ) << 16 ) | ( uint32_t( blob[ 3 ] ) << 24 );
    std::string out;
    out.reserve( orig_size );

    size_t pos = 4;
    while ( pos < blob.size() ) {
        uint8_t token = blob[ pos++ ];
        if ( token == 0x00 ) {
            if ( pos >= blob.size() )
                break; // malformed -> stop
            out.push_back( static_cast< char >( blob[ pos++ ] ) );
        } else if ( token == 0x01 ) {
            if ( pos + 3 > blob.size() )
                break; // malformed
            uint16_t offset =
                uint16_t( blob[ pos ] ) | ( uint16_t( blob[ pos + 1 ] ) << 8 );
            uint8_t length = blob[ pos + 2 ];
            pos += 3;
            if ( offset == 0 || offset > out.size() ) {
                // malformed offset -- graceful exit
                break;
            }
            size_t src = out.size() - offset;
            for ( unsigned k = 0; k < length; ++k ) {
                // support overlap copying
                char ch = out[ src + k ];
                out.push_back( ch );
            }
        } else {
            // unknown token → stop
            break;
        }
    }

    // If we know original size, we can truncate/validate
    if ( out.size() > orig_size )
        out.resize( orig_size );
    return out;
}

// ----------------- demo -----------------
#include <iostream>
auto main() -> int {
#define MAP                                                          \
    "+------+     +-------+   M#                  K####T         \n" \
    "|....K.|  ###}.......|    #             +----/----+        C\n" \
    "|.@.....###  |.......|    #             |.........|        #\n" \
    "|......|  #  |........###### ###########}.........|     W##M\n" \
    "|......|     |.M.....|      K           |.........|     #   \n" \
    "+------+     |...C...|            +-----+C........|     #   \n" \
    "|.G....|     +-------+            |.....C.M.......{#####    \n" \
    "|......+-----+                    |..........M....|         \n" \
    ">............}K###################}....W..........|         \n" \
    "+------------+                    +---------------+         \n"

    std::string_view map = MAP;
    auto compressed = lz77_compress( map );
    auto decompressed = lz77_decompress( compressed );

    std::cout << "original size:   " << map.size() << '\n';
    std::cout << "compressed size: " << compressed.size() << '\n';
    std::cout << "decompressed size: " << decompressed.size() << '\n';
    std::cout << "decompressed: " << decompressed << '\n';
    std::cout << ( decompressed == map ? "OK: match\n" : "ERROR: mismatch\n" );
    return 0;
}
