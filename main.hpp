#include <array>
#include <bit>
#include <concepts>
#include <cstdint>
#include <string_view>

static_assert( ( sizeof( void* ) == 4 ), "This is for i386 (32-bit) only" );

namespace {

#define FORCE_INLINE [[gnu::always_inline]] inline
#define NOINLINE [[gnu::noinline, used]]
#define NO_RETURN [[gnu::noreturn]]
#define EXPORT extern "C"

template < typename SymbolTypes, SymbolTypes... _symbols >
    requires( ( std::convertible_to< SymbolTypes, char > ) &&
              ( sizeof...( _symbols ) > 0 ) )
[[nodiscard]] consteval auto operator""_array() {
    return ( std::array< char, ( sizeof...( _symbols ) + 1 ) >{
        char{ _symbols }..., '\0' } );
}

using call_t = enum class call : uint8_t {
    exit = 1,
    write = 4,
};

using descriptor_t = enum class descriptor : uint8_t {
    out = 1,
};

template < typename... Arguments >
    requires( sizeof...( Arguments ) <= 3 &&
              ( std::convertible_to< Arguments, uintptr_t > && ... ) )
FORCE_INLINE void syscall( call_t _systemCall, Arguments... _arguments ) {
    const auto l_systemCall = static_cast< uint32_t >( _systemCall );

    constexpr std::size_t l_amount = sizeof...( Arguments );

    std::array l_arguments = { static_cast< uintptr_t >( _arguments )... };

    if constexpr ( l_amount == 0 ) {
        __asm__ volatile( "int $0x80"
                          :
                          : "a"( l_systemCall )
                          : "memory", "cc" );

    } else if constexpr ( l_amount == 1 ) {
        const uintptr_t l_ebx = l_arguments[ 0 ];

        __asm__ volatile( "int $0x80"
                          :
                          : "a"( l_systemCall ), "b"( l_ebx )
                          : "memory", "cc" );

    } else if constexpr ( l_amount == 2 ) {
        const uintptr_t l_ebx = l_arguments[ 0 ];
        const uintptr_t l_ecx = l_arguments[ 1 ];

        __asm__ volatile( "int $0x80"
                          :
                          : "a"( l_systemCall ), "b"( l_ebx ), "c"( l_ecx )
                          : "memory", "cc" );

    } else if constexpr ( l_amount == 3 ) {
        const uintptr_t l_ebx = l_arguments[ 0 ];
        const uintptr_t l_ecx = l_arguments[ 1 ];
        const uintptr_t l_edx = l_arguments[ 2 ];

        __asm__ volatile( "int $0x80"
                          :
                          : "a"( l_systemCall ), "b"( l_ebx ), "c"( l_ecx ),
                            "d"( l_edx )
                          : "memory", "cc" );
    }
}

NO_RETURN void exit( int _exitCode ) {
    syscall( call_t::exit, _exitCode );

    __builtin_unreachable();
}

template < std::size_t N >
    requires( N > 0 )
FORCE_INLINE void write( descriptor_t _descriptor,
                         const std::array< char, N >& _buffer ) {
    syscall( call_t::write, static_cast< uintptr_t >( _descriptor ),
             std::bit_cast< uintptr_t >( _buffer.data() ), _buffer.size() );
}

FORCE_INLINE void write( descriptor_t _descriptor,
                         const std::string_view& _buffer ) {
    syscall( call_t::write, static_cast< uintptr_t >( _descriptor ),
             std::bit_cast< uintptr_t >( _buffer.data() ), _buffer.size() );
}

template < std::size_t N >
void print( const std::array< char, N >& _text ) {
    write( descriptor_t::out, _text );
}

[[gnu::used]] void print( const std::string_view& _text ) {
    write( descriptor_t::out, _text );
}

FORCE_INLINE void clearScreen() {
    static constexpr std::string_view l_x = "\033[H\033[J";

    print( l_x );
}

} // namespace

EXPORT void _start() {
    clearScreen();

    static constexpr std::string_view l_text = "Hello, world!\n";
    print( l_text );

    static constexpr auto l_text1 = "Hello,T world!\n"_array;
    print( l_text1 );

    exit( 0 );
}
